/**
 * @file chip_value_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "chip_value_gen.h"
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

lv_obj_t * chip_value_create(lv_obj_t * parent, lv_style_t * tone, lv_style_t * tone_dot, lv_style_t * tone_text, lv_subject_t * value, const char * fmt, const char * placeholder)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * card_0 = card_create(parent);
        lv_obj_set_name_static(card_0, "chip_value_#");
        lv_obj_set_flex_flow(card_0, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_cross_place(card_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_track_place(card_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_left(card_0, 10, 0);
        lv_obj_set_style_pad_right(card_0, 10, 0);
        lv_obj_set_style_pad_top(card_0, 5, 0);
        lv_obj_set_style_pad_bottom(card_0, 5, 0);
        lv_obj_set_style_pad_column(card_0, 6, 0);
        lv_obj_set_width(card_0, LV_SIZE_CONTENT);
        lv_obj_set_height(card_0, LV_SIZE_CONTENT);
        lv_obj_set_style_radius(card_0, CONST_RADIUS_PILL, 0);

        lv_obj_add_style(card_0, tone, 0);
        lv_obj_t * card_1 = card_create(card_0);
        lv_obj_set_width(card_1, 6);
        lv_obj_set_height(card_1, 6);
        lv_obj_set_style_radius(card_1, 32767, 0);
        lv_obj_add_style(card_1, tone_dot, 0);

        lv_obj_t * label_0 = label_create(card_0);
        lv_label_set_text(label_0, placeholder);
        lv_label_bind_text(label_0, value, fmt);
        lv_obj_add_style(label_0, &style_chip, 0);
        lv_obj_add_style(label_0, tone_text, 0);

        the_root = card_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

