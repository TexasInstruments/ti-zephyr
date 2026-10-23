/**
 * @file list_item_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "list_item_gen.h"
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

lv_obj_t * list_item_create(lv_obj_t * parent, const char * title, const char * subtitle)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_height(&style_base, LV_SIZE_CONTENT);
        lv_style_set_width(&style_base, lv_pct(100));
        lv_style_set_bg_opa(&style_base, (255 * 0 / 100));
        lv_style_set_border_side(&style_base, LV_BORDER_SIDE_BOTTOM);
        lv_style_set_border_width(&style_base, 1);
        lv_style_set_radius(&style_base, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * panel_0 = panel_create(parent, BASE_LIB_UNIT_LG);
        lv_obj_set_name_static(panel_0, "list_item_#");

        lv_obj_add_style(panel_0, &style_base, 0);
        lv_obj_t * column_0 = column_create(panel_0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_0, lv_pct(100));
        lv_obj_set_height(column_0, LV_SIZE_CONTENT);
        lv_obj_t * h3_0 = h3_create(column_0, title);
        lv_obj_set_width(h3_0, lv_pct(100));
        lv_obj_set_height(h3_0, LV_SIZE_CONTENT);
        lv_obj_set_flag(h3_0, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK, true);

        lv_obj_t * h4_0 = h4_create(column_0, subtitle);
        lv_obj_set_width(h4_0, lv_pct(100));
        lv_obj_set_height(h4_0, LV_SIZE_CONTENT);
        lv_obj_set_style_text_opa(h4_0, (255 * 60 / 100), 0);

        the_root = panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

