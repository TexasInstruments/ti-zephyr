/**
 * @file date_gen.h
 */

#ifndef LVGL_PRO_DATE_GEN_H
#define LVGL_PRO_DATE_GEN_H

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

lv_obj_t * date_create(lv_obj_t * parent, lv_subject_t * bind_subject_day_name, lv_subject_t * bind_subject_day_index, lv_subject_t * bind_subject_year, lv_subject_t * bind_subject_month_name, lv_font_t * font);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_DATE_GEN_H*/