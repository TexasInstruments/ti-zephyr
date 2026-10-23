/**
 * @file screen_smart_home_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_smart_home_gen.h"
#include "../ti_smart_home.h"

/*********************
 *      DEFINES
 *********************/

#define COLOR_VIEW_BG lv_color_hex(0xF5F1DB)
#define COLOR_CARD1_BG lv_color_hex(0xFFFEF9)
#define COLOR_CARD1_TEXT lv_color_hex(0x484242)
#define COLOR_CARD2_BG lv_color_hex(0xF6EAB8)
#define COLOR_CARD2_TEXT lv_color_hex(0x564500)
#define COLOR_CARD3_BG lv_color_hex(0xFFB598)
#define COLOR_CARD3_TEXT lv_color_hex(0x532715)
#define COLOR_CARD4_BG lv_color_hex(0xAA895D)
#define COLOR_CARD4_TEXT lv_color_hex3(0xfff)
#define COLOR_CARD5_BG lv_color_hex(0xFFFEF9)
#define COLOR_CARD5_TEXT lv_color_hex(0x0E3E02)


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

lv_obj_t * screen_smart_home_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_thermo_arc;
    static lv_style_t style_button_pause;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_thermo_arc);
        lv_style_init(&style_button_pause);

        lv_style_set_arc_color(&style_thermo_arc, COLOR_CARD1_TEXT);
        lv_style_set_bg_color(&style_button_pause, lv_color_hex(0x003199));
        lv_style_set_bg_image_src(&style_button_pause, icon_pause);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "screen_smart_home_#");

        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_nav_index, LV_EVENT_SCREEN_LOADED, REF_NAV_SMART_HOME);
        lv_obj_t * root = column_create(lv_obj_0, 0, BASE_LIB_UNIT_MD, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(root, "root");
        lv_obj_set_width(root, lv_pct(100));
        lv_obj_set_height(root, lv_pct(100));
        lv_obj_set_flag(root, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_pad_top(root, 12, 0);
        lv_obj_set_style_bg_color(root, COLOR_VIEW_BG, 0);
        lv_obj_set_style_bg_opa(root, (255 * 100 / 100), 0);
        lv_obj_t * statusbar = header_create(root);
        lv_obj_set_name(statusbar, "statusbar");
        lv_obj_set_style_pad_hor(statusbar, BASE_LIB_UNIT_XL, 0);

        lv_obj_t * content = column_create(root, 0, BASE_LIB_UNIT_XL, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(content, "content");
        lv_obj_set_width(content, lv_pct(100));
        lv_obj_set_style_pad_hor(content, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_style_pad_bottom(content, BASE_LIB_UNIT_MD, 0);
        lv_obj_set_flag(content, LV_OBJ_FLAG_SCROLLABLE, true);
        lv_obj_set_flag(content, LV_OBJ_FLAG_SCROLL_CHAIN_VER, false);
        lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
        lv_obj_t * column_0 = column_create(content, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_0, lv_pct(100));
        h1_create(column_0, "Smart Home");

        lv_obj_t * main_stats = row_create(content, 0, BASE_LIB_UNIT_XL, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(main_stats, "main_stats");
        lv_obj_set_width(main_stats, lv_pct(100));
        lv_obj_set_height(main_stats, 340);
        lv_obj_t * card_thermostat = card_empty_create(main_stats);
        lv_obj_set_name(card_thermostat, "card_thermostat");
        lv_obj_set_flex_flow(card_thermostat, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_thermostat, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_grow(card_thermostat, 1, 0);
        lv_obj_set_height(card_thermostat, lv_pct(100));
        lv_obj_set_style_bg_color(card_thermostat, COLOR_CARD1_BG, 0);
        lv_obj_set_style_text_color(card_thermostat, COLOR_CARD1_TEXT, 0);
        lv_obj_set_flag(card_thermostat, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_thermostat, LV_OBJ_FLAG_OVERFLOW_VISIBLE, false);
        lv_obj_t * row_0 = row_create(card_thermostat, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_t * image_mono_0 = image_mono_create(row_0, icon_temp);
        lv_obj_set_style_image_recolor(image_mono_0, COLOR_CARD1_TEXT, 0);

        h3_create(row_0, "Thermostat");

        lv_obj_t * column_1 = column_create(card_thermostat, 0, 0, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_1, lv_pct(100));
        lv_obj_set_style_flex_track_place(column_1, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_t * arc_0 = arc_create(column_1, &subject_temp);
        lv_arc_set_min_value(arc_0, 16);
        lv_arc_set_max_value(arc_0, 32);
        lv_obj_set_width(arc_0, 196);
        lv_obj_set_height(arc_0, 196);
        lv_obj_add_style(arc_0, &style_thermo_arc, 0);
        lv_obj_add_style(arc_0, &style_thermo_arc, LV_PART_INDICATOR);
        lv_obj_t * value_large_0 = value_large_create(arc_0, &subject_temp, "%d°", "");
        lv_obj_set_align(value_large_0, LV_ALIGN_CENTER);

        lv_obj_t * button_circle_0 = button_circle_create(column_1, icon_minus, COLOR_ACCENT, (255 * 20 / 100), COLOR_ACCENT, 56);
        lv_obj_set_flag(button_circle_0, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(button_circle_0, LV_ALIGN_BOTTOM_LEFT);
        lv_subject_increment_dsc_t * subject_increment_event_0 = lv_obj_add_subject_increment_event(button_circle_0, &subject_temp, LV_EVENT_CLICKED, -1);
        lv_obj_set_subject_increment_event_min_value(button_circle_0, subject_increment_event_0, 16);
        lv_subject_increment_dsc_t * subject_increment_event_1 = lv_obj_add_subject_increment_event(button_circle_0, &subject_temp, LV_EVENT_LONG_PRESSED_REPEAT, -1);
        lv_obj_set_subject_increment_event_min_value(button_circle_0, subject_increment_event_1, 16);

        lv_obj_t * button_circle_1 = button_circle_create(column_1, icon_plus, COLOR_ACCENT, (255 * 20 / 100), COLOR_ACCENT, 56);
        lv_obj_set_flag(button_circle_1, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(button_circle_1, LV_ALIGN_BOTTOM_RIGHT);
        lv_subject_increment_dsc_t * subject_increment_event_2 = lv_obj_add_subject_increment_event(button_circle_1, &subject_temp, LV_EVENT_CLICKED, 1);
        lv_obj_set_subject_increment_event_max_value(button_circle_1, subject_increment_event_2, 32);
        lv_subject_increment_dsc_t * subject_increment_event_3 = lv_obj_add_subject_increment_event(button_circle_1, &subject_temp, LV_EVENT_LONG_PRESSED_REPEAT, 1);
        lv_obj_set_subject_increment_event_max_value(button_circle_1, subject_increment_event_3, 32);

        lv_obj_t * card_weather = card_empty_create(main_stats);
        lv_obj_set_name(card_weather, "card_weather");
        lv_obj_set_flex_flow(card_weather, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_weather, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_grow(card_weather, 1, 0);
        lv_obj_set_height(card_weather, lv_pct(100));
        lv_obj_set_style_bg_color(card_weather, COLOR_CARD2_BG, 0);
        lv_obj_set_style_text_color(card_weather, COLOR_CARD2_TEXT, 0);
        lv_obj_set_flag(card_weather, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_weather, LV_OBJ_FLAG_OVERFLOW_VISIBLE, false);
        lv_obj_t * image_0 = image_create(card_weather, image_weather_cloudy);
        lv_obj_set_flag(image_0, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_x(image_0, 38);
        lv_obj_set_y(image_0, 90);

        lv_obj_t * row_1 = row_create(card_weather, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_t * image_mono_1 = image_mono_create(row_1, icon_weather);
        lv_obj_set_style_image_recolor(image_mono_1, COLOR_CARD2_TEXT, 0);

        h3_create(row_1, "Current Weather");

        lv_obj_t * column_2 = column_create(card_weather, 0, -8, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_2, lv_pct(100));
        lv_obj_set_flag(column_2, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_y(column_2, 80);
        lv_obj_t * row_2 = row_create(column_2, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_2, lv_pct(100));
        value_large_create(row_2, &subject_outside_temp_current, "%d°", "");

        lv_obj_t * value_medium_0 = value_medium_create(row_2, &subject_outside_temp_highest, "/ %d°", "");
        lv_obj_set_style_text_opa(value_medium_0, (255 * 60 / 100), 0);
        lv_obj_set_style_pad_bottom(value_medium_0, 10, 0);

        lv_obj_t * text_0 = text_create(card_weather, font_md);
        lv_label_bind_text(text_0, &subject_weather_desc, NULL);
        lv_obj_set_style_text_opa(text_0, (255 * 64 / 100), 0);

        lv_obj_t * card_music_player = card_empty_create(content);
        lv_obj_set_name(card_music_player, "card_music_player");
        lv_obj_set_flex_flow(card_music_player, LV_FLEX_FLOW_ROW);
        lv_obj_set_width(card_music_player, lv_pct(100));
        lv_obj_set_height(card_music_player, 180);
        lv_obj_set_style_flex_cross_place(card_music_player, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_all(card_music_player, 0, 0);
        lv_obj_set_style_pad_column(card_music_player, 0, 0);
        lv_obj_set_style_flex_main_place(card_music_player, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_bg_color(card_music_player, COLOR_CARD5_BG, 0);
        lv_obj_set_style_text_color(card_music_player, COLOR_TEXT, 0);
        lv_obj_set_flag(card_music_player, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * album_cover = image_create(card_music_player, image_album_cover);
        lv_obj_set_name(album_cover, "album_cover");
        lv_image_bind_src(album_cover, &subject_album_cover);
        lv_obj_set_style_radius(album_cover, 16, 0);

        lv_obj_t * image_1 = image_create(card_music_player, icon_spotify);
        lv_obj_set_flag(image_1, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(image_1, LV_ALIGN_TOP_LEFT);
        lv_obj_set_x(image_1, 120);
        lv_obj_set_y(image_1, 8);

        lv_obj_t * column_3 = column_create(card_music_player, 12, 0, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(column_3, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_height(column_3, lv_pct(100));
        lv_obj_t * column_4 = column_create(column_3, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_style_pad_hor(column_4, BASE_LIB_UNIT_LG, 0);
        lv_obj_set_width(column_4, lv_pct(100));
        lv_obj_t * h4_0 = h4_create(column_4, "Heading 4");
        lv_label_bind_text(h4_0, &subject_album_name, NULL);
        lv_obj_set_style_opa(h4_0, (255 * 60 / 100), 0);

        lv_obj_t * h3_2 = h3_create(column_4, "Heading 3");
        lv_label_bind_text(h3_2, &subject_track_name, NULL);
        lv_obj_set_width(h3_2, lv_pct(100));
        lv_label_set_long_mode(h3_2, LV_LABEL_LONG_MODE_DOTS);

        lv_obj_t * row_3 = row_create(column_3, 0, 0, 1, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(row_3, lv_pct(100));
        lv_obj_set_style_pad_hor(row_3, 6, 0);
        button_circle_create(row_3, icon_shuffle, COLOR_ACCENT, 0, COLOR_TEXT, 56);

        lv_obj_t * button_circle_3 = button_circle_create(row_3, icon_prev, COLOR_ACCENT, 0, COLOR_TEXT, 56);
        lv_obj_add_event_cb(button_circle_3, on_music_previous_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t * container_0 = container_create(row_3);
        lv_obj_t * button_circle_4 = button_circle_create(container_0, icon_play, COLOR_TEXT, (255 * 100 / 100), COLOR_BG, 64);
        lv_obj_remove_style(button_circle_4, NULL, LV_STATE_CHECKED);
        lv_obj_bind_state_if_eq(button_circle_4, &subject_play, LV_STATE_CHECKED, 1);
        lv_obj_bind_style(button_circle_4, &style_button_pause, LV_STATE_CHECKED, &subject_play, 1);
        lv_obj_add_subject_toggle_event(button_circle_4, &subject_play, LV_EVENT_CLICKED);

        lv_obj_t * button_circle_5 = button_circle_create(row_3, icon_next, COLOR_ACCENT, 0, COLOR_TEXT, 56);
        lv_obj_add_event_cb(button_circle_5, on_music_next_cb, LV_EVENT_CLICKED, NULL);

        button_circle_create(row_3, icon_repeat, COLOR_ACCENT, 0, COLOR_TEXT, 56);

        lv_obj_t * devices = column_create(content, 0, BASE_LIB_UNIT_XL, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(devices, "devices");
        lv_obj_set_width(devices, lv_pct(100));
        lv_obj_set_height(devices, 424);
        lv_obj_set_style_layout(devices, LV_LAYOUT_GRID, 0);
        static const int32_t devices_style_grid_column_dsc_array_0[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        lv_obj_set_style_grid_column_dsc_array(devices, devices_style_grid_column_dsc_array_0, 0);
        static const int32_t devices_style_grid_row_dsc_array_0[] = {240, 160, LV_GRID_TEMPLATE_LAST};
        lv_obj_set_style_grid_row_dsc_array(devices, devices_style_grid_row_dsc_array_0, 0);
        lv_obj_set_flag(devices, LV_OBJ_FLAG_OVERFLOW_VISIBLE, true);
        lv_obj_t * card_range = card_empty_create(devices);
        lv_obj_set_name(card_range, "card_range");
        lv_obj_set_flex_flow(card_range, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_range, LV_FLEX_ALIGN_START, 0);
        lv_obj_set_style_bg_color(card_range, COLOR_CARD3_BG, 0);
        lv_obj_set_style_text_color(card_range, COLOR_CARD3_TEXT, 0);
        lv_obj_set_style_grid_cell_column_pos(card_range, 0, 0);
        lv_obj_set_style_grid_cell_row_pos(card_range, 0, 0);
        lv_obj_set_style_grid_cell_y_align(card_range, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_style_grid_cell_x_align(card_range, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_style_pad_all(card_range, 20, 0);
        lv_obj_set_style_radius(card_range, 16, 0);
        lv_obj_set_flag(card_range, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * image_2 = image_create(card_range, image_kettle);
        lv_obj_set_flag(image_2, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_x(image_2, 70);
        lv_obj_set_y(image_2, 83);

        lv_obj_t * column_5 = column_create(card_range, 0, -2, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        h4_create(column_5, "Smart Kettle");

        lv_obj_t * text_1 = text_create(column_5, font_sm);
        lv_label_set_text(text_1, "MI Smart Kettle Pro");
        lv_obj_set_style_text_opa(text_1, (255 * 60 / 100), 0);

        lv_obj_t * column_6 = column_create(card_range, 0, -16, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        value_large_create(column_6, &subject_kettle_temp, "%d°", "");

        lv_obj_t * text_2 = text_create(column_6, font_md);
        lv_label_set_text(text_2, "Cooling Down");

        lv_obj_t * switch_0 = switch_create(card_range);
        lv_obj_set_flag(switch_0, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(switch_0, LV_ALIGN_TOP_RIGHT);
        lv_obj_bind_checked(switch_0, &subject_kettle_on);

        lv_obj_t * card_kettle = card_empty_create(devices);
        lv_obj_set_name(card_kettle, "card_kettle");
        lv_obj_set_flex_flow(card_kettle, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_kettle, LV_FLEX_ALIGN_START, 0);
        lv_obj_set_style_bg_color(card_kettle, COLOR_CARD4_BG, 0);
        lv_obj_set_style_text_color(card_kettle, COLOR_CARD4_TEXT, 0);
        lv_obj_set_style_grid_cell_column_pos(card_kettle, 1, 0);
        lv_obj_set_style_grid_cell_row_pos(card_kettle, 0, 0);
        lv_obj_set_style_grid_cell_y_align(card_kettle, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_style_grid_cell_x_align(card_kettle, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_style_pad_all(card_kettle, 20, 0);
        lv_obj_set_style_radius(card_kettle, 16, 0);
        lv_obj_set_flag(card_kettle, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * image_3 = image_create(card_kettle, image_vacuum);
        lv_obj_set_flag(image_3, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_x(image_3, 70);
        lv_obj_set_y(image_3, 96);

        lv_obj_t * column_7 = column_create(card_kettle, 0, -2, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        h4_create(column_7, "Vacuum Cleaner");

        lv_obj_t * text_3 = text_create(column_7, font_sm);
        lv_label_set_text(text_3, "MI Robot Vacuum S20");
        lv_obj_set_style_text_opa(text_3, (255 * 60 / 100), 0);

        lv_obj_t * column_8 = column_create(card_kettle, 0, -16, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        value_large_create(column_8, &subject_vacuum_battery, "%d", "%");

        lv_obj_t * text_4 = text_create(column_8, font_md);
        lv_label_set_text(text_4, "Battery: Low");

        lv_obj_t * switch_1 = switch_create(card_kettle);
        lv_obj_set_flag(switch_1, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(switch_1, LV_ALIGN_TOP_RIGHT);
        lv_obj_bind_checked(switch_1, &subject_vacuum_on);

        lv_obj_t * card_wifi = card_empty_create(devices);
        lv_obj_set_name(card_wifi, "card_wifi");
        lv_obj_set_flex_flow(card_wifi, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(card_wifi, 20, 0);
        lv_obj_set_style_radius(card_wifi, 16, 0);
        lv_obj_set_style_flex_main_place(card_wifi, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_bg_color(card_wifi, COLOR_CARD5_BG, 0);
        lv_obj_set_style_text_color(card_wifi, COLOR_CARD5_TEXT, 0);
        lv_obj_set_style_grid_cell_column_pos(card_wifi, 0, 0);
        lv_obj_set_style_grid_cell_row_pos(card_wifi, 1, 0);
        lv_obj_set_style_grid_cell_y_align(card_wifi, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_style_grid_cell_x_align(card_wifi, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_flag(card_wifi, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * image_4 = image_create(card_wifi, image_wifi);
        lv_obj_set_flag(image_4, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(image_4, LV_ALIGN_CENTER);
        lv_obj_set_y(image_4, 15);

        lv_obj_t * column_9 = column_create(card_wifi, 0, -2, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        h4_create(column_9, "On");

        lv_obj_t * text_5 = text_create(column_9, font_sm);
        lv_label_set_text(text_5, "Living room");
        lv_obj_set_style_text_opa(text_5, (255 * 60 / 100), 0);

        lv_obj_t * switch_2 = switch_create(card_wifi);
        lv_obj_set_flag(switch_2, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(switch_2, LV_ALIGN_TOP_RIGHT);
        lv_obj_bind_checked(switch_2, &subject_wifi_on);

        lv_obj_t * card_cam = card_empty_create(devices);
        lv_obj_set_name(card_cam, "card_cam");
        lv_obj_set_flex_flow(card_cam, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_cam, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_bg_color(card_cam, COLOR_CARD3_BG, 0);
        lv_obj_set_style_text_color(card_cam, COLOR_CARD3_TEXT, 0);
        lv_obj_set_style_grid_cell_column_pos(card_cam, 1, 0);
        lv_obj_set_style_grid_cell_row_pos(card_cam, 1, 0);
        lv_obj_set_style_grid_cell_y_align(card_cam, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_style_grid_cell_x_align(card_cam, LV_GRID_ALIGN_STRETCH, 0);
        lv_obj_set_style_radius(card_cam, 16, 0);
        lv_obj_set_style_pad_all(card_cam, 0, 0);
        lv_obj_set_flag(card_cam, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * image_5 = image_create(card_cam, image_livecam);
        lv_obj_set_flag(image_5, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(image_5, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(image_5, 16, 0);

        lv_obj_t * text_6 = text_create(card_cam, font_sm);
        lv_obj_set_style_margin_all(text_6, 12, 0);
        lv_label_set_text(text_6, "LIVE");
        lv_obj_set_width(text_6, 48);
        lv_obj_set_height(text_6, 24);
        lv_obj_set_style_pad_top(text_6, 2, 0);
        lv_obj_set_style_text_align(text_6, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(text_6, lv_color_hex3(0xfff), 0);
        lv_obj_set_style_bg_color(text_6, lv_color_hex(0xEC4346), 0);
        lv_obj_set_style_bg_opa(text_6, (255 * 100 / 100), 0);
        lv_obj_set_style_radius(text_6, 12, 0);

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

