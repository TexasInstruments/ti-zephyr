/**
 * @file motor_control_gen.h
 */

#ifndef LVGL_PRO_MOTOR_CONTROL_GEN_H
#define LVGL_PRO_MOTOR_CONTROL_GEN_H

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

void motor_control_set_target(uint32_t target);
uint32_t motor_control_get_target(void);
bool motor_control_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define MOTOR_CONTROL_TARGET_UNDEFINED  (0 << 1)
#define MOTOR_CONTROL_TARGET_TARGET1    (1 << 1)
#define MOTOR_CONTROL_TARGET_ALL        0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef MOTOR_CONTROL_COMPILE_TARGET
#define MOTOR_CONTROL_COMPILE_TARGET MOTOR_CONTROL_TARGET_ALL
#endif

#define MOTOR_CONTROL_CHECK_COMPILE_TARGET(target) (MOTOR_CONTROL_COMPILE_TARGET & (target) ? 1 : 0)

#define CONST_CANVAS lv_color_hex(0x0E1113)
#define CONST_CANVAS_GRAD lv_color_hex(0x151B20)
#define CONST_SURFACE_CARD lv_color_hex(0x191E23)
#define CONST_SURFACE_INSET lv_color_hex(0x0B0E10)
#define CONST_SURFACE_RAISED lv_color_hex(0x232930)
#define CONST_BORDER lv_color_hex(0x2B333A)
#define CONST_BORDER_STRONG lv_color_hex(0x3A444C)
#define CONST_DIVIDER lv_color_hex(0x222930)
#define CONST_TRACK lv_color_hex(0x262E35)
#define CONST_TEXT_PRIMARY lv_color_hex(0xF1F4F6)
#define CONST_TEXT_SECONDARY lv_color_hex(0x939CA3)
#define CONST_TEXT_TERTIARY lv_color_hex(0x69737A)
#define CONST_TEXT_OVERLINE lv_color_hex(0x7E888F)
#define CONST_ACCENT lv_color_hex(0x1BC46E)
#define CONST_ACCENT_PRESSED lv_color_hex(0x14A25A)
#define CONST_WARN lv_color_hex(0xF0A22E)
#define CONST_DANGER lv_color_hex(0xF25449)
#define CONST_DANGER_PRESSED lv_color_hex(0xC93E35)
#define CONST_ON_ACCENT lv_color_hex(0x07130C)
#define CONST_ON_DANGER lv_color_hex(0xFFFFFF)
#define CONST_ON_DANGER_SOLID lv_color_hex(0x150404)
#define CONST_TINT_NEUTRAL lv_color_hex(0x232A31)
#define CONST_TINT_ACCENT lv_color_hex(0x11301F)
#define CONST_TINT_DANGER lv_color_hex(0x361A18)
#define CONST_TINT_WARN lv_color_hex(0x33260E)
#define CONST_SPACE_2 2
#define CONST_SPACE_4 4
#define CONST_SPACE_8 8
#define CONST_SPACE_12 12
#define CONST_SPACE_16 16
#define CONST_SPACE_20 20
#define CONST_SPACE_24 24
#define CONST_RADIUS_SM 8
#define CONST_RADIUS_MD 12
#define CONST_RADIUS_LG 16
#define CONST_RADIUS_PILL 999
#define CONST_SCREEN_W 320
#define CONST_SCREEN_H 480
#define CONST_GUTTER 16
#define CONST_CONTENT_W 288
#define CONST_HEADER_H 56
#define CONST_TABBAR_H 76
#define CONST_TOUCH_LG 72
#define CONST_NAV_PRIMARY_W 160
#define CONST_HEADER_ACTION 40
#define CONST_SPEED_MIN 0
#define CONST_SPEED_MAX 3600
#define CONST_SPEED_STEP 50
#define CONST_SPEED_STEP_DOWN -50
#define REF_STATE_STOPPED 0
#define REF_STATE_RUNNING 1
#define REF_STATE_FAULT 2
#define REF_DIR_CW 0
#define REF_DIR_CCW 1
#define REF_OFF 0
#define REF_ON 1


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

