/**
 * @file faults_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "faults_gen.h"
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

lv_obj_t * faults_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "faults_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_add_style(lv_obj_0, &style_canvas, 0);
        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_tab, LV_EVENT_SCREEN_LOADED, 1);
        lv_obj_t * column_0 = column_create(lv_obj_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_0, CONST_SCREEN_W);
        lv_obj_set_height(column_0, CONST_SCREEN_H);
        header_create(column_0);

        lv_obj_t * column_1 = column_create(column_0, 0, 0, CONST_SPACE_8, 0, CONST_SPACE_12, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_1, CONST_CONTENT_W);
        lv_obj_set_height(column_1, LV_SIZE_CONTENT);
        lv_obj_t * row_0 = row_create(column_1, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_0, lv_pct(100));
        lv_obj_t * label_0 = label_create(row_0);
        lv_label_set_text(label_0, "Fault log");
        lv_obj_add_style(label_0, &style_heading, 0);
        lv_obj_add_style(label_0, &style_ink, 0);

        lv_obj_t * chip_0 = chip_create(row_0, "All clear", &style_chip_accent, &style_dot_accent, &style_ink_accent);
        lv_obj_bind_flag_if_gt(chip_0, &subject_fault_count, LV_OBJ_FLAG_HIDDEN, 0);

        lv_obj_t * chip_1 = chip_create(row_0, "Latched", &style_chip_danger, &style_dot_danger, &style_ink_danger);
        lv_obj_bind_flag_if_le(chip_1, &subject_fault_count, LV_OBJ_FLAG_HIDDEN, 0);

        lv_obj_t * card_0 = card_create(column_1, 16, CONST_SPACE_8);
        lv_obj_bind_flag_if_le(card_0, &subject_fault_count, LV_OBJ_FLAG_HIDDEN, 0);
        overline_create(card_0, "LATCHED FAULTS");

        lv_obj_t * fault_table = lv_table_create(card_0);
        lv_obj_set_name(fault_table, "fault_table");
        lv_obj_set_width(fault_table, lv_pct(100));
        lv_obj_set_height(fault_table, LV_SIZE_CONTENT);
        lv_table_set_column_count(fault_table, 3);
        lv_table_set_row_count(fault_table, 5);
        lv_obj_add_style(fault_table, &style_table, 0);
        lv_obj_add_style(fault_table, &style_table_cell, LV_PART_ITEMS);
        lv_obj_add_style(fault_table, &style_scrollbar_hidden, LV_PART_SCROLLBAR);
        lv_table_set_column_width(fault_table, 0, 52);
        lv_table_set_column_width(fault_table, 1, 132);
        lv_table_set_column_width(fault_table, 2, 72);
        lv_table_set_cell_value(fault_table, 0, 0, "CODE");
        lv_table_set_cell_value(fault_table, 0, 1, "DESCRIPTION");
        lv_table_set_cell_value(fault_table, 0, 2, "SEVERITY");
        lv_table_set_cell_value(fault_table, 1, 0, "F102");
        lv_table_set_cell_value(fault_table, 1, 1, "Overcurrent trip");
        lv_table_set_cell_value(fault_table, 1, 2, "Critical");
        lv_table_set_cell_value(fault_table, 2, 0, "F210");
        lv_table_set_cell_value(fault_table, 2, 1, "Stage over-temp");
        lv_table_set_cell_value(fault_table, 2, 2, "Warning");
        lv_table_set_cell_value(fault_table, 3, 0, "F045");
        lv_table_set_cell_value(fault_table, 3, 1, "Hall sequence");
        lv_table_set_cell_value(fault_table, 3, 2, "Critical");
        lv_table_set_cell_value(fault_table, 4, 0, "F311");
        lv_table_set_cell_value(fault_table, 4, 1, "DC bus sag");
        lv_table_set_cell_value(fault_table, 4, 2, "Warning");

        lv_obj_t * card_1 = card_create(column_1, 20, CONST_SPACE_4);
        lv_obj_bind_flag_if_gt(card_1, &subject_fault_count, LV_OBJ_FLAG_HIDDEN, 0);
        lv_obj_t * label_1 = label_create(card_1);
        lv_label_set_text(label_1, "No faults latched");
        lv_obj_add_style(label_1, &style_subtitle, 0);
        lv_obj_add_style(label_1, &style_ink, 0);

        lv_obj_t * label_2 = label_create(card_1);
        lv_label_set_text(label_2, "The drive is free to start.");
        lv_obj_add_style(label_2, &style_meta, 0);
        lv_obj_add_style(label_2, &style_ink_tertiary, 0);

        lv_obj_t * card_2 = card_create(column_1, 16, CONST_SPACE_12);
        lv_obj_bind_flag_if_gt(card_2, &subject_fault_count, LV_OBJ_FLAG_HIDDEN, 0);
        overline_create(card_2, "DRIVE HEALTH");

        lv_obj_t * row_1 = row_create(card_2, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_1, lv_pct(100));
        stat_create(row_1, "BUS", &subject_bus_str, "%s", "24.0", "V", 85, &style_stat_md, &style_ink);

        stat_create(row_1, "TEMP", &subject_temp, "%d", "31", "°C", 85, &style_stat_md, &style_ink);

        stat_create(row_1, "RUNTIME", &subject_runtime_str, "%s", "00:00", "", 85, &style_stat_md, &style_ink);

        lv_obj_t * lv_button_0 = lv_button_create(column_1);
        lv_obj_set_width(lv_button_0, lv_pct(100));
        lv_obj_set_height(lv_button_0, CONST_TOUCH_LG);
        lv_obj_add_style(lv_button_0, &style_btn_base, 0);
        lv_obj_add_style(lv_button_0, &style_btn_ghost, 0);
        lv_obj_add_style(lv_button_0, &style_btn_ghost_pressed, LV_STATE_PRESSED);
        lv_obj_bind_flag_if_le(lv_button_0, &subject_fault_count, LV_OBJ_FLAG_HIDDEN, 0);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_fault_count, LV_EVENT_CLICKED, 0);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_fault_active, LV_EVENT_CLICKED, REF_OFF);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_state, LV_EVENT_CLICKED, REF_STATE_STOPPED);
        lv_obj_t * row_2 = row_create(lv_button_0, 0, 0, 0, 0, CONST_SPACE_8, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_flag(row_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_width(row_2, lv_pct(100));
        lv_obj_set_height(row_2, lv_pct(100));
        lv_obj_t * lv_image_0 = lv_image_create(row_2);
        lv_image_set_src(lv_image_0, icon_clear);
        lv_obj_set_width(lv_image_0, 16);
        lv_obj_set_height(lv_image_0, 16);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_ink, 0);

        lv_obj_t * label_3 = label_create(row_2);
        lv_label_set_text(label_3, "Clear motor faults");
        lv_obj_set_flag(label_3, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_3, &style_title, 0);
        lv_obj_add_style(label_3, &style_ink, 0);

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

