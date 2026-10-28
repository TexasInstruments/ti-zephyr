#include "wash_logic.h"

#include "../ti_washing_machine.h"

static const int32_t wm_program_minutes[] = {
    [WASH_PROG_COTTON] = 135,   /* 2h 15m */
    [WASH_PROG_ECO]    = 185,   /* 3h 05m */
    [WASH_PROG_SYNTH]  = 100,   /* 1h 40m */
    [WASH_PROG_QUICK]  = 30,
};
#define WM_PROGRAM_COUNT ((int32_t)(sizeof(wm_program_minutes) / sizeof(wm_program_minutes[0])))

static const char * const wm_program_names[WM_PROGRAM_COUNT] = {
    [WASH_PROG_COTTON] = "Cotton",
    [WASH_PROG_ECO]    = "Eco 40",
    [WASH_PROG_SYNTH]  = "Synthetics",
    [WASH_PROG_QUICK]  = "Quick 30",
};

static const int32_t wm_temp_degrees[] = { 0, 20, 40, 60, 90 };
#define WM_TEMP_COUNT ((int32_t)(sizeof(wm_temp_degrees) / sizeof(wm_temp_degrees[0])))

#define WM_TEMP_MIN_SELECTED  20

#define WM_END_PREWASH   12
#define WM_END_WASH      62
#define WM_END_RINSE     84
/* everything past WM_END_RINSE is the spin */

static lv_observer_t * wm_observer;

static void wm_recompute(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(observer);
    LV_UNUSED(subject);

    int32_t progress = lv_subject_get_int(&subject_progress);
    int32_t program  = lv_subject_get_int(&subject_program);

    const bool chosen = (program >= 0 && program < WM_PROGRAM_COUNT);
    const int32_t timing_program = chosen ? program : WASH_PROG_COTTON;

    int32_t temp_idx = lv_subject_get_int(&subject_temp_idx);
    if(temp_idx < 0)               temp_idx = 0;
    if(temp_idx >= WM_TEMP_COUNT)  temp_idx = WM_TEMP_COUNT - 1;
    lv_subject_set_int(&subject_temp_c, wm_temp_degrees[temp_idx]);

    /* ── the phase ──────────────────────────────────────────────────────── */
    int32_t phase;
    if(progress < WM_END_PREWASH)   phase = WASH_PHASE_PREWASH;
    else if(progress < WM_END_WASH) phase = WASH_PHASE_WASH;
    else if(progress < WM_END_RINSE) phase = WASH_PHASE_RINSE;
    else                            phase = WASH_PHASE_SPIN;
    lv_subject_set_int(&subject_phase, phase);

    /* ── the remaining time ─────────────────────────────────────────────── */
    int32_t total = wm_program_minutes[timing_program];
    int32_t left  = (total * (100 - progress)) / 100;

    if(left == 0 && progress < 100) left = 1;

    char buf[8];
    if(left >= 60) {
        lv_snprintf(buf, sizeof(buf), "%d:%02d", (int)(left / 60), (int)(left % 60));
        lv_subject_copy_string(&subject_time_unit_str, "h");
    }
    else {
        lv_snprintf(buf, sizeof(buf), "%d", (int)left);
        lv_subject_copy_string(&subject_time_unit_str, "min");
    }
    lv_subject_copy_string(&subject_time_left_str, buf);

    lv_subject_copy_string(&subject_program_name,
                           chosen ? wm_program_names[program] : "");

    char dur[10];
    if(total >= 60) lv_snprintf(dur, sizeof(dur), "%dh %02dm", (int)(total / 60), (int)(total % 60));
    else            lv_snprintf(dur, sizeof(dur), "%d min", (int)total);
    lv_subject_copy_string(&subject_duration_str, dur);

    const int32_t temp = lv_subject_get_int(&subject_temp_c);
    const int32_t door = lv_subject_get_int(&subject_door);

    if(!chosen) {
        lv_subject_copy_string(&subject_hint_str, "Select a fabric");
        lv_subject_set_int(&subject_ready, 0);
    }
    else if(temp < WM_TEMP_MIN_SELECTED) {
        lv_subject_copy_string(&subject_hint_str, "Select a temperature");
        lv_subject_set_int(&subject_ready, 0);
    }
    else if(door != WASH_DOOR_LOCKED) {
        lv_subject_copy_string(&subject_hint_str, "Close the door");
        lv_subject_set_int(&subject_ready, 0);
    }
    else {
        lv_subject_copy_string(&subject_hint_str, "");
        lv_subject_set_int(&subject_ready, 1);
    }
}

void wash_logic_init(void)
{
    static lv_subject_t * inputs[4];
    static lv_subject_t group;

    inputs[0] = &subject_progress;
    inputs[1] = &subject_program;
    inputs[2] = &subject_temp_idx;
    inputs[3] = &subject_door;
    lv_subject_init_group(&group, inputs, 4);

    wm_observer = lv_subject_add_observer(&group, wm_recompute, NULL);

    wm_recompute(NULL, NULL);
}
