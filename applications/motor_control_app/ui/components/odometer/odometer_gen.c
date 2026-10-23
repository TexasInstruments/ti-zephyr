/**
 * @file odometer_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "odometer_gen.h"
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

lv_obj_t * odometer_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END);
        lv_obj_set_name_static(row_0, "odometer_#");
        lv_obj_set_width(row_0, LV_SIZE_CONTENT);
        lv_obj_set_height(row_0, LV_SIZE_CONTENT);
        lv_obj_set_flag(row_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * label_0 = label_create(row_0);
        lv_label_bind_text(label_0, &subject_rpm_act, "%04d");
        lv_label_set_text(label_0, "0000");
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_hero, 0);
        lv_obj_bind_style(label_0, &style_hero_dim, 0, &subject_state, REF_STATE_STOPPED);
        lv_obj_bind_style(label_0, &style_hero_fault, 0, &subject_state, REF_STATE_FAULT);

        lv_obj_t * label_1 = label_create(row_0);
        lv_label_set_text(label_1, "RPM");
        lv_obj_set_style_pad_left(label_1, 8, 0);
        lv_obj_set_style_pad_bottom(label_1, 12, 0);
        lv_obj_set_flag(label_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_1, &style_hero_unit, 0);
        lv_obj_add_style(label_1, &style_ink_tertiary, 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

