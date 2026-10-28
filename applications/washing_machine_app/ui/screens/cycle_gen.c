/**
 * @file cycle_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "cycle_gen.h"
#include "../ti_washing_machine.h"

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

lv_obj_t * cycle_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "cycle_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_add_event_cb(lv_obj_0, wash_sim_screen_loaded_cb, LV_EVENT_SCREEN_LOAD_START, NULL);
        lv_obj_t * nav_header_0 = nav_header_create(lv_obj_0, "Washing");
        lv_obj_bind_flag_if_eq(nav_header_0, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);

        lv_obj_t * btn_back_0 = btn_back_create(lv_obj_0);
        lv_obj_set_x(btn_back_0, 0);
        lv_obj_set_y(btn_back_0, 0);
        lv_obj_bind_flag_if_eq(btn_back_0, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);
        lv_obj_add_screen_create_event(btn_back_0, LV_EVENT_CLICKED, home_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        lv_obj_t * water_level_0 = water_level_create(lv_obj_0);
        lv_obj_set_x(water_level_0, 4);
        lv_obj_set_y(water_level_0, 148);

        lv_obj_t * lv_image_0 = lv_image_create(lv_obj_0);
        lv_image_set_src(lv_image_0, img_machine);
        lv_obj_set_x(lv_image_0, 18);
        lv_obj_set_y(lv_image_0, 108);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * hero_drum = drum_create(lv_obj_0);
        lv_obj_set_name(hero_drum, "hero_drum");
        lv_obj_set_x(hero_drum, 15);
        lv_obj_set_y(hero_drum, 137);

        lv_obj_t * lv_button_0 = lv_button_create(lv_obj_0);
        lv_obj_set_x(lv_button_0, 54);
        lv_obj_set_y(lv_button_0, 175);
        lv_obj_set_width(lv_button_0, 125);
        lv_obj_set_height(lv_button_0, 125);
        lv_obj_set_style_bg_opa(lv_button_0, 0, 0);
        lv_obj_set_style_border_width(lv_button_0, 0, 0);
        lv_obj_set_style_shadow_width(lv_button_0, 0, 0);
        lv_obj_set_style_radius(lv_button_0, 32767, 0);
        lv_obj_set_style_pad_all(lv_button_0, 0, 0);
        lv_obj_bind_flag_if_not_eq(lv_button_0, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_door, LV_EVENT_CLICKED, REF_DOOR_OPEN);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_state, LV_EVENT_CLICKED, REF_STATE_IDLE);
        lv_obj_add_screen_create_event(lv_button_0, LV_EVENT_CLICKED, home_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        lv_obj_t * grp_running = lv_obj_create(lv_obj_0);
        lv_obj_set_name(grp_running, "grp_running");
        lv_obj_set_width(grp_running, 226);
        lv_obj_set_height(grp_running, 150);
        lv_obj_set_x(grp_running, CONST_COL_HOME);
        lv_obj_set_y(grp_running, 146);
        lv_obj_set_flag(grp_running, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(grp_running, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(grp_running, 0, 0);
        lv_obj_set_style_border_width(grp_running, 0, 0);
        lv_obj_set_style_shadow_width(grp_running, 0, 0);
        lv_obj_set_style_pad_all(grp_running, 0, 0);
        lv_obj_set_style_pad_row(grp_running, 8, 0);
        lv_obj_set_style_layout(grp_running, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(grp_running, LV_FLEX_FLOW_COLUMN, 0);
        lv_obj_set_style_flex_cross_place(grp_running, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_track_place(grp_running, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_bind_flag_if_eq(grp_running, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);
        lv_obj_t * row_0 = row_create(grp_running, 0, 0, 0, 0, 7, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END);
        lv_obj_t * label_0 = label_create(row_0);
        lv_label_set_text(label_0, "2:15");
        lv_label_bind_text(label_0, &subject_time_left_str, NULL);
        lv_obj_add_style(label_0, &style_hero, 0);
        lv_obj_add_style(label_0, &style_ink, 0);

        lv_obj_t * label_1 = label_create(row_0);
        lv_label_set_text(label_1, "h");
        lv_label_bind_text(label_1, &subject_time_unit_str, NULL);
        lv_obj_set_style_translate_y(label_1, -8, 0);
        lv_obj_add_style(label_1, &style_hero_unit, 0);
        lv_obj_add_style(label_1, &style_ink_2, 0);

        lv_obj_t * row_1 = row_create(grp_running, 0, 0, 0, 0, 10, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        chip_create(row_1, "Running", &style_chip_fill_accent, &style_chip_dot_accent, &style_chip_text_accent);

        lv_obj_t * label_2 = label_create(row_1);
        lv_label_set_text(label_2, "0");
        lv_label_bind_text(label_2, &subject_progress, "%d%%");
        lv_obj_add_style(label_2, &style_meta, 0);
        lv_obj_add_style(label_2, &style_ink_2, 0);

        lv_obj_t * card_phase_0 = card_phase_create(lv_obj_0, 445);
        lv_obj_set_x(card_phase_0, 17);
        lv_obj_set_y(card_phase_0, 57);
        lv_obj_bind_flag_if_eq(card_phase_0, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);

        lv_obj_t * lv_image_1 = lv_image_create(lv_obj_0);
        lv_image_set_src(lv_image_1, img_logostrip);
        lv_obj_set_x(lv_image_1, 202);
        lv_obj_set_y(lv_image_1, 20);
        lv_obj_set_flag(lv_image_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_bind_flag_if_not_eq(lv_image_1, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);

        lv_obj_t * grp_complete = lv_obj_create(lv_obj_0);
        lv_obj_set_name(grp_complete, "grp_complete");
        lv_obj_set_width(grp_complete, 226);
        lv_obj_set_height(grp_complete, 228);
        lv_obj_set_x(grp_complete, CONST_COL_HOME);
        lv_obj_set_y(grp_complete, 60);
        lv_obj_set_flag(grp_complete, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(grp_complete, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(grp_complete, 0, 0);
        lv_obj_set_style_border_width(grp_complete, 0, 0);
        lv_obj_set_style_shadow_width(grp_complete, 0, 0);
        lv_obj_set_style_pad_all(grp_complete, 0, 0);
        lv_obj_set_style_pad_top(grp_complete, 44, 0);
        lv_obj_set_style_pad_row(grp_complete, 22, 0);
        lv_obj_set_style_layout(grp_complete, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(grp_complete, LV_FLEX_FLOW_COLUMN, 0);
        lv_obj_set_style_flex_cross_place(grp_complete, LV_FLEX_ALIGN_START, 0);
        lv_obj_set_style_flex_track_place(grp_complete, LV_FLEX_ALIGN_START, 0);
        lv_obj_bind_flag_if_not_eq(grp_complete, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);
        lv_obj_t * column_0 = column_create(grp_complete, 0, 0, 0, 0, 3, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_t * label_3 = label_create(column_0);
        lv_label_set_text(label_3, "Cycle complete");
        lv_obj_add_style(label_3, &style_heading, 0);
        lv_obj_add_style(label_3, &style_ink, 0);

        lv_obj_t * label_4 = label_create(column_0);
        lv_label_set_text(label_4, "Open the door to unload");
        lv_obj_add_style(label_4, &style_body, 0);
        lv_obj_add_style(label_4, &style_ink_2, 0);

        lv_obj_t * column_1 = column_create(grp_complete, 0, 0, 0, 0, 4, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        overline_create(column_1, "THIS WASH");

        lv_obj_t * row_2 = row_create(column_1, 0, 0, 0, 0, 7, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * label_5 = label_create(row_2);
        lv_label_set_text(label_5, "Cotton");
        lv_label_bind_text(label_5, &subject_program_name, NULL);
        lv_obj_add_style(label_5, &style_body_strong, 0);
        lv_obj_add_style(label_5, &style_ink, 0);
        lv_obj_bind_flag_if_lt(label_5, &subject_program, LV_OBJ_FLAG_HIDDEN, REF_PROG_COTTON);

        lv_obj_t * label_6 = label_create(row_2);
        lv_label_set_text(label_6, "40");
        lv_label_bind_text(label_6, &subject_temp_c, "at %d°");
        lv_obj_add_style(label_6, &style_meta, 0);
        lv_obj_add_style(label_6, &style_ink_2, 0);
        lv_obj_bind_flag_if_lt(label_6, &subject_temp_c, LV_OBJ_FLAG_HIDDEN, 20);

        lv_obj_t * row_3 = row_create(column_1, 0, 0, 0, 0, 7, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * label_7 = label_create(row_3);
        lv_label_set_text(label_7, "2h 15m");
        lv_label_bind_text(label_7, &subject_duration_str, NULL);
        lv_obj_add_style(label_7, &style_body_strong, 0);
        lv_obj_add_style(label_7, &style_ink, 0);

        lv_obj_t * label_8 = label_create(row_3);
        lv_label_set_text(label_8, "1200");
        lv_label_bind_text(label_8, &subject_spin_rpm, "spun at %d rpm");
        lv_obj_add_style(label_8, &style_meta, 0);
        lv_obj_add_style(label_8, &style_ink_2, 0);

        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_x(lv_obj_1, CONST_COL_HOME);
        lv_obj_set_y(lv_obj_1, 0);
        lv_obj_set_width(lv_obj_1, 226);
        lv_obj_set_height(lv_obj_1, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(lv_obj_1, 0, 0);
        lv_obj_set_style_border_width(lv_obj_1, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_1, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_1, 0, 0);
        lv_obj_t * key_hint_0 = key_hint_create(lv_obj_1, "STOP", "Long press", false, &style_key_danger, &style_key_danger_pressed);
        lv_obj_bind_flag_if_eq(key_hint_0, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_COMPLETE);
        lv_obj_add_subject_set_int_event(key_hint_0, &subject_state, LV_EVENT_LONG_PRESSED, REF_STATE_COMPLETE);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

