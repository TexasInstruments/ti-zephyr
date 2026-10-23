/**
 * @file ti_smart_home_gen.h
 */

#ifndef LVGL_PRO_TI_SMART_HOME_GEN_H
#define LVGL_PRO_TI_SMART_HOME_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif



/* Prototypes for target functions, needed by responsive const definitions */

void ti_smart_home_set_target(uint32_t target);
uint32_t ti_smart_home_get_target(void);
bool ti_smart_home_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define TI_SMART_HOME_TARGET_UNDEFINED  (0 << 1)
#define TI_SMART_HOME_TARGET_TARGET1    (1 << 1)
#define TI_SMART_HOME_TARGET_ALL        0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef TI_SMART_HOME_COMPILE_TARGET
#define TI_SMART_HOME_COMPILE_TARGET TI_SMART_HOME_TARGET_ALL
#endif

#define TI_SMART_HOME_CHECK_COMPILE_TARGET(target) (TI_SMART_HOME_COMPILE_TARGET & (target) ? 1 : 0)

/**
 * Use this as base unit to set spaces and sizes
 */
#define BASE_LIB_UNIT_XS 2
/**
 * Use this as base unit to set spaces and sizes
 */
#define BASE_LIB_UNIT_SM 4
/**
 * Use this as base unit to set spaces and sizes
 */
#define BASE_LIB_UNIT_MD 8
/**
 * Use this as base unit to set spaces and sizes
 */
#define BASE_LIB_UNIT_LG 16
/**
 * Use this as base unit to set spaces and sizes
 */
#define BASE_LIB_UNIT_XL 24
/**
 * Use this as base unit to set spaces and sizes
 */
#define BASE_LIB_UNIT_XXL 32
/**
 * Use this as base unit to set spaces and sizes
 */
#define BASE_LIB_UNIT_XXXL 40
/**
 * Used as radius in styles
 */
#define BASE_LIB_RADIUS_LG 32
/**
 * Used as radius in styles
 */
#define BASE_LIB_RADIUS_MD 14
/**
 * navbar index of screen_home
 */
#define REF_NAV_HOME 0
/**
 * navbar index of screen_smart_home
 */
#define REF_NAV_SMART_HOME 1
/**
 * navbar index of screen_ev_charging
 */
#define REF_NAV_EV 2
/**
 * navbar index of screen_messages
 */
#define REF_NAV_MESSAGES 3
#define COLOR_BG lv_color_hex(0xFFFEF9)
#define COLOR_PANEL lv_color_hex(0xF4EDF8)
#define COLOR_TEXT lv_color_hex(0x484242)
#define COLOR_BUTTON lv_color_hex(0xD2BAFF)
#define COLOR_BUTTON_TEXT lv_color_hex(0x5C428A)
#define COLOR_ACCENT lv_color_hex(0xBD7C0C)
#define COLOR_ACCENT_TEXT lv_color_hex3(0xFFF)


#ifndef LV_XML_EVAL_STRING_BUF_SIZE
    #define LV_XML_EVAL_STRING_BUF_SIZE 256
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

/*----------------
 * Global styles
 *----------------*/

extern lv_style_t style_bg;
extern lv_style_t style_panel;
extern lv_style_t style_button;
extern lv_style_t style_text;
extern lv_style_t style_text_accent;
extern lv_style_t style_button_pressed;
extern lv_style_t style_scrollbar;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_xs;
extern lv_font_t * font_sm;
extern lv_font_t * font_md;
extern lv_font_t * font_lg;
extern lv_font_t * font_xl;
extern lv_font_t * font_xxl;
extern lv_font_t * font_xxxl;


/*----------------
 * Images
 *----------------*/

