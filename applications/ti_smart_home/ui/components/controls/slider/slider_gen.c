/**
 * @file slider_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "slider_gen.h"
#include "../../../ti_smart_home.h"

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

lv_obj_t * slider_create(lv_obj_t * parent, lv_subject_t * subject)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_slider;
    static lv_style_t style_slider_indicator;
    static lv_style_t style_slider_knob;
    static lv_style_t style_slider_knob_pressed;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_slider);
        lv_style_init(&style_slider_indicator);
        lv_style_init(&style_slider_knob);
        lv_style_init(&style_slider_knob_pressed);

        lv_style_set_height(&style_slider, BASE_LIB_UNIT_LG);
        lv_style_set_bg_color(&style_slider, COLOR_ACCENT);
        lv_style_set_bg_opa(&style_slider, (255 * 16 / 100));
        lv_style_set_radius(&style_slider, BASE_LIB_RADIUS_MD);
        lv_style_set_bg_color(&style_slider_indicator, COLOR_ACCENT);
        lv_style_set_bg_opa(&style_slider_indicator, (255 * 100 / 100));
        lv_style_set_radius(&style_slider_indicator, BASE_LIB_RADIUS_MD);
        lv_style_set_bg_color(&style_slider_knob, COLOR_ACCENT_TEXT);
        lv_style_set_pad_left(&style_slider_knob, 3);
        lv_style_set_pad_right(&style_slider_knob, -10);
        lv_style_set_pad_ver(&style_slider_knob, -4);
        lv_style_set_transform_height(&style_slider_knob_pressed, 0);
        lv_style_set_transform_width(&style_slider_knob_pressed, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_slider_0 = lv_slider_create(parent);
        lv_obj_set_name_static(lv_slider_0, "slider_#");
        lv_slider_set_value(lv_slider_0, 50, false);
        lv_slider_bind_value(lv_slider_0, subject);

        lv_obj_add_style(lv_slider_0, &style_slider, 0);
        lv_obj_add_style(lv_slider_0, &style_slider_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(lv_slider_0, &style_slider_knob, LV_PART_KNOB);
        lv_obj_add_style(lv_slider_0, &style_slider_knob_pressed, LV_PART_KNOB | LV_STATE_PRESSED);

        the_root = lv_slider_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

