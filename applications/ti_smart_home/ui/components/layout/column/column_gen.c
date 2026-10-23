/**
 * @file column_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "column_gen.h"
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

lv_obj_t * column_create(lv_obj_t * parent, int32_t padding_side, int32_t padding_inside, int32_t grow, lv_flex_align_t horizontal_align, lv_flex_align_t vertical_align, lv_flex_align_t content_align)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_width(&style_base, LV_SIZE_CONTENT);
        lv_style_set_height(&style_base, LV_SIZE_CONTENT);
        lv_style_set_layout(&style_base, LV_LAYOUT_FLEX);
        lv_style_set_flex_flow(&style_base, LV_FLEX_FLOW_COLUMN);
        lv_style_set_pad_all(&style_base, BASE_LIB_UNIT_MD);
        lv_style_set_pad_row(&style_base, BASE_LIB_UNIT_MD);
        lv_style_set_pad_column(&style_base, BASE_LIB_UNIT_MD);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * container_0 = container_create(parent);
        lv_obj_set_name_static(container_0, "column_#");
        lv_obj_set_style_pad_all(container_0, padding_side, 0);
        lv_obj_set_style_pad_row(container_0, padding_inside, 0);
        lv_obj_set_style_pad_column(container_0, padding_inside, 0);
        lv_obj_set_style_flex_cross_place(container_0, horizontal_align, 0);
        lv_obj_set_style_flex_main_place(container_0, vertical_align, 0);
        lv_obj_set_style_flex_track_place(container_0, content_align, 0);
        lv_obj_set_style_flex_grow(container_0, grow, 0);

        lv_obj_add_style(container_0, &style_base, 0);
        lv_obj_add_style(container_0, &style_scrollbar, LV_PART_SCROLLBAR);

        the_root = container_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

