/**
 * @file value_setter_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "value_setter_gen.h"
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

lv_obj_t * value_setter_create(lv_obj_t * parent, lv_subject_t * subject, const char * format)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_main;
    static lv_style_t style_label;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_main);
        lv_style_init(&style_label);

        lv_style_set_width(&style_main, LV_SIZE_CONTENT);
        lv_style_set_text_font(&style_label, font_lg);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, BASE_LIB_UNIT_SM, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(row_0, "value_setter_#");

        lv_obj_add_style(row_0, &style_main, 0);

        lv_obj_t * lv_label_0 = lv_label_create(row_0);
        lv_label_bind_text(lv_label_0, subject, format);
        lv_obj_add_style(lv_label_0, &style_label, 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