extern lv_style_t style_hero_unit;
extern lv_style_t style_stat_lg;
extern lv_style_t style_heading;
extern lv_style_t style_stat_md;
extern lv_style_t style_title;
extern lv_style_t style_subtitle;
extern lv_style_t style_body_strong;
extern lv_style_t style_chip;
extern lv_style_t style_meta;
extern lv_style_t style_badge;
extern lv_style_t style_overline;
extern lv_style_t style_ink;
extern lv_style_t style_ink_secondary;
extern lv_style_t style_ink_tertiary;
extern lv_style_t style_ink_overline;
extern lv_style_t style_ink_accent;
extern lv_style_t style_ink_danger;
extern lv_style_t style_ink_on_accent;
extern lv_style_t style_ink_on_danger_solid;
extern lv_style_t style_screen_base;
extern lv_style_t style_canvas;
extern lv_style_t style_card;
extern lv_style_t style_inset;
extern lv_style_t style_chip_base;
extern lv_style_t style_chip_accent;
extern lv_style_t style_chip_danger;
extern lv_style_t style_dot_neutral;
extern lv_style_t style_dot_accent;
extern lv_style_t style_dot_danger;
extern lv_style_t style_btn_base;
extern lv_style_t style_btn_accent;
extern lv_style_t style_btn_accent_pressed;
extern lv_style_t style_btn_danger;
extern lv_style_t style_btn_danger_pressed;
extern lv_style_t style_btn_ghost;
extern lv_style_t style_btn_ghost_pressed;
extern lv_style_t style_btn_flat;
extern lv_style_t style_btn_disabled;
extern lv_style_t style_icon_ink;
extern lv_style_t style_icon_secondary;
extern lv_style_t style_icon_tertiary;
extern lv_style_t style_icon_accent;
extern lv_style_t style_icon_danger;
extern lv_style_t style_icon_on_accent;
extern lv_style_t style_icon_on_danger;
extern lv_style_t style_icon_on_danger_solid;
extern lv_style_t style_hero;
extern lv_style_t style_hero_dim;
extern lv_style_t style_hero_fault;
extern lv_style_t style_scrollbar_hidden;
extern lv_style_t style_table;
extern lv_style_t style_table_cell;
extern lv_style_t style_chart;
extern lv_style_t style_chart_items;
extern lv_style_t style_checkbox;
extern lv_style_t style_checkbox_ind;
extern lv_style_t style_checkbox_ind_checked;
extern lv_style_t style_seg;
extern lv_style_t style_seg_selected;
extern lv_style_t style_bar_accent;
extern lv_style_t style_bar_warn;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_hero_64;
extern lv_font_t * font_semibold_28;
extern lv_font_t * font_semibold_20;
extern lv_font_t * font_semibold_15;
extern lv_font_t * font_semibold_12;


/*----------------
 * Images
 *----------------*/

/* Targets: any */
extern const void * image_logostrip;
extern const void * icon_tab_run;
extern const void * icon_tab_trend;
extern const void * icon_tab_faults;
extern const void * icon_tab_config;
extern const void * icon_minus;
extern const void * icon_plus;
extern const void * icon_estop;
extern const void * icon_dir_cw;
extern const void * icon_dir_ccw;
extern const void * icon_clear;
extern const void * icon_check;

/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t subject_state;
extern lv_subject_t subject_rpm_ref;
extern lv_subject_t subject_rpm_act;
extern lv_subject_t subject_current_str;
extern lv_subject_t subject_temp;
extern lv_subject_t subject_load;
extern lv_subject_t subject_bus_str;
extern lv_subject_t subject_peak_rpm;
extern lv_subject_t subject_avg_load;
extern lv_subject_t subject_runtime_str;
extern lv_subject_t subject_fault_count;
extern lv_subject_t subject_fault_active;
extern lv_subject_t subject_dir;
extern lv_subject_t subject_ramp;
extern lv_subject_t subject_cfg_softstart;
extern lv_subject_t subject_cfg_reverse;
extern lv_subject_t subject_cfg_derate;
extern lv_subject_t subject_alert_never;
extern lv_subject_t subject_tab;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

/**
 * Initialize the component library
 */

void motor_control_init_gen(const char * asset_path);

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
#include "components/btn_run/btn_run_gen.h"
#include "components/btn_step/btn_step_gen.h"
#include "components/check_row/check_row_gen.h"
#include "components/chip/chip_gen.h"
#include "components/estop_tag/estop_tag_gen.h"
#include "components/header_action/header_action_gen.h"
#include "components/header/header_gen.h"
#include "components/odometer/odometer_gen.h"
#include "components/overline/overline_gen.h"
#include "components/stat_tile/stat_tile_gen.h"
#include "components/stat/stat_gen.h"
#include "components/tab_bar/tab_bar_gen.h"
#include "components/tab_item/tab_item_gen.h"
#include "screens/config_gen.h"
#include "screens/faults_gen.h"
#include "screens/run_gen.h"
#include "screens/trend_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_MOTOR_CONTROL_GEN_H*/