/**
 * @file value_small_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "value_small_gen.h"
#include "../../../ti_smart_home.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * value_small_create(lv_obj_t * parent, lv_subject_t * value, const char * format, const char * unit)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_value;
    static lv_style_t style_unit;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_value);
        lv_style_init(&style_unit);

        lv_style_set_width(&style_base, LV_SIZE_CONTENT);
        lv_style_set_height(&style_base, LV_SIZE_CONTENT);
        lv_style_set_text_font(&style_value, font_md);
        lv_style_set_text_font(&style_unit, font_xs);
        lv_style_set_text_opa(&style_unit, (255 * 50 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "value_small_#");

        lv_obj_remove_style_all(lv_obj_0);
        lv_obj_add_style(lv_obj_0, &style_base, 0);
        lv_obj_t * lv_spangroup_0 = lv_spangroup_create(lv_obj_0);
        lv_span_t * lv_spangroup_span_0 = lv_spangroup_add_span(lv_spangroup_0);
        lv_spangroup_bind_span_text(lv_spangroup_0, lv_spangroup_span_0, value, format);
        lv_spangroup_set_span_style(lv_spangroup_0, lv_spangroup_span_0, &style_value);
        lv_span_t * lv_spangroup_span_1 = lv_spangroup_add_span(lv_spangroup_0);
        lv_spangroup_set_span_text(lv_spangroup_0, lv_spangroup_span_1, unit);
        lv_spangroup_set_span_style(lv_spangroup_0, lv_spangroup_span_1, &style_unit);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

