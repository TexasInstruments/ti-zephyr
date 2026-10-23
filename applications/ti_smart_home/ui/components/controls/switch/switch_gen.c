/**
 * @file switch_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "switch_gen.h"
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

lv_obj_t * switch_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_indicator_checked;
    static lv_style_t style_knob;
    static lv_style_t style_knob_checked;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_indicator_checked);
        lv_style_init(&style_knob);
        lv_style_init(&style_knob_checked);

        lv_style_set_bg_color(&style_base, lv_color_hex(0x0E3E02));
        lv_style_set_bg_opa(&style_base, (255 * 24 / 100));
        lv_style_set_bg_color(&style_indicator_checked, lv_color_hex(0x0E3E02));
        lv_style_set_bg_opa(&style_indicator_checked, (255 * 100 / 100));
        lv_style_set_bg_color(&style_knob, lv_color_hex(0x0E3E02));
        lv_style_set_pad_all(&style_knob, -4);
        lv_style_set_bg_color(&style_knob_checked, COLOR_ACCENT_TEXT);
        lv_style_set_bg_opa(&style_knob_checked, (255 * 100 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_switch_0 = lv_switch_create(parent);
        lv_obj_set_name_static(lv_switch_0, "switch_#");
        lv_obj_set_height(lv_switch_0, 28);
        lv_obj_set_width(lv_switch_0, 48);

        lv_obj_add_style(lv_switch_0, &style_base, 0);
        lv_obj_add_style(lv_switch_0, &style_knob, LV_PART_KNOB);
        lv_obj_add_style(lv_switch_0, &style_indicator_checked, LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_style(lv_switch_0, &style_knob_checked, LV_PART_KNOB | LV_STATE_CHECKED);

        the_root = lv_switch_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

