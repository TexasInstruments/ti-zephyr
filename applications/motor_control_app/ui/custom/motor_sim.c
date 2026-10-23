/**
 * @file motor_sim.c
 *
 * The application logic the UI is a view of - a small model of a BLDC drive,
 * standing in for the LP-AM13E230 until the real control loop is wired up.
 *
 * Everything the UI needs arrives as a subject, and nothing in here touches a
 * widget. That is deliberate: it is the same boundary the sibling thermostat
 * draws, and it is what lets motor_sim_publish() be swapped for real sensor
 * reads without a single XML change.
 *
 * What is NOT here is as important as what is. Start/stop, the speed reference,
 * the interlock ticks, the direction choice and clearing the fault log are all
 * written straight from XML by subject_set_int_event / subject_increment_event /
 * bind_checked. This file only ever READS those, and writes what is measured.
 */

#include "motor_sim.h"

#include <stdio.h>

/*======================= ENUM MIRRORS =======================*/
/* Keep in sync with the ref_* consts in globals.xml. */

#define STATE_STOPPED  0
#define STATE_RUNNING  1
#define STATE_FAULT    2

/*======================= TUNING =======================*/

#define TICK_MS          100
/* Ramp in RPM per tick. subject_ramp is in units of 100 RPM/s, and a tick is a
 * tenth of a second, so one ramp unit moves 10 RPM per tick. */
#define RAMP_PER_UNIT    10
/* Free-running deceleration when stopped: a loaded shaft coasts down faster
 * than the commanded ramp brings it up. */
#define COAST_PER_TICK   45

#define TEMP_AMBIENT     31
#define TEMP_WARN        80
/* Tenths of a degree of stage heating per tick at full load, and the passive
 * cooling that opposes it. Integer tenths keep the model exact. */
#define HEAT_PER_TICK    3
#define COOL_PER_TICK    1

/*======================= STATIC PROTOTYPES =======================*/

static void tick_cb(lv_timer_t * timer);
static void format_milliamps(char * buf, size_t len, int32_t ma);
static int32_t approach(int32_t value, int32_t target, int32_t step);

/*======================= STATIC VARIABLES =======================*/

/* Display strings are owned here for the lifetime of the app: lv_subject_copy_string
 * copies into the subject's own buffer, but the subject was declared in XML with
 * a literal, so we always write through the API rather than aliasing. */
static char current_buf[16];
static char bus_buf[16];
static char runtime_buf[16];

/* Stage temperature in TENTHS of a degree, so a slow ramp is representable. The
 * subject carries whole degrees; this is the model's own finer state. */
static int32_t temp_tenths = TEMP_AMBIENT * 10;

/* Session accumulators for the trend screen. */
static int32_t run_ticks;
static int64_t load_sum;
static int32_t load_samples;

/*======================= GLOBAL FUNCTIONS =======================*/

void motor_sim_init(void)
{
    lv_timer_create(tick_cb, TICK_MS, NULL);
}

void motor_sim_publish(int32_t rpm, int32_t milliamps, int32_t temp_c, int32_t load_pct)
{
    lv_subject_set_int(&subject_rpm_act, rpm);
    lv_subject_set_int(&subject_temp, temp_c);
    lv_subject_set_int(&subject_load, load_pct);

    format_milliamps(current_buf, sizeof(current_buf), milliamps);
    lv_subject_copy_string(&subject_current_str, current_buf);

    if(rpm > lv_subject_get_int(&subject_peak_rpm)) {
        lv_subject_set_int(&subject_peak_rpm, rpm);
    }
}

void motor_sim_trip(void)
{
    lv_subject_set_int(&subject_fault_count, 2);
    lv_subject_set_int(&subject_fault_active, 1);
    lv_subject_set_int(&subject_state, STATE_FAULT);
}

/*======================= STATIC FUNCTIONS =======================*/

