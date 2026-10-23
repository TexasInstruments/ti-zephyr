/**
 * @file header_action_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "header_action_gen.h"
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

lv_obj_t * header_action_create(lv_obj_t * parent, const void * icon, int32_t index)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "header_action_#");
        lv_obj_set_width(lv_button_0, CONST_HEADER_ACTION);
        lv_obj_set_height(lv_button_0, CONST_HEADER_ACTION);

        lv_obj_add_style(lv_button_0, &style_btn_flat, 0);
        lv_obj_add_style(lv_button_0, &style_btn_ghost_pressed, LV_STATE_PRESSED);
        lv_obj_bind_flag_if_not_eq(lv_button_0, &subject_tab, LV_OBJ_FLAG_CLICKABLE, index);
        lv_obj_t * row_0 = row_create(lv_button_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_flag(row_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_width(row_0, lv_pct(100));
        lv_obj_set_height(row_0, lv_pct(100));
        lv_obj_t * lv_image_0 = lv_image_create(row_0);
        lv_image_set_src(lv_image_0, icon);
        lv_obj_set_width(lv_image_0, 21);
        lv_obj_set_height(lv_image_0, 21);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_tertiary, 0);
        lv_obj_bind_style(lv_image_0, &style_icon_accent, 0, &subject_tab, index);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

