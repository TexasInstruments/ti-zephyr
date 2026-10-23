/**
 * @file check_row_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "check_row_gen.h"
#include "../../motor_control.h"

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

lv_obj_t * check_row_create(lv_obj_t * parent, const char * text, const char * meta, lv_subject_t * value)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * column_0 = column_create(parent, 0, 0, 0, 0, CONST_SPACE_2, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(column_0, "check_row_#");
        lv_obj_set_width(column_0, lv_pct(100));
        lv_obj_set_height(column_0, LV_SIZE_CONTENT);

        lv_obj_t * lv_checkbox_0 = lv_checkbox_create(column_0);
        lv_checkbox_set_text(lv_checkbox_0, text);
        lv_obj_bind_checked(lv_checkbox_0, value);
        lv_obj_set_style_pad_top(lv_checkbox_0, 8, 0);
        lv_obj_set_style_pad_bottom(lv_checkbox_0, 2, 0);
        lv_obj_set_width(lv_checkbox_0, lv_pct(100));
        lv_obj_add_style(lv_checkbox_0, &style_checkbox, 0);
        lv_obj_add_style(lv_checkbox_0, &style_checkbox_ind, LV_PART_INDICATOR);
        lv_obj_add_style(lv_checkbox_0, &style_checkbox_ind_checked, LV_PART_INDICATOR | LV_STATE_CHECKED);

        lv_obj_t * label_0 = label_create(column_0);
        lv_label_set_text(label_0, meta);
        lv_obj_set_style_pad_left(label_0, 36, 0);
        lv_obj_set_style_pad_bottom(label_0, 8, 0);
        lv_obj_add_style(label_0, &style_meta, 0);
        lv_obj_add_style(label_0, &style_ink_tertiary, 0);

        the_root = column_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

