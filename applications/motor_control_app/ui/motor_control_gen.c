/**
 * @file motor_control_gen.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "motor_control_gen.h"

#if defined(LV_USE_XML) && LV_USE_XML
#endif /* LV_USE_XML */

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

#if defined(LV_USE_XML) && LV_USE_XML
static void check_font(lv_font_t ** font, const char * name);
#endif /* LV_USE_XML */

/**********************
 *  STATIC VARIABLES
 **********************/

static uint32_t motor_control_target = MOTOR_CONTROL_TARGET_ALL;

/*----------------
 * Translations
 *----------------*/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/*--------------------
 *  Permanent screens
 *-------------------*/

/*----------------
 * Fonts
 *----------------*/

lv_font_t * font_hero_64;
extern lv_font_t font_hero_64_data;
lv_font_t * font_semibold_28;
extern lv_font_t font_semibold_28_data;
lv_font_t * font_semibold_20;
extern lv_font_t font_semibold_20_data;
lv_font_t * font_semibold_15;
extern lv_font_t font_semibold_15_data;
lv_font_t * font_semibold_12;
extern lv_font_t font_semibold_12_data;

/*----------------
 * Images
 *----------------*/

/* Targets: any */
const void * image_logostrip = NULL;
extern const void * image_logostrip_data;
const void * icon_tab_run = NULL;
extern const void * icon_tab_run_data;
const void * icon_tab_trend = NULL;
extern const void * icon_tab_trend_data;
const void * icon_tab_faults = NULL;
extern const void * icon_tab_faults_data;
const void * icon_tab_config = NULL;
extern const void * icon_tab_config_data;
const void * icon_minus = NULL;
extern const void * icon_minus_data;
const void * icon_plus = NULL;
extern const void * icon_plus_data;
const void * icon_estop = NULL;
extern const void * icon_estop_data;
const void * icon_dir_cw = NULL;
extern const void * icon_dir_cw_data;
const void * icon_dir_ccw = NULL;
extern const void * icon_dir_ccw_data;
const void * icon_clear = NULL;
extern const void * icon_clear_data;
const void * icon_check = NULL;
extern const void * icon_check_data;

/*----------------
 * Global styles
 *----------------*/

lv_style_t style_hero_unit;
lv_style_t style_stat_lg;
lv_style_t style_heading;
lv_style_t style_stat_md;
lv_style_t style_title;
lv_style_t style_subtitle;
lv_style_t style_body_strong;
lv_style_t style_chip;
lv_style_t style_meta;
lv_style_t style_badge;
lv_style_t style_overline;
lv_style_t style_ink;
lv_style_t style_ink_secondary;
lv_style_t style_ink_tertiary;
lv_style_t style_ink_overline;
lv_style_t style_ink_accent;
lv_style_t style_ink_danger;
lv_style_t style_ink_on_accent;
lv_style_t style_ink_on_danger_solid;
lv_style_t style_screen_base;
lv_style_t style_canvas;
lv_style_t style_card;
lv_style_t style_inset;
lv_style_t style_chip_base;
lv_style_t style_chip_accent;
lv_style_t style_chip_danger;
lv_style_t style_dot_neutral;
lv_style_t style_dot_accent;
lv_style_t style_dot_danger;
lv_style_t style_btn_base;
lv_style_t style_btn_accent;
lv_style_t style_btn_accent_pressed;
lv_style_t style_btn_danger;
lv_style_t style_btn_danger_pressed;
lv_style_t style_btn_ghost;
lv_style_t style_btn_ghost_pressed;
lv_style_t style_btn_flat;
lv_style_t style_btn_disabled;
lv_style_t style_icon_ink;
lv_style_t style_icon_secondary;
lv_style_t style_icon_tertiary;
lv_style_t style_icon_accent;
lv_style_t style_icon_danger;
lv_style_t style_icon_on_accent;
lv_style_t style_icon_on_danger;
lv_style_t style_icon_on_danger_solid;
lv_style_t style_hero;
lv_style_t style_hero_dim;
lv_style_t style_hero_fault;
lv_style_t style_scrollbar_hidden;
lv_style_t style_table;
lv_style_t style_table_cell;
lv_style_t style_chart;
lv_style_t style_chart_items;
lv_style_t style_checkbox;
lv_style_t style_checkbox_ind;
lv_style_t style_checkbox_ind_checked;
lv_style_t style_seg;
lv_style_t style_seg_selected;
lv_style_t style_bar_accent;
lv_style_t style_bar_warn;