/* Targets: any */
extern const void * icon_check;
extern const void * icon_minus;
extern const void * icon_plus;
extern const void * icon_wifi;
extern const void * icon_charge;
extern const void * icon_messages;
extern const void * icon_smart_home;
extern const void * icon_back;
extern const void * icon_chart;
extern const void * icon_range;
extern const void * icon_temp;
extern const void * icon_weather;
extern const void * icon_shuffle;
extern const void * icon_pause;
extern const void * icon_play;
extern const void * icon_prev;
extern const void * icon_next;
extern const void * icon_repeat;
extern const void * icon_spotify;
extern const void * icon_contacts;
extern const void * icon_contact;
extern const void * icon_email;
extern const void * icon_message;
extern const void * icon_call;
extern const void * image_weather_cloudy;
extern const void * image_home_hero;
extern const void * image_car_small;
extern const void * image_car_large;
extern const void * image_avatar_lisa;
extern const void * image_livecam;
extern const void * image_vacuum;
extern const void * image_kettle;
extern const void * image_wifi;
extern const void * image_album_cover;
extern const void * image_album_leaving;
extern const void * image_album_home;
extern const void * image_album_esja;
extern const void * image_album_ghosts;
extern const void * image_contact_large;
extern const void * image_contact_small;
extern const void * image_contact_alex;
extern const void * image_contact_lukas;
extern const void * image_contact_marco;
extern const void * image_contact_oliver;
extern const void * image_contact_emma;
extern const void * image_contact_thomas;
extern const void * image_contact_sofia;
extern const void * image_contact_isabella;
extern const void * image_contact_elena;
extern const void * image_contact_noah;
extern const void * image_contact_lisa;
extern const void * image_avatar_lisa_small;
extern const void * image_contact_alex_small;
extern const void * image_contact_lukas_small;
extern const void * image_contact_marco_small;
extern const void * image_contact_oliver_small;
extern const void * image_contact_emma_small;
extern const void * image_contact_thomas_small;
extern const void * image_contact_sofia_small;
extern const void * image_contact_isabella_small;
extern const void * image_contact_elena_small;
extern const void * image_contact_noah_small;
extern const void * frame;
extern const void * icon_inbox;
extern const void * image_alarm;
extern const void * image_contact_marta;
extern const void * image_contact_marta_small;
extern const void * logostrip_large;
extern const void * logostrip_small;

/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t subject_dark_mode;
extern lv_subject_t subject_nav_index;
extern lv_subject_t subject_hour;
extern lv_subject_t subject_minute;
extern lv_subject_t subject_day_index;
extern lv_subject_t subject_day_name;
extern lv_subject_t subject_month_name;
extern lv_subject_t subject_year;
extern lv_subject_t subject_temp;
extern lv_subject_t subject_ev_soc;
extern lv_subject_t subject_ev_range_km;
extern lv_subject_t subject_ev_power_kw;
extern lv_subject_t subject_outside_temp_current;
extern lv_subject_t subject_outside_temp_highest;
extern lv_subject_t subject_weather_desc;
extern lv_subject_t subject_album_name;
extern lv_subject_t subject_track_name;
extern lv_subject_t subject_play;
extern lv_subject_t subject_shuffle;
extern lv_subject_t subject_repeat;
extern lv_subject_t subject_music_progress;
extern lv_subject_t subject_vacuum_battery;
extern lv_subject_t subject_kettle_temp;
extern lv_subject_t subject_kettle_on;
extern lv_subject_t subject_vacuum_on;
extern lv_subject_t subject_wifi_on;
extern lv_subject_t subject_album_cover;
extern lv_subject_t subject_contact_img;
extern lv_subject_t subject_contact_name;
extern lv_subject_t subject_contact_email;
extern lv_subject_t subject_contact_phone;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

void on_contact_clicked_cb(lv_event_t * e);
void on_music_previous_cb(lv_event_t * e);
void on_music_next_cb(lv_event_t * e);

/**
 * Initialize the component library
 */

void ti_smart_home_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "components/buttons/button_circle/button_circle_gen.h"
#include "components/buttons/button/button_gen.h"
#include "components/cards/card_empty/card_empty_gen.h"
#include "components/controls/arc/arc_gen.h"
#include "components/controls/checkbox/checkbox_gen.h"
#include "components/controls/slider/slider_gen.h"
#include "components/controls/switch/switch_gen.h"
#include "components/controls/value_setter/value_setter_gen.h"
#include "components/header/header/header_gen.h"
#include "components/header/icon/icon_gen.h"
#include "components/headings/h1/h1_gen.h"
#include "components/headings/h2/h2_gen.h"
#include "components/headings/h3/h3_gen.h"
#include "components/headings/h4/h4_gen.h"
#include "components/layout/column/column_gen.h"
#include "components/layout/container/container_gen.h"
#include "components/layout/panel/panel_gen.h"
#include "components/layout/row/row_gen.h"
#include "components/layout/spacer/spacer_gen.h"
#include "components/list/list_contact/list_contact_gen.h"
#include "components/list/list_container/list_container_gen.h"
#include "components/list/list_item/list_item_gen.h"
#include "components/list/list_message/list_message_gen.h"
#include "components/navbar/navbar_gen.h"
#include "components/text/text_gen.h"
#include "components/visualizers/chart/chart_gen.h"
#include "components/visualizers/date/date_gen.h"
#include "components/visualizers/image_mono/image_mono_gen.h"
#include "components/visualizers/image/image_gen.h"
#include "components/visualizers/time/time_gen.h"
#include "components/visualizers/value_large/value_large_gen.h"
#include "components/visualizers/value_medium/value_medium_gen.h"
#include "components/visualizers/value_small/value_small_gen.h"
#include "components/visualizers/value_xlarge/value_xlarge_gen.h"
#include "screens/screen_ev_charging_gen.h"
#include "screens/screen_home_gen.h"
#include "screens/screen_messages_gen.h"
#include "screens/screen_smart_home_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_TI_SMART_HOME_GEN_H*/