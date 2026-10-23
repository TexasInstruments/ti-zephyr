/**
 * @file run_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "run_gen.h"
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

lv_obj_t * run_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_body_pad;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_body_pad);

        lv_style_set_pad_left(&style_body_pad, CONST_GUTTER);
        lv_style_set_pad_right(&style_body_pad, CONST_GUTTER);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "run_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_add_style(lv_obj_0, &style_canvas, 0);
        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_tab, LV_EVENT_SCREEN_LOADED, 0);
        lv_obj_t * column_0 = column_create(lv_obj_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_0, CONST_SCREEN_W);
        lv_obj_set_height(column_0, CONST_SCREEN_H);
        header_create(column_0);

        lv_obj_t * row_0 = row_create(column_0, 0, CONST_GUTTER, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_0, CONST_SCREEN_W);
        lv_obj_set_height(row_0, 30);
        estop_tag_create(row_0);

        lv_obj_t * row_1 = row_create(row_0, 0, 0, 0, 0, CONST_SPACE_4, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * lv_image_0 = lv_image_create(row_1);
        lv_image_set_src(lv_image_0, icon_dir_cw);
        lv_obj_set_width(lv_image_0, 16);
        lv_obj_set_height(lv_image_0, 16);
        lv_obj_add_style(lv_image_0, &style_icon_tertiary, 0);
        lv_obj_bind_flag_if_not_eq(lv_image_0, &subject_dir, LV_OBJ_FLAG_HIDDEN, REF_DIR_CW);

        lv_obj_t * lv_image_1 = lv_image_create(row_1);
        lv_image_set_src(lv_image_1, icon_dir_ccw);
        lv_obj_set_width(lv_image_1, 16);
        lv_obj_set_height(lv_image_1, 16);
        lv_obj_add_style(lv_image_1, &style_icon_tertiary, 0);
        lv_obj_bind_flag_if_not_eq(lv_image_1, &subject_dir, LV_OBJ_FLAG_HIDDEN, REF_DIR_CCW);

        lv_obj_t * label_0 = label_create(row_1);
        lv_label_set_text(label_0, "CW");
        lv_obj_add_style(label_0, &style_overline, 0);
        lv_obj_add_style(label_0, &style_ink_overline, 0);
        lv_obj_bind_flag_if_not_eq(label_0, &subject_dir, LV_OBJ_FLAG_HIDDEN, REF_DIR_CW);

        lv_obj_t * label_1 = label_create(row_1);
        lv_label_set_text(label_1, "CCW");
        lv_obj_add_style(label_1, &style_overline, 0);
        lv_obj_add_style(label_1, &style_ink_overline, 0);
        lv_obj_bind_flag_if_not_eq(label_1, &subject_dir, LV_OBJ_FLAG_HIDDEN, REF_DIR_CCW);

        lv_obj_t * column_1 = column_create(column_0, 0, 0, CONST_SPACE_24, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_1, CONST_CONTENT_W);
        lv_obj_set_height(column_1, LV_SIZE_CONTENT);
        odometer_create(column_1);

        lv_obj_t * row_2 = row_create(column_0, 0, 0, CONST_SPACE_20, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_2, CONST_CONTENT_W);
        lv_obj_set_height(row_2, LV_SIZE_CONTENT);
        lv_obj_t * btn_step_0 = btn_step_create(row_2, icon_minus);
        lv_subject_increment_dsc_t * subject_increment_event_0 = lv_obj_add_subject_increment_event(btn_step_0, &subject_rpm_ref, LV_EVENT_CLICKED, CONST_SPEED_STEP_DOWN);
        lv_obj_set_subject_increment_event_min_value(btn_step_0, subject_increment_event_0, CONST_SPEED_MIN);
        lv_obj_set_subject_increment_event_max_value(btn_step_0, subject_increment_event_0, CONST_SPEED_MAX);
        lv_subject_increment_dsc_t * subject_increment_event_1 = lv_obj_add_subject_increment_event(btn_step_0, &subject_rpm_ref, LV_EVENT_LONG_PRESSED_REPEAT, CONST_SPEED_STEP_DOWN);
        lv_obj_set_subject_increment_event_min_value(btn_step_0, subject_increment_event_1, CONST_SPEED_MIN);
        lv_obj_set_subject_increment_event_max_value(btn_step_0, subject_increment_event_1, CONST_SPEED_MAX);

        lv_obj_t * column_2 = column_create(row_2, 0, 0, 0, 0, CONST_SPACE_2, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * label_2 = label_create(column_2);
        lv_label_set_text(label_2, "REFERENCE");
        lv_obj_add_style(label_2, &style_overline, 0);
        lv_obj_add_style(label_2, &style_ink_overline, 0);

        lv_obj_t * row_3 = row_create(column_2, 0, 0, 0, 0, CONST_SPACE_4, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END);
        lv_obj_t * label_3 = label_create(row_3);
        lv_label_bind_text(label_3, &subject_rpm_ref, "%d");
        lv_label_set_text(label_3, "1200");
        lv_obj_add_style(label_3, &style_stat_lg, 0);
        lv_obj_add_style(label_3, &style_ink, 0);

        lv_obj_t * label_4 = label_create(row_3);
        lv_label_set_text(label_4, "RPM");
        lv_obj_set_style_pad_bottom(label_4, 4, 0);
        lv_obj_add_style(label_4, &style_meta, 0);
        lv_obj_add_style(label_4, &style_ink_tertiary, 0);

        lv_obj_t * btn_step_1 = btn_step_create(row_2, icon_plus);
        lv_subject_increment_dsc_t * subject_increment_event_2 = lv_obj_add_subject_increment_event(btn_step_1, &subject_rpm_ref, LV_EVENT_CLICKED, CONST_SPEED_STEP);
        lv_obj_set_subject_increment_event_min_value(btn_step_1, subject_increment_event_2, CONST_SPEED_MIN);
        lv_obj_set_subject_increment_event_max_value(btn_step_1, subject_increment_event_2, CONST_SPEED_MAX);
        lv_subject_increment_dsc_t * subject_increment_event_3 = lv_obj_add_subject_increment_event(btn_step_1, &subject_rpm_ref, LV_EVENT_LONG_PRESSED_REPEAT, CONST_SPEED_STEP);
        lv_obj_set_subject_increment_event_min_value(btn_step_1, subject_increment_event_3, CONST_SPEED_MIN);
        lv_obj_set_subject_increment_event_max_value(btn_step_1, subject_increment_event_3, CONST_SPEED_MAX);

        lv_obj_t * row_4 = row_create(column_0, 0, 0, CONST_SPACE_12, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_4, CONST_CONTENT_W);
        lv_obj_set_height(row_4, LV_SIZE_CONTENT);
        stat_create(row_4, "CURRENT", &subject_current_str, "%s", "0.0", "A", 96, &style_stat_lg, &style_ink);

        stat_create(row_4, "TEMP", &subject_temp, "%d", "31", "°C", 96, &style_stat_lg, &style_ink);

        stat_create(row_4, "LOAD", &subject_load, "%d", "0", "%", 96, &style_stat_lg, &style_ink);

        lv_obj_t * hero_space = container_create(column_0);
        lv_obj_set_name(hero_space, "hero_space");
        lv_obj_set_width(hero_space, CONST_SCREEN_W);
        lv_obj_set_flex_grow(hero_space, 1);

        btn_run_create(column_0);

        lv_obj_t * btn_gap = container_create(column_0);
        lv_obj_set_name(btn_gap, "btn_gap");
        lv_obj_set_width(btn_gap, CONST_SCREEN_W);
        lv_obj_set_height(btn_gap, CONST_SPACE_16);

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

