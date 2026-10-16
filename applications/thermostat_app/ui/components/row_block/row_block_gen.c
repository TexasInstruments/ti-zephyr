/**
 * @file row_block_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "row_block_gen.h"
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

lv_obj_t * row_block_create(lv_obj_t * parent, const char * time, const char * name, const char * temp, lv_style_t * tone, const char * note)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_width(&style_base, CONST_CONTENT_W);
        lv_style_set_height(&style_base, 56);
        lv_style_set_radius(&style_base, CONST_RADIUS_MD);
        lv_style_set_bg_color(&style_base, CONST_SURFACE_CARD);
        lv_style_set_bg_opa(&style_base, (255 * 100 / 100));
        lv_style_set_border_color(&style_base, CONST_BORDER);
        lv_style_set_border_width(&style_base, 1);
        lv_style_set_pad_left(&style_base, 14);
        lv_style_set_pad_right(&style_base, 14);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_ROW);
        lv_style_set_flex_main_place(&style_base, LV_FLEX_ALIGN_START);
        lv_style_set_flex_cross_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_base, LV_FLEX_ALIGN_CENTER);
        lv_style_set_pad_column(&style_base, 12);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * card_0 = card_create(parent);
        lv_obj_set_name_static(card_0, "row_block_#");

        lv_obj_add_style(card_0, &style_base, 0);
        lv_obj_add_style(card_0, tone, 0);
        lv_obj_t * label_0 = label_create(card_0);
        lv_label_set_text(label_0, time);
        lv_obj_set_style_text_color(label_0, CONST_TEXT_PRIMARY, 0);
        lv_obj_set_width(label_0, 46);
        lv_obj_add_style(label_0, &style_body_strong, 0);

        lv_obj_t * column_0 = column_create(card_0, 0, 0, 0, 0, 1, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_0, 0);
        lv_obj_set_height(column_0, LV_SIZE_CONTENT);
        lv_obj_t * label_1 = label_create(column_0);
        lv_label_set_text(label_1, name);
        lv_obj_set_style_text_color(label_1, CONST_TEXT_PRIMARY, 0);
        lv_obj_add_style(label_1, &style_body_strong, 0);

        lv_obj_t * label_2 = label_create(column_0);
        lv_label_set_text(label_2, note);
        lv_obj_set_style_text_color(label_2, CONST_TEXT_SECONDARY, 0);
        lv_obj_add_style(label_2, &style_micro, 0);

        lv_obj_t * row_0 = row_create(card_0, 0, 0, 0, 0, 1, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_0, LV_SIZE_CONTENT);
        lv_obj_set_height(row_0, LV_SIZE_CONTENT);
        lv_obj_t * label_3 = label_create(row_0);
        lv_label_set_text(label_3, temp);
        lv_obj_set_style_text_color(label_3, CONST_TEXT_PRIMARY, 0);
        lv_obj_add_style(label_3, &style_stat_md, 0);

        lv_obj_t * label_4 = label_create(row_0);
        lv_label_set_text(label_4, "°");
        lv_obj_set_style_text_color(label_4, CONST_TEXT_SECONDARY, 0);
        lv_obj_add_style(label_4, &style_meta, 0);

        the_root = card_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

