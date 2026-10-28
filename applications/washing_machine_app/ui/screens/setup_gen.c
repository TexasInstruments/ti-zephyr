/**
 * @file setup_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "setup_gen.h"
#include "../ti_washing_machine.h"

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

lv_obj_t * setup_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "setup_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        nav_header_create(lv_obj_0, "Options");

        lv_obj_t * btn_back_0 = btn_back_create(lv_obj_0);
        lv_obj_set_x(btn_back_0, 0);
        lv_obj_set_y(btn_back_0, 0);
        lv_obj_add_screen_create_event(btn_back_0, LV_EVENT_CLICKED, home_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_x(lv_obj_1, CONST_GUTTER);
        lv_obj_set_y(lv_obj_1, 68);
        lv_obj_set_width(lv_obj_1, CONST_CONTENT_W);
        lv_obj_set_height(lv_obj_1, LV_SIZE_CONTENT);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(lv_obj_1, 0, 0);
        lv_obj_set_style_border_width(lv_obj_1, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_1, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_1, 0, 0);
        lv_obj_set_style_layout(lv_obj_1, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(lv_obj_1, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_flex_main_place(lv_obj_1, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_1, LV_FLEX_ALIGN_END, 0);
        lv_obj_set_style_flex_track_place(lv_obj_1, LV_FLEX_ALIGN_END, 0);
        lv_obj_t * label_0 = label_create(lv_obj_1);
        lv_label_set_text(label_0, "Spin speed");
        lv_obj_add_style(label_0, &style_body_strong, 0);
        lv_obj_add_style(label_0, &style_ink, 0);

        lv_obj_t * label_1 = label_create(lv_obj_1);
        lv_label_set_text(label_1, "rpm");
        lv_obj_add_style(label_1, &style_micro, 0);
        lv_obj_add_style(label_1, &style_ink_3, 0);

        lv_obj_t * row_0 = row_create(lv_obj_0, 0, 0, 0, 0, 10, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_x(row_0, CONST_GUTTER);
        lv_obj_set_y(row_0, 97);
        pill_spin_create(row_0, "400", 400, 65);

        pill_spin_create(row_0, "600", 600, 65);

        pill_spin_create(row_0, "800", 800, 65);

        pill_spin_create(row_0, "1000", 1000, 65);

        pill_spin_create(row_0, "1200", 1200, 65);

        pill_spin_create(row_0, "1400", 1400, 65);

        lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
        lv_obj_set_x(lv_obj_2, CONST_GUTTER);
        lv_obj_set_y(lv_obj_2, 167);
        lv_obj_set_width(lv_obj_2, CONST_CONTENT_W);
        lv_obj_set_height(lv_obj_2, LV_SIZE_CONTENT);
        lv_obj_set_flag(lv_obj_2, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(lv_obj_2, 0, 0);
        lv_obj_set_style_border_width(lv_obj_2, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_2, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_2, 0, 0);
        lv_obj_set_style_layout(lv_obj_2, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(lv_obj_2, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_flex_main_place(lv_obj_2, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_2, LV_FLEX_ALIGN_END, 0);
        lv_obj_set_style_flex_track_place(lv_obj_2, LV_FLEX_ALIGN_END, 0);
        lv_obj_t * label_2 = label_create(lv_obj_2);
        lv_label_set_text(label_2, "Water level");
        lv_obj_add_style(label_2, &style_body_strong, 0);
        lv_obj_add_style(label_2, &style_ink, 0);

        lv_obj_t * row_1 = row_create(lv_obj_2, 0, 0, 0, 0, 2, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END);
        lv_obj_t * label_3 = label_create(row_1);
        lv_label_set_text(label_3, "62");
        lv_label_bind_text(label_3, &subject_water_target, "%d");
        lv_obj_add_style(label_3, &style_body_strong, 0);
        lv_obj_add_style(label_3, &style_ink, 0);

        lv_obj_t * label_4 = label_create(row_1);
        lv_label_set_text(label_4, "%");
        lv_obj_add_style(label_4, &style_micro, 0);
        lv_obj_add_style(label_4, &style_ink_3, 0);

        lv_obj_t * slider_water_0 = slider_water_create(lv_obj_0);
        lv_obj_set_x(slider_water_0, CONST_GUTTER);
        lv_obj_set_y(slider_water_0, 200);

        lv_obj_t * row_2 = row_create(lv_obj_0, 0, 0, 0, 0, 10, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_x(row_2, CONST_GUTTER);
        lv_obj_set_y(row_2, 240);
        row_option_create(row_2, "Prewash", "Adds 18 min", &subject_opt_prewash, icon_opt_prewash, 140);

        row_option_create(row_2, "Extra rinse", "Adds 12 min", &subject_opt_rinse, icon_opt_rinse, 140);

        row_option_create(row_2, "Delay start", "Waits 3 hrs", &subject_opt_delay, icon_opt_delay, 140);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

