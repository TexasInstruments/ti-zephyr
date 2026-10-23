/**
 * @file btn_step_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "btn_step_gen.h"
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

lv_obj_t * btn_step_create(lv_obj_t * parent, const void * icon)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "btn_step_#");
        lv_obj_set_width(lv_button_0, 64);
        lv_obj_set_height(lv_button_0, 64);

        lv_obj_add_style(lv_button_0, &style_btn_base, 0);
        lv_obj_add_style(lv_button_0, &style_btn_ghost, 0);
        lv_obj_add_style(lv_button_0, &style_btn_ghost_pressed, LV_STATE_PRESSED);
        lv_obj_t * row_0 = row_create(lv_button_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_flag(row_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_width(row_0, lv_pct(100));
        lv_obj_set_height(row_0, lv_pct(100));
        lv_obj_t * lv_image_0 = lv_image_create(row_0);
        lv_image_set_src(lv_image_0, icon);
        lv_obj_set_width(lv_image_0, 28);
        lv_obj_set_height(lv_image_0, 28);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_ink, 0);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

