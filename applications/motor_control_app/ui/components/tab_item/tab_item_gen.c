/**
 * @file tab_item_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "tab_item_gen.h"
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

lv_obj_t * tab_item_create(lv_obj_t * parent, const void * icon, const char * text, int32_t index, lv_subject_t * alert)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * column_0 = column_create(parent, 0, 0, 0, 0, CONST_SPACE_4, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(column_0, "tab_item_#");
        lv_obj_set_width(column_0, CONST_NAV_PRIMARY_W);
        lv_obj_set_height(column_0, CONST_TABBAR_H);

        lv_obj_bind_flag_if_not_eq(column_0, &subject_tab, LV_OBJ_FLAG_CLICKABLE, index);
        lv_obj_t * lv_image_0 = lv_image_create(column_0);
        lv_image_set_src(lv_image_0, icon);
        lv_obj_set_width(lv_image_0, 26);
        lv_obj_set_height(lv_image_0, 26);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_tertiary, 0);
        lv_obj_bind_style(lv_image_0, &style_icon_accent, 0, &subject_tab, index);
        lv_obj_bind_style(lv_image_0, &style_icon_danger, 0, alert, 1);

        lv_obj_t * label_0 = label_create(column_0);
        lv_label_set_text(label_0, text);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_chip, 0);
        lv_obj_add_style(label_0, &style_ink_tertiary, 0);
        lv_obj_bind_style(label_0, &style_ink_accent, 0, &subject_tab, index);
        lv_obj_bind_style(label_0, &style_ink_danger, 0, alert, 1);

        the_root = column_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

