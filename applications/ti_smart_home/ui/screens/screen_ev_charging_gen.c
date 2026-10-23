/**
 * @file screen_ev_charging_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_ev_charging_gen.h"
#include "../ti_smart_home.h"

/*********************
 *      DEFINES
 *********************/

#define COLOR_CARD1_BG lv_color_hex(0xFFFFFF)
#define COLOR_CARD1_TEXT lv_color_hex(0x003199)
#define COLOR_CARD2_BG lv_color_hex(0x98D79A)
#define COLOR_CARD2_TEXT lv_color_hex(0x0E3E02)
#define COLOR_CARD3_BG lv_color_hex(0xC0D4F4)
#define COLOR_CARD3_TEXT lv_color_hex(0x003199)


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

lv_obj_t * screen_ev_charging_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t ev_charging_bar;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&ev_charging_bar);

        lv_style_set_bg_color(&ev_charging_bar, COLOR_CARD1_TEXT);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "screen_ev_charging_#");

        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_nav_index, LV_EVENT_SCREEN_LOADED, REF_NAV_EV);
        lv_obj_t * root = column_create(lv_obj_0, 0, BASE_LIB_UNIT_MD, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(root, "root");
        lv_obj_set_width(root, lv_pct(100));
        lv_obj_set_height(root, lv_pct(100));
        lv_obj_set_flag(root, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_pad_top(root, 12, 0);
        lv_obj_set_style_bg_color(root, lv_color_hex(0xDEE5F0), 0);
        lv_obj_set_style_bg_opa(root, (255 * 100 / 100), 0);
        lv_obj_t * statusbar = header_create(root);
        lv_obj_set_name(statusbar, "statusbar");
        lv_obj_set_style_pad_hor(statusbar, BASE_LIB_UNIT_XL, 0);

        lv_obj_t * content = column_create(root, 0, BASE_LIB_UNIT_XXL, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(content, "content");
        lv_obj_set_width(content, lv_pct(100));
        lv_obj_set_style_pad_hor(content, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_flag(content, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * column_0 = column_create(content, 0, -8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_0, lv_pct(100));
        lv_obj_t * h2_0 = h2_create(column_0, "EV Charging");
        lv_obj_set_style_text_color(h2_0, lv_color_hex(0x003199), 0);

        h1_create(column_0, "Tesla Model 3");

        lv_obj_t * image_0 = image_create(content, image_car_large);
        lv_obj_set_width(image_0, 672);
        lv_obj_set_height(image_0, 248);
        lv_image_set_inner_align(image_0, LV_IMAGE_ALIGN_STRETCH);
        lv_obj_set_flag(image_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * card_ev = card_empty_create(content);
        lv_obj_set_name(card_ev, "card_ev");
        lv_obj_set_flex_flow(card_ev, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_ev, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_width(card_ev, lv_pct(100));
        lv_obj_set_height(card_ev, 270);
        lv_obj_set_style_bg_color(card_ev, COLOR_CARD1_BG, 0);
        lv_obj_set_style_text_color(card_ev, COLOR_CARD1_TEXT, 0);
        lv_obj_set_flag(card_ev, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_ev, LV_OBJ_FLAG_OVERFLOW_VISIBLE, false);
        lv_obj_t * row_0 = row_create(card_ev, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_t * image_mono_0 = image_mono_create(row_0, icon_charge);
        lv_obj_set_style_image_recolor(image_mono_0, COLOR_CARD1_TEXT, 0);

        h3_create(row_0, "Charging");

        lv_obj_t * column_1 = column_create(card_ev, 0, -8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_1, lv_pct(100));
        lv_obj_t * row_1 = row_create(column_1, 0, 0, 0, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_1, lv_pct(100));
        value_large_create(row_1, &subject_ev_soc, "%d", "%");

        lv_obj_t * text_0 = text_create(row_1, font_md);
        lv_label_set_text(text_0, "Full in ~2 h 15 min");
        lv_obj_set_height(text_0, 40);

        lv_obj_t * lv_bar_0 = lv_bar_create(column_1);
        lv_bar_bind_value(lv_bar_0, &subject_ev_soc);
        lv_obj_set_width(lv_bar_0, lv_pct(100));
        lv_obj_set_height(lv_bar_0, 12);
        lv_obj_add_style(lv_bar_0, &ev_charging_bar, 0);
        lv_obj_add_style(lv_bar_0, &ev_charging_bar, LV_PART_INDICATOR);

        lv_obj_t * text_1 = text_create(card_ev, font_md);
        lv_label_set_text(text_1, "Started at 14:32");
        lv_obj_set_style_text_opa(text_1, (255 * 64 / 100), 0);

        lv_obj_t * stat_strip = row_create(root, 0, BASE_LIB_UNIT_LG, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(stat_strip, "stat_strip");
        lv_obj_set_width(stat_strip, lv_pct(100));
        lv_obj_set_height(stat_strip, 340);
        lv_obj_set_style_pad_left(stat_strip, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_style_pad_right(stat_strip, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_flag(stat_strip, LV_OBJ_FLAG_CLICKABLE, true);
        lv_obj_set_scroll_snap_x(stat_strip, LV_SCROLL_SNAP_START);
        lv_obj_set_scrollbar_mode(stat_strip, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_flag(stat_strip, LV_OBJ_FLAG_SCROLL_CHAIN_HOR, false);
        lv_obj_t * card_range = card_empty_create(stat_strip);
        lv_obj_set_name(card_range, "card_range");
        lv_obj_set_flex_flow(card_range, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_range, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_width(card_range, 280);
        lv_obj_set_height(card_range, lv_pct(100));
        lv_obj_set_style_bg_color(card_range, COLOR_CARD2_BG, 0);
        lv_obj_set_style_text_color(card_range, COLOR_CARD2_TEXT, 0);
        lv_obj_set_flag(card_range, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_range, LV_OBJ_FLAG_OVERFLOW_VISIBLE, false);
        lv_obj_set_flag(card_range, LV_OBJ_FLAG_SNAPPABLE, true);
        lv_obj_t * row_2 = row_create(card_range, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_t * image_mono_1 = image_mono_create(row_2, icon_range);
        lv_obj_set_style_image_recolor(image_mono_1, COLOR_CARD2_TEXT, 0);

        h3_create(row_2, "Range");

        lv_obj_t * column_2 = column_create(card_range, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_style_margin_bottom(column_2, -28, 0);
        lv_obj_set_width(column_2, lv_pct(100));
        lv_obj_t * ev_range = value_xlarge_create(column_2, &subject_ev_range_km, "%d", " km");
        lv_obj_set_name(ev_range, "ev_range");

        lv_obj_t * text_2 = text_create(card_range, font_md);
        lv_label_set_text(text_2, "360 km after full charge");
        lv_obj_set_style_text_opa(text_2, (255 * 64 / 100), 0);

        lv_obj_t * card_chart = card_empty_create(stat_strip);
        lv_obj_set_name(card_chart, "card_chart");
        lv_obj_set_flex_flow(card_chart, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_chart, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_width(card_chart, 600);
        lv_obj_set_height(card_chart, lv_pct(100));
        lv_obj_set_style_bg_color(card_chart, COLOR_CARD3_BG, 0);
        lv_obj_set_style_text_color(card_chart, COLOR_CARD3_TEXT, 0);
        lv_obj_set_flag(card_chart, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_chart, LV_OBJ_FLAG_OVERFLOW_VISIBLE, false);
        lv_obj_set_flag(card_chart, LV_OBJ_FLAG_SNAPPABLE, true);
        lv_obj_t * row_3 = row_create(card_chart, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_t * image_mono_2 = image_mono_create(row_3, icon_chart);
        lv_obj_set_style_image_recolor(image_mono_2, COLOR_CARD3_TEXT, 0);

        h3_create(row_3, "Weekly Consumption");

        lv_obj_t * weekly_consumption = chart_create(card_chart, LV_CHART_TYPE_BAR, COLOR_CARD3_TEXT, 0, 25, 5, 7);
        lv_obj_set_name(weekly_consumption, "weekly_consumption");
        lv_obj_set_width(weekly_consumption, lv_pct(100));
        lv_obj_set_height(weekly_consumption, 200);

        lv_obj_t * row_4 = row_create(card_chart, 0, 10, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_4, lv_pct(100));
        lv_obj_set_style_pad_left(row_4, 48, 0);
        lv_obj_set_style_pad_right(row_4, 16, 0);
        lv_obj_t * text_3 = text_create(row_4, font_sm);
        lv_label_set_text(text_3, "Today");
        lv_obj_set_flex_grow(text_3, 1);
        lv_obj_set_style_text_align(text_3, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t * text_4 = text_create(row_4, font_sm);
        lv_label_set_text(text_4, "Jan 11");
        lv_obj_set_flex_grow(text_4, 1);
        lv_obj_set_style_text_align(text_4, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t * text_5 = text_create(row_4, font_sm);
        lv_label_set_text(text_5, "Jan 10");
        lv_obj_set_flex_grow(text_5, 1);
        lv_obj_set_style_text_align(text_5, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t * text_6 = text_create(row_4, font_sm);
        lv_label_set_text(text_6, "Jan 9");
        lv_obj_set_flex_grow(text_6, 1);
        lv_obj_set_style_text_align(text_6, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t * text_7 = text_create(row_4, font_sm);
        lv_label_set_text(text_7, "Jan 8");
        lv_obj_set_flex_grow(text_7, 1);
        lv_obj_set_style_text_align(text_7, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t * text_8 = text_create(row_4, font_sm);
        lv_label_set_text(text_8, "Jan 7");
        lv_obj_set_flex_grow(text_8, 1);
        lv_obj_set_style_text_align(text_8, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t * text_9 = text_create(row_4, font_sm);
        lv_label_set_text(text_9, "Jan 6");
        lv_obj_set_flex_grow(text_9, 1);
        lv_obj_set_style_text_align(text_9, LV_TEXT_ALIGN_CENTER, 0);

        navbar_create(root);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

