/**
 * @file trend_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "trend_gen.h"
#include "../motor_control.h"

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

lv_obj_t * trend_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "trend_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_add_style(lv_obj_0, &style_canvas, 0);
        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_tab, LV_EVENT_SCREEN_LOADED, 2);
        lv_obj_t * column_0 = column_create(lv_obj_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_0, CONST_SCREEN_W);
        lv_obj_set_height(column_0, CONST_SCREEN_H);
        header_create(column_0);

        lv_obj_t * column_1 = column_create(column_0, 0, 0, CONST_SPACE_8, 0, CONST_SPACE_12, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_1, CONST_CONTENT_W);
        lv_obj_set_height(column_1, LV_SIZE_CONTENT);
        lv_obj_t * label_0 = label_create(column_1);
        lv_label_set_text(label_0, "Speed trend");
        lv_obj_add_style(label_0, &style_heading, 0);
        lv_obj_add_style(label_0, &style_ink, 0);

        lv_obj_t * card_0 = card_create(column_1, 16, CONST_SPACE_12);
        lv_obj_t * row_0 = row_create(card_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_0, lv_pct(100));
        overline_create(row_0, "LAST 30 SECONDS");

        lv_obj_t * row_1 = row_create(row_0, 0, 0, 0, 0, CONST_SPACE_12, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * row_2 = row_create(row_1, 0, 0, 0, 0, CONST_SPACE_4, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * container_0 = container_create(row_2);
        lv_obj_set_width(container_0, 8);
        lv_obj_set_height(container_0, 2);
        lv_obj_set_style_radius(container_0, CONST_RADIUS_PILL, 0);
        lv_obj_add_style(container_0, &style_bar_accent, 0);

        lv_obj_t * label_1 = label_create(row_2);
        lv_label_set_text(label_1, "RPM");
        lv_obj_add_style(label_1, &style_badge, 0);
        lv_obj_add_style(label_1, &style_ink_secondary, 0);

        lv_obj_t * row_3 = row_create(row_1, 0, 0, 0, 0, CONST_SPACE_4, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * container_1 = container_create(row_3);
        lv_obj_set_width(container_1, 8);
        lv_obj_set_height(container_1, 2);
        lv_obj_set_style_radius(container_1, CONST_RADIUS_PILL, 0);
        lv_obj_add_style(container_1, &style_bar_warn, 0);

        lv_obj_t * label_2 = label_create(row_3);
        lv_label_set_text(label_2, "LOAD");
        lv_obj_add_style(label_2, &style_badge, 0);
        lv_obj_add_style(label_2, &style_ink_secondary, 0);

        lv_obj_t * trend_chart = lv_chart_create(card_0);
        lv_obj_set_name(trend_chart, "trend_chart");
        lv_obj_set_width(trend_chart, lv_pct(100));
        lv_obj_set_height(trend_chart, 116);
        lv_chart_set_type(trend_chart, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(trend_chart, 30);
        lv_chart_set_update_mode(trend_chart, LV_CHART_UPDATE_MODE_SHIFT);
        lv_chart_set_hor_div_line_count(trend_chart, 5);
        lv_chart_set_ver_div_line_count(trend_chart, 7);
        lv_obj_add_style(trend_chart, &style_chart, 0);
        lv_obj_add_style(trend_chart, &style_chart_items, LV_PART_ITEMS);
        lv_obj_add_style(trend_chart, &style_chart_items, LV_PART_INDICATOR);
        lv_chart_set_axis_min_value(trend_chart, LV_CHART_AXIS_PRIMARY_Y, 0);
        lv_chart_set_axis_max_value(trend_chart, LV_CHART_AXIS_PRIMARY_Y, 3600);
        lv_chart_set_axis_min_value(trend_chart, LV_CHART_AXIS_SECONDARY_Y, 0);
        lv_chart_set_axis_max_value(trend_chart, LV_CHART_AXIS_SECONDARY_Y, 100);
        lv_chart_series_t * lv_chart_series_0 = lv_chart_add_series(trend_chart, CONST_ACCENT, LV_CHART_AXIS_PRIMARY_Y);
        static const int32_t trend_chart_values_0[] = {0, 0, 120, 340, 600, 880, 1120, 1200, 1195, 1205, 1200, 1198, 1202, 1200, 1190, 1210, 1200, 1160, 1080, 1140, 1200, 1205, 1198, 1200, 1202, 1199, 1201, 1200, 1198, 1200};
        lv_chart_set_series_values(trend_chart, lv_chart_series_0, trend_chart_values_0, 30);
        lv_chart_series_t * lv_chart_series_1 = lv_chart_add_series(trend_chart, CONST_WARN, LV_CHART_AXIS_SECONDARY_Y);
        static const int32_t trend_chart_values_1[] = {0, 0, 18, 42, 58, 64, 55, 38, 36, 37, 36, 35, 37, 36, 40, 38, 36, 52, 61, 48, 38, 36, 37, 36, 36, 37, 36, 36, 37, 36};
        lv_chart_set_series_values(trend_chart, lv_chart_series_1, trend_chart_values_1, 30);

        lv_obj_t * column_2 = column_create(column_1, 0, 0, 0, 0, CONST_SPACE_8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_2, lv_pct(100));
        lv_obj_set_height(column_2, LV_SIZE_CONTENT);
        overline_create(column_2, "THIS RUN");

        lv_obj_t * row_4 = row_create(column_2, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_4, lv_pct(100));
        stat_tile_create(row_4, "PEAK", &subject_peak_rpm, "%d", "1210", "RPM", 140);

        stat_tile_create(row_4, "MEAN LOAD", &subject_avg_load, "%d", "38", "%", 140);

        lv_obj_t * grow = container_create(column_0);
        lv_obj_set_name(grow, "grow");
        lv_obj_set_width(grow, CONST_SCREEN_W);
        lv_obj_set_flex_grow(grow, 1);

        tab_bar_create(column_0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

