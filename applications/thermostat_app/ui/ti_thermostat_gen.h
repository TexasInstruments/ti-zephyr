/**
 * @file ti_thermostat_gen.h
 */

#ifndef LVGL_PRO_TI_THERMOSTAT_GEN_H
#define LVGL_PRO_TI_THERMOSTAT_GEN_H

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

void ti_thermostat_set_target(uint32_t target);
uint32_t ti_thermostat_get_target(void);
bool ti_thermostat_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define TI_THERMOSTAT_TARGET_UNDEFINED  (0 << 1)
#define TI_THERMOSTAT_TARGET_TARGET1    (1 << 1)
#define TI_THERMOSTAT_TARGET_ALL        0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef TI_THERMOSTAT_COMPILE_TARGET
#define TI_THERMOSTAT_COMPILE_TARGET TI_THERMOSTAT_TARGET_ALL
#endif

#define TI_THERMOSTAT_CHECK_COMPILE_TARGET(target) (TI_THERMOSTAT_COMPILE_TARGET & (target) ? 1 : 0)

#define CONST_TEXT_PRIMARY lv_color_hex(0x14181B)
#define CONST_TEXT_SECONDARY lv_color_hex(0x8B9197)
#define CONST_TEXT_TERTIARY lv_color_hex(0xAEB4B9)
#define CONST_SURFACE_CANVAS lv_color_hex(0xF0F4F6)
#define CONST_SURFACE_CARD lv_color_hex(0xFBFBFC)
#define CONST_BORDER lv_color_hex(0xE4E6EB)
#define CONST_DIVIDER lv_color_hex(0xEDEFF1)
#define CONST_TRACK lv_color_hex(0xE4E7E9)
#define CONST_ON_ACCENT lv_color_hex(0xFFFFFF)
#define CONST_CONTROL_OFF lv_color_hex(0xCDD2D6)
#define CONST_HEAT lv_color_hex(0xE8630A)
#define CONST_COOL lv_color_hex(0x2A7FE0)
#define CONST_ACCENT lv_color_hex(0x0FA958)
#define CONST_WARN lv_color_hex(0xD97706)
#define CONST_DANGER lv_color_hex(0xDC2626)
#define CONST_TINT_NEUTRAL lv_color_hex(0xEEF1F3)
#define CONST_TINT_HEAT lv_color_hex(0xFDEEE3)
#define CONST_TINT_COOL lv_color_hex(0xE6F0FC)
#define CONST_TINT_ACCENT lv_color_hex(0xE7F5EE)
#define CONST_TINT_WARN lv_color_hex(0xFBF0E1)
#define CONST_SPACE_2 2
#define CONST_SPACE_12 12
#define CONST_RADIUS_MD 12
#define CONST_RADIUS_LG 16
#define CONST_RADIUS_PILL 999
#define CONST_SCREEN_W 320
#define CONST_SCREEN_H 480
#define CONST_GUTTER 16
#define CONST_CONTENT_W 288
#define CONST_TOPBAR_H 44
#define CONST_TABBAR_H 56
#define REF_TAB_HOME 0
#define REF_TAB_SCHEDULE 1
#define REF_SCHED_CURVE 0
#define REF_SCHED_BLOCKS 1
#define REF_MODE_HEAT 0
#define REF_MODE_COOL 1
#define REF_MODE_AUTO 2
#define REF_MODE_OFF 3
#define REF_HVAC_IDLE 0
#define REF_HVAC_HEATING 1
#define REF_HVAC_COOLING 2


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

