/**
 * @file chip_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "chip_gen.h"
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

lv_obj_t * chip_create(lv_obj_t * parent, const char * text, lv_style_t * tone, lv_style_t * tone_dot, lv_style_t * tone_label)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 12, 14, 7, 7, CONST_SPACE_8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "chip_#");
        lv_obj_set_height(row_0, LV_SIZE_CONTENT);
        lv_obj_set_width(row_0, LV_SIZE_CONTENT);

        lv_obj_add_style(row_0, &style_chip_base, 0);
        lv_obj_add_style(row_0, tone, 0);
        lv_obj_t * dot = container_create(row_0);
        lv_obj_set_name(dot, "dot");
        lv_obj_set_width(dot, 7);
        lv_obj_set_height(dot, 7);
        lv_obj_set_style_radius(dot, CONST_RADIUS_PILL, 0);
        lv_obj_add_style(dot, &style_dot_neutral, 0);
        lv_obj_add_style(dot, tone_dot, 0);

        lv_obj_t * label_0 = label_create(row_0);
        lv_label_set_text(label_0, text);
        lv_obj_add_style(label_0, &style_chip, 0);
        lv_obj_add_style(label_0, &style_ink_secondary, 0);
        lv_obj_add_style(label_0, tone_label, 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

