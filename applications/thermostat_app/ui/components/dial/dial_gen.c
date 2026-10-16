/**
 * @file dial_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "dial_gen.h"
#include "../../ti_thermostat.h"

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

lv_obj_t * dial_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_box;
    static lv_style_t style_arc_bg;
    static lv_style_t style_arc_ind;
    static lv_style_t style_arc_knob;
    static lv_style_t style_stack;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_box);
        lv_style_init(&style_arc_bg);
        lv_style_init(&style_arc_ind);
        lv_style_init(&style_arc_knob);
        lv_style_init(&style_stack);

        lv_style_set_width(&style_box, 288);
        lv_style_set_height(&style_box, 288);
        lv_style_set_width(&style_arc_bg, 248);
        lv_style_set_height(&style_arc_bg, 248);
        lv_style_set_align(&style_arc_bg, LV_ALIGN_CENTER);
        lv_style_set_arc_width(&style_arc_bg, 18);
        lv_style_set_arc_color(&style_arc_bg, CONST_TRACK);
        lv_style_set_arc_opa(&style_arc_bg, (255 * 100 / 100));
        lv_style_set_arc_rounded(&style_arc_bg, true);
        lv_style_set_bg_opa(&style_arc_bg, 0);
        lv_style_set_border_width(&style_arc_bg, 0);
        lv_style_set_pad_all(&style_arc_bg, 0);
        lv_style_set_arc_width(&style_arc_ind, 18);
        lv_style_set_arc_opa(&style_arc_ind, (255 * 100 / 100));
        lv_style_set_arc_rounded(&style_arc_ind, true);
        lv_style_set_arc_color(&style_arc_ind, CONST_TEXT_TERTIARY);
        lv_style_set_bg_color(&style_arc_knob, CONST_ON_ACCENT);
        lv_style_set_bg_opa(&style_arc_knob, (255 * 100 / 100));
        lv_style_set_border_width(&style_arc_knob, 0);
        lv_style_set_pad_all(&style_arc_knob, 7);
        lv_style_set_radius(&style_arc_knob, 32767);
        lv_style_set_layout(&style_stack, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_stack, LV_FLEX_FLOW_ROW);
        lv_style_set_flex_main_place(&style_stack, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_stack, LV_FLEX_ALIGN_START);
        lv_style_set_flex_track_place(&style_stack, LV_FLEX_ALIGN_START);
        lv_style_set_pad_column(&style_stack, 1);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * container_0 = container_create(parent);
        lv_obj_set_name_static(container_0, "dial_#");
        lv_obj_set_flag(container_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(container_0, &style_box, 0);
        lv_obj_t * arc = lv_arc_create(container_0);
        lv_obj_set_name(arc, "arc");
        lv_arc_set_bg_start_angle(arc, 155);
        lv_arc_set_bg_end_angle(arc, 25);
        lv_arc_set_min_value(arc, 5);
        lv_arc_set_max_value(arc, 30);
        lv_arc_bind_value(arc, &subject_target_temp);
        lv_arc_set_change_rate(arc, 240);
        lv_obj_remove_style_all(arc);
        lv_obj_add_style(arc, &style_arc_bg, 0);
        lv_obj_add_style(arc, &style_arc_ind, LV_PART_INDICATOR);
        lv_obj_add_style(arc, &style_arc_knob, LV_PART_KNOB);
        lv_obj_add_style(arc, &style_shadow_knob, LV_PART_KNOB);
        lv_obj_bind_style(arc, &style_arc_ind_heat, LV_PART_INDICATOR, &subject_hvac_state, REF_HVAC_HEATING);
        lv_obj_bind_style(arc, &style_arc_ind_cool, LV_PART_INDICATOR, &subject_hvac_state, REF_HVAC_COOLING);
        lv_obj_bind_style(arc, &style_arc_ind_idle, LV_PART_INDICATOR, &subject_hvac_state, REF_HVAC_IDLE);

        lv_obj_t * readout = container_create(container_0);
        lv_obj_set_name(readout, "readout");
        lv_obj_set_align(readout, LV_ALIGN_CENTER);
        lv_obj_set_width(readout, LV_SIZE_CONTENT);
        lv_obj_set_height(readout, LV_SIZE_CONTENT);
        lv_obj_set_flag(readout, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(readout, &style_stack, 0);
        lv_obj_t * label_0 = label_create(readout);
        lv_label_bind_text(label_0, &subject_target_temp, "%d");
        lv_obj_set_style_text_color(label_0, CONST_TEXT_PRIMARY, 0);
        lv_obj_add_style(label_0, &style_hero, 0);
        lv_obj_bind_flag_if_eq(label_0, &subject_mode, LV_OBJ_FLAG_HIDDEN, REF_MODE_OFF);

        lv_obj_t * label_1 = label_create(readout);
        lv_label_set_text(label_1, "°");
        lv_obj_set_style_text_color(label_1, CONST_TEXT_SECONDARY, 0);
        lv_obj_set_style_pad_top(label_1, 4, 0);
        lv_obj_add_style(label_1, &style_hero_unit, 0);
        lv_obj_bind_flag_if_eq(label_1, &subject_mode, LV_OBJ_FLAG_HIDDEN, REF_MODE_OFF);

        lv_obj_t * label_2 = label_create(readout);
        lv_label_set_text(label_2, "-");
        lv_obj_set_style_text_color(label_2, CONST_TEXT_TERTIARY, 0);
        lv_obj_add_style(label_2, &style_hero, 0);
        lv_obj_bind_flag_if_not_eq(label_2, &subject_mode, LV_OBJ_FLAG_HIDDEN, REF_MODE_OFF);

        lv_obj_t * now = row_create(container_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(now, "now");
        lv_obj_set_align(now, LV_ALIGN_CENTER);
        lv_obj_set_y(now, 64);
        lv_obj_set_flag(now, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_width(now, LV_SIZE_CONTENT);
        lv_obj_set_height(now, LV_SIZE_CONTENT);
        lv_obj_t * label_3 = label_create(now);
        lv_label_set_text(label_3, "Now ");
        lv_obj_set_style_text_color(label_3, CONST_TEXT_SECONDARY, 0);
        lv_obj_add_style(label_3, &style_stat_sm, 0);

        lv_obj_t * label_4 = label_create(now);
        lv_label_bind_text(label_4, &subject_now_temp, "%.1f°");
        lv_obj_set_style_text_color(label_4, CONST_TEXT_SECONDARY, 0);
        lv_obj_add_style(label_4, &style_stat_sm, 0);

        lv_obj_t * label_5 = label_create(now);
        lv_label_bind_text(label_5, &subject_humidity, "%d");
        lv_obj_set_style_text_color(label_5, CONST_TEXT_SECONDARY, 0);
        lv_obj_set_style_pad_left(label_5, 14, 0);
        lv_obj_add_style(label_5, &style_stat_sm, 0);

        lv_obj_t * label_6 = label_create(now);
        lv_label_set_text(label_6, "% RH");
        lv_obj_set_style_text_color(label_6, CONST_TEXT_SECONDARY, 0);
        lv_obj_add_style(label_6, &style_stat_sm, 0);

        lv_obj_t * btn_step_0 = btn_step_create(container_0, "-", -1);
        lv_obj_set_align(btn_step_0, LV_ALIGN_BOTTOM_LEFT);
        lv_obj_set_x(btn_step_0, 12);
        lv_obj_set_y(btn_step_0, -12);

        lv_obj_t * btn_step_1 = btn_step_create(container_0, "+", 1);
        lv_obj_set_align(btn_step_1, LV_ALIGN_BOTTOM_RIGHT);
        lv_obj_set_x(btn_step_1, -12);
        lv_obj_set_y(btn_step_1, -12);

        lv_obj_t * hold = column_create(container_0, 0, 0, 0, 0, 1, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name(hold, "hold");
        lv_obj_set_align(hold, LV_ALIGN_BOTTOM_MID);
        lv_obj_set_y(hold, -22);
        lv_obj_set_width(hold, LV_SIZE_CONTENT);
        lv_obj_set_height(hold, LV_SIZE_CONTENT);
        lv_obj_set_flag(hold, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * label_7 = label_create(hold);
        lv_label_set_text(label_7, "Hold");
        lv_obj_set_style_text_color(label_7, CONST_TEXT_PRIMARY, 0);
        lv_obj_add_style(label_7, &style_body_strong, 0);

        lv_obj_t * label_8 = label_create(hold);
        lv_label_bind_text(label_8, &subject_hold_until, "until %s");
        lv_obj_set_style_text_color(label_8, CONST_TEXT_SECONDARY, 0);
        lv_obj_add_style(label_8, &style_stat_sm, 0);

        the_root = container_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

