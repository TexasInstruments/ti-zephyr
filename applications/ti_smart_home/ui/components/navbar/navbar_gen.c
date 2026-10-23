/**
 * @file navbar_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "navbar_gen.h"
#include "../../ti_smart_home.h"

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

lv_obj_t * navbar_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_tab;
    static lv_style_t style_tab_active;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_tab);
        lv_style_init(&style_tab_active);

        lv_style_set_width(&style_base, lv_pct(100));
        lv_style_set_height(&style_base, 96);
        lv_style_set_bg_color(&style_base, COLOR_BG);
        lv_style_set_bg_opa(&style_base, (255 * 100 / 100));
        lv_style_set_border_side(&style_base, LV_BORDER_SIDE_TOP);
        lv_style_set_border_width(&style_base, 1);
        lv_style_set_border_color(&style_base, COLOR_TEXT);
        lv_style_set_border_opa(&style_base, (255 * 10 / 100));
        lv_style_set_radius(&style_base, 0);
        lv_style_set_pad_all(&style_base, 0);
        lv_style_set_height(&style_tab, lv_pct(100));
        lv_style_set_bg_opa(&style_tab, (255 * 0 / 100));
        lv_style_set_border_width(&style_tab, 0);
        lv_style_set_radius(&style_tab, 0);
        lv_style_set_pad_hor(&style_tab, BASE_LIB_UNIT_LG);
        lv_style_set_text_color(&style_tab_active, COLOR_ACCENT);
        lv_style_set_image_recolor(&style_tab_active, COLOR_ACCENT);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 0, 0, 0, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(row_0, "navbar_#");
        lv_obj_set_flag(row_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(row_0, &style_base, 0);
        lv_obj_t * lv_button_0 = lv_button_create(row_0);
        lv_obj_remove_style_all(lv_button_0);
        lv_obj_add_style(lv_button_0, &style_tab, 0);
        lv_obj_add_style(lv_button_0, &style_button_pressed, LV_STATE_PRESSED);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_nav_index, LV_EVENT_CLICKED, REF_NAV_SMART_HOME);
        lv_obj_add_screen_create_event(lv_button_0, LV_EVENT_CLICKED, screen_smart_home_create, LV_SCREEN_LOAD_ANIM_FADE_IN, 200, 0);
        lv_obj_bind_flag_if_not_eq(lv_button_0, &subject_nav_index, LV_OBJ_FLAG_CLICKABLE, REF_NAV_SMART_HOME);
        lv_obj_t * column_0 = column_create(lv_button_0, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column_0, lv_pct(100));
        lv_obj_set_flag(column_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(column_0, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_t * image_mono_0 = image_mono_create(column_0, icon_smart_home);
        lv_obj_bind_style(image_mono_0, &style_tab_active, 0, &subject_nav_index, REF_NAV_SMART_HOME);

        lv_obj_t * text_0 = text_create(column_0, font_md);
        lv_label_set_text(text_0, "Smart Home");
        lv_obj_bind_style(text_0, &style_tab_active, 0, &subject_nav_index, REF_NAV_SMART_HOME);

        lv_obj_t * lv_button_1 = lv_button_create(row_0);
        lv_obj_remove_style_all(lv_button_1);
        lv_obj_add_style(lv_button_1, &style_tab, 0);
        lv_obj_add_style(lv_button_1, &style_button_pressed, LV_STATE_PRESSED);
        lv_obj_add_subject_set_int_event(lv_button_1, &subject_nav_index, LV_EVENT_CLICKED, REF_NAV_EV);
        lv_obj_add_screen_create_event(lv_button_1, LV_EVENT_CLICKED, screen_ev_charging_create, LV_SCREEN_LOAD_ANIM_FADE_IN, 200, 0);
        lv_obj_bind_flag_if_not_eq(lv_button_1, &subject_nav_index, LV_OBJ_FLAG_CLICKABLE, REF_NAV_EV);
        lv_obj_t * column_1 = column_create(lv_button_1, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column_1, lv_pct(100));
        lv_obj_set_flag(column_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(column_1, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_t * image_mono_1 = image_mono_create(column_1, icon_charge);
        lv_obj_bind_style(image_mono_1, &style_tab_active, 0, &subject_nav_index, REF_NAV_EV);

        lv_obj_t * text_1 = text_create(column_1, font_md);
        lv_label_set_text(text_1, "EV Charging");
        lv_obj_bind_style(text_1, &style_tab_active, 0, &subject_nav_index, REF_NAV_EV);

        lv_obj_t * lv_button_2 = lv_button_create(row_0);
        lv_obj_remove_style_all(lv_button_2);
        lv_obj_add_style(lv_button_2, &style_tab, 0);
        lv_obj_add_style(lv_button_2, &style_button_pressed, LV_STATE_PRESSED);
        lv_obj_add_subject_set_int_event(lv_button_2, &subject_nav_index, LV_EVENT_CLICKED, REF_NAV_MESSAGES);
        lv_obj_add_screen_create_event(lv_button_2, LV_EVENT_CLICKED, screen_messages_create, LV_SCREEN_LOAD_ANIM_FADE_IN, 200, 0);
        lv_obj_bind_flag_if_not_eq(lv_button_2, &subject_nav_index, LV_OBJ_FLAG_CLICKABLE, REF_NAV_MESSAGES);
        lv_obj_t * column_2 = column_create(lv_button_2, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column_2, lv_pct(100));
        lv_obj_set_flag(column_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(column_2, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_t * image_mono_2 = image_mono_create(column_2, icon_messages);
        lv_obj_bind_style(image_mono_2, &style_tab_active, 0, &subject_nav_index, REF_NAV_MESSAGES);

        lv_obj_t * text_2 = text_create(column_2, font_md);
        lv_label_set_text(text_2, "Messages");
        lv_obj_bind_style(text_2, &style_tab_active, 0, &subject_nav_index, REF_NAV_MESSAGES);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

