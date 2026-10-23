/**
 * @file header_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "header_gen.h"
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

lv_obj_t * header_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_logo_button;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_logo_button);

        lv_style_set_width(&style_base, lv_pct(100));
        lv_style_set_height(&style_base, 40);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_width(&style_logo_button, LV_SIZE_CONTENT);
        lv_style_set_height(&style_logo_button, lv_pct(100));
        lv_style_set_bg_opa(&style_logo_button, (255 * 0 / 100));
        lv_style_set_border_width(&style_logo_button, 0);
        lv_style_set_pad_all(&style_logo_button, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 0, BASE_LIB_UNIT_MD, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "header_#");
        lv_obj_set_flag(row_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(row_0, &style_base, 0);
        lv_obj_add_style(row_0, &style_text, 0);
        lv_obj_t * lv_button_0 = lv_button_create(row_0);
        lv_obj_remove_style_all(lv_button_0);
        lv_obj_add_style(lv_button_0, &style_logo_button, 0);
        lv_obj_add_style(lv_button_0, &style_button_pressed, LV_STATE_PRESSED);
        lv_obj_add_subject_set_int_event(lv_button_0, &subject_nav_index, LV_EVENT_CLICKED, REF_NAV_HOME);
        lv_obj_add_screen_create_event(lv_button_0, LV_EVENT_CLICKED, screen_home_create, LV_SCREEN_LOAD_ANIM_FADE_IN, 200, 0);
        lv_obj_t * image_mono_0 = image_mono_create(lv_button_0, logostrip_small);
        lv_obj_set_flag(image_mono_0, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_set_flag(image_mono_0, LV_OBJ_FLAG_CLICKABLE, false);

        spacer_create(row_0);

        time_create(row_0, &subject_hour, &subject_minute, font_sm);

        date_create(row_0, &subject_day_name, &subject_day_index, &subject_year, &subject_month_name, font_sm);

        image_mono_create(row_0, icon_wifi);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

