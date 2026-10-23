/**
 * @file list_container_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "list_container_gen.h"
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

lv_obj_t * list_container_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_scrollbar_inset;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_scrollbar_inset);

        lv_style_set_bg_color(&style_base, COLOR_BG);
        lv_style_set_height(&style_base, LV_SIZE_CONTENT);
        lv_style_set_width(&style_base, lv_pct(100));
        lv_style_set_bg_opa(&style_base, (255 * 100 / 100));
        lv_style_set_pad_all(&style_base, BASE_LIB_UNIT_XL);
        lv_style_set_pad_row(&style_base, 0);
        lv_style_set_flex_grow(&style_base, 1);
        lv_style_set_radius(&style_base, BASE_LIB_UNIT_XL);
        lv_style_set_pad_top(&style_scrollbar_inset, BASE_LIB_UNIT_XL);
        lv_style_set_pad_bottom(&style_scrollbar_inset, BASE_LIB_UNIT_XL);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * column_0 = column_create(parent, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(column_0, "list_container_#");

        lv_obj_add_style(column_0, &style_base, 0);
        lv_obj_add_style(column_0, &style_scrollbar, LV_PART_SCROLLBAR);
        lv_obj_add_style(column_0, &style_scrollbar_inset, LV_PART_SCROLLBAR);

        the_root = column_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

