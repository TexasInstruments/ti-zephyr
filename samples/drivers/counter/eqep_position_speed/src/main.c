/*
 * SPDX-FileCopyrightText: Copyright (c) 2026 Texas Instruments
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/counter/ti_am3352_eqep.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

#define EQEP_NODE DT_ALIAS(eqep0)
#define EPWM_A_NODE DT_NODELABEL(main_epwm0)

/* Decoder is configured for 4x counting (all edges of both A and B). */
#define EQEP_COUNTS_PER_REV (CONFIG_APP_EQEP_LINES_PER_REV * 4U)

static const struct gpio_dt_spec index_gpio = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), index_gpios);

static struct counter_alarm_cfg index_alarm_cfg;

static struct pwm_event_callback period_cb;
static uint32_t period_count;

static int64_t old_pos;

/*
 * The real EQEP unit-timer alarm (mirrors EQEP_INT_UNIT_TIME_OUT). Only
 * latches QPOSLAT - the value the hardware captured at the exact timeout
 * instant, the same register EQEP_getPositionLatch() reads - and flags the
 * event; pos_speed_calculate() below, called every EPWM tick just like the
 * reference's App_epwmIntrISR() calls PosSpeed_calculate(), does the math
 * for both speed(FR) and speed(PR) - the capture unit's QCTLAT/QCPRDLAT are
 * latched by hardware on this same timeout event (capture_latch =
 * TI_EQEP_CAPTURE_LATCH_TIMEOUT below).
 */
static void eqep_index_callback(const struct device *dev, uint8_t chan_id, uint32_t position,
				  void *user_data)
{
	ARG_UNUSED(user_data);

	/* 1000 is the number of periods of EPWM the index event occurs */
	printf("Current position: %d, RPM : %lld\n", position, 
		((old_pos - position) * CONFIG_APP_EPWM_OUTPUT_FREQ_HZ * 60) 
		/ (1000 * EQEP_COUNTS_PER_REV));
	old_pos = position;

	/* Alarms are one-shot: re-arm the unit timeout for the next update. */
	(void)counter_set_channel_alarm(dev, chan_id, &index_alarm_cfg);
}

/*
 * Fires once per simulated quadrature cycle (main_epwm0's PERIOD event).
 * Once per simulated revolution, pulses the index line for exactly one EPWM
 * period ( ticks = one full simulated
 * revolution; the EQEP's own hardware reset_mode = INDEX picks this up and
 * resets QPOSCNT with no interrupt/callback needed on our side). Also calls
 * pos_speed_calculate() every tick, exactly like the reference's EPWM ISR
 * calls PosSpeed_calculate(), and prints a report whenever it produced a
 * fresh speed(FR) sample.
 */
static void index_pulse_handler(const struct device *dev, struct pwm_event_callback *cb,
				uint32_t channel, pwm_events_t events)
{
	ARG_UNUSED(cb);
	ARG_UNUSED(channel);
	ARG_UNUSED(events);

	period_count++;
	if (period_count >= CONFIG_APP_EQEP_LINES_PER_REV) {
		period_count = 0;
		gpio_pin_set_dt(&index_gpio, 1);
	} else if (period_count == 1) {
		gpio_pin_set_dt(&index_gpio, 0);
	}
}

static int configure_pwm_channel(const struct device *dev, uint32_t channel)
{
	uint64_t cycles_per_sec;
	uint32_t period_cycles;
	int err;

	err = pwm_get_cycles_per_sec(dev, 0, &cycles_per_sec);
	if (err != 0) {
		return err;
	}

	period_cycles = (uint32_t)(cycles_per_sec / CONFIG_APP_EPWM_OUTPUT_FREQ_HZ);

	if(channel == 0) {
		return pwm_set_cycles(dev, 0, period_cycles, period_cycles / 2, PWM_POLARITY_NORMAL);	
	} else {
		return pwm_set_cycles(dev, 1, period_cycles, period_cycles / 2, PWM_POLARITY_NORMAL
			 | (TI_EHRPWM_USE_AQ_SET_ZRO_PRD));
	}
	}

