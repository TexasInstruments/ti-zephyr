/**
 * @file btn_run_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "btn_run_gen.h"
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

lv_obj_t * btn_run_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * column_0 = column_create(parent, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(column_0, "btn_run_#");
        lv_obj_set_width(column_0, CONST_CONTENT_W);
        lv_obj_set_height(column_0, LV_SIZE_CONTENT);

        lv_obj_t * lv_button_0 = lv_button_create(column_0);
        lv_obj_set_width(lv_button_0, lv_pct(100));
        lv_obj_set_height(lv_button_0, CONST_TOUCH_LG);
        lv_obj_add_style(lv_button_0, &style_btn_base, 0);
        lv_obj_add_style(lv_button_0, &style_btn_accent, 0);
        lv_obj_add_style(lv_button_0, &style_btn_accent_pressed, LV_STATE_PRESSED);
        lv_obj_bind_flag_if_not_eq(lv_button_0, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_STOPPED);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_state, LV_EVENT_CLICKED, REF_STATE_RUNNING);
        lv_obj_t * label_0 = label_create(lv_button_0);
        lv_label_set_text(label_0, "Start Motor");
        lv_obj_set_align(label_0, LV_ALIGN_CENTER);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_title, 0);
        lv_obj_add_style(label_0, &style_ink_on_accent, 0);

        lv_obj_t * lv_button_1 = lv_button_create(column_0);
        lv_obj_set_width(lv_button_1, lv_pct(100));
        lv_obj_set_height(lv_button_1, CONST_TOUCH_LG);
        lv_obj_add_style(lv_button_1, &style_btn_base, 0);
        lv_obj_add_style(lv_button_1, &style_btn_danger, 0);
        lv_obj_add_style(lv_button_1, &style_btn_danger_pressed, LV_STATE_PRESSED);
        lv_obj_bind_flag_if_not_eq(lv_button_1, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_RUNNING);
        lv_obj_add_subject_set_int_event(lv_button_1, &subject_state, LV_EVENT_CLICKED, REF_STATE_STOPPED);
        lv_obj_t * label_1 = label_create(lv_button_1);
        lv_label_set_text(label_1, "Stop Motor");
        lv_obj_set_align(label_1, LV_ALIGN_CENTER);
        lv_obj_set_flag(label_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_1, &style_title, 0);
        lv_obj_add_style(label_1, &style_ink, 0);

        lv_obj_t * lv_button_2 = lv_button_create(column_0);
        lv_obj_set_width(lv_button_2, lv_pct(100));
        lv_obj_set_height(lv_button_2, CONST_TOUCH_LG);
        lv_obj_set_flag(lv_button_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_button_2, &style_btn_base, 0);
        lv_obj_add_style(lv_button_2, &style_btn_disabled, 0);
        lv_obj_bind_flag_if_not_eq(lv_button_2, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_FAULT);
        lv_obj_t * label_2 = label_create(lv_button_2);
        lv_label_set_text(label_2, "Clear faults to start");
        lv_obj_set_align(label_2, LV_ALIGN_CENTER);
        lv_obj_set_flag(label_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_2, &style_title, 0);
        lv_obj_add_style(label_2, &style_ink_tertiary, 0);

        the_root = column_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

