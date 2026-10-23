/**
 * @file button_circle_gen.h
 */

#ifndef LVGL_PRO_BUTTON_CIRCLE_GEN_H
#define LVGL_PRO_BUTTON_CIRCLE_GEN_H

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

lv_obj_t * button_circle_create(lv_obj_t * parent, const void * icon, lv_color_t bg_color, lv_opa_t bg_opa, lv_color_t color, int32_t size);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_BUTTON_CIRCLE_GEN_H*/