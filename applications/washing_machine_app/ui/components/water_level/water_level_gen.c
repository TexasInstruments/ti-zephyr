/**
 * @file water_level_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "water_level_gen.h"
#include "../../ti_washing_machine.h"

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

lv_obj_t * water_level_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_box;
    static lv_style_t style_lane;
    static lv_style_t style_track;
    static lv_style_t style_fill;
    static lv_style_t style_tick;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_box);
        lv_style_init(&style_lane);
        lv_style_init(&style_track);
        lv_style_init(&style_fill);
        lv_style_init(&style_tick);

        lv_style_set_width(&style_box, 14);
        lv_style_set_height(&style_box, 180);
        lv_style_set_bg_opa(&style_box, 0);
        lv_style_set_border_width(&style_box, 0);
        lv_style_set_pad_all(&style_box, 0);
        lv_style_set_x(&style_lane, 4);
        lv_style_set_width(&style_lane, 8);
        lv_style_set_height(&style_lane, 180);
        lv_style_set_bg_color(&style_track, lv_color_hex(0xD5DBDF));
        lv_style_set_bg_opa(&style_track, (255 * 100 / 100));
        lv_style_set_radius(&style_track, 32767);
        lv_style_set_border_width(&style_track, 0);
        lv_style_set_pad_all(&style_track, 0);
        lv_style_set_bg_color(&style_fill, CONST_ACCENT);
        lv_style_set_bg_opa(&style_fill, (255 * 100 / 100));
        lv_style_set_radius(&style_fill, 32767);
        lv_style_set_bg_color(&style_tick, CONST_TEXT_PRIMARY);
        lv_style_set_bg_opa(&style_tick, (255 * 100 / 100));
        lv_style_set_radius(&style_tick, 1);
        lv_style_set_border_width(&style_tick, 0);
        lv_style_set_pad_left(&style_tick, 4);
        lv_style_set_pad_right(&style_tick, 2);
        lv_style_set_pad_top(&style_tick, -3);
        lv_style_set_pad_bottom(&style_tick, -3);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "water_level_#");
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_remove_style_all(lv_obj_0);
        lv_obj_add_style(lv_obj_0, &style_box, 0);
        lv_obj_t * lv_bar_0 = lv_bar_create(lv_obj_0);
        lv_bar_set_min_value(lv_bar_0, 0);
        lv_bar_set_max_value(lv_bar_0, 100);
        lv_bar_set_orientation(lv_bar_0, LV_BAR_ORIENTATION_VERTICAL);
        lv_bar_bind_value(lv_bar_0, &subject_water_level);
        lv_obj_remove_style_all(lv_bar_0);
        lv_obj_add_style(lv_bar_0, &style_lane, 0);
        lv_obj_add_style(lv_bar_0, &style_track, 0);
        lv_obj_add_style(lv_bar_0, &style_fill, LV_PART_INDICATOR);

        lv_obj_t * lv_slider_0 = lv_slider_create(lv_obj_0);
        lv_slider_set_min_value(lv_slider_0, 0);
        lv_slider_set_max_value(lv_slider_0, 100);
        lv_slider_set_orientation(lv_slider_0, LV_SLIDER_ORIENTATION_VERTICAL);
        lv_slider_bind_value(lv_slider_0, &subject_water_target);
        lv_obj_set_flag(lv_slider_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_remove_style_all(lv_slider_0);
        lv_obj_add_style(lv_slider_0, &style_lane, 0);
        lv_obj_add_style(lv_slider_0, &style_tick, LV_PART_KNOB);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