extern lv_style_t style_hero;
extern lv_style_t style_hero_unit;
extern lv_style_t style_heading;
extern lv_style_t style_stat_md;
extern lv_style_t style_stat_sm;
extern lv_style_t style_title;
extern lv_style_t style_body_strong;
extern lv_style_t style_meta;
extern lv_style_t style_chip;
extern lv_style_t style_caption;
extern lv_style_t style_badge;
extern lv_style_t style_overline;
extern lv_style_t style_micro;
extern lv_style_t style_shadow_card;
extern lv_style_t style_shadow_knob;
extern lv_style_t style_screen_base;
extern lv_style_t style_card_surface;
extern lv_style_t style_chip_fill_neutral;
extern lv_style_t style_chip_fill_heat;
extern lv_style_t style_chip_fill_cool;
extern lv_style_t style_chip_fill_warn;
extern lv_style_t style_chip_dot_neutral;
extern lv_style_t style_chip_dot_heat;
extern lv_style_t style_chip_dot_cool;
extern lv_style_t style_chip_dot_warn;
extern lv_style_t style_chip_text_neutral;
extern lv_style_t style_chip_text_heat;
extern lv_style_t style_chip_text_cool;
extern lv_style_t style_chip_text_warn;
extern lv_style_t style_bg_idle;
extern lv_style_t style_bg_heat;
extern lv_style_t style_bg_cool;
extern lv_style_t style_arc_ind_heat;
extern lv_style_t style_arc_ind_cool;
extern lv_style_t style_arc_ind_idle;
extern lv_style_t style_block_idle;
extern lv_style_t style_block_active;
extern lv_style_t style_mode_on_heat;
extern lv_style_t style_mode_on_cool;
extern lv_style_t style_mode_on_auto;
extern lv_style_t style_mode_label_heat;
extern lv_style_t style_mode_label_cool;
extern lv_style_t style_mode_label_auto;
extern lv_style_t style_day_on;
extern lv_style_t style_day_label_on;
extern lv_style_t style_icon_heat;
extern lv_style_t style_icon_cool;
extern lv_style_t style_icon_danger;
extern lv_style_t style_icon_ink;
extern lv_style_t style_tab_active;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_hero_88;
extern lv_font_t * font_semibold_30;
extern lv_font_t * font_semibold_22;
extern lv_font_t * font_semibold_20;
extern lv_font_t * font_semibold_17;
extern lv_font_t * font_semibold_13;
extern lv_font_t * font_regular_12;


/*----------------
 * Images
 *----------------*/

/* Targets: any */
extern const void * image_logo;
extern const void * icon_flame;
extern const void * icon_snowflake;
extern const void * icon_auto;
extern const void * icon_power;

/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t subject_target_temp;
extern lv_subject_t subject_now_temp;
extern lv_subject_t subject_humidity;
extern lv_subject_t subject_outdoor_temp;
extern lv_subject_t subject_mode;
extern lv_subject_t subject_hvac_state;
extern lv_subject_t subject_hold_until;
extern lv_subject_t subject_time;
extern lv_subject_t subject_tab;
extern lv_subject_t subject_sched_day;
extern lv_subject_t subject_sched_view;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

/**
 * Initialize the component library
 */

void ti_thermostat_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "components/base/card/card_gen.h"
#include "components/base/column/column_gen.h"
#include "components/base/container/container_gen.h"
#include "components/base/label/label_gen.h"
#include "components/base/panel/panel_gen.h"
#include "components/base/row/row_gen.h"
#include "components/btn_step/btn_step_gen.h"
#include "components/chip_value/chip_value_gen.h"
#include "components/chip/chip_gen.h"
#include "components/day_chip/day_chip_gen.h"
#include "components/dial/dial_gen.h"
#include "components/mode_pill_auto/mode_pill_auto_gen.h"
#include "components/mode_pill_cool/mode_pill_cool_gen.h"
#include "components/mode_pill_heat/mode_pill_heat_gen.h"
#include "components/overline/overline_gen.h"
#include "components/row_block/row_block_gen.h"
#include "components/seg_tab/seg_tab_gen.h"
#include "components/surface/surface_gen.h"
#include "components/tab_bar/tab_bar_gen.h"
#include "components/tab_item/tab_item_gen.h"
#include "components/top_bar/top_bar_gen.h"
#include "screens/home_gen.h"
#include "screens/schedule_gen.h"
#include "widgets/wd_schedule_chart/wd_schedule_chart_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_TI_THERMOSTAT_GEN_H*/