/**
 * @file screen_home_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_home_gen.h"
#include "../ti_smart_home.h"

/*********************
 *      DEFINES
 *********************/

#define COLOR_CARD1_BG lv_color_hex(0xF6EAB8)
#define COLOR_CARD1_TEXT lv_color_hex(0x564500)
#define COLOR_CARD2_BG lv_color_hex(0xDDE5F2)
#define COLOR_CARD2_TEXT lv_color_hex(0x003199)
#define COLOR_CARD3_BG lv_color_hex(0xE3F5DB)
#define COLOR_CARD3_TEXT lv_color_hex(0x0E3E02)
#define COLOR_NOTIFICATION_BADGE_BG lv_color_hex(0xEC4346)


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

lv_obj_t * screen_home_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t ev_charging_bar;
    static lv_style_t hero_box;
    static lv_style_t messages_notification_badge;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&ev_charging_bar);
        lv_style_init(&hero_box);
        lv_style_init(&messages_notification_badge);

        lv_style_set_bg_color(&ev_charging_bar, COLOR_CARD2_TEXT);
        lv_style_set_border_width(&hero_box, 0);
        lv_style_set_radius(&hero_box, 0);
        lv_style_set_shadow_width(&hero_box, 0);
        lv_style_set_pad_all(&hero_box, 0);
        lv_style_set_bg_opa(&hero_box, (255 * 0 / 100));
        lv_style_set_width(&messages_notification_badge, 12);
        lv_style_set_height(&messages_notification_badge, 12);
        lv_style_set_radius(&messages_notification_badge, 6);
        lv_style_set_border_width(&messages_notification_badge, 0);
        lv_style_set_bg_color(&messages_notification_badge, COLOR_NOTIFICATION_BADGE_BG);
        lv_style_set_shadow_color(&messages_notification_badge, COLOR_NOTIFICATION_BADGE_BG);
        lv_style_set_shadow_spread(&messages_notification_badge, 2);
        lv_style_set_shadow_opa(&messages_notification_badge, (255 * 40 / 100));
        lv_style_set_shadow_width(&messages_notification_badge, 12);
        lv_style_set_margin_all(&messages_notification_badge, 6);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "screen_home_#");

        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_nav_index, LV_EVENT_SCREEN_LOADED, REF_NAV_HOME);
        lv_obj_t * root = column_create(lv_obj_0, 0, BASE_LIB_UNIT_XXL, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(root, "root");
        lv_obj_set_width(root, lv_pct(100));
        lv_obj_set_height(root, lv_pct(100));
        lv_obj_set_flag(root, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_pad_top(root, 0, 0);
        lv_obj_set_style_bg_color(root, COLOR_BG, 0);
        lv_obj_set_style_bg_opa(root, (255 * 100 / 100), 0);
        lv_obj_t * hero = lv_obj_create(root);
        lv_obj_set_name(hero, "hero");
        lv_obj_set_width(hero, lv_pct(100));
        lv_obj_set_style_flex_grow(hero, 1, 0);
        lv_obj_set_flag(hero, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_add_style(hero, &hero_box, 0);
        lv_obj_t * image_0 = image_create(hero, image_home_hero);
        lv_obj_set_flag(image_0, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(image_0, LV_ALIGN_CENTER);
        lv_obj_set_width(image_0, 728);
        lv_obj_set_height(image_0, 728);
        lv_image_set_inner_align(image_0, LV_IMAGE_ALIGN_STRETCH);
        lv_obj_set_flag(image_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * image_1 = image_create(hero, logostrip_large);
        lv_obj_set_flag(image_1, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(image_1, LV_ALIGN_TOP_RIGHT);
        lv_obj_set_x(image_1, -24);
        lv_obj_set_y(image_1, 24);
        lv_obj_set_flag(image_1, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * intro = column_create(hero, 0, -24, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(intro, "intro");
        lv_obj_set_flag(intro, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(intro, LV_ALIGN_BOTTOM_LEFT);
        lv_obj_set_x(intro, BASE_LIB_UNIT_XL);
        lv_obj_set_y(intro, -32);
        lv_obj_t * row_0 = row_create(intro, 0, BASE_LIB_UNIT_LG, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);
        lv_obj_t * time_0 = time_create(row_0, &subject_hour, &subject_minute, font_xxxl);
        lv_obj_set_style_text_color(time_0, lv_color_hex(0xFFFFFF), 0);

        lv_obj_t * date_0 = date_create(row_0, &subject_day_name, &subject_day_index, &subject_year, &subject_month_name, font_xl);
        lv_obj_set_style_pad_bottom(date_0, 22, 0);
        lv_obj_set_style_text_color(date_0, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_text_opa(date_0, (255 * 75 / 100), 0);

        lv_obj_t * welcome_title = h1_create(intro, "Morning, Gábor!");
        lv_obj_set_name(welcome_title, "welcome_title");
        lv_obj_set_style_text_color(welcome_title, lv_color_hex(0xFFFFFF), 0);

        lv_obj_t * cards = row_create(root, 0, BASE_LIB_UNIT_LG, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(cards, "cards");
        lv_obj_set_width(cards, lv_pct(100));
        lv_obj_set_height(cards, 400);
        lv_obj_set_style_pad_left(cards, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_style_pad_right(cards, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_flag(cards, LV_OBJ_FLAG_CLICKABLE, true);
        lv_obj_set_scroll_snap_x(cards, LV_SCROLL_SNAP_START);
        lv_obj_set_scrollbar_mode(cards, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_flag(cards, LV_OBJ_FLAG_SCROLL_CHAIN_HOR, false);
        lv_obj_t * card_smart_home = card_empty_create(cards);
        lv_obj_set_name(card_smart_home, "card_smart_home");
        lv_obj_set_flex_flow(card_smart_home, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_smart_home, LV_FLEX_ALIGN_START, 0);
        lv_obj_set_width(card_smart_home, 280);
        lv_obj_set_height(card_smart_home, lv_pct(100));
        lv_obj_set_style_bg_color(card_smart_home, COLOR_CARD1_BG, 0);
        lv_obj_set_style_text_color(card_smart_home, COLOR_CARD1_TEXT, 0);
        lv_obj_set_flag(card_smart_home, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_smart_home, LV_OBJ_FLAG_SNAPPABLE, true);
        lv_obj_t * image_2 = image_create(card_smart_home, image_weather_cloudy);
        lv_obj_set_flag(image_2, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_set_flag(image_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(image_2, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(image_2, LV_ALIGN_BOTTOM_LEFT);
        lv_obj_set_x(image_2, -40);
        lv_obj_set_y(image_2, -24);

        lv_obj_t * row_1 = row_create(card_smart_home, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(row_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(row_1, LV_OBJ_FLAG_EVENT_BUBBLE, true);
        lv_obj_t * image_mono_0 = image_mono_create(row_1, icon_charge);
        lv_obj_set_style_image_recolor(image_mono_0, COLOR_CARD1_TEXT, 0);

        h3_create(row_1, "Smart Home");

        lv_obj_t * column_0 = column_create(card_smart_home, 0, BASE_LIB_UNIT_SM, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(column_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_width(column_0, lv_pct(100));
        lv_obj_t * row_2 = row_create(column_0, 0, BASE_LIB_UNIT_XS, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(row_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * value_xlarge_0 = value_xlarge_create(row_2, &subject_outside_temp_current, "%d°", "out");
        lv_obj_set_flag(value_xlarge_0, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * value_medium_0 = value_medium_create(card_smart_home, &subject_temp, "%d° in", "");
        lv_obj_set_flag(value_medium_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_flag(value_medium_0, LV_OBJ_FLAG_EVENT_BUBBLE, true);

        lv_obj_add_subject_set_int_event(card_smart_home, &subject_nav_index, LV_EVENT_CLICKED, REF_NAV_SMART_HOME);
        lv_obj_add_screen_create_event(card_smart_home, LV_EVENT_CLICKED, screen_smart_home_create, LV_SCREEN_LOAD_ANIM_FADE_IN, 200, 0);

        lv_obj_t * card_ev = card_empty_create(cards);
        lv_obj_set_name(card_ev, "card_ev");
        lv_obj_set_flex_flow(card_ev, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_ev, LV_FLEX_ALIGN_START, 0);
        lv_obj_set_width(card_ev, 280);
        lv_obj_set_height(card_ev, lv_pct(100));
        lv_obj_set_style_bg_color(card_ev, COLOR_CARD2_BG, 0);
        lv_obj_set_style_text_color(card_ev, COLOR_CARD2_TEXT, 0);
        lv_obj_set_flag(card_ev, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_ev, LV_OBJ_FLAG_SNAPPABLE, true);
        lv_obj_t * image_3 = image_create(card_ev, image_car_small);
        lv_obj_set_flag(image_3, LV_OBJ_FLAG_IGNORE_LAYOUT, true);
        lv_obj_set_align(image_3, LV_ALIGN_BOTTOM_LEFT);
        lv_obj_set_x(image_3, -16);
        lv_obj_set_y(image_3, 8);
        lv_obj_set_flag(image_3, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * row_3 = row_create(card_ev, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(row_3, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * image_mono_1 = image_mono_create(row_3, icon_charge);
        lv_obj_set_style_image_recolor(image_mono_1, COLOR_CARD2_TEXT, 0);

        h3_create(row_3, "EV Charging");

        lv_obj_t * column_1 = column_create(card_ev, 0, -20, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_1, lv_pct(100));
        lv_obj_set_flag(column_1, LV_OBJ_FLAG_CLICKABLE, false);
        value_xlarge_create(column_1, &subject_ev_soc, "%d", "%");

        lv_obj_t * lv_bar_0 = lv_bar_create(column_1);
        lv_bar_bind_value(lv_bar_0, &subject_ev_soc);
        lv_obj_set_width(lv_bar_0, lv_pct(100));
        lv_obj_set_height(lv_bar_0, 12);
        lv_obj_set_flag(lv_bar_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_bar_0, &ev_charging_bar, 0);
        lv_obj_add_style(lv_bar_0, &ev_charging_bar, LV_PART_INDICATOR);

        lv_obj_add_subject_set_int_event(card_ev, &subject_nav_index, LV_EVENT_CLICKED, REF_NAV_EV);
        lv_obj_add_screen_create_event(card_ev, LV_EVENT_CLICKED, screen_ev_charging_create, LV_SCREEN_LOAD_ANIM_FADE_IN, 200, 0);

        lv_obj_t * card_messages = card_empty_create(cards);
        lv_obj_set_name(card_messages, "card_messages");
        lv_obj_set_flex_flow(card_messages, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(card_messages, LV_FLEX_ALIGN_START, 0);
        lv_obj_set_width(card_messages, 280);
        lv_obj_set_height(card_messages, lv_pct(100));
        lv_obj_set_style_bg_color(card_messages, COLOR_CARD3_BG, 0);
        lv_obj_set_style_text_color(card_messages, COLOR_CARD3_TEXT, 0);
        lv_obj_set_flag(card_messages, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(card_messages, LV_OBJ_FLAG_SNAPPABLE, true);
        lv_obj_t * row_4 = row_create(card_messages, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(row_4, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * image_mono_2 = image_mono_create(row_4, icon_messages);
        lv_obj_set_style_image_recolor(image_mono_2, COLOR_CARD3_TEXT, 0);

        h3_create(row_4, "Messages");

        lv_obj_t * column_2 = column_create(card_messages, 0, 0, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_2, lv_pct(100));
        lv_obj_set_flag(column_2, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * image_4 = image_create(column_2, image_avatar_lisa);
        lv_obj_set_style_margin_top(image_4, BASE_LIB_UNIT_MD, 0);
        lv_obj_set_style_margin_bottom(image_4, BASE_LIB_UNIT_SM, 0);

        lv_obj_t * text_0 = text_create(column_2, font_md);
        lv_label_set_text(text_0, "8:32  ·  Lisa");

        lv_obj_t * text_1 = text_create(column_2, font_md);
        lv_obj_set_width(text_1, lv_pct(100));
        lv_label_set_text(text_1, "Could you pick up milk on the way, babe?");
        lv_obj_set_style_text_opa(text_1, (255 * 60 / 100), 0);

        lv_obj_t * row_5 = row_create(card_messages, 0, BASE_LIB_UNIT_MD, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_flag(row_5, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_t * lv_obj_1 = lv_obj_create(row_5);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_add_style(lv_obj_1, &messages_notification_badge, 0);

        lv_obj_t * text_2 = text_create(row_5, font_xl);
        lv_label_set_text(text_2, "3 new");

        lv_obj_add_subject_set_int_event(card_messages, &subject_nav_index, LV_EVENT_CLICKED, REF_NAV_MESSAGES);
        lv_obj_add_screen_create_event(card_messages, LV_EVENT_CLICKED, screen_messages_create, LV_SCREEN_LOAD_ANIM_FADE_IN, 200, 0);

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

