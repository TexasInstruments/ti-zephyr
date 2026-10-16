/**
 * @file btn_step_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "btn_step_gen.h"
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

lv_obj_t * btn_step_create(lv_obj_t * parent, const char * text, int32_t step)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_pressed;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_pressed);

        lv_style_set_width(&style_base, 52);
        lv_style_set_height(&style_base, 52);
        lv_style_set_radius(&style_base, 32767);
        lv_style_set_bg_color(&style_base, CONST_SURFACE_CARD);
        lv_style_set_bg_opa(&style_base, (255 * 100 / 100));
        lv_style_set_border_color(&style_base, CONST_BORDER);
        lv_style_set_border_width(&style_base, 1);
        lv_style_set_pad_all(&style_base, 0);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_main_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_bg_color(&style_pressed, CONST_DIVIDER);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "btn_step_#");

        lv_obj_remove_style_all(lv_button_0);
        lv_obj_add_style(lv_button_0, &style_base, 0);
        lv_obj_add_style(lv_button_0, &style_shadow_card, 0);
        lv_obj_add_style(lv_button_0, &style_pressed, LV_STATE_PRESSED);
        lv_subject_increment_dsc_t * subject_increment_event_0 = lv_obj_add_subject_increment_event(lv_button_0, &subject_target_temp, LV_EVENT_CLICKED, step);
        lv_obj_set_subject_increment_event_min_value(lv_button_0, subject_increment_event_0, 5);
        lv_obj_set_subject_increment_event_max_value(lv_button_0, subject_increment_event_0, 30);
        lv_subject_increment_dsc_t * subject_increment_event_1 = lv_obj_add_subject_increment_event(lv_button_0, &subject_target_temp, LV_EVENT_LONG_PRESSED_REPEAT, step);
        lv_obj_set_subject_increment_event_min_value(lv_button_0, subject_increment_event_1, 5);
        lv_obj_set_subject_increment_event_max_value(lv_button_0, subject_increment_event_1, 30);
        lv_obj_t * label_0 = label_create(lv_button_0);
        lv_label_set_text(label_0, text);
        lv_obj_set_style_text_color(label_0, CONST_TEXT_PRIMARY, 0);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_heading, 0);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

