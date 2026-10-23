/**
 * @file stat_tile_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "stat_tile_gen.h"
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

lv_obj_t * stat_tile_create(lv_obj_t * parent, const char * caption, lv_subject_t * value, const char * fmt, const char * placeholder, const char * unit, int32_t col_w)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * column_0 = column_create(parent, CONST_SPACE_12, CONST_SPACE_12, CONST_SPACE_12, CONST_SPACE_12, CONST_SPACE_4, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(column_0, "stat_tile_#");
        lv_obj_set_width(column_0, col_w);
        lv_obj_set_height(column_0, LV_SIZE_CONTENT);

        lv_obj_add_style(column_0, &style_inset, 0);
        lv_obj_t * label_0 = label_create(column_0);
        lv_label_set_text(label_0, caption);
        lv_obj_add_style(label_0, &style_overline, 0);
        lv_obj_add_style(label_0, &style_ink_overline, 0);

        lv_obj_t * row_0 = row_create(column_0, 0, 0, 0, 0, CONST_SPACE_2, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END);
        lv_obj_t * label_1 = label_create(row_0);
        lv_label_bind_text(label_1, value, fmt);
        lv_label_set_text(label_1, placeholder);
        lv_obj_add_style(label_1, &style_stat_md, 0);
        lv_obj_add_style(label_1, &style_ink, 0);

        lv_obj_t * label_2 = label_create(row_0);
        lv_label_set_text(label_2, unit);
        lv_obj_set_style_pad_bottom(label_2, 2, 0);
        lv_obj_add_style(label_2, &style_meta, 0);
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

