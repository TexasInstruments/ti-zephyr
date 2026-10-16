/**
 * @file schedule_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "schedule_gen.h"
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

lv_obj_t * schedule_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "schedule_#");
        lv_obj_set_width(lv_obj_0, CONST_SCREEN_W);
        lv_obj_set_height(lv_obj_0, CONST_SCREEN_H);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_bg_color(lv_obj_0, CONST_SURFACE_CANVAS, 0);
        lv_obj_set_style_bg_opa(lv_obj_0, (255 * 100 / 100), 0);
        lv_obj_set_style_bg_grad_color(lv_obj_0, lv_color_hex(0xE9EEF1), 0);
        lv_obj_set_style_bg_grad_dir(lv_obj_0, LV_GRAD_DIR_VER, 0);

        lv_obj_add_style(lv_obj_0, &style_screen_base, 0);
        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_tab, LV_EVENT_SCREEN_LOADED, REF_TAB_SCHEDULE);
        top_bar_create(lv_obj_0, "Schedule");

        lv_obj_t * column_0 = column_create(lv_obj_0, 0, 0, 0, 0, 10, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_0, CONST_SCREEN_W);
        lv_obj_set_height(column_0, 380);
        lv_obj_set_y(column_0, 44);
        lv_obj_set_flag(column_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_pad_top(column_0, 8, 0);
        lv_obj_set_style_pad_bottom(column_0, 8, 0);
        lv_obj_t * row_0 = row_create(column_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_0, CONST_CONTENT_W);
        lv_obj_set_height(row_0, 36);
        lv_obj_set_style_pad_all(row_0, 3, 0);
        lv_obj_set_style_radius(row_0, CONST_RADIUS_PILL, 0);
        lv_obj_set_style_bg_color(row_0, CONST_TRACK, 0);
        lv_obj_set_style_bg_opa(row_0, (255 * 100 / 100), 0);
        seg_tab_create(row_0, "Curve", REF_SCHED_CURVE);

        seg_tab_create(row_0, "Blocks", REF_SCHED_BLOCKS);

        lv_obj_t * row_1 = row_create(column_0, 0, 0, 0, 0, 8, 0, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_1, CONST_CONTENT_W);
        lv_obj_set_height(row_1, 32);
        day_chip_create(row_1, "M", 0);

        day_chip_create(row_1, "T", 1);

        day_chip_create(row_1, "W", 2);

        day_chip_create(row_1, "T", 3);

        day_chip_create(row_1, "F", 4);

        day_chip_create(row_1, "S", 5);

        day_chip_create(row_1, "S", 6);

        lv_obj_t * column_1 = column_create(column_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_1, CONST_CONTENT_W);
        lv_obj_set_height(column_1, 276);
        lv_obj_bind_flag_if_not_eq(column_1, &subject_sched_view, LV_OBJ_FLAG_HIDDEN, REF_SCHED_CURVE);
        lv_obj_t * surface_0 = surface_create(column_1);
        lv_obj_set_height(surface_0, lv_pct(100));
        lv_obj_set_flex_flow(surface_0, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(surface_0, 12, 0);
        lv_obj_set_style_pad_row(surface_0, 8, 0);
        overline_create(surface_0, "SETPOINT CURVE");

        lv_obj_t * wd_schedule_chart_0 = wd_schedule_chart_create(surface_0);
        wd_schedule_chart_bind_day(wd_schedule_chart_0, &subject_sched_day);
        lv_obj_set_height(wd_schedule_chart_0, 200);

        lv_obj_t * row_2 = row_create(surface_0, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_2, 264);
        lv_obj_set_height(row_2, LV_SIZE_CONTENT);
        lv_obj_t * label_0 = label_create(row_2);
        lv_label_set_text(label_0, "00");
        lv_obj_set_style_text_color(label_0, CONST_TEXT_TERTIARY, 0);
        lv_obj_add_style(label_0, &style_micro, 0);

        lv_obj_t * label_1 = label_create(row_2);
        lv_label_set_text(label_1, "06");
        lv_obj_set_style_text_color(label_1, CONST_TEXT_TERTIARY, 0);
        lv_obj_add_style(label_1, &style_micro, 0);

        lv_obj_t * label_2 = label_create(row_2);
        lv_label_set_text(label_2, "12");
        lv_obj_set_style_text_color(label_2, CONST_TEXT_TERTIARY, 0);
        lv_obj_add_style(label_2, &style_micro, 0);

        lv_obj_t * label_3 = label_create(row_2);
        lv_label_set_text(label_3, "18");
        lv_obj_set_style_text_color(label_3, CONST_TEXT_TERTIARY, 0);
        lv_obj_add_style(label_3, &style_micro, 0);

        lv_obj_t * label_4 = label_create(row_2);
        lv_label_set_text(label_4, "24");
        lv_obj_set_style_text_color(label_4, CONST_TEXT_TERTIARY, 0);
        lv_obj_add_style(label_4, &style_micro, 0);

        lv_obj_t * column_2 = column_create(column_0, 0, 0, 0, 0, 6, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(column_2, CONST_CONTENT_W);
        lv_obj_set_height(column_2, 276);
        lv_obj_bind_flag_if_not_eq(column_2, &subject_sched_view, LV_OBJ_FLAG_HIDDEN, REF_SCHED_BLOCKS);
        lv_obj_t * row_3 = row_create(column_2, 0, 0, 0, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_3, CONST_CONTENT_W);
        lv_obj_set_height(row_3, LV_SIZE_CONTENT);
        overline_create(row_3, "COMFORT BLOCKS");

        chip_create(row_3, &style_chip_fill_warn, &style_chip_dot_warn, &style_chip_text_warn, "Hold active", false);

        row_block_create(column_2, "06:30", "Wake", "21", &style_block_active, "running now");

        row_block_create(column_2, "08:00", "Away", "17", &style_block_idle, "holds until next block");

        row_block_create(column_2, "17:30", "Home", "21", &style_block_idle, "holds until next block");

        row_block_create(column_2, "22:30", "Sleep", "18", &style_block_idle, "holds until next block");

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

