/**
 * @file config_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "config_gen.h"
#include "../motor_control.h"

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

lv_obj_t * config_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "config_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_add_style(lv_obj_0, &style_canvas, 0);
        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_tab, LV_EVENT_SCREEN_LOADED, 3);
        lv_obj_t * column_0 = column_create(lv_obj_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_0, CONST_SCREEN_W);
        lv_obj_set_height(column_0, CONST_SCREEN_H);
        header_create(column_0);

        lv_obj_t * column_1 = column_create(column_0, 0, 0, CONST_SPACE_8, 0, CONST_SPACE_12, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_1, CONST_CONTENT_W);
        lv_obj_set_height(column_1, LV_SIZE_CONTENT);
        lv_obj_t * label_0 = label_create(column_1);
        lv_label_set_text(label_0, "Drive setup");
        lv_obj_add_style(label_0, &style_heading, 0);
        lv_obj_add_style(label_0, &style_ink, 0);

        lv_obj_t * column_2 = column_create(column_1, 0, 0, 0, 0, CONST_SPACE_8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_2, lv_pct(100));
        lv_obj_set_height(column_2, LV_SIZE_CONTENT);
        overline_create(column_2, "ROTATION");

        lv_obj_t * row_0 = row_create(column_2, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_0, lv_pct(100));
        lv_obj_t * lv_button_0 = lv_button_create(row_0);
        lv_obj_set_width(lv_button_0, 138);
        lv_obj_set_height(lv_button_0, 48);
        lv_obj_add_style(lv_button_0, &style_seg, 0);
        lv_obj_bind_style(lv_button_0, &style_seg_selected, 0, &subject_dir, REF_DIR_CW);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_dir, LV_EVENT_CLICKED, REF_DIR_CW);
        lv_obj_t * row_1 = row_create(lv_button_0, 0, 0, 0, 0, CONST_SPACE_8, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_flag(row_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_width(row_1, lv_pct(100));
        lv_obj_set_height(row_1, lv_pct(100));
        lv_obj_t * lv_image_0 = lv_image_create(row_1);
        lv_image_set_src(lv_image_0, icon_dir_cw);
        lv_obj_set_width(lv_image_0, 16);
        lv_obj_set_height(lv_image_0, 16);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_secondary, 0);
        lv_obj_bind_style(lv_image_0, &style_icon_accent, 0, &subject_dir, REF_DIR_CW);

        lv_obj_t * label_1 = label_create(row_1);
        lv_label_set_text(label_1, "Clockwise");
        lv_obj_set_flag(label_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_1, &style_body_strong, 0);
        lv_obj_add_style(label_1, &style_ink_secondary, 0);
        lv_obj_bind_style(label_1, &style_ink_accent, 0, &subject_dir, REF_DIR_CW);

        lv_obj_t * lv_button_1 = lv_button_create(row_0);
        lv_obj_set_width(lv_button_1, 138);
        lv_obj_set_height(lv_button_1, 48);
        lv_obj_add_style(lv_button_1, &style_seg, 0);
        lv_obj_bind_style(lv_button_1, &style_seg_selected, 0, &subject_dir, REF_DIR_CCW);
        lv_obj_add_subject_set_int_event(lv_button_1, &subject_dir, LV_EVENT_CLICKED, REF_DIR_CCW);
        lv_obj_t * row_2 = row_create(lv_button_1, 0, 0, 0, 0, CONST_SPACE_8, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_flag(row_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_width(row_2, lv_pct(100));
        lv_obj_set_height(row_2, lv_pct(100));
        lv_obj_t * lv_image_1 = lv_image_create(row_2);
        lv_image_set_src(lv_image_1, icon_dir_ccw);
        lv_obj_set_width(lv_image_1, 16);
        lv_obj_set_height(lv_image_1, 16);
        lv_obj_set_flag(lv_image_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_1, &style_icon_secondary, 0);
        lv_obj_bind_style(lv_image_1, &style_icon_accent, 0, &subject_dir, REF_DIR_CCW);

        lv_obj_t * label_2 = label_create(row_2);
        lv_label_set_text(label_2, "Counter");
        lv_obj_set_flag(label_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_2, &style_body_strong, 0);
        lv_obj_add_style(label_2, &style_ink_secondary, 0);
        lv_obj_bind_style(label_2, &style_ink_accent, 0, &subject_dir, REF_DIR_CCW);

        lv_obj_t * card_0 = card_create(column_1, 12, CONST_SPACE_4);
        overline_create(card_0, "MOTOR INTERLOCKS");

        check_row_create(card_0, "Soft start", "Ramp to reference instead of stepping", &subject_cfg_softstart);

        check_row_create(card_0, "Allow reversal", "Permit direction change while running", &subject_cfg_reverse);

        check_row_create(card_0, "Thermal derate", "Fold back current above 80 °C", &subject_cfg_derate);

        lv_obj_t * grow = container_create(column_0);
        lv_obj_set_name(grow, "grow");
        lv_obj_set_width(grow, CONST_SCREEN_W);
        lv_obj_set_flex_grow(grow, 1);

        tab_bar_create(column_0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

