/**
 * @file checkbox_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "checkbox_gen.h"
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

lv_obj_t * checkbox_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_indicator;
    static lv_style_t style_indicator_checked;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_indicator);
        lv_style_init(&style_indicator_checked);

        lv_style_set_text_font(&style_base, font_md);
        lv_style_set_pad_column(&style_base, BASE_LIB_UNIT_MD);
        lv_style_set_border_color(&style_indicator, COLOR_ACCENT);
        lv_style_set_border_width(&style_indicator, 2);
        lv_style_set_bg_color(&style_indicator, COLOR_ACCENT);
        lv_style_set_bg_opa(&style_indicator, (255 * 24 / 100));
        lv_style_set_radius(&style_indicator, BASE_LIB_UNIT_SM);
        lv_style_set_border_color(&style_indicator_checked, COLOR_ACCENT);
        lv_style_set_bg_color(&style_indicator_checked, COLOR_ACCENT);
        lv_style_set_bg_opa(&style_indicator_checked, (255 * 100 / 100));
        lv_style_set_bg_image_src(&style_indicator_checked, icon_check);
        lv_style_set_bg_image_recolor(&style_indicator_checked, COLOR_ACCENT_TEXT);
        lv_style_set_bg_image_recolor_opa(&style_indicator_checked, (255 * 100 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_checkbox_0 = lv_checkbox_create(parent);
        lv_obj_set_name_static(lv_checkbox_0, "checkbox_#");

        lv_obj_remove_style_all(lv_checkbox_0);
        lv_obj_add_style(lv_checkbox_0, &style_base, 0);
        lv_obj_add_style(lv_checkbox_0, &style_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(lv_checkbox_0, &style_indicator_checked, LV_PART_INDICATOR | LV_STATE_CHECKED);

        the_root = lv_checkbox_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

