/**
 * @file pill_spin_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "pill_spin_gen.h"
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

lv_obj_t * pill_spin_create(lv_obj_t * parent, const char * text, int32_t value, int32_t w)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_height(&style_base, 46);
        lv_style_set_radius(&style_base, 32767);
        lv_style_set_pad_all(&style_base, 0);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_ROW);
        lv_style_set_flex_main_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "pill_spin_#");
        lv_obj_set_width(lv_button_0, w);
        lv_obj_set_flag(lv_button_0, LV_OBJ_FLAG_STATE_TRICKLE, true);
        lv_obj_set_style_shadow_width(lv_button_0, 0, 0);
        lv_obj_set_style_outline_width(lv_button_0, 0, 0);

        lv_obj_add_style(lv_button_0, &style_base, 0);
        lv_obj_add_style(lv_button_0, &style_card_surface, 0);
        lv_obj_add_style(lv_button_0, &style_btn_ghost_pressed, LV_STATE_PRESSED);
        lv_obj_add_style(lv_button_0, &style_card_selected, LV_STATE_CHECKED);
        lv_obj_add_style(lv_button_0, &style_card_selected_pressed, LV_STATE_CHECKED | LV_STATE_PRESSED);
        lv_obj_add_style(lv_button_0, &style_pill, 0);
        lv_obj_t * label_0 = label_create(lv_button_0);
        lv_label_set_text(label_0, text);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_body_strong, 0);
        lv_obj_add_style(label_0, &style_ink, 0);
        lv_obj_add_style(label_0, &style_ink_inv, LV_STATE_CHECKED);

        lv_obj_bind_state_if_eq(lv_button_0, &subject_spin_rpm, LV_STATE_CHECKED, value);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_spin_rpm, LV_EVENT_CLICKED, value);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

