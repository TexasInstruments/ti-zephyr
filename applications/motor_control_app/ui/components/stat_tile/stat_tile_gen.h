/**
 * @file stat_tile_gen.h
 */

#ifndef LVGL_PRO_STAT_TILE_GEN_H
#define LVGL_PRO_STAT_TILE_GEN_H

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

lv_obj_t * stat_tile_create(lv_obj_t * parent, const char * caption, lv_subject_t * value, const char * fmt, const char * placeholder, const char * unit, int32_t col_w);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_STAT_TILE_GEN_H*/