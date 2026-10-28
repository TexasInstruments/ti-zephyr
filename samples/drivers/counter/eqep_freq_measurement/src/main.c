/*
 * SPDX-FileCopyrightText: Copyright (c) 2026 Texas Instruments
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/counter/ti_am3352_eqep.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>

#define SIGNAL_FREQ_HZ 100U
/* Duty cycle : 25% */
#define SIGNAL_STEPS   4U
#define SIGNAL_STEP_US (USEC_PER_SEC / (SIGNAL_FREQ_HZ * SIGNAL_STEPS))

/* Kept small so the counter wraps every few windows, exercising the
 * wraparound path. It must exceed the counts seen in a single window.
 */
#define POSITION_TOP 999U
BUILD_ASSERT(POSITION_TOP > 2U * SIGNAL_FREQ_HZ, "position counter wraps within a window");

#define FREQ_TOLERANCE_HZ 1U

static const struct device *const eqep = DEVICE_DT_GET(DT_ALIAS(eqep0));
static const struct gpio_dt_spec signal = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), signal_gpios);

K_MSGQ_DEFINE(msgq, sizeof(uint32_t), 8, 4);

static struct counter_alarm_cfg window_cfg;

static void signal_step(struct k_timer *timer)
{
	static uint32_t step;

	ARG_UNUSED(timer);

	gpio_pin_set_dt(&signal, step == 0U);
	step = (step + 1U) % SIGNAL_STEPS;
}

K_TIMER_DEFINE(signal_timer, signal_step, NULL);

static void window_expired(const struct device *dev, uint8_t chan_id, uint32_t ticks,
			   void *user_data)
{
	int ret;

	ARG_UNUSED(user_data);

	(void)k_msgq_put(&msgq, &ticks, K_NO_WAIT);

	ret = counter_set_channel_alarm(dev, chan_id, &window_cfg);
	if (ret != 0) {
		printf("Failed to re-arm unit timer (%d)\n", ret);
	}
}

static int eqep_setup(void)
{
	const struct ti_eqep_dec_cfg dec_cfg = {
		.source = TI_EQEP_SRC_UP,
		.rising_edge_only = true,
	};
	const struct ti_eqep_qep_cfg qep_cfg = {
		.reset_mode = TI_EQEP_RESET_MODE_MAX,
		.capture_latch = TI_EQEP_CAPTURE_LATCH_TIMEOUT,
	};
	const struct counter_top_cfg top_cfg = {
		.ticks = POSITION_TOP,
	};
	int ret;

	if (!device_is_ready(eqep)) {
		printf("EQEP device %s not ready\n", eqep->name);
		return -ENODEV;
	}

	ti_eqep_configure_decoder(eqep, &dec_cfg);
	ti_eqep_configure_qep(eqep, &qep_cfg);

	ret = counter_set_top_value(eqep, &top_cfg);
	if (ret != 0) {
		printf("Failed to set top value (%d)\n", ret);
		return ret;
	}

	uint32_t ticks = counter_get_frequency(eqep);

	/* One unit timer period of eQEP clocks is a 1 s window, so the
	 * position delta per window is the signal frequency in Hz.
	 */
	window_cfg.callback = window_expired;
	window_cfg.ticks = ticks;

	ret = counter_set_channel_alarm(eqep, TI_EQEP_ALARM_CHAN_TIMEOUT, &window_cfg);
	if (ret != 0) {
		printf("Failed to arm unit timer (%d)\n", ret);
		return ret;
	}

	ret = counter_start(eqep);
	if (ret != 0) {
		printf("Failed to start counter (%d)\n", ret);
		return ret;
	}

	printf("eQEP clock %u Hz; position top %u\n", ticks, POSITION_TOP);

	return 0;
}

static int signal_setup(void)
{
	int ret;

	if (!gpio_is_ready_dt(&signal)) {
		printf("Signal GPIO not ready\n");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&signal, GPIO_OUTPUT_INACTIVE);
	if (ret != 0) {
		printf("Failed to configure signal GPIO (%d)\n", ret);
		return ret;
	}

	k_timer_start(&signal_timer, K_USEC(SIGNAL_STEP_US), K_USEC(SIGNAL_STEP_US));

	printf("Signal: %u Hz, %u%% duty\n", SIGNAL_FREQ_HZ, 100U / SIGNAL_STEPS);

	return 0;
}

int main(void)
{
	uint32_t position;
	uint32_t prev_position = 0U;
	int ret;

	printf("TI EQEP frequency measurement sample\n");

	ret = eqep_setup();
	if (ret != 0) {
		return ret;
	}

	ret = signal_setup();
	if (ret != 0) {
		return ret;
	}

	while (true) {
		uint32_t delta;
		bool wrapped;

		k_msgq_get(&msgq, &position, K_FOREVER);

		/* The position counter runs 0..top and then restarts from 0. */
		if (position < prev_position) {
			wrapped = true;
			delta = (POSITION_TOP - prev_position) + position + 1U;
		} else {
			wrapped = false;
			delta = position - prev_position;
		}
		prev_position = position;

		printf("position %4u, measured %u Hz", position, delta);
		if (wrapped) {
			printf(" (counter wrapped)");
		}
		if (delta + FREQ_TOLERANCE_HZ < SIGNAL_FREQ_HZ ||
		    delta > SIGNAL_FREQ_HZ + FREQ_TOLERANCE_HZ) {
			printf(" [out of tolerance]");
		}
		printf("\n");
	}

	return 0;
}