/*----------------
 * Subjects
 *----------------*/

lv_subject_t subject_state;
lv_subject_t subject_rpm_ref;
lv_subject_t subject_rpm_act;
lv_subject_t subject_current_str;
lv_subject_t subject_temp;
lv_subject_t subject_load;
lv_subject_t subject_bus_str;
lv_subject_t subject_peak_rpm;
lv_subject_t subject_avg_load;
lv_subject_t subject_runtime_str;
lv_subject_t subject_fault_count;
lv_subject_t subject_fault_active;
lv_subject_t subject_dir;
lv_subject_t subject_ramp;
lv_subject_t subject_cfg_softstart;
lv_subject_t subject_cfg_reverse;
lv_subject_t subject_cfg_derate;
lv_subject_t subject_alert_never;
lv_subject_t subject_tab;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void motor_control_init_gen(const char * asset_path)
{
    /* When running from the editor the theme set from the XML should overwrite this */
#if !defined(LV_EDITOR_PREVIEW)
#if LV_USE_THEME_SIMPLE
    lv_display_t * disp = lv_display_get_default();
    lv_theme_t * th = lv_theme_simple_init(disp);
    lv_display_set_theme(disp, th);
#else
    LV_LOG_WARN("Simple theme is selected in project.xml but LV_USE_THEME_SIMPLE is disabled");
#endif
#endif /*LV_EDITOR_PREVIEW*/


    /*----------------
     * Fonts
     *----------------*/

    /* Targets: any */

    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        if (!font_hero_64) {
            /* font_hero_64 */
            /* get font 'font_hero_64' from a C array */
            font_hero_64 = &font_hero_64_data;

        }
        if (!font_semibold_28) {
            /* font_semibold_28 */
            /* get font 'font_semibold_28' from a C array */
            font_semibold_28 = &font_semibold_28_data;

        }
        if (!font_semibold_20) {
            /* font_semibold_20 */
            /* get font 'font_semibold_20' from a C array */
            font_semibold_20 = &font_semibold_20_data;

        }
        if (!font_semibold_15) {
            /* font_semibold_15 */
            /* get font 'font_semibold_15' from a C array */
            font_semibold_15 = &font_semibold_15_data;

        }
        if (!font_semibold_12) {
            /* font_semibold_12 */
            /* get font 'font_semibold_12' from a C array */
            font_semibold_12 = &font_semibold_12_data;

        }
    }
    #endif

    /*----------------
     * Images
     *----------------*/

    /* Targets: any */
    #if MOTOR_CONTROL_CHECK_COMPILE_TARGET(MOTOR_CONTROL_TARGET_ALL)
    if (motor_control_check_target(MOTOR_CONTROL_TARGET_ALL)) {
        /* image_logostrip */
        if (!image_logostrip) {
            image_logostrip = &image_logostrip_data;
        }
        /* icon_tab_run */
        if (!icon_tab_run) {
            icon_tab_run = &icon_tab_run_data;
        }
        /* icon_tab_trend */
        if (!icon_tab_trend) {
            icon_tab_trend = &icon_tab_trend_data;
        }
        /* icon_tab_faults */
        if (!icon_tab_faults) {
            icon_tab_faults = &icon_tab_faults_data;
        }
        /* icon_tab_config */
        if (!icon_tab_config) {
            icon_tab_config = &icon_tab_config_data;
        }
        /* icon_minus */
        if (!icon_minus) {
            icon_minus = &icon_minus_data;
        }
        /* icon_plus */
        if (!icon_plus) {
            icon_plus = &icon_plus_data;
        }
        /* icon_estop */
        if (!icon_estop) {
            icon_estop = &icon_estop_data;
        }
        /* icon_dir_cw */
        if (!icon_dir_cw) {
            icon_dir_cw = &icon_dir_cw_data;
        }
        /* icon_dir_ccw */
        if (!icon_dir_ccw) {
            icon_dir_ccw = &icon_dir_ccw_data;
        }
        /* icon_clear */
        if (!icon_clear) {
            icon_clear = &icon_clear_data;
        }
        /* icon_check */
        if (!icon_check) {
            icon_check = &icon_check_data;
        }
    }
    #endif

    /*----------------
     * Global styles
     *----------------*/

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_hero_unit);
        lv_style_init(&style_stat_lg);
        lv_style_init(&style_heading);
        lv_style_init(&style_stat_md);
        lv_style_init(&style_title);
        lv_style_init(&style_subtitle);
        lv_style_init(&style_body_strong);
        lv_style_init(&style_chip);
        lv_style_init(&style_meta);
        lv_style_init(&style_badge);
        lv_style_init(&style_overline);
        lv_style_init(&style_ink);
        lv_style_init(&style_ink_secondary);
        lv_style_init(&style_ink_tertiary);
        lv_style_init(&style_ink_overline);
        lv_style_init(&style_ink_accent);
        lv_style_init(&style_ink_danger);
        lv_style_init(&style_ink_on_accent);
        lv_style_init(&style_ink_on_danger_solid);
        lv_style_init(&style_screen_base);
        lv_style_init(&style_canvas);
        lv_style_init(&style_card);
        lv_style_init(&style_inset);
        lv_style_init(&style_chip_base);
        lv_style_init(&style_chip_accent);
        lv_style_init(&style_chip_danger);
        lv_style_init(&style_dot_neutral);
        lv_style_init(&style_dot_accent);
        lv_style_init(&style_dot_danger);
        lv_style_init(&style_btn_base);
        lv_style_init(&style_btn_accent);
        lv_style_init(&style_btn_accent_pressed);
        lv_style_init(&style_btn_danger);
        lv_style_init(&style_btn_danger_pressed);
        lv_style_init(&style_btn_ghost);
        lv_style_init(&style_btn_ghost_pressed);
        lv_style_init(&style_btn_flat);
        lv_style_init(&style_btn_disabled);
        lv_style_init(&style_icon_ink);
        lv_style_init(&style_icon_secondary);
        lv_style_init(&style_icon_tertiary);
        lv_style_init(&style_icon_accent);
        lv_style_init(&style_icon_danger);
        lv_style_init(&style_icon_on_accent);
        lv_style_init(&style_icon_on_danger);
        lv_style_init(&style_icon_on_danger_solid);
        lv_style_init(&style_hero);
        lv_style_init(&style_hero_dim);
        lv_style_init(&style_hero_fault);
        lv_style_init(&style_scrollbar_hidden);
        lv_style_init(&style_table);
        lv_style_init(&style_table_cell);
        lv_style_init(&style_chart);
        lv_style_init(&style_chart_items);
        lv_style_init(&style_checkbox);
        lv_style_init(&style_checkbox_ind);
        lv_style_init(&style_checkbox_ind_checked);
        lv_style_init(&style_seg);
        lv_style_init(&style_seg_selected);
        lv_style_init(&style_bar_accent);
        lv_style_init(&style_bar_warn);

        lv_style_set_text_font(&style_hero_unit, font_semibold_20);
        lv_style_set_text_letter_space(&style_hero_unit, -1);
        lv_style_set_text_font(&style_stat_lg, font_semibold_28);
        lv_style_set_text_letter_space(&style_stat_lg, -1);
        lv_style_set_text_font(&style_heading, font_semibold_20);
        lv_style_set_text_font(&style_stat_md, font_semibold_20);
        lv_style_set_text_font(&style_title, font_semibold_15);
        lv_style_set_text_font(&style_subtitle, font_semibold_15);
        lv_style_set_text_font(&style_body_strong, font_semibold_15);
        lv_style_set_text_font(&style_chip, font_semibold_12);
        lv_style_set_text_font(&style_meta, font_semibold_12);
        lv_style_set_text_font(&style_badge, font_semibold_12);
        lv_style_set_text_font(&style_overline, font_semibold_12);
        lv_style_set_text_letter_space(&style_overline, 1);
        lv_style_set_text_color(&style_ink, CONST_TEXT_PRIMARY);
        lv_style_set_text_color(&style_ink_secondary, CONST_TEXT_SECONDARY);
        lv_style_set_text_color(&style_ink_tertiary, CONST_TEXT_TERTIARY);
        lv_style_set_text_color(&style_ink_overline, CONST_TEXT_OVERLINE);
        lv_style_set_text_color(&style_ink_accent, CONST_ACCENT);
        lv_style_set_text_color(&style_ink_danger, CONST_DANGER);
        lv_style_set_text_color(&style_ink_on_accent, CONST_ON_ACCENT);
        lv_style_set_text_color(&style_ink_on_danger_solid, CONST_ON_DANGER_SOLID);
        lv_style_set_border_width(&style_screen_base, 0);
        lv_style_set_radius(&style_screen_base, 0);
        lv_style_set_shadow_width(&style_screen_base, 0);
        lv_style_set_shadow_opa(&style_screen_base, 0);
        lv_style_set_pad_all(&style_screen_base, 0);
        lv_style_set_bg_color(&style_canvas, CONST_CANVAS);
        lv_style_set_bg_opa(&style_canvas, (255 * 100 / 100));
        lv_style_set_bg_grad_color(&style_canvas, CONST_CANVAS_GRAD);
        lv_style_set_bg_grad_dir(&style_canvas, LV_GRAD_DIR_VER);
        lv_style_set_bg_color(&style_card, CONST_SURFACE_CARD);
        lv_style_set_bg_opa(&style_card, (255 * 100 / 100));
        lv_style_set_border_color(&style_card, CONST_BORDER);
        lv_style_set_border_width(&style_card, 1);
        lv_style_set_radius(&style_card, CONST_RADIUS_LG);
        lv_style_set_shadow_width(&style_card, 0);
        lv_style_set_shadow_opa(&style_card, 0);
        lv_style_set_bg_color(&style_inset, CONST_SURFACE_INSET);
        lv_style_set_bg_opa(&style_inset, (255 * 100 / 100));
        lv_style_set_border_width(&style_inset, 0);
        lv_style_set_radius(&style_inset, CONST_RADIUS_MD);
        lv_style_set_shadow_width(&style_inset, 0);
        lv_style_set_shadow_opa(&style_inset, 0);
        lv_style_set_radius(&style_chip_base, CONST_RADIUS_PILL);
        lv_style_set_border_width(&style_chip_base, 0);
        lv_style_set_bg_color(&style_chip_accent, CONST_TINT_ACCENT);
        lv_style_set_bg_opa(&style_chip_accent, (255 * 100 / 100));
        lv_style_set_bg_color(&style_chip_danger, CONST_TINT_DANGER);
        lv_style_set_bg_opa(&style_chip_danger, (255 * 100 / 100));
        lv_style_set_bg_color(&style_dot_neutral, CONST_TEXT_SECONDARY);
        lv_style_set_bg_opa(&style_dot_neutral, (255 * 100 / 100));
        lv_style_set_bg_color(&style_dot_accent, CONST_ACCENT);
        lv_style_set_bg_opa(&style_dot_accent, (255 * 100 / 100));
        lv_style_set_bg_color(&style_dot_danger, CONST_DANGER);
        lv_style_set_bg_opa(&style_dot_danger, (255 * 100 / 100));
        lv_style_set_radius(&style_btn_base, CONST_RADIUS_MD);
        lv_style_set_border_width(&style_btn_base, 0);
        lv_style_set_shadow_width(&style_btn_base, 0);
        lv_style_set_shadow_opa(&style_btn_base, 0);
        lv_style_set_bg_color(&style_btn_accent, CONST_ACCENT);
        lv_style_set_bg_opa(&style_btn_accent, (255 * 100 / 100));
        lv_style_set_bg_color(&style_btn_accent_pressed, CONST_ACCENT_PRESSED);
        lv_style_set_bg_opa(&style_btn_accent_pressed, (255 * 100 / 100));
        lv_style_set_bg_color(&style_btn_danger, CONST_DANGER);
        lv_style_set_bg_opa(&style_btn_danger, (255 * 100 / 100));
        lv_style_set_bg_color(&style_btn_danger_pressed, CONST_DANGER_PRESSED);
        lv_style_set_bg_opa(&style_btn_danger_pressed, (255 * 100 / 100));
        lv_style_set_bg_color(&style_btn_ghost, CONST_SURFACE_RAISED);
        lv_style_set_bg_opa(&style_btn_ghost, (255 * 100 / 100));
        lv_style_set_border_color(&style_btn_ghost, CONST_BORDER_STRONG);
        lv_style_set_border_width(&style_btn_ghost, 1);
        lv_style_set_bg_color(&style_btn_ghost_pressed, CONST_BORDER_STRONG);
        lv_style_set_bg_opa(&style_btn_ghost_pressed, (255 * 100 / 100));
        lv_style_set_bg_opa(&style_btn_flat, 0);
        lv_style_set_border_width(&style_btn_flat, 0);
        lv_style_set_shadow_width(&style_btn_flat, 0);
        lv_style_set_shadow_opa(&style_btn_flat, 0);
        lv_style_set_pad_all(&style_btn_flat, 0);
        lv_style_set_radius(&style_btn_flat, CONST_RADIUS_SM);
        lv_style_set_bg_color(&style_btn_disabled, CONST_SURFACE_RAISED);
        lv_style_set_bg_opa(&style_btn_disabled, (255 * 100 / 100));
        lv_style_set_border_width(&style_btn_disabled, 0);
        lv_style_set_image_recolor(&style_icon_ink, CONST_TEXT_PRIMARY);
        lv_style_set_image_recolor_opa(&style_icon_ink, 255);
        lv_style_set_image_recolor(&style_icon_secondary, CONST_TEXT_SECONDARY);
        lv_style_set_image_recolor_opa(&style_icon_secondary, 255);
        lv_style_set_image_recolor(&style_icon_tertiary, CONST_TEXT_TERTIARY);
        lv_style_set_image_recolor_opa(&style_icon_tertiary, 255);
        lv_style_set_image_recolor(&style_icon_accent, CONST_ACCENT);
        lv_style_set_image_recolor_opa(&style_icon_accent, 255);
        lv_style_set_image_recolor(&style_icon_danger, CONST_DANGER);
        lv_style_set_image_recolor_opa(&style_icon_danger, 255);
        lv_style_set_image_recolor(&style_icon_on_accent, CONST_ON_ACCENT);
        lv_style_set_image_recolor_opa(&style_icon_on_accent, 255);
        lv_style_set_image_recolor(&style_icon_on_danger, CONST_ON_DANGER);
        lv_style_set_image_recolor_opa(&style_icon_on_danger, 255);
        lv_style_set_image_recolor(&style_icon_on_danger_solid, CONST_ON_DANGER_SOLID);
        lv_style_set_image_recolor_opa(&style_icon_on_danger_solid, 255);
        lv_style_set_text_font(&style_hero, font_hero_64);
        lv_style_set_text_letter_space(&style_hero, -3);
        lv_style_set_text_color(&style_hero, CONST_TEXT_PRIMARY);
        lv_style_set_text_color(&style_hero_dim, CONST_TEXT_TERTIARY);
        lv_style_set_text_color(&style_hero_fault, CONST_DANGER);
        lv_style_set_width(&style_scrollbar_hidden, 0);
        lv_style_set_bg_opa(&style_scrollbar_hidden, 0);
        lv_style_set_pad_all(&style_scrollbar_hidden, 0);
        lv_style_set_bg_opa(&style_table, 0);
        lv_style_set_border_width(&style_table, 0);
        lv_style_set_pad_all(&style_table, 0);
        lv_style_set_radius(&style_table, 0);
        lv_style_set_shadow_width(&style_table, 0);
        lv_style_set_shadow_opa(&style_table, 0);
        lv_style_set_bg_color(&style_table_cell, CONST_SURFACE_CARD);
        lv_style_set_bg_opa(&style_table_cell, 0);
        lv_style_set_border_color(&style_table_cell, CONST_DIVIDER);
        lv_style_set_border_width(&style_table_cell, 1);
        lv_style_set_border_side(&style_table_cell, LV_BORDER_SIDE_BOTTOM);
        lv_style_set_pad_left(&style_table_cell, 0);
        lv_style_set_pad_right(&style_table_cell, 8);
        lv_style_set_pad_top(&style_table_cell, 7);
        lv_style_set_pad_bottom(&style_table_cell, 7);
        lv_style_set_text_font(&style_table_cell, font_semibold_12);
        lv_style_set_text_color(&style_table_cell, CONST_TEXT_PRIMARY);
        lv_style_set_bg_opa(&style_chart, 0);
        lv_style_set_border_width(&style_chart, 0);
        lv_style_set_radius(&style_chart, 0);
        lv_style_set_pad_all(&style_chart, 0);
        lv_style_set_shadow_width(&style_chart, 0);
        lv_style_set_shadow_opa(&style_chart, 0);
        lv_style_set_line_color(&style_chart, CONST_DIVIDER);
        lv_style_set_line_width(&style_chart, 1);
        lv_style_set_line_dash_width(&style_chart, 3);
        lv_style_set_line_dash_gap(&style_chart, 4);
        lv_style_set_width(&style_chart_items, 0);
        lv_style_set_height(&style_chart_items, 0);
        lv_style_set_line_width(&style_chart_items, 2);
        lv_style_set_bg_opa(&style_checkbox, 0);
        lv_style_set_border_width(&style_checkbox, 0);
        lv_style_set_pad_all(&style_checkbox, 0);
        lv_style_set_pad_column(&style_checkbox, 12);
        lv_style_set_text_font(&style_checkbox, font_semibold_15);
        lv_style_set_text_color(&style_checkbox, CONST_TEXT_PRIMARY);
        lv_style_set_width(&style_checkbox_ind, 24);
        lv_style_set_height(&style_checkbox_ind, 24);
        lv_style_set_radius(&style_checkbox_ind, 6);
        lv_style_set_bg_opa(&style_checkbox_ind, 0);
        lv_style_set_border_color(&style_checkbox_ind, CONST_BORDER_STRONG);
        lv_style_set_border_width(&style_checkbox_ind, 2);
        lv_style_set_bg_color(&style_checkbox_ind_checked, CONST_ACCENT);
        lv_style_set_bg_opa(&style_checkbox_ind_checked, (255 * 100 / 100));
        lv_style_set_border_color(&style_checkbox_ind_checked, CONST_ACCENT);
        lv_style_set_border_width(&style_checkbox_ind_checked, 2);
        lv_style_set_bg_image_src(&style_checkbox_ind_checked, icon_check);
        lv_style_set_bg_image_recolor(&style_checkbox_ind_checked, CONST_ON_ACCENT);
        lv_style_set_bg_image_recolor_opa(&style_checkbox_ind_checked, 255);
        lv_style_set_bg_color(&style_seg, CONST_SURFACE_RAISED);
        lv_style_set_bg_opa(&style_seg, (255 * 100 / 100));
        lv_style_set_border_color(&style_seg, CONST_BORDER_STRONG);
        lv_style_set_border_width(&style_seg, 1);
        lv_style_set_radius(&style_seg, CONST_RADIUS_MD);
        lv_style_set_bg_color(&style_seg_selected, CONST_TINT_ACCENT);
        lv_style_set_bg_opa(&style_seg_selected, (255 * 100 / 100));
        lv_style_set_border_color(&style_seg_selected, CONST_ACCENT);
        lv_style_set_border_width(&style_seg_selected, 2);
        lv_style_set_radius(&style_seg_selected, CONST_RADIUS_MD);
        lv_style_set_radius(&style_bar_accent, CONST_RADIUS_PILL);
        lv_style_set_bg_color(&style_bar_accent, CONST_ACCENT);
        lv_style_set_bg_opa(&style_bar_accent, (255 * 100 / 100));
        lv_style_set_radius(&style_bar_warn, CONST_RADIUS_PILL);
        lv_style_set_bg_color(&style_bar_warn, CONST_WARN);
        lv_style_set_bg_opa(&style_bar_warn, (255 * 100 / 100));

        style_inited = true;
    }

    /*----------------
     * Subjects
     *----------------*/
    lv_subject_init_int(&subject_state, 0);
    lv_subject_set_min_value_int(&subject_state, 0);
    lv_subject_set_max_value_int(&subject_state, 2);
    lv_subject_init_int(&subject_rpm_ref, 1200);
    lv_subject_set_min_value_int(&subject_rpm_ref, 0);
    lv_subject_set_max_value_int(&subject_rpm_ref, 3600);
    lv_subject_init_int(&subject_rpm_act, 0);
    lv_subject_set_min_value_int(&subject_rpm_act, 0);
    lv_subject_set_max_value_int(&subject_rpm_act, 3600);
    static char subject_current_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_current_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_current_str,
                           subject_current_str_buf,
                           subject_current_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "0.0"
                          );
    lv_subject_init_int(&subject_temp, 31);
    lv_subject_init_int(&subject_load, 0);
    lv_subject_set_min_value_int(&subject_load, 0);
    lv_subject_set_max_value_int(&subject_load, 100);
    static char subject_bus_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_bus_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_bus_str,
                           subject_bus_str_buf,
                           subject_bus_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "24.0"
                          );
    lv_subject_init_int(&subject_peak_rpm, 0);
    lv_subject_init_int(&subject_avg_load, 0);
    static char subject_runtime_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_runtime_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_runtime_str,
                           subject_runtime_str_buf,
                           subject_runtime_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "00:00"
                          );
    lv_subject_init_int(&subject_fault_count, 0);
    lv_subject_set_min_value_int(&subject_fault_count, 0);
    lv_subject_set_max_value_int(&subject_fault_count, 99);
    lv_subject_init_int(&subject_fault_active, 0);
    lv_subject_set_min_value_int(&subject_fault_active, 0);
    lv_subject_set_max_value_int(&subject_fault_active, 1);
    lv_subject_init_int(&subject_dir, 0);
    lv_subject_set_min_value_int(&subject_dir, 0);
    lv_subject_set_max_value_int(&subject_dir, 1);
    lv_subject_init_int(&subject_ramp, 5);
    lv_subject_set_min_value_int(&subject_ramp, 1);
    lv_subject_set_max_value_int(&subject_ramp, 20);
    lv_subject_init_int(&subject_cfg_softstart, 1);
    lv_subject_init_int(&subject_cfg_reverse, 0);
    lv_subject_init_int(&subject_cfg_derate, 1);
    lv_subject_init_int(&subject_alert_never, 0);
    lv_subject_set_min_value_int(&subject_alert_never, 0);
    lv_subject_set_max_value_int(&subject_alert_never, 0);
    lv_subject_init_int(&subject_tab, 0);
    lv_subject_set_min_value_int(&subject_tab, 0);
    lv_subject_set_max_value_int(&subject_tab, 3);

    /*----------------
     * Translations
     *----------------*/

