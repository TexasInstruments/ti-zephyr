/**
 * @file row_option_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "row_option_gen.h"
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

lv_obj_t * row_option_create(lv_obj_t * parent, const char * title, const char * subtitle, lv_subject_t * value, const void * icon, int32_t w)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_edge;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_edge);

        lv_style_set_pad_top(&style_base, 8);
        lv_style_set_pad_bottom(&style_base, 8);
        lv_style_set_pad_left(&style_base, 5);
        lv_style_set_pad_right(&style_base, 5);
        lv_style_set_pad_column(&style_base, 10);
        lv_style_set_radius(&style_base, CONST_RADIUS_LG);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_ROW);
        lv_style_set_flex_main_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_border_color(&style_edge, CONST_BORDER);
        lv_style_set_border_width(&style_edge, 2);
        lv_style_set_border_opa(&style_edge, (255 * 100 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "row_option_#");
        lv_obj_set_width(lv_button_0, w);
        lv_obj_set_height(lv_button_0, LV_SIZE_CONTENT);
        lv_obj_set_flag(lv_button_0, LV_OBJ_FLAG_STATE_TRICKLE, true);
        lv_obj_set_style_shadow_width(lv_button_0, 0, 0);
        lv_obj_set_style_outline_width(lv_button_0, 0, 0);

        lv_obj_add_style(lv_button_0, &style_base, 0);
        lv_obj_add_style(lv_button_0, &style_card_surface, 0);
        lv_obj_add_style(lv_button_0, &style_edge, 0);
        lv_obj_add_style(lv_button_0, &style_btn_ghost_pressed, LV_STATE_PRESSED);
        lv_obj_add_style(lv_button_0, &style_card_selected_edge, LV_STATE_CHECKED);
        lv_obj_t * lv_image_0 = lv_image_create(lv_button_0);
        lv_image_set_src(lv_image_0, icon);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_3, 0);
        lv_obj_add_style(lv_image_0, &style_icon_accent, LV_STATE_CHECKED);

        lv_obj_t * column_0 = column_create(lv_button_0, 0, 0, 0, 0, 1, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(column_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * label_0 = label_create(column_0);
        lv_label_set_text(label_0, title);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_body_strong, 0);
        lv_obj_add_style(label_0, &style_ink, 0);

        lv_obj_t * label_1 = label_create(column_0);
        lv_label_set_text(label_1, subtitle);
        lv_obj_set_flag(label_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_1, &style_micro, 0);
        lv_obj_add_style(label_1, &style_ink_3, 0);

        lv_obj_bind_state_if_eq(lv_button_0, value, LV_STATE_CHECKED, REF_ON);
        lv_obj_add_subject_toggle_event(lv_button_0, value, LV_EVENT_CLICKED);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

