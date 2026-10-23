/**
 * @file chart_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "chart_gen.h"
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

lv_obj_t * chart_create(lv_obj_t * parent, lv_chart_type_t chart_type, lv_color_t color, int32_t min_value, int32_t max_value, int32_t tick_count, int32_t point_count)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_base;
    static lv_style_t style_chart;
    static lv_style_t style_series_items;
    static lv_style_t style_series_indicator;
    static lv_style_t style_scale;
    static lv_style_t style_scale_indicator;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&style_chart);
        lv_style_init(&style_series_items);
        lv_style_init(&style_series_indicator);
        lv_style_init(&style_scale);
        lv_style_init(&style_scale_indicator);

        lv_style_set_pad_column(&style_base, 0);
        lv_style_set_pad_all(&style_base, 0);
        lv_style_set_flex_grow(&style_chart, 1);
        lv_style_set_height(&style_chart, lv_pct(100));
        lv_style_set_pad_ver(&style_chart, 16);
        lv_style_set_pad_hor(&style_chart, 16);
        lv_style_set_pad_column(&style_chart, BASE_LIB_UNIT_MD);
        lv_style_set_border_width(&style_chart, 0);
        lv_style_set_line_width(&style_chart, 1);
        lv_style_set_line_opa(&style_chart, (255 * 12 / 100));
        lv_style_set_margin_ver(&style_chart, 0);
        lv_style_set_line_width(&style_series_items, 0);
        lv_style_set_radius(&style_series_items, 50);
        lv_style_set_width(&style_series_indicator, BASE_LIB_UNIT_SM);
        lv_style_set_height(&style_series_indicator, BASE_LIB_UNIT_SM);
        lv_style_set_height(&style_scale, lv_pct(95));
        lv_style_set_pad_top(&style_scale, 16);
        lv_style_set_width(&style_scale, BASE_LIB_UNIT_XXL);
        lv_style_set_text_font(&style_scale, font_sm);
        lv_style_set_line_width(&style_scale, 0);
        lv_style_set_length(&style_scale_indicator, 0);
        lv_style_set_pad_left(&style_scale_indicator, BASE_LIB_UNIT_XS);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * row_0 = row_create(parent, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name_static(row_0, "chart_#");
        lv_obj_set_style_pad_ver(row_0, 0, 0);

        lv_obj_add_style(row_0, &style_base, 0);
        lv_obj_t * lv_scale_0 = lv_scale_create(row_0);
        lv_scale_set_mode(lv_scale_0, LV_SCALE_MODE_VERTICAL_LEFT);
        lv_scale_set_min_value(lv_scale_0, min_value);
        lv_scale_set_max_value(lv_scale_0, max_value);
        lv_scale_set_major_tick_every(lv_scale_0, 1);
        lv_scale_set_total_tick_count(lv_scale_0, tick_count);
        lv_obj_add_style(lv_scale_0, &style_scale, 0);
        lv_obj_add_style(lv_scale_0, &style_scale_indicator, LV_PART_INDICATOR);

        lv_obj_t * lv_chart_0 = lv_chart_create(row_0);
        lv_obj_set_style_line_width(lv_chart_0, 4, 0);
        lv_chart_set_type(lv_chart_0, LV_CHART_TYPE_BAR);
        lv_chart_set_point_count(lv_chart_0, point_count);
        lv_chart_set_hor_div_line_count(lv_chart_0, tick_count);
        lv_chart_set_ver_div_line_count(lv_chart_0, 0);
        lv_obj_remove_style_all(lv_chart_0);
        lv_obj_add_style(lv_chart_0, &style_chart, 0);
        lv_obj_add_style(lv_chart_0, &style_series_items, LV_PART_ITEMS);
        lv_obj_add_style(lv_chart_0, &style_series_indicator, LV_PART_INDICATOR);
        lv_chart_series_t * lv_chart_series_0 = lv_chart_add_series(lv_chart_0, color, LV_CHART_AXIS_PRIMARY_Y);
        static const int32_t lv_chart_0_values_0[] = {12, 11, 22, 21, 4, 8, 14};
        lv_chart_set_series_values(lv_chart_0, lv_chart_series_0, lv_chart_0_values_0, 7);
        lv_chart_set_axis_min_value(lv_chart_0, LV_CHART_AXIS_PRIMARY_Y, min_value);
        lv_chart_set_axis_max_value(lv_chart_0, LV_CHART_AXIS_PRIMARY_Y, max_value);

        the_root = row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

