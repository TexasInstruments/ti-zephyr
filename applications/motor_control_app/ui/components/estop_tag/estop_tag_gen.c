/**
 * @file estop_tag_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "estop_tag_gen.h"
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

lv_obj_t * estop_tag_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_estop;
    static lv_style_t style_estop_pressed;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_estop);
        lv_style_init(&style_estop_pressed);

        lv_style_set_bg_color(&style_estop, CONST_DANGER);
        lv_style_set_bg_opa(&style_estop, (255 * 100 / 100));
        lv_style_set_radius(&style_estop, CONST_RADIUS_PILL);
        lv_style_set_border_width(&style_estop, 0);
        lv_style_set_shadow_width(&style_estop, 0);
        lv_style_set_shadow_opa(&style_estop, 0);
        lv_style_set_bg_color(&style_estop_pressed, CONST_DANGER_PRESSED);
        lv_style_set_bg_opa(&style_estop_pressed, (255 * 100 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 24, 14, 0, 0, CONST_SPACE_8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "estop_tag_#");
        lv_obj_set_width(row_0, LV_SIZE_CONTENT);
        lv_obj_set_height(row_0, 28);
        lv_obj_set_style_translate_x(row_0, -14, 0);

        lv_obj_add_style(row_0, &style_estop, 0);
        lv_obj_add_style(row_0, &style_estop_pressed, LV_STATE_PRESSED);
        lv_obj_add_subject_set_int_event(row_0, &subject_state, LV_EVENT_CLICKED, REF_STATE_STOPPED);
        lv_obj_t * lv_image_0 = lv_image_create(row_0);
        lv_image_set_src(lv_image_0, icon_estop);
        lv_obj_set_width(lv_image_0, 16);
        lv_obj_set_height(lv_image_0, 16);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_on_danger_solid, 0);

        lv_obj_t * label_0 = label_create(row_0);
        lv_label_set_text(label_0, "E-STOP");
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_overline, 0);
        lv_obj_add_style(label_0, &style_ink_on_danger_solid, 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

