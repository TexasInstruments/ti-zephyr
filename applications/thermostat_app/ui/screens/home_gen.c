/**
 * @file home_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "home_gen.h"
#include "../ti_thermostat.h"

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

lv_obj_t * home_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "home_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_bg_color(lv_obj_0, CONST_SURFACE_CANVAS, 0);
        lv_obj_set_style_bg_opa(lv_obj_0, (255 * 100 / 100), 0);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_bind_style(lv_obj_0, &style_bg_idle, LV_PART_MAIN, &subject_hvac_state, REF_HVAC_IDLE);
        lv_obj_bind_style(lv_obj_0, &style_bg_heat, LV_PART_MAIN, &subject_hvac_state, REF_HVAC_HEATING);
        lv_obj_bind_style(lv_obj_0, &style_bg_cool, LV_PART_MAIN, &subject_hvac_state, REF_HVAC_COOLING);
        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_tab, LV_EVENT_SCREEN_LOADED, REF_TAB_HOME);
        top_bar_create(lv_obj_0, "Living room");

        lv_obj_t * column_0 = column_create(lv_obj_0, 0, 0, 0, 0, 6, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_0, CONST_SCREEN_W);
        lv_obj_set_height(column_0, LV_SIZE_CONTENT);
        lv_obj_set_y(column_0, 50);
        lv_obj_t * container_0 = container_create(column_0);
        lv_obj_set_width(container_0, CONST_CONTENT_W);
        lv_obj_set_height(container_0, 26);
        lv_obj_t * chip_value_0 = chip_value_create(container_0, &style_chip_fill_heat, &style_chip_dot_heat, &style_chip_text_heat, &subject_target_temp, "Heating to %d°", "Heating to 22°");
        lv_obj_set_align(chip_value_0, LV_ALIGN_CENTER);
        lv_obj_bind_flag_if_not_eq(chip_value_0, &subject_hvac_state, LV_OBJ_FLAG_HIDDEN, REF_HVAC_HEATING);

        lv_obj_t * chip_value_1 = chip_value_create(container_0, &style_chip_fill_cool, &style_chip_dot_cool, &style_chip_text_cool, &subject_target_temp, "Cooling to %d°", "Cooling to 21°");
        lv_obj_set_align(chip_value_1, LV_ALIGN_CENTER);
        lv_obj_bind_flag_if_not_eq(chip_value_1, &subject_hvac_state, LV_OBJ_FLAG_HIDDEN, REF_HVAC_COOLING);

        lv_obj_t * chip_value_2 = chip_value_create(container_0, &style_chip_fill_neutral, &style_chip_dot_neutral, &style_chip_text_neutral, &subject_target_temp, "System off", "System off");
        lv_obj_set_align(chip_value_2, LV_ALIGN_CENTER);
        lv_obj_bind_flag_if_not_eq(chip_value_2, &subject_hvac_state, LV_OBJ_FLAG_HIDDEN, REF_HVAC_IDLE);

        dial_create(column_0);

        lv_obj_t * row_0 = row_create(column_0, 0, 0, 0, 0, 8, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_0, CONST_CONTENT_W);
        lv_obj_set_height(row_0, 40);
        mode_pill_heat_create(row_0);

        mode_pill_cool_create(row_0);

        mode_pill_auto_create(row_0);

        lv_obj_t * tab_bar_0 = tab_bar_create(lv_obj_0);
        lv_obj_set_y(tab_bar_0, 424);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

