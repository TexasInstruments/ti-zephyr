/**
 * @file home_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "home_gen.h"
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

lv_obj_t * home_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "home_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_state, LV_EVENT_SCREEN_LOADED, REF_STATE_IDLE);
        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_width(lv_obj_1, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_1, CONST_SCREEN_H);
        lv_obj_set_x(lv_obj_1, 0);
        lv_obj_set_y(lv_obj_1, 0);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_obj_1, &style_frame_door_outer, 0);
        lv_obj_bind_flag_if_eq(lv_obj_1, &subject_door, LV_OBJ_FLAG_HIDDEN, REF_DOOR_LOCKED);

        lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
        lv_obj_set_width(lv_obj_2, 468);
        lv_obj_set_height(lv_obj_2, 308);
        lv_obj_set_x(lv_obj_2, 6);
        lv_obj_set_y(lv_obj_2, 6);
        lv_obj_set_flag(lv_obj_2, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_obj_2, &style_frame_door_inner, 0);
        lv_obj_bind_flag_if_eq(lv_obj_2, &subject_door, LV_OBJ_FLAG_HIDDEN, REF_DOOR_LOCKED);

        lv_obj_t * column_0 = column_create(lv_obj_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_x(column_0, CONST_GUTTER);
        lv_obj_set_y(column_0, 22);
        lv_obj_t * lv_image_0 = lv_image_create(column_0);
        lv_image_set_src(lv_image_0, img_logostrip);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_margin_bottom(lv_image_0, 3, 0);
        lv_obj_set_style_margin_left(lv_image_0, 2, 0);

        lv_obj_t * label_0 = label_create(column_0);
        lv_label_set_text(label_0, "Bubble Wash");
        lv_obj_add_style(label_0, &style_heading, 0);
        lv_obj_add_style(label_0, &style_ink, 0);

        lv_obj_t * row_0 = row_create(column_0, 0, 0, 0, 0, 5, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * label_1 = label_create(row_0);
        lv_label_set_text(label_1, "Cotton");
        lv_label_bind_text(label_1, &subject_program_name, NULL);
        lv_obj_add_style(label_1, &style_meta, 0);
        lv_obj_add_style(label_1, &style_ink, 0);
        lv_obj_bind_flag_if_lt(label_1, &subject_program, LV_OBJ_FLAG_HIDDEN, REF_PROG_COTTON);

        lv_obj_t * label_2 = label_create(row_0);
        lv_label_set_text(label_2, ".");
        lv_obj_add_style(label_2, &style_meta, 0);
        lv_obj_add_style(label_2, &style_ink_3, 0);
        lv_obj_bind_flag_if_lt(label_2, &subject_program, LV_OBJ_FLAG_HIDDEN, REF_PROG_COTTON);

        lv_obj_t * label_3 = label_create(row_0);
        lv_label_set_text(label_3, "1200");
        lv_label_bind_text(label_3, &subject_spin_rpm, "%d rpm");
        lv_obj_add_style(label_3, &style_meta, 0);
        lv_obj_add_style(label_3, &style_ink_2, 0);

        lv_obj_t * label_4 = label_create(row_0);
        lv_label_set_text(label_4, ".");
        lv_obj_add_style(label_4, &style_meta, 0);
        lv_obj_add_style(label_4, &style_ink_3, 0);

        lv_obj_t * label_5 = label_create(row_0);
        lv_label_set_text(label_5, "62");
        lv_label_bind_text(label_5, &subject_water_target, "%d%% water");
        lv_obj_add_style(label_5, &style_meta, 0);
        lv_obj_add_style(label_5, &style_ink_2, 0);

        lv_obj_t * column_1 = column_create(lv_obj_0, 0, 0, 0, 0, 9, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_x(column_1, CONST_COL_HOME);
        lv_obj_set_y(column_1, 108);
        overline_create(column_1, "FABRIC");

        lv_obj_t * row_1 = row_create(column_1, 0, 0, 0, 0, 6, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        btn_fabric_create(row_1, icon_prog_cotton, REF_PROG_COTTON);

        btn_fabric_create(row_1, icon_prog_eco, REF_PROG_ECO);

        btn_fabric_create(row_1, icon_prog_synth, REF_PROG_SYNTH);

        btn_fabric_create(row_1, icon_prog_quick, REF_PROG_QUICK);

        lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_0);
        lv_obj_set_x(lv_obj_3, CONST_COL_HOME);
        lv_obj_set_y(lv_obj_3, 205);
        lv_obj_set_width(lv_obj_3, 226);
        lv_obj_set_height(lv_obj_3, LV_SIZE_CONTENT);
        lv_obj_set_flag(lv_obj_3, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_3, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(lv_obj_3, 0, 0);
        lv_obj_set_style_border_width(lv_obj_3, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_3, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_3, 0, 0);
        lv_obj_set_style_layout(lv_obj_3, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(lv_obj_3, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_flex_main_place(lv_obj_3, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_3, LV_FLEX_ALIGN_END, 0);
        lv_obj_set_style_flex_track_place(lv_obj_3, LV_FLEX_ALIGN_END, 0);
        overline_create(lv_obj_3, "TEMPERATURE");

        lv_obj_t * label_6 = label_create(lv_obj_3);
        lv_label_set_text(label_6, "Not set");
        lv_obj_add_style(label_6, &style_body_strong, 0);
        lv_obj_add_style(label_6, &style_ink_3, 0);
        lv_obj_bind_flag_if_not_eq(label_6, &subject_temp_idx, LV_OBJ_FLAG_HIDDEN, 0);

        lv_obj_t * label_7 = label_create(lv_obj_3);
        lv_label_set_text(label_7, "40");
        lv_label_bind_text(label_7, &subject_temp_c, "%d°");
        lv_obj_add_style(label_7, &style_body_strong, 0);
        lv_obj_add_style(label_7, &style_ink, 0);
        lv_obj_bind_flag_if_eq(label_7, &subject_temp_idx, LV_OBJ_FLAG_HIDDEN, 0);

        lv_obj_t * slider_temp_0 = slider_temp_create(lv_obj_0);
        lv_obj_set_x(slider_temp_0, CONST_COL_HOME);
        lv_obj_set_y(slider_temp_0, 240);

        lv_obj_t * water_level_0 = water_level_create(lv_obj_0);
        lv_obj_set_x(water_level_0, 4);
        lv_obj_set_y(water_level_0, 148);

        lv_obj_t * lv_image_1 = lv_image_create(lv_obj_0);
        lv_image_set_src(lv_image_1, img_machine);
        lv_obj_set_x(lv_image_1, 18);
        lv_obj_set_y(lv_image_1, 108);
        lv_obj_set_flag(lv_image_1, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * drum_0 = drum_create(lv_obj_0);
        lv_obj_set_x(drum_0, 15);
        lv_obj_set_y(drum_0, 137);
        lv_obj_bind_flag_if_not_eq(drum_0, &subject_ready, LV_OBJ_FLAG_HIDDEN, REF_ON);

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
        lv_obj_add_subject_toggle_event(lv_button_0, &subject_door, LV_EVENT_CLICKED);

        lv_obj_t * lv_obj_4 = lv_obj_create(lv_obj_0);
        lv_obj_set_x(lv_obj_4, 98);
        lv_obj_set_y(lv_obj_4, 214);
        lv_obj_set_width(lv_obj_4, 36);
        lv_obj_set_height(lv_obj_4, 36);
        lv_obj_set_flag(lv_obj_4, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_4, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_radius(lv_obj_4, 32767, 0);
        lv_obj_set_style_bg_color(lv_obj_4, CONST_ON_ACCENT, 0);
        lv_obj_set_style_bg_opa(lv_obj_4, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(lv_obj_4, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_4, 0, 0);
        lv_obj_set_style_layout(lv_obj_4, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(lv_obj_4, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_flex_main_place(lv_obj_4, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_4, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_track_place(lv_obj_4, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_bind_flag_if_eq(lv_obj_4, &subject_door, LV_OBJ_FLAG_HIDDEN, REF_DOOR_LOCKED);
        lv_obj_t * lv_image_2 = lv_image_create(lv_obj_4);
        lv_image_set_src(lv_image_2, icon_unlock);
        lv_obj_set_flag(lv_image_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_2, &style_icon_accent, 0);

        lv_obj_t * lv_obj_5 = lv_obj_create(lv_obj_0);
        lv_obj_set_x(lv_obj_5, CONST_COL_HOME);
        lv_obj_set_y(lv_obj_5, 0);
        lv_obj_set_width(lv_obj_5, 226);
        lv_obj_set_height(lv_obj_5, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_5, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_5, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(lv_obj_5, 0, 0);
        lv_obj_set_style_border_width(lv_obj_5, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_5, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_5, 0, 0);
        lv_obj_t * key_hint_0 = key_hint_create(lv_obj_5, "START", "Long press", true, &style_key_accent, &style_key_accent_pressed);
        lv_obj_set_y(key_hint_0, 30);
        lv_obj_bind_flag_if_not_eq(key_hint_0, &subject_ready, LV_OBJ_FLAG_HIDDEN, REF_ON);
        lv_obj_add_subject_set_int_event(key_hint_0, &subject_state, LV_EVENT_CLICKED, REF_STATE_RUNNING);
        lv_obj_add_screen_create_event(key_hint_0, LV_EVENT_CLICKED, cycle_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        lv_obj_t * key_blocked_0 = key_blocked_create(lv_obj_5);
        lv_obj_bind_flag_if_eq(key_blocked_0, &subject_ready, LV_OBJ_FLAG_HIDDEN, REF_ON);

        lv_obj_t * btn_chip_0 = btn_chip_create(lv_obj_0, icon_options);
        lv_obj_set_x(btn_chip_0, 414);
        lv_obj_set_y(btn_chip_0, 30);
        lv_obj_add_screen_create_event(btn_chip_0, LV_EVENT_CLICKED, setup_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

