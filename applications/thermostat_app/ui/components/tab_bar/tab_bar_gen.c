/**
 * @file tab_bar_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "tab_bar_gen.h"
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

lv_obj_t * tab_bar_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "tab_bar_#");
        lv_obj_set_width(row_0, CONST_SCREEN_W);
        lv_obj_set_height(row_0, CONST_TABBAR_H);
        lv_obj_set_style_bg_color(row_0, CONST_SURFACE_CARD, 0);
        lv_obj_set_style_bg_opa(row_0, (255 * 95 / 100), 0);
        lv_obj_set_style_border_color(row_0, CONST_BORDER, 0);
        lv_obj_set_style_border_width(row_0, 1, 0);
        lv_obj_set_style_border_side(row_0, LV_BORDER_SIDE_TOP, 0);

        lv_obj_t * tab_0 = tab_item_create(row_0, "Home", REF_TAB_HOME);
        lv_obj_set_name(tab_0, "tab_0");
        lv_obj_add_screen_create_event(tab_0, LV_EVENT_CLICKED, home_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        lv_obj_t * tab_1 = tab_item_create(row_0, "Schedule", REF_TAB_SCHEDULE);
        lv_obj_set_name(tab_1, "tab_1");
        lv_obj_add_screen_create_event(tab_1, LV_EVENT_CLICKED, schedule_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