int main(void)
{
	const struct device *const eqep_dev = DEVICE_DT_GET(EQEP_NODE);
	const struct device *const epwm_a_dev = DEVICE_DT_GET(EPWM_A_NODE);
	const struct ti_eqep_dec_cfg dec_cfg = {
		.source = TI_EQEP_SRC_QUADRATURE,
		.rising_edge_only = false,
		.swap_inputs = false,
	};
	const struct ti_eqep_qep_cfg qep_cfg = {
		.reset_mode = TI_EQEP_RESET_MODE_MAX,
		.index_latch = TI_EQEP_INDEX_LATCH_RISING,
		.strobe_latch = TI_EQEP_STROBE_LATCH_RISING,
		.capture_latch = TI_EQEP_CAPTURE_LATCH_TIMEOUT,
	};
	/*
	 * With reset_mode = INDEX, the position counter is reset by the index
	 * pulse each revolution, not by reaching a top value - QPOSMAX is only
	 * the reverse-direction wrap boundary. Match the reference's maxPosition
	 * argument (CSL_EQEP_QPOSCNT_QPOSCNT_MAX) rather than EQEP_COUNTS_PER_REV.
	 */
	struct counter_top_cfg top_cfg = {
		.ticks = 0xFFFFFFFFU,
		.flags = 0U,
	};
	int err;

	printf("EQEP position/speed sample on %s\n", CONFIG_BOARD_TARGET);
	printf("Expected speed: %u RPM\n",
	       CONFIG_APP_EPWM_OUTPUT_FREQ_HZ * 60U / CONFIG_APP_EQEP_LINES_PER_REV);

	if (!device_is_ready(eqep_dev) || !device_is_ready(epwm_a_dev) ||
	    !device_is_ready(index_gpio.port)) {
		printf("A required device is not ready\n");
		return 0;
	}

	printf("eqep0: frequency=%u Hz\n", counter_get_frequency(eqep_dev));


	/*
	 * Drive the index line to a stable known-low state *before* the
	 * decoder (index_latch = RISING) is started below - otherwise the
	 * pin's undriven/default power-on state, or the mode-switch glitch
	 * while gpio_pin_configure_dt() takes ownership of it, can look like
	 * a spurious rising edge and trigger a bogus early INDEX reset.
	 */
	err = gpio_pin_configure_dt(&index_gpio, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		printf("Failed to configure index GPIO (err %d)\n", err);
		return 0;
	}

	ti_eqep_configure_decoder(eqep_dev, &dec_cfg);
	ti_eqep_configure_qep(eqep_dev, &qep_cfg);

	err = counter_set_top_value(eqep_dev, &top_cfg);
	if (err != 0) {
		printf("Failed to set top value (err %d)\n", err);
		return 0;
	}

	counter_start(eqep_dev);

	/* Issued back-to-back to keep the two instances' relative start skew small. */
	err = configure_pwm_channel(epwm_a_dev, 0);
	err |= configure_pwm_channel(epwm_a_dev, 1);
	if (err != 0) {
		printf("Failed to configure simulated encoder signals\n");
		return 0;
	}

	pwm_init_event_callback(&period_cb, index_pulse_handler, 0, PWM_EVENT_TYPE_PERIOD);
	err = pwm_add_event_callback(epwm_a_dev, &period_cb);
	if (err != 0) {
		printf("Failed to register EPWM period callback (err %d)\n", err);
		return 0;
	}

	/* Mirrors the reference's EQEP_enableUnitTimer(base, FCLK/EQEP_UNIT_TIMEOUT_FREQ). */
	index_alarm_cfg.callback = eqep_index_callback;
	index_alarm_cfg.user_data = NULL;
	index_alarm_cfg.flags = 0;

	err = counter_set_channel_alarm(eqep_dev, TI_EQEP_ALARM_CHAN_INDEX, &index_alarm_cfg);
	if (err != 0) {
		printf("Failed to enable EQEP unit timer (err %d)\n", err);
		return 0;
	}

	printf("Reporting position/speed every %u ms...\n", CONFIG_APP_EQEP_UPDATE_INTERVAL_MS);

	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}
