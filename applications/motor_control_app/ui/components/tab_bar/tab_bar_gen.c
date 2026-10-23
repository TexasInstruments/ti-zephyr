/**
 * @file tab_bar_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "tab_bar_gen.h"
#include "../../motor_control.h"

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

    static lv_style_t style_bar_top;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_bar_top);

        lv_style_set_bg_color(&style_bar_top, CONST_CANVAS);
        lv_style_set_bg_opa(&style_bar_top, (255 * 100 / 100));
        lv_style_set_border_color(&style_bar_top, CONST_BORDER);
        lv_style_set_border_width(&style_bar_top, 1);
        lv_style_set_border_side(&style_bar_top, LV_BORDER_SIDE_TOP);
        lv_style_set_radius(&style_bar_top, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(row_0, "tab_bar_#");
        lv_obj_set_width(row_0, CONST_SCREEN_W);
        lv_obj_set_height(row_0, CONST_TABBAR_H);

        lv_obj_add_style(row_0, &style_bar_top, 0);
        lv_obj_t * tab_item_0 = tab_item_create(row_0, icon_tab_run, "Operate", 0, &subject_alert_never);
        lv_obj_add_screen_create_event(tab_item_0, LV_EVENT_CLICKED, run_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        lv_obj_t * tab_item_1 = tab_item_create(row_0, icon_tab_faults, "Faults", 1, &subject_fault_active);
        lv_obj_add_screen_create_event(tab_item_1, LV_EVENT_CLICKED, faults_create, LV_SCREEN_LOAD_ANIM_NONE, 0, 0);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

