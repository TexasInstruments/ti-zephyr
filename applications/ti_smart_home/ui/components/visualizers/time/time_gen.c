/**
 * @file time_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "time_gen.h"
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

lv_obj_t * time_create(lv_obj_t * parent, lv_subject_t * bind_subject_hour, lv_subject_t * bind_subject_minute, lv_font_t * font)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "time_#");
        lv_obj_set_style_text_font(row_0, font, 0);

        lv_obj_t * lv_label_0 = lv_label_create(row_0);
        lv_label_bind_text(lv_label_0, bind_subject_hour, "%d");
        lv_obj_remove_style_all(lv_label_0);

        lv_obj_t * lv_label_1 = lv_label_create(row_0);
        lv_label_bind_text(lv_label_1, bind_subject_minute, ":%02d");
        lv_obj_remove_style_all(lv_label_1);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