#if defined(LV_USE_XML) && LV_USE_XML
    /* Register widgets */

    /* Check all fonts / default if needed. This prevents fonts that are used in one target but
       defined in another from causing assertion failures during rendering of the Preview. */
    check_font(&font_hero_64, "font_hero_64");
    check_font(&font_semibold_28, "font_semibold_28");
    check_font(&font_semibold_20, "font_semibold_20");
    check_font(&font_semibold_15, "font_semibold_15");
    check_font(&font_semibold_12, "font_semibold_12");

    /* Register fonts */
    lv_xml_register_font(NULL, "font_hero_64", font_hero_64);
    lv_xml_register_font(NULL, "font_semibold_28", font_semibold_28);
    lv_xml_register_font(NULL, "font_semibold_20", font_semibold_20);
    lv_xml_register_font(NULL, "font_semibold_15", font_semibold_15);
    lv_xml_register_font(NULL, "font_semibold_12", font_semibold_12);

    /* Register subjects */
    lv_xml_register_subject(NULL, "subject_state", &subject_state);
    lv_xml_register_subject(NULL, "subject_rpm_ref", &subject_rpm_ref);
    lv_xml_register_subject(NULL, "subject_rpm_act", &subject_rpm_act);
    lv_xml_register_subject(NULL, "subject_current_str", &subject_current_str);
    lv_xml_register_subject(NULL, "subject_temp", &subject_temp);
    lv_xml_register_subject(NULL, "subject_load", &subject_load);
    lv_xml_register_subject(NULL, "subject_bus_str", &subject_bus_str);
    lv_xml_register_subject(NULL, "subject_peak_rpm", &subject_peak_rpm);
    lv_xml_register_subject(NULL, "subject_avg_load", &subject_avg_load);
    lv_xml_register_subject(NULL, "subject_runtime_str", &subject_runtime_str);
    lv_xml_register_subject(NULL, "subject_fault_count", &subject_fault_count);
    lv_xml_register_subject(NULL, "subject_fault_active", &subject_fault_active);
    lv_xml_register_subject(NULL, "subject_dir", &subject_dir);
    lv_xml_register_subject(NULL, "subject_ramp", &subject_ramp);
    lv_xml_register_subject(NULL, "subject_cfg_softstart", &subject_cfg_softstart);
    lv_xml_register_subject(NULL, "subject_cfg_reverse", &subject_cfg_reverse);
    lv_xml_register_subject(NULL, "subject_cfg_derate", &subject_cfg_derate);
    lv_xml_register_subject(NULL, "subject_alert_never", &subject_alert_never);
    lv_xml_register_subject(NULL, "subject_tab", &subject_tab);

    /* Register callbacks */
