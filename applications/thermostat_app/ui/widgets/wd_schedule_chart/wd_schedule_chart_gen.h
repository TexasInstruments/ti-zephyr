/**
 * @file wd_schedule_chart_gen.h
 *
 */

#ifndef LVGL_PRO_WD_SCHEDULE_CHART_GEN_H
#define LVGL_PRO_WD_SCHEDULE_CHART_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif

#include "../../ti_thermostat_gen.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a wd_schedule_chart object
 * @param parent pointer to an object, it will be the parent of the new wd_schedule_chart
 * @return pointer to the created wd_schedule_chart
 */
lv_obj_t * wd_schedule_chart_create(lv_obj_t * parent);
/**
 * Int subject holding the selected day, 0=Mon. The curve redraws                 whenever it changes.
 * @param obj   pointer to a wd_schedule_chart
 * @param bind_day  Int subject holding the selected day, 0=Mon. The curve redraws                 whenever it changes.
 */
void wd_schedule_chart_bind_day(lv_obj_t * wd_schedule_chart, lv_subject_t * bind_day);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_WD_SCHEDULE_CHART_GEN_H*/