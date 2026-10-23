/**
 * @file header_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "header_gen.h"
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

lv_obj_t * header_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, CONST_GUTTER, CONST_SPACE_8, 0, 0, CONST_SPACE_12, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "header_#");
        lv_obj_set_width(row_0, CONST_SCREEN_W);
        lv_obj_set_height(row_0, CONST_HEADER_H);

        lv_obj_t * logostrip = lv_image_create(row_0);
        lv_obj_set_name(logostrip, "logostrip");
        lv_image_set_src(logostrip, image_logostrip);
        lv_obj_set_width(logostrip, 75);
        lv_obj_set_height(logostrip, 20);
        lv_obj_set_flag(logostrip, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * status_led = container_create(row_0);
        lv_obj_set_name(status_led, "status_led");
        lv_obj_set_width(status_led, 10);
        lv_obj_set_height(status_led, 10);
        lv_obj_set_style_radius(status_led, CONST_RADIUS_PILL, 0);
        lv_obj_add_style(status_led, &style_dot_neutral, 0);
        lv_obj_bind_style(status_led, &style_dot_accent, 0, &subject_state, REF_STATE_RUNNING);
        lv_obj_bind_style(status_led, &style_dot_danger, 0, &subject_state, REF_STATE_FAULT);

        lv_obj_t * spacer = container_create(row_0);
        lv_obj_set_name(spacer, "spacer");
        lv_obj_set_flex_grow(spacer, 1);
        lv_obj_set_height(spacer, 1);

        lv_obj_t * header_action_0 = header_action_create(row_0, icon_tab_trend, 2);
        lv_obj_add_screen_create_event(header_action_0, LV_EVENT_CLICKED, trend_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        lv_obj_t * header_action_1 = header_action_create(row_0, icon_tab_config, 3);
        lv_obj_add_screen_create_event(header_action_1, LV_EVENT_CLICKED, config_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

