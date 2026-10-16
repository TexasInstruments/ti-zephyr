/**
 * @file wd_schedule_chart_private_gen.h
 *
 */

#ifndef LVGL_PRO_WD_SCHEDULE_CHART_PRIVATE_GEN_H
#define LVGL_PRO_WD_SCHEDULE_CHART_PRIVATE_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl_private.h"
#endif
#include "wd_schedule_chart.h"

/*********************
 *      DEFINES
 *********************/

#ifndef WD_SCHEDULE_CHART_USER_DATA
#define WD_SCHEDULE_CHART_USER_DATA
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_chart_t obj;  /* Base widget to extend */
    lv_subject_t * bind_day;
    lv_chart_series_t * lv_chart_series_0;
    void * lv_chart_axis_0;
    WD_SCHEDULE_CHART_USER_DATA
} wd_schedule_chart_t;

extern const lv_obj_class_t wd_schedule_chart_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if defined(LV_USE_XML) && LV_USE_XML
    void wd_schedule_chart_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_WD_SCHEDULE_CHART_PRIVATE_GEN_H*/