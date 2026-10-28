/**
 * @file ti_washing_machine_gen.h
 */

#ifndef LVGL_PRO_TI_WASHING_MACHINE_GEN_H
#define LVGL_PRO_TI_WASHING_MACHINE_GEN_H

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

void ti_washing_machine_set_target(uint32_t target);
uint32_t ti_washing_machine_get_target(void);
bool ti_washing_machine_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define TI_WASHING_MACHINE_TARGET_UNDEFINED  (0 << 1)
#define TI_WASHING_MACHINE_TARGET_TARGET1    (1 << 1)
#define TI_WASHING_MACHINE_TARGET_ALL        0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef TI_WASHING_MACHINE_COMPILE_TARGET
#define TI_WASHING_MACHINE_COMPILE_TARGET TI_WASHING_MACHINE_TARGET_ALL
#endif

#define TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(target) (TI_WASHING_MACHINE_COMPILE_TARGET & (target) ? 1 : 0)

#define CONST_TEXT_PRIMARY lv_color_hex(0x14181B)
#define CONST_TEXT_SECONDARY lv_color_hex(0x8B9197)
#define CONST_TEXT_TERTIARY lv_color_hex(0xAEB4B9)
#define CONST_TEXT_OVERLINE lv_color_hex(0x4E565C)
#define CONST_SURFACE_CANVAS lv_color_hex(0xF0F4F6)
#define CONST_SURFACE_CARD lv_color_hex(0xFBFBFC)
#define CONST_BORDER lv_color_hex(0xE4E6EB)
#define CONST_DIVIDER lv_color_hex(0xEDEFF1)
#define CONST_TRACK lv_color_hex(0xE4E7E9)
#define CONST_ON_ACCENT lv_color_hex(0xFFFFFF)
#define CONST_ACCENT lv_color_hex(0x3488AF)
#define CONST_ACCENT_BRIGHT lv_color_hex(0x5FB4D6)
#define CONST_ACCENT_PRESSED lv_color_hex(0x27688A)
#define CONST_DANGER lv_color_hex(0xDC2626)
#define CONST_DANGER_PRESSED lv_color_hex(0xB91C1C)
#define CONST_TEMP_20 lv_color_hex(0x3A6FA0)
#define CONST_TEMP_90 lv_color_hex(0xB03A24)
#define CONST_TINT_ACCENT lv_color_hex(0xE3EEF6)
#define CONST_RADIUS_MD 12
#define CONST_RADIUS_LG 16
#define CONST_SCREEN_W 480
#define CONST_SCREEN_H 320
#define CONST_GUTTER 20
#define CONST_CONTENT_W 440
#define CONST_COL_HOME 234
#define CONST_TOUCH 58
#define REF_ON 1
#define REF_STATE_IDLE 0
#define REF_STATE_RUNNING 1
#define REF_STATE_COMPLETE 3
#define REF_PROG_COTTON 0
#define REF_PROG_ECO 1
#define REF_PROG_SYNTH 2
#define REF_PROG_QUICK 3
#define REF_PHASE_PREWASH 0
#define REF_PHASE_WASH 1
#define REF_PHASE_RINSE 2
#define REF_PHASE_SPIN 3
#define REF_DOOR_OPEN 0
#define REF_DOOR_LOCKED 1


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
extern lv_style_t style_body_strong;
extern lv_style_t style_key_legend;
extern lv_style_t style_body;
extern lv_style_t style_meta;
extern lv_style_t style_chip;
extern lv_style_t style_micro;
extern lv_style_t style_overline;
extern lv_style_t style_key_hint;
extern lv_style_t style_ink;
extern lv_style_t style_ink_2;
extern lv_style_t style_ink_3;
extern lv_style_t style_ink_overline;
extern lv_style_t style_ink_inv;
extern lv_style_t style_icon_ink;
extern lv_style_t style_icon_2;
extern lv_style_t style_icon_accent;
extern lv_style_t style_icon_3;
extern lv_style_t style_icon_inv;
extern lv_style_t style_screen_base;
extern lv_style_t style_card_surface;
extern lv_style_t style_pill;
extern lv_style_t style_frame_door_outer;
extern lv_style_t style_frame_door_inner;
extern lv_style_t style_chip_fill_accent;
extern lv_style_t style_chip_dot_accent;
extern lv_style_t style_chip_text_accent;
extern lv_style_t style_arc_track;
extern lv_style_t style_arc_ind;
extern lv_style_t style_btn_ghost_pressed;
extern lv_style_t style_card_selected;
extern lv_style_t style_card_selected_pressed;
extern lv_style_t style_card_selected_edge;
extern lv_style_t style_key_accent;
extern lv_style_t style_key_accent_pressed;
extern lv_style_t style_key_danger;
extern lv_style_t style_key_danger_pressed;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_semibold_26;
extern lv_font_t * font_semibold_17;
extern lv_font_t * font_regular_15;
extern lv_font_t * font_medium_13;
extern lv_font_t * font_hero_76;
extern lv_font_t * font_medium_24;