#endif

    /* Register all the global assets so that they won't be created again when globals.xml is parsed.
     * While running in the editor skip this step to update the preview when the XML changes */
#if defined(LV_USE_XML) && LV_USE_XML && !defined(LV_EDITOR_PREVIEW)
    /* Register images */
    lv_xml_register_image(NULL, "image_logostrip", image_logostrip);
    lv_xml_register_image(NULL, "icon_tab_run", icon_tab_run);
    lv_xml_register_image(NULL, "icon_tab_trend", icon_tab_trend);
    lv_xml_register_image(NULL, "icon_tab_faults", icon_tab_faults);
    lv_xml_register_image(NULL, "icon_tab_config", icon_tab_config);
    lv_xml_register_image(NULL, "icon_minus", icon_minus);
    lv_xml_register_image(NULL, "icon_plus", icon_plus);
    lv_xml_register_image(NULL, "icon_estop", icon_estop);
    lv_xml_register_image(NULL, "icon_dir_cw", icon_dir_cw);
    lv_xml_register_image(NULL, "icon_dir_ccw", icon_dir_ccw);
    lv_xml_register_image(NULL, "icon_clear", icon_clear);
    lv_xml_register_image(NULL, "icon_check", icon_check);
#endif

#if !defined(LV_USE_XML) || LV_USE_XML == 0
    /*--------------------
     *  Permanent screens
     *-------------------*/
    /* If XML is enabled it's assumed that the permanent screens are created
     * manually from XML using lv_xml_create() */
#endif
}

void motor_control_set_target(uint32_t target)
{
    motor_control_target = target;
}

uint32_t motor_control_get_target(void)
{
    return motor_control_target;
}

bool motor_control_check_target(uint32_t target)
{
    return (motor_control_target & target) ? true : false;
}

/* Callbacks */

/**********************
 *   STATIC FUNCTIONS
 **********************/

#if defined(LV_USE_XML) && LV_USE_XML
static void check_font(lv_font_t ** font, const char * name)
{
    if (!(*font)) {
        *font = (lv_font_t *)LV_FONT_DEFAULT;
        LV_LOG_WARN("font `%s` was not set. Using `LV_FONT_DEFAULT` instead", name);
    }
}
#endif /* LV_USE_XML */