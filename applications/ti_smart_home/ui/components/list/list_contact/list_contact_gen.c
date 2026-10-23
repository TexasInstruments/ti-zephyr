/**
 * @file list_contact_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "list_contact_gen.h"
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

lv_obj_t * list_contact_create(lv_obj_t * parent, const void * thumbnail, const char * contact_name, const char * email, const char * phone)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_thumbnail;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_thumbnail);

        lv_style_set_height(&style_base, LV_SIZE_CONTENT);
        lv_style_set_width(&style_base, lv_pct(100));
        lv_style_set_bg_opa(&style_base, (255 * 0 / 100));
        lv_style_set_border_side(&style_base, LV_BORDER_SIDE_BOTTOM);
        lv_style_set_border_width(&style_base, 1);
        lv_style_set_border_opa(&style_base, (255 * 12 / 100));
        lv_style_set_width(&style_thumbnail, 40);
        lv_style_set_height(&style_thumbnail, 40);
        lv_style_set_radius(&style_thumbnail, 20);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 12, 12, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(row_0, "list_contact_#");
        lv_obj_set_height(row_0, LV_SIZE_CONTENT);
        lv_obj_set_style_pad_left(row_0, 0, 0);
        lv_obj_set_flag(row_0, LV_OBJ_FLAG_CLICKABLE, true);

        lv_obj_add_style(row_0, &style_base, 0);
        lv_obj_t * image_0 = image_create(row_0, thumbnail);
        lv_obj_add_style(image_0, &style_thumbnail, 0);

        lv_obj_t * column_0 = column_create(row_0, 0, -6, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(column_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * h3_0 = h3_create(column_0, contact_name);
        lv_obj_set_width(h3_0, lv_pct(100));
        lv_obj_set_height(h3_0, LV_SIZE_CONTENT);
        lv_obj_set_flag(h3_0, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK, true);
        lv_label_set_long_mode(h3_0, LV_LABEL_LONG_MODE_DOTS);
        lv_obj_set_style_text_line_space(h3_0, -8, 0);

        lv_obj_t * h4_0 = h4_create(column_0, email);
        lv_obj_set_width(h4_0, lv_pct(100));
        lv_obj_set_height(h4_0, LV_SIZE_CONTENT);
        lv_obj_set_style_text_opa(h4_0, (255 * 60 / 100), 0);
        lv_obj_set_style_text_line_space(h4_0, -6, 0);
        lv_label_set_long_mode(h4_0, LV_LABEL_LONG_MODE_DOTS);

        lv_obj_add_subject_set_string_event(row_0, &subject_contact_email, LV_EVENT_CLICKED, email);
        lv_obj_add_subject_set_string_event(row_0, &subject_contact_name, LV_EVENT_CLICKED, contact_name);
        lv_obj_add_subject_set_string_event(row_0, &subject_contact_phone, LV_EVENT_CLICKED, phone);
        lv_obj_add_event_cb(row_0, on_contact_clicked_cb, LV_EVENT_CLICKED, (void *)email);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

