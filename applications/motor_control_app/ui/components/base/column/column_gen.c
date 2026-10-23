/**
 * @file column_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "column_gen.h"
#include "../../../motor_control.h"

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

lv_obj_t * column_create(lv_obj_t * parent, int32_t pad_left, int32_t pad_right, int32_t pad_top, int32_t pad_bottom, int32_t gap, int32_t grow, lv_flex_align_t main_align, lv_flex_align_t cross_align)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_COLUMN);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * container_0 = container_create(parent);
        lv_obj_set_name_static(container_0, "column_#");
        lv_obj_set_style_pad_left(container_0, pad_left, 0);
        lv_obj_set_style_pad_right(container_0, pad_right, 0);
        lv_obj_set_style_pad_top(container_0, pad_top, 0);
        lv_obj_set_style_pad_bottom(container_0, pad_bottom, 0);
        lv_obj_set_style_pad_row(container_0, gap, 0);
        lv_obj_set_flex_grow(container_0, grow);
        lv_obj_set_style_flex_main_place(container_0, main_align, 0);
        lv_obj_set_style_flex_cross_place(container_0, cross_align, 0);
        lv_obj_set_style_flex_track_place(container_0, cross_align, 0);

        lv_obj_add_style(container_0, &style_base, 0);

        the_root = container_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

