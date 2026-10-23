/**
 * @file list_message_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "list_message_gen.h"
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

lv_obj_t * list_message_create(lv_obj_t * parent, const void * thumbnail, const char * time, const char * title, const char * message)
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
        lv_obj_t * row_0 = row_create(parent, 12, 12, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(row_0, "list_message_#");
        lv_obj_set_height(row_0, LV_SIZE_CONTENT);
        lv_obj_set_style_pad_left(row_0, 0, 0);

        lv_obj_add_style(row_0, &style_base, 0);
        lv_obj_t * image_0 = image_create(row_0, thumbnail);
        lv_obj_add_style(image_0, &style_thumbnail, 0);

        lv_obj_t * column_0 = column_create(row_0, 0, -4, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column_0, LV_SIZE_CONTENT);
        lv_obj_t * h4_0 = h4_create(column_0, title);
        lv_obj_set_width(h4_0, lv_pct(100));
        lv_obj_set_height(h4_0, LV_SIZE_CONTENT);
        lv_obj_set_style_text_line_space(h4_0, -6, 0);

        lv_obj_t * text_0 = text_create(column_0, font_md);
        lv_obj_set_width(text_0, lv_pct(100));
        lv_obj_set_height(text_0, LV_SIZE_CONTENT);
        lv_label_set_text(text_0, message);
        lv_obj_set_style_text_opa(text_0, (255 * 64 / 100), 0);
        lv_obj_set_style_text_line_space(text_0, -6, 0);

        lv_obj_t * text_1 = text_create(column_0, font_xs);
        lv_obj_set_width(text_1, lv_pct(100));
        lv_obj_set_height(text_1, LV_SIZE_CONTENT);
        lv_label_set_text(text_1, time);
        lv_obj_set_style_pad_top(text_1, 8, 0);
        lv_obj_set_style_text_opa(text_1, (255 * 40 / 100), 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