/*----------------
 * Images
 *----------------*/

/* Targets: any */
extern const void * icon_prog_cotton;
extern const void * icon_prog_eco;
extern const void * icon_prog_synth;
extern const void * icon_prog_quick;
extern const void * icon_ph_pre;
extern const void * icon_ph_wash;
extern const void * icon_ph_rinse;
extern const void * icon_ph_spin;
extern const void * icon_unlock;
extern const void * icon_back;
extern const void * icon_options;
extern const void * icon_opt_prewash;
extern const void * icon_opt_rinse;
extern const void * icon_opt_delay;
extern const void * img_machine;
extern const void * img_logostrip;
extern const void * img_door;
extern const void * img_clothes;

/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t subject_program;
extern lv_subject_t subject_temp_idx;
extern lv_subject_t subject_temp_c;
extern lv_subject_t subject_spin_rpm;
extern lv_subject_t subject_water_target;
extern lv_subject_t subject_water_level;
extern lv_subject_t subject_opt_prewash;
extern lv_subject_t subject_opt_rinse;
extern lv_subject_t subject_opt_delay;
extern lv_subject_t subject_progress;
extern lv_subject_t subject_state;
extern lv_subject_t subject_door;
extern lv_subject_t subject_time_left_str;
extern lv_subject_t subject_time_unit_str;
extern lv_subject_t subject_phase;
extern lv_subject_t subject_ready;
extern lv_subject_t subject_hint_str;
extern lv_subject_t subject_duration_str;
extern lv_subject_t subject_program_name;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

void wash_sim_screen_loaded_cb(lv_event_t * e);

/**
 * Initialize the component library
 */

void ti_washing_machine_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "components/base/column/column_gen.h"
#include "components/base/container/container_gen.h"
#include "components/base/label/label_gen.h"
#include "components/base/row/row_gen.h"
#include "components/btn_back/btn_back_gen.h"
#include "components/btn_chip/btn_chip_gen.h"
#include "components/btn_fabric/btn_fabric_gen.h"
#include "components/card_phase/card_phase_gen.h"
#include "components/chip/chip_gen.h"
#include "components/drum/drum_gen.h"
#include "components/key_blocked/key_blocked_gen.h"
#include "components/key_hint/key_hint_gen.h"
#include "components/nav_header/nav_header_gen.h"
#include "components/overline/overline_gen.h"
#include "components/phase_step/phase_step_gen.h"
#include "components/pill_spin/pill_spin_gen.h"
#include "components/row_option/row_option_gen.h"
#include "components/slider_temp/slider_temp_gen.h"
#include "components/slider_water/slider_water_gen.h"
#include "components/water_level/water_level_gen.h"
#include "screens/cycle_gen.h"
#include "screens/home_gen.h"
#include "screens/setup_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_TI_WASHING_MACHINE_GEN_H*/