/**
 * @file mode_pill_auto_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "mode_pill_auto_gen.h"
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

lv_obj_t * mode_pill_auto_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_pressed;
    static lv_style_t style_icon;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_pressed);
        lv_style_init(&style_icon);

        lv_style_set_width(&style_base, 88);
        lv_style_set_height(&style_base, 36);
        lv_style_set_radius(&style_base, 32767);
        lv_style_set_bg_color(&style_base, CONST_SURFACE_CARD);
        lv_style_set_bg_opa(&style_base, (255 * 100 / 100));
        lv_style_set_border_color(&style_base, CONST_BORDER);
        lv_style_set_border_width(&style_base, 1);
        lv_style_set_pad_all(&style_base, 0);
        lv_style_set_pad_column(&style_base, 6);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_ROW);
        lv_style_set_flex_main_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_bg_color(&style_pressed, CONST_DIVIDER);
        lv_style_set_image_recolor(&style_icon, CONST_TEXT_SECONDARY);
        lv_style_set_image_recolor_opa(&style_icon, 255);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "mode_pill_auto_#");
        lv_obj_set_flag(lv_button_0, LV_OBJ_FLAG_STATE_TRICKLE, true);

        lv_obj_remove_style_all(lv_button_0);
        lv_obj_add_style(lv_button_0, &style_base, 0);
        lv_obj_add_style(lv_button_0, &style_pressed, LV_STATE_PRESSED);
        lv_obj_add_style(lv_button_0, &style_mode_on_auto, LV_STATE_CHECKED);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_mode, LV_EVENT_CLICKED, REF_MODE_AUTO);
        lv_obj_bind_state_if_eq(lv_button_0, &subject_mode, LV_STATE_CHECKED, REF_MODE_AUTO);
        lv_obj_t * lv_image_0 = lv_image_create(lv_button_0);
        lv_image_set_src(lv_image_0, icon_auto);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon, 0);
        lv_obj_add_style(lv_image_0, &style_icon_ink, LV_STATE_CHECKED);

        lv_obj_t * label_0 = label_create(lv_button_0);
        lv_label_set_text(label_0, "Auto");
        lv_obj_set_style_text_color(label_0, CONST_TEXT_SECONDARY, 0);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_pad_top(label_0, 3, 0);
        lv_obj_add_style(label_0, &style_body_strong, 0);
        lv_obj_add_style(label_0, &style_mode_label_auto, LV_STATE_CHECKED);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

