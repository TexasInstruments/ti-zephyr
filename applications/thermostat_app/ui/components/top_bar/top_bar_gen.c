/**
 * @file top_bar_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "top_bar_gen.h"
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

lv_obj_t * top_bar_create(lv_obj_t * parent, const char * title)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_power;
    static lv_style_t style_power_pressed;
    static lv_style_t style_power_icon;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_power);
        lv_style_init(&style_power_pressed);
        lv_style_init(&style_power_icon);

        lv_style_set_width(&style_power, 30);
        lv_style_set_height(&style_power, 30);
        lv_style_set_radius(&style_power, 32767);
        lv_style_set_bg_opa(&style_power, 0);
        lv_style_set_border_width(&style_power, 0);
        lv_style_set_shadow_width(&style_power, 0);
        lv_style_set_pad_all(&style_power, 0);
        lv_style_set_layout(&style_power, LV_LAYOUT_FLEX);
        lv_style_set_flex_main_place(&style_power, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_power, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_power, LV_FLEX_ALIGN_CENTER);
        lv_style_set_bg_color(&style_power_pressed, CONST_DIVIDER);
        lv_style_set_bg_opa(&style_power_pressed, (255 * 100 / 100));
        lv_style_set_image_recolor(&style_power_icon, CONST_TEXT_TERTIARY);
        lv_style_set_image_recolor_opa(&style_power_icon, 255);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, CONST_GUTTER, 12, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "top_bar_#");
        lv_obj_set_width(row_0, CONST_SCREEN_W);
        lv_obj_set_height(row_0, CONST_TOPBAR_H);
        lv_obj_set_style_border_color(row_0, CONST_DIVIDER, 0);
        lv_obj_set_style_border_width(row_0, 1, 0);
        lv_obj_set_style_border_side(row_0, LV_BORDER_SIDE_BOTTOM, 0);
        lv_obj_set_style_border_opa(row_0, (255 * 100 / 100), 0);

        lv_obj_t * row_1 = row_create(row_0, 0, 0, 0, 0, 8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_1, LV_SIZE_CONTENT);
        lv_obj_set_height(row_1, LV_SIZE_CONTENT);
        lv_obj_t * lv_image_0 = lv_image_create(row_1);
        lv_image_set_src(lv_image_0, image_logo);

        lv_obj_t * label_0 = label_create(row_1);
        lv_label_set_text(label_0, title);
        lv_obj_set_style_text_color(label_0, CONST_TEXT_PRIMARY, 0);
        lv_obj_add_style(label_0, &style_title, 0);

        lv_obj_t * row_2 = row_create(row_0, 0, 0, 0, 0, 8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_2, LV_SIZE_CONTENT);
        lv_obj_set_height(row_2, LV_SIZE_CONTENT);
        lv_obj_t * label_1 = label_create(row_2);
        lv_label_bind_text(label_1, &subject_outdoor_temp, "%d° out");
        lv_obj_set_style_text_color(label_1, CONST_TEXT_SECONDARY, 0);
        lv_obj_add_style(label_1, &style_stat_sm, 0);

        lv_obj_t * label_2 = label_create(row_2);
        lv_label_bind_text(label_2, &subject_time, NULL);
        lv_obj_set_style_text_color(label_2, CONST_TEXT_PRIMARY, 0);
        lv_obj_add_style(label_2, &style_stat_sm, 0);

        lv_obj_t * lv_button_0 = lv_button_create(row_2);
        lv_obj_remove_style_all(lv_button_0);
        lv_obj_add_style(lv_button_0, &style_power, 0);
        lv_obj_add_style(lv_button_0, &style_power_pressed, LV_STATE_PRESSED);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_mode, LV_EVENT_CLICKED, REF_MODE_OFF);
        lv_obj_t * lv_image_1 = lv_image_create(lv_button_0);
        lv_image_set_src(lv_image_1, icon_power);
        lv_obj_set_flag(lv_image_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_1, &style_power_icon, 0);
        lv_obj_bind_style(lv_image_1, &style_icon_danger, LV_PART_MAIN, &subject_mode, REF_MODE_OFF);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

