/**
 * @file slider_water_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "slider_water_gen.h"
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

lv_obj_t * slider_water_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_main;
    static lv_style_t style_indicator;
    static lv_style_t style_knob;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_main);
        lv_style_init(&style_indicator);
        lv_style_init(&style_knob);

        lv_style_set_width(&style_main, CONST_CONTENT_W);
        lv_style_set_height(&style_main, 20);
        lv_style_set_bg_color(&style_main, CONST_TRACK);
        lv_style_set_bg_opa(&style_main, (255 * 100 / 100));
        lv_style_set_radius(&style_main, 32767);
        lv_style_set_border_width(&style_main, 0);
        lv_style_set_pad_all(&style_main, 0);
        lv_style_set_bg_color(&style_indicator, CONST_ACCENT);
        lv_style_set_bg_opa(&style_indicator, (255 * 100 / 100));
        lv_style_set_radius(&style_indicator, 32767);
        lv_style_set_bg_color(&style_knob, CONST_ON_ACCENT);
        lv_style_set_bg_opa(&style_knob, (255 * 100 / 100));
        lv_style_set_radius(&style_knob, 32767);
        lv_style_set_pad_all(&style_knob, 4);
        lv_style_set_border_color(&style_knob, CONST_BORDER);
        lv_style_set_border_width(&style_knob, 1);
        lv_style_set_border_opa(&style_knob, (255 * 100 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_slider_0 = lv_slider_create(parent);
        lv_obj_set_name_static(lv_slider_0, "slider_water_#");
        lv_slider_set_min_value(lv_slider_0, 0);
        lv_slider_set_max_value(lv_slider_0, 100);
        lv_slider_bind_value(lv_slider_0, &subject_water_target);
        lv_obj_set_ext_click_area(lv_slider_0, 18);

        lv_obj_remove_style_all(lv_slider_0);
        lv_obj_add_style(lv_slider_0, &style_main, 0);
        lv_obj_add_style(lv_slider_0, &style_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(lv_slider_0, &style_knob, LV_PART_KNOB);

        the_root = lv_slider_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

