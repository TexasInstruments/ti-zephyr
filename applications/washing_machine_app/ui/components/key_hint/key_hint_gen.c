/**
 * @file key_hint_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "key_hint_gen.h"
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

lv_obj_t * key_hint_create(lv_obj_t * parent, const char * text, const char * hint, bool hide_hint, lv_style_t * tone, lv_style_t * tone_pressed)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_align(&style_base, LV_ALIGN_BOTTOM_MID);
        lv_style_set_y(&style_base, 24);
        lv_style_set_width(&style_base, 112);
        lv_style_set_height(&style_base, 70);
        lv_style_set_radius(&style_base, CONST_RADIUS_LG);
        lv_style_set_pad_all(&style_base, 0);
        lv_style_set_pad_bottom(&style_base, 24);
        lv_style_set_pad_row(&style_base, 1);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_COLUMN);
        lv_style_set_flex_main_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "key_hint_#");
        lv_obj_set_style_shadow_width(lv_button_0, 0, 0);
        lv_obj_set_style_outline_width(lv_button_0, 0, 0);

        lv_obj_remove_style_all(lv_button_0);
        lv_obj_add_style(lv_button_0, &style_base, 0);
        lv_obj_add_style(lv_button_0, tone, 0);
        lv_obj_add_style(lv_button_0, tone_pressed, LV_STATE_PRESSED);
        lv_obj_t * label_0 = label_create(lv_button_0);
        lv_label_set_text(label_0, hint);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_HIDDEN, hide_hint);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_add_style(label_0, &style_key_hint, 0);
        lv_obj_add_style(label_0, &style_ink_inv, 0);

        lv_obj_t * label_1 = label_create(lv_button_0);
        lv_label_set_text(label_1, text);
        lv_obj_set_flag(label_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(label_1, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_add_style(label_1, &style_key_legend, 0);
        lv_obj_add_style(label_1, &style_ink_inv, 0);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