static void tick_cb(lv_timer_t * timer)
{
    LV_UNUSED(timer);

    const int32_t state = lv_subject_get_int(&subject_state);
    const int32_t ref   = lv_subject_get_int(&subject_rpm_ref);
    int32_t rpm         = lv_subject_get_int(&subject_rpm_act);

    /* ── Speed ────────────────────────────────────────────────────────────
     * Running chases the reference; anything else coasts to rest. Soft start
     * is what the ramp models - with it off the drive steps straight to the
     * commanded speed, which is exactly what that interlock means. */
    if(state == STATE_RUNNING) {
        if(lv_subject_get_int(&subject_cfg_softstart)) {
            int32_t step = lv_subject_get_int(&subject_ramp) * RAMP_PER_UNIT;
            if(step < RAMP_PER_UNIT) step = RAMP_PER_UNIT;
            rpm = approach(rpm, ref, step);
        }
        else {
            rpm = ref;
        }
        run_ticks++;
    }
    else {
        rpm = approach(rpm, 0, COAST_PER_TICK);
    }

    /* ── Load and current ─────────────────────────────────────────────────
     * Torque demand rises with speed and with the gap still being closed, so
     * accelerating draws more than holding the same speed - which is what makes
     * the two trend series diverge during a ramp instead of tracking. */
    const int32_t gap  = (ref > rpm) ? (ref - rpm) : 0;
    int32_t load = (rpm * 34) / 3600 + (gap * 60) / 3600;
    if(rpm == 0) load = 0;
    if(load > 100) load = 100;

    /* ~0.9 A at rated load, plus a small no-load draw whenever it is turning. */
    int32_t milliamps = (load * 900) / 100 + (rpm > 0 ? 180 : 0);

    /* ── Thermals ─────────────────────────────────────────────────────────
     * Heating is proportional to load; cooling is constant and always applies,
     * so the stage settles at an equilibrium rather than climbing forever. */
    temp_tenths += (load * HEAT_PER_TICK) / 100 - COOL_PER_TICK;
    if(temp_tenths < TEMP_AMBIENT * 10) temp_tenths = TEMP_AMBIENT * 10;

    /* Thermal derate is an interlock the operator can switch off, so the model
     * has to honour it - an interlock the simulation ignores is a lie in the UI. */
    if(lv_subject_get_int(&subject_cfg_derate) && temp_tenths > TEMP_WARN * 10) {
        milliamps = (milliamps * 70) / 100;
        temp_tenths -= 2;
    }

    const int32_t temp_c = temp_tenths / 10;

    motor_sim_publish(rpm, milliamps, temp_c, load);

    /* ── Bus voltage ──────────────────────────────────────────────────────
     * Sags slightly under load; this is what the F311 fault in the log means. */
    const int32_t bus_mv = 24000 - load * 18;
    format_milliamps(bus_buf, sizeof(bus_buf), bus_mv);
    lv_subject_copy_string(&subject_bus_str, bus_buf);

    /* ── Session summary for the trend screen ─────────────────────────────── */
    if(state == STATE_RUNNING) {
        load_sum += load;
        load_samples++;
        if(load_samples > 0) {
            lv_subject_set_int(&subject_avg_load, (int32_t)(load_sum / load_samples));
        }

        const int32_t secs = run_ticks / (1000 / TICK_MS);
        lv_snprintf(runtime_buf, sizeof(runtime_buf), "%02d:%02d",
                    (int)(secs / 60), (int)(secs % 60));
        lv_subject_copy_string(&subject_runtime_str, runtime_buf);
    }
}

/** Moves `value` toward `target` by at most `step`, without overshooting. */
static int32_t approach(int32_t value, int32_t target, int32_t step)
{
    if(value < target) {
        value += step;
        if(value > target) value = target;
    }
    else if(value > target) {
        value -= step;
        if(value < target) value = target;
    }
    return value;
}

/**
 * Renders thousandths as a one-decimal string: 2400 -> "2.4".
 *
 * This exists because bind_text-fmt has no fixed-point conversion - it forwards
 * to lv_label_set_text_fmt, which on an embedded build has no float support at
 * all. Every decimal in this UI is a pre-formatted string subject for that
 * reason, not because the value is genuinely textual.
 */
static void format_milliamps(char * buf, size_t len, int32_t ma)
{
    if(ma < 0) ma = 0;
    lv_snprintf(buf, len, "%d.%d", (int)(ma / 1000), (int)((ma % 1000) / 100));
}
