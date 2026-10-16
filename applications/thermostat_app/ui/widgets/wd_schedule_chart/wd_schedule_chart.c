/**
 * @file wd_schedule_chart.c
 *
 * Turns a day index into 24 setpoints and pushes them into the chart.
 *
 * This is the whole reason the schedule curve is a widget and not a component.
 * `lv_chart` has no bind_* in its XML surface — `values=` is a creation-time
 * int array — and subjects are only int and string, so there is no array
 * subject to bind to even if the attribute existed. Feeding a chart from a
 * subject is the canonical "a subject whose value is not directly a widget
 * property" case, and that is a C job.
 *
 * Layout and styling stay in wd_schedule_chart.xml. The only thing here is the
 * data and the observer that applies it.
 */

/*********************
 *      INCLUDES
 *********************/

#include "wd_schedule_chart_private_gen.h"

/*********************
 *      DEFINES
 *********************/

#define DAYS   7
#define HOURS  24

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void day_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
static void apply_day(lv_obj_t * obj, int32_t day);

/**********************
 *  STATIC VARIABLES
 **********************/

/* Hardcoded demo schedule: degrees C per hour, 0=Mon .. 6=Sun.
 *
 * This table is the single place the application layer will later hand real
 * data in — swap it for a setter that copies into a RAM buffer and calls
 * apply_day() again, and nothing above this file changes.
 *
 * Every day has its own shape, and each one is a story you can read off the
 * curve — that is what makes flicking through the chips worth doing. Seven
 * identical weekdays would have been decoration.
 *
 *   Mon  the baseline week: warm 06-08, away all day, warm 17-21
 *   Tue  out of the house earlier and back later — a longer cold trough
 *   Wed  worked from home, so the trough never happens
 *   Thu  gym after work: a notch cut out of the evening block
 *   Fri  early finish, warm from mid-afternoon and a degree warmer all evening
 *   Sat  late start, warm right through, latest night of the week
 *   Sun  late start, then a long slow wind-down into the evening
 *
 * Named day_setpoints, not `schedule`: the generator emits an lv_obj_t * called
 * `schedule` for screens/schedule.xml, and a file-static of the same name is a
 * redefinition error at link time.
 *
 *                   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 */
static const int8_t day_setpoints[DAYS][HOURS] = {
    /* Mon */      { 17,17,17,17,17,17,21,21,21,17,17,17,17,17,17,17,17,21,21,21,21,21,18,17 },
    /* Tue */      { 17,17,17,17,17,16,20,21,17,17,17,17,17,17,17,17,17,17,21,21,21,21,19,17 },
    /* Wed */      { 17,17,17,17,17,17,20,21,21,21,21,21,21,21,21,21,21,21,21,21,21,20,18,17 },
    /* Thu */      { 17,17,17,17,17,17,21,21,21,17,17,17,17,17,17,17,17,21,17,17,20,21,19,17 },
    /* Fri */      { 17,17,17,17,17,17,21,21,21,17,17,17,17,17,17,21,21,22,22,22,22,22,21,19 },
    /* Sat */      { 18,18,18,18,18,18,18,18,19,21,21,22,22,22,22,22,22,22,22,22,22,21,20,19 },
    /* Sun */      { 18,18,18,18,18,18,18,18,20,21,21,21,21,21,21,21,21,21,20,20,19,19,18,18 },
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/* Named by the <api> prop: `bind_day` -> wd_schedule_chart_bind_day().
 * The generator declares it and leaves the body to us. */
void wd_schedule_chart_bind_day(lv_obj_t * obj, lv_subject_t * bind_day)
{
    wd_schedule_chart_t * widget = (wd_schedule_chart_t *)obj;

    widget->bind_day = bind_day;

    /* _obj variant ties the observer's lifetime to this widget, so it is torn
     * down with the screen instead of firing into freed memory. */
    lv_subject_add_observer_obj(bind_day, day_observer_cb, obj, NULL);

    apply_day(obj, lv_subject_get_int(bind_day));
}

void wd_schedule_chart_constructor_hook(lv_obj_t * obj)
{
    LV_UNUSED(obj);
}

void wd_schedule_chart_destructor_hook(lv_obj_t * obj)
{
    LV_UNUSED(obj);
}

void wd_schedule_chart_event_hook(lv_event_t * e)
{
    LV_UNUSED(e);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void day_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    apply_day(obj, lv_subject_get_int(subject));
}

static void apply_day(lv_obj_t * obj, int32_t day)
{
    wd_schedule_chart_t * widget = (wd_schedule_chart_t *)obj;
    lv_chart_series_t * series = widget->lv_chart_series_0;

    if(series == NULL) return;

    /* Clamp rather than trust: the subject is writable from XML and a stray
     * index here would read off the end of the table. */
    if(day < 0) day = 0;
    if(day >= DAYS) day = DAYS - 1;

    for(uint32_t hour = 0; hour < HOURS; hour++) {
        lv_chart_set_value_by_id(obj, series, hour, day_setpoints[day][hour]);
    }

    lv_chart_refresh(obj);
}
