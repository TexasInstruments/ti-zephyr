/**
 * @file chart_gen.h
 */

#ifndef LVGL_PRO_CHART_GEN_H
#define LVGL_PRO_CHART_GEN_H

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

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t * chart_create(lv_obj_t * parent, lv_chart_type_t chart_type, lv_color_t color, int32_t min_value, int32_t max_value, int32_t tick_count, int32_t point_count);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_CHART_GEN_H*/