/**
 * @file tab_item_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "tab_item_gen.h"
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

lv_obj_t * tab_item_create(lv_obj_t * parent, const char * text, int32_t index)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_width(&style_base, 72);
        lv_style_set_height(&style_base, CONST_TABBAR_H);
        lv_style_set_bg_opa(&style_base, 0);
        lv_style_set_border_width(&style_base, 0);
        lv_style_set_shadow_width(&style_base, 0);
        lv_style_set_radius(&style_base, 0);
        lv_style_set_pad_all(&style_base, 0);
        lv_style_set_pad_row(&style_base, 9);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_COLUMN);
        lv_style_set_flex_main_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "tab_item_#");

        lv_obj_remove_style_all(lv_button_0);
        lv_obj_add_style(lv_button_0, &style_base, 0);
        lv_obj_bind_flag_if_not_eq(lv_button_0, &subject_tab, LV_OBJ_FLAG_CLICKABLE, index);
        lv_obj_t * card_0 = card_create(lv_button_0);
        lv_obj_set_width(card_0, 18);
        lv_obj_set_height(card_0, 3);
        lv_obj_set_style_radius(card_0, 32767, 0);
        lv_obj_set_style_bg_color(card_0, CONST_TEXT_PRIMARY, 0);
        lv_obj_set_style_bg_opa(card_0, (255 * 100 / 100), 0);
        lv_obj_bind_flag_if_not_eq(card_0, &subject_tab, LV_OBJ_FLAG_HIDDEN, index);

        lv_obj_t * label_0 = label_create(lv_button_0);
        lv_label_set_text(label_0, text);
        lv_obj_set_style_text_color(label_0, CONST_TEXT_TERTIARY, 0);
        lv_obj_add_style(label_0, &style_badge, 0);
        lv_obj_bind_style(label_0, &style_tab_active, LV_PART_MAIN, &subject_tab, index);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

