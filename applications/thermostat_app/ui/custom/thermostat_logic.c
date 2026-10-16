/**
 * @file thermostat_logic.c
 *
 * See thermostat_logic.h for why this module exists at all.
 */

/*********************
 *      INCLUDES
 *********************/

#include "thermostat_logic.h"

#include "../ti_thermostat.h"

/*********************
 *      DEFINES
 *********************/

/* Mirrors of the ref_* int consts in globals.xml. XML cannot share enum members
 * with C, so the vocabulary is duplicated by hand on both sides — this is the
 * corpus-standard arrangement and the one place drift will bite, so keep the
 * two lists adjacent in review. */
#define REF_MODE_HEAT      0
#define REF_MODE_COOL      1
#define REF_MODE_AUTO      2
#define REF_MODE_OFF       3

#define REF_HVAC_IDLE      0
#define REF_HVAC_HEATING   1
#define REF_HVAC_COOLING   2

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void climate_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
static void recompute(void);

/**********************
 *  STATIC VARIABLES
 **********************/

/* One group over every input that can change the answer. LVGL calls the
 * observer once per member change; the callback recomputes rather than trying
 * to work out which input moved, because the output is cheap and partial
 * updates are how these modules rot. */
static lv_subject_t   climate_group;
static lv_subject_t * climate_inputs[3];

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void thermostat_logic_init(void)
{
    climate_inputs[0] = &subject_mode;
    climate_inputs[1] = &subject_target_temp;
    climate_inputs[2] = &subject_now_temp;

    lv_subject_init_group(&climate_group, climate_inputs, 3);
    lv_subject_add_observer(&climate_group, climate_observer_cb, NULL);

    /* Group observers do not fire on registration, so seed the derived state
     * once — otherwise the first frame shows whatever the XML default was. */
    recompute();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void climate_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(observer);
    LV_UNUSED(subject);
    recompute();
}

static void recompute(void)
{
    const int32_t mode   = lv_subject_get_int(&subject_mode);
    const int32_t target = lv_subject_get_int(&subject_target_temp);
    const float   now    = lv_subject_get_float(&subject_now_temp);

    /* This function is the ONLY writer of subject_hvac_state. It used to share
     * the job with the mode pills, and two writers on one subject is a race
     * decided by child order — the pill's literal landed after the observer's
     * comparison and Auto was permanently stuck on whatever the XML hardcoded.
     * One writer, no ordering to reason about.
     *
     * Heat and Cool are explicit instructions and are mirrored straight through:
     * asking for heat shows heat. Deliberately NOT gated on whether there is
     * demand — an earlier version was, and pressing Cool in a room already below
     * its setpoint produced a grey ring, which reads as a broken button rather
     * than as "nothing to do".
     *
     * Auto is the only mode with a decision to make, and it needs no threshold
     * of its own: the measured temperature IS the threshold. Ask for more than
     * the room has and it heats; ask for less and it cools. That comparison —
     * two subjects against each other, rather than one against a literal — is
     * the single thing the XML comparators cannot express, and now the only
     * reason this file exists.
     *
     * Ties go to heating, which keeps IDLE meaning exactly one thing: off. The
     * Home status chips depend on that, since their three states are keyed on
     * hvac_state and must be exhaustive. */
    int32_t hvac;

    switch(mode) {
        case REF_MODE_HEAT:
            hvac = REF_HVAC_HEATING;
            break;

        case REF_MODE_COOL:
            hvac = REF_HVAC_COOLING;
            break;

        case REF_MODE_AUTO:
            hvac = ((float)target >= now) ? REF_HVAC_HEATING : REF_HVAC_COOLING;
            break;

        case REF_MODE_OFF:
        default:
            hvac = REF_HVAC_IDLE;
            break;
    }

    /* Safe from recursion: the observer group covers the inputs read above,
     * not subject_hvac_state. */
    lv_subject_set_int(&subject_hvac_state, hvac);
}
