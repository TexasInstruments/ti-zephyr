/**
 * @file ti_smart_home_gen.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ti_smart_home_gen.h"

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

/**********************
 *  STATIC VARIABLES
 **********************/

static uint32_t ti_smart_home_target = TI_SMART_HOME_TARGET_ALL;

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

lv_font_t * font_xs;
extern lv_font_t font_xs_data;
lv_font_t * font_sm;
extern lv_font_t font_sm_data;
lv_font_t * font_md;
extern lv_font_t font_md_data;
lv_font_t * font_lg;
extern lv_font_t font_lg_data;
lv_font_t * font_xl;
extern lv_font_t font_xl_data;
lv_font_t * font_xxl;
extern lv_font_t font_xxl_data;
lv_font_t * font_xxxl;
extern lv_font_t font_xxxl_data;

/*----------------
 * Images
 *----------------*/

/* Targets: any */
const void * icon_check = NULL;
extern const void * icon_check_data;
const void * icon_minus = NULL;
extern const void * icon_minus_data;
const void * icon_plus = NULL;
extern const void * icon_plus_data;
const void * icon_wifi = NULL;
extern const void * icon_wifi_data;
const void * icon_charge = NULL;
extern const void * icon_charge_data;
const void * icon_messages = NULL;
extern const void * icon_messages_data;
const void * icon_smart_home = NULL;
extern const void * icon_smart_home_data;
const void * icon_back = NULL;
extern const void * icon_back_data;
const void * icon_chart = NULL;
extern const void * icon_chart_data;
const void * icon_range = NULL;
extern const void * icon_range_data;
const void * icon_temp = NULL;
extern const void * icon_temp_data;
const void * icon_weather = NULL;
extern const void * icon_weather_data;
const void * icon_shuffle = NULL;
extern const void * icon_shuffle_data;
const void * icon_pause = NULL;
extern const void * icon_pause_data;
const void * icon_play = NULL;
extern const void * icon_play_data;
const void * icon_prev = NULL;
extern const void * icon_prev_data;
const void * icon_next = NULL;
extern const void * icon_next_data;
const void * icon_repeat = NULL;
extern const void * icon_repeat_data;
const void * icon_spotify = NULL;
extern const void * icon_spotify_data;
const void * icon_contacts = NULL;
extern const void * icon_contacts_data;
const void * icon_contact = NULL;
extern const void * icon_contact_data;
const void * icon_email = NULL;
extern const void * icon_email_data;
const void * icon_message = NULL;
extern const void * icon_message_data;
const void * icon_call = NULL;
extern const void * icon_call_data;
const void * image_weather_cloudy = NULL;
extern const void * image_weather_cloudy_data;
const void * image_home_hero = NULL;
extern const void * image_home_hero_data;
const void * image_car_small = NULL;
extern const void * image_car_small_data;
const void * image_car_large = NULL;
extern const void * image_car_large_data;
const void * image_avatar_lisa = NULL;
extern const void * image_avatar_lisa_data;
const void * image_livecam = NULL;
extern const void * image_livecam_data;
const void * image_vacuum = NULL;
extern const void * image_vacuum_data;
const void * image_kettle = NULL;
extern const void * image_kettle_data;
const void * image_wifi = NULL;
extern const void * image_wifi_data;
const void * image_album_cover = NULL;
extern const void * image_album_cover_data;
const void * image_album_leaving = NULL;
extern const void * image_album_leaving_data;
const void * image_album_home = NULL;
extern const void * image_album_home_data;
const void * image_album_esja = NULL;
extern const void * image_album_esja_data;
const void * image_album_ghosts = NULL;
extern const void * image_album_ghosts_data;
const void * image_contact_large = NULL;
extern const void * image_contact_large_data;
const void * image_contact_small = NULL;
extern const void * image_contact_small_data;
const void * image_contact_alex = NULL;
extern const void * image_contact_alex_data;
const void * image_contact_lukas = NULL;
extern const void * image_contact_lukas_data;
const void * image_contact_marco = NULL;
extern const void * image_contact_marco_data;
const void * image_contact_oliver = NULL;
extern const void * image_contact_oliver_data;
const void * image_contact_emma = NULL;
extern const void * image_contact_emma_data;
const void * image_contact_thomas = NULL;
extern const void * image_contact_thomas_data;
const void * image_contact_sofia = NULL;
extern const void * image_contact_sofia_data;
const void * image_contact_isabella = NULL;
extern const void * image_contact_isabella_data;
const void * image_contact_elena = NULL;
extern const void * image_contact_elena_data;
const void * image_contact_noah = NULL;
extern const void * image_contact_noah_data;
const void * image_contact_lisa = NULL;
extern const void * image_contact_lisa_data;
const void * image_avatar_lisa_small = NULL;
extern const void * image_avatar_lisa_small_data;
const void * image_contact_alex_small = NULL;
extern const void * image_contact_alex_small_data;
const void * image_contact_lukas_small = NULL;
extern const void * image_contact_lukas_small_data;
const void * image_contact_marco_small = NULL;
extern const void * image_contact_marco_small_data;
const void * image_contact_oliver_small = NULL;
extern const void * image_contact_oliver_small_data;
const void * image_contact_emma_small = NULL;
extern const void * image_contact_emma_small_data;
const void * image_contact_thomas_small = NULL;
extern const void * image_contact_thomas_small_data;
const void * image_contact_sofia_small = NULL;
extern const void * image_contact_sofia_small_data;
const void * image_contact_isabella_small = NULL;
extern const void * image_contact_isabella_small_data;
const void * image_contact_elena_small = NULL;
extern const void * image_contact_elena_small_data;
const void * image_contact_noah_small = NULL;
extern const void * image_contact_noah_small_data;
const void * frame = NULL;
extern const void * frame_data;
const void * icon_inbox = NULL;
extern const void * icon_inbox_data;
const void * image_alarm = NULL;
extern const void * image_alarm_data;
const void * image_contact_marta = NULL;
extern const void * image_contact_marta_data;
const void * image_contact_marta_small = NULL;
extern const void * image_contact_marta_small_data;
const void * logostrip_large = NULL;
extern const void * logostrip_large_data;
const void * logostrip_small = NULL;
extern const void * logostrip_small_data;

/*----------------
 * Global styles
 *----------------*/

lv_style_t style_bg;
lv_style_t style_panel;
lv_style_t style_button;
lv_style_t style_text;
lv_style_t style_text_accent;
lv_style_t style_button_pressed;
lv_style_t style_scrollbar;

/*----------------
 * Subjects
 *----------------*/

lv_subject_t subject_dark_mode;
lv_subject_t subject_nav_index;
lv_subject_t subject_hour;
lv_subject_t subject_minute;
lv_subject_t subject_day_index;
lv_subject_t subject_day_name;
lv_subject_t subject_month_name;
lv_subject_t subject_year;
lv_subject_t subject_temp;
lv_subject_t subject_ev_soc;
lv_subject_t subject_ev_range_km;
lv_subject_t subject_ev_power_kw;
lv_subject_t subject_outside_temp_current;
lv_subject_t subject_outside_temp_highest;
lv_subject_t subject_weather_desc;
lv_subject_t subject_album_name;
lv_subject_t subject_track_name;
lv_subject_t subject_play;
lv_subject_t subject_shuffle;
lv_subject_t subject_repeat;
lv_subject_t subject_music_progress;
lv_subject_t subject_vacuum_battery;
lv_subject_t subject_kettle_temp;
lv_subject_t subject_kettle_on;
lv_subject_t subject_vacuum_on;
lv_subject_t subject_wifi_on;
lv_subject_t subject_album_cover;
lv_subject_t subject_contact_img;
lv_subject_t subject_contact_name;
lv_subject_t subject_contact_email;
lv_subject_t subject_contact_phone;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ti_smart_home_init_gen(const char * asset_path)
{

    /*----------------
     * Fonts
     *----------------*/

    /* Targets: any */

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        if (!font_xs) {
            /* font_xs */
            /* get font 'font_xs' from a C array */
            font_xs = &font_xs_data;

        }
        if (!font_sm) {
            /* font_sm */
            /* get font 'font_sm' from a C array */
            font_sm = &font_sm_data;

        }
        if (!font_md) {
            /* font_md */
            /* get font 'font_md' from a C array */
            font_md = &font_md_data;

        }
        if (!font_lg) {
            /* font_lg */
            /* get font 'font_lg' from a C array */
            font_lg = &font_lg_data;

        }
        if (!font_xl) {
            /* font_xl */
            /* get font 'font_xl' from a C array */
            font_xl = &font_xl_data;

        }
        if (!font_xxl) {
            /* font_xxl */
            /* get font 'font_xxl' from a C array */
            font_xxl = &font_xxl_data;

        }
        if (!font_xxxl) {
            /* font_xxxl */
            /* get font 'font_xxxl' from a C array */
            font_xxxl = &font_xxxl_data;

        }
    }
    #endif

    /*----------------
     * Images
     *----------------*/

    /* Targets: any */
    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        /* icon_check */
        if (!icon_check) {
            icon_check = &icon_check_data;
        }
        /* icon_minus */
        if (!icon_minus) {
            icon_minus = &icon_minus_data;
        }
        /* icon_plus */
        if (!icon_plus) {
            icon_plus = &icon_plus_data;
        }
        /* icon_wifi */
        if (!icon_wifi) {
            icon_wifi = &icon_wifi_data;
        }
        /* icon_charge */
        if (!icon_charge) {
            icon_charge = &icon_charge_data;
        }
        /* icon_messages */
        if (!icon_messages) {
            icon_messages = &icon_messages_data;
        }
        /* icon_smart_home */
        if (!icon_smart_home) {
            icon_smart_home = &icon_smart_home_data;
        }
        /* icon_back */
        if (!icon_back) {
            icon_back = &icon_back_data;
        }
        /* icon_chart */
        if (!icon_chart) {
            icon_chart = &icon_chart_data;
        }
        /* icon_range */
        if (!icon_range) {
            icon_range = &icon_range_data;
        }
        /* icon_temp */
        if (!icon_temp) {
            icon_temp = &icon_temp_data;
        }
        /* icon_weather */
        if (!icon_weather) {
            icon_weather = &icon_weather_data;
        }
        /* icon_shuffle */
        if (!icon_shuffle) {
            icon_shuffle = &icon_shuffle_data;
        }
        /* icon_pause */
        if (!icon_pause) {
            icon_pause = &icon_pause_data;
        }
        /* icon_play */
        if (!icon_play) {
            icon_play = &icon_play_data;
        }
        /* icon_prev */
        if (!icon_prev) {
            icon_prev = &icon_prev_data;
        }
        /* icon_next */
        if (!icon_next) {
            icon_next = &icon_next_data;
        }
        /* icon_repeat */
        if (!icon_repeat) {
            icon_repeat = &icon_repeat_data;
        }
        /* icon_spotify */
        if (!icon_spotify) {
            icon_spotify = &icon_spotify_data;
        }
        /* icon_contacts */
        if (!icon_contacts) {
            icon_contacts = &icon_contacts_data;
        }
        /* icon_contact */
        if (!icon_contact) {
            icon_contact = &icon_contact_data;
        }
        /* icon_email */
        if (!icon_email) {
            icon_email = &icon_email_data;
        }
        /* icon_message */
        if (!icon_message) {
            icon_message = &icon_message_data;
        }
        /* icon_call */
        if (!icon_call) {
            icon_call = &icon_call_data;
        }
        /* image_weather_cloudy */
        if (!image_weather_cloudy) {
            image_weather_cloudy = &image_weather_cloudy_data;
        }
        /* image_home_hero */
        if (!image_home_hero) {
            image_home_hero = &image_home_hero_data;
        }
        /* image_car_small */
        if (!image_car_small) {
            image_car_small = &image_car_small_data;
        }
        /* image_car_large */
        if (!image_car_large) {
            image_car_large = &image_car_large_data;
        }
        /* image_avatar_lisa */
        if (!image_avatar_lisa) {
            image_avatar_lisa = &image_avatar_lisa_data;
        }
        /* image_livecam */
        if (!image_livecam) {
            image_livecam = &image_livecam_data;
        }
        /* image_vacuum */
        if (!image_vacuum) {
            image_vacuum = &image_vacuum_data;
        }
        /* image_kettle */
        if (!image_kettle) {
            image_kettle = &image_kettle_data;
        }
        /* image_wifi */
        if (!image_wifi) {
            image_wifi = &image_wifi_data;
        }
        /* image_album_cover */
        if (!image_album_cover) {
            image_album_cover = &image_album_cover_data;
        }
        /* image_album_leaving */
        if (!image_album_leaving) {
            image_album_leaving = &image_album_leaving_data;
        }
        /* image_album_home */
        if (!image_album_home) {
            image_album_home = &image_album_home_data;
        }
        /* image_album_esja */
        if (!image_album_esja) {
            image_album_esja = &image_album_esja_data;
        }
        /* image_album_ghosts */
        if (!image_album_ghosts) {
            image_album_ghosts = &image_album_ghosts_data;
        }
        /* image_contact_large */
        if (!image_contact_large) {
            image_contact_large = &image_contact_large_data;
        }
        /* image_contact_small */
        if (!image_contact_small) {
            image_contact_small = &image_contact_small_data;
        }
        /* image_contact_alex */
        if (!image_contact_alex) {
            image_contact_alex = &image_contact_alex_data;
        }
        /* image_contact_lukas */
        if (!image_contact_lukas) {
            image_contact_lukas = &image_contact_lukas_data;
        }
        /* image_contact_marco */
        if (!image_contact_marco) {
            image_contact_marco = &image_contact_marco_data;
        }
        /* image_contact_oliver */
        if (!image_contact_oliver) {
            image_contact_oliver = &image_contact_oliver_data;
        }
        /* image_contact_emma */
        if (!image_contact_emma) {
            image_contact_emma = &image_contact_emma_data;
        }
        /* image_contact_thomas */
        if (!image_contact_thomas) {
            image_contact_thomas = &image_contact_thomas_data;
        }
        /* image_contact_sofia */
        if (!image_contact_sofia) {
            image_contact_sofia = &image_contact_sofia_data;
        }
        /* image_contact_isabella */
        if (!image_contact_isabella) {
            image_contact_isabella = &image_contact_isabella_data;
        }
        /* image_contact_elena */
        if (!image_contact_elena) {
            image_contact_elena = &image_contact_elena_data;
        }
        /* image_contact_noah */
        if (!image_contact_noah) {
            image_contact_noah = &image_contact_noah_data;
        }
        /* image_contact_lisa */
        if (!image_contact_lisa) {
            image_contact_lisa = &image_contact_lisa_data;
        }
        /* image_avatar_lisa_small */
        if (!image_avatar_lisa_small) {
            image_avatar_lisa_small = &image_avatar_lisa_small_data;
        }
        /* image_contact_alex_small */
        if (!image_contact_alex_small) {
            image_contact_alex_small = &image_contact_alex_small_data;
        }
        /* image_contact_lukas_small */
        if (!image_contact_lukas_small) {
            image_contact_lukas_small = &image_contact_lukas_small_data;
        }
        /* image_contact_marco_small */
        if (!image_contact_marco_small) {
            image_contact_marco_small = &image_contact_marco_small_data;
        }
        /* image_contact_oliver_small */
        if (!image_contact_oliver_small) {
            image_contact_oliver_small = &image_contact_oliver_small_data;
        }
        /* image_contact_emma_small */
        if (!image_contact_emma_small) {
            image_contact_emma_small = &image_contact_emma_small_data;
        }
        /* image_contact_thomas_small */
        if (!image_contact_thomas_small) {
            image_contact_thomas_small = &image_contact_thomas_small_data;
        }
        /* image_contact_sofia_small */
        if (!image_contact_sofia_small) {
            image_contact_sofia_small = &image_contact_sofia_small_data;
        }
        /* image_contact_isabella_small */
        if (!image_contact_isabella_small) {
            image_contact_isabella_small = &image_contact_isabella_small_data;
        }
        /* image_contact_elena_small */
        if (!image_contact_elena_small) {
            image_contact_elena_small = &image_contact_elena_small_data;
        }
        /* image_contact_noah_small */
        if (!image_contact_noah_small) {
            image_contact_noah_small = &image_contact_noah_small_data;
        }
        /* frame */
        if (!frame) {
            frame = &frame_data;
        }
        /* icon_inbox */
        if (!icon_inbox) {
            icon_inbox = &icon_inbox_data;
        }
        /* image_alarm */
        if (!image_alarm) {
            image_alarm = &image_alarm_data;
        }
        /* image_contact_marta */
        if (!image_contact_marta) {
            image_contact_marta = &image_contact_marta_data;
        }
        /* image_contact_marta_small */
        if (!image_contact_marta_small) {
            image_contact_marta_small = &image_contact_marta_small_data;
        }
        /* logostrip_large */
        if (!logostrip_large) {
            logostrip_large = &logostrip_large_data;
        }
        /* logostrip_small */
        if (!logostrip_small) {
            logostrip_small = &logostrip_small_data;
        }
    }
    #endif

    /*----------------
     * Global styles
     *----------------*/

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_bg);
        lv_style_init(&style_panel);
        lv_style_init(&style_button);
        lv_style_init(&style_text);
        lv_style_init(&style_text_accent);
        lv_style_init(&style_button_pressed);
        lv_style_init(&style_scrollbar);

        lv_style_set_bg_color(&style_bg, COLOR_BG);
        lv_style_set_text_color(&style_bg, COLOR_TEXT);
        lv_style_set_image_recolor(&style_bg, COLOR_TEXT);
        lv_style_set_bg_opa(&style_bg, (255 * 100 / 100));
        lv_style_set_bg_color(&style_panel, COLOR_PANEL);
        lv_style_set_border_color(&style_panel, COLOR_TEXT);
        lv_style_set_text_color(&style_panel, COLOR_TEXT);
        lv_style_set_line_color(&style_panel, COLOR_TEXT);
        lv_style_set_image_recolor(&style_panel, COLOR_TEXT);
        lv_style_set_bg_opa(&style_panel, (255 * 100 / 100));
        lv_style_set_border_opa(&style_panel, (255 * 20 / 100));
        lv_style_set_line_opa(&style_panel, (255 * 20 / 100));
        lv_style_set_bg_color(&style_button, COLOR_BUTTON);
        lv_style_set_text_color(&style_button, COLOR_BUTTON_TEXT);
        lv_style_set_image_recolor(&style_button, COLOR_BUTTON_TEXT);
        lv_style_set_bg_opa(&style_button, (255 * 100 / 100));
        lv_style_set_text_color(&style_text, COLOR_TEXT);
        lv_style_set_text_color(&style_text_accent, COLOR_ACCENT);
        lv_style_set_recolor(&style_button_pressed, COLOR_TEXT);
        lv_style_set_transform_width(&style_button_pressed, 0);
        lv_style_set_transform_height(&style_button_pressed, 0);
        lv_style_set_recolor_opa(&style_button_pressed, (255 * 40 / 100));
        lv_style_set_width(&style_scrollbar, BASE_LIB_UNIT_SM);
        lv_style_set_radius(&style_scrollbar, BASE_LIB_UNIT_SM);
        lv_style_set_bg_color(&style_scrollbar, lv_color_hex3(0x888));
        lv_style_set_bg_opa(&style_scrollbar, (255 * 50 / 100));

        style_inited = true;
    }

    /*----------------
     * Subjects
     *----------------*/
    lv_subject_init_int(&subject_dark_mode, 0);
    lv_subject_init_int(&subject_nav_index, 0);
    lv_subject_init_int(&subject_hour, 9);
    lv_subject_init_int(&subject_minute, 43);
    lv_subject_init_int(&subject_day_index, 12);
    static char subject_day_name_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_day_name_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_day_name,
                           subject_day_name_buf,
                           subject_day_name_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "Wed"
                          );
    static char subject_month_name_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_month_name_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_month_name,
                           subject_month_name_buf,
                           subject_month_name_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "Jan"
                          );
    lv_subject_init_int(&subject_year, 2026);
    lv_subject_init_int(&subject_temp, 23);
    lv_subject_init_int(&subject_ev_soc, 78);
    lv_subject_init_int(&subject_ev_range_km, 312);
    lv_subject_init_int(&subject_ev_power_kw, 11);
    lv_subject_init_int(&subject_outside_temp_current, 6);
    lv_subject_init_int(&subject_outside_temp_highest, 9);
    static char subject_weather_desc_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_weather_desc_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_weather_desc,
                           subject_weather_desc_buf,
                           subject_weather_desc_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "Cloudy"
                          );
    static char subject_album_name_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_album_name_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_album_name,
                           subject_album_name_buf,
                           subject_album_name_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "Sentimental Value"
                          );
    static char subject_track_name_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_track_name_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_track_name,
                           subject_track_name_buf,
                           subject_track_name_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "Hana Rani  —  Lighter and Lighter"
                          );
    lv_subject_init_int(&subject_play, 0);
    lv_subject_init_int(&subject_shuffle, 0);
    lv_subject_init_int(&subject_repeat, 0);
    lv_subject_init_int(&subject_music_progress, 45);
    lv_subject_init_int(&subject_vacuum_battery, 19);
    lv_subject_init_int(&subject_kettle_temp, 67);
    lv_subject_init_int(&subject_kettle_on, 0);
    lv_subject_init_int(&subject_vacuum_on, 0);
    lv_subject_init_int(&subject_wifi_on, 0);
    lv_subject_init_pointer(&subject_album_cover, (void *)image_album_cover);
    lv_subject_init_pointer(&subject_contact_img, (void *)image_contact_large);
    static char subject_contact_name_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_contact_name_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_contact_name,
                           subject_contact_name_buf,
                           subject_contact_name_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "Noah Davis"
                          );
    static char subject_contact_email_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_contact_email_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_contact_email,
                           subject_contact_email_buf,
                           subject_contact_email_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "noah.davis@gmail.com"
                          );
    static char subject_contact_phone_buf[UI_SUBJECT_STRING_LENGTH];
    static char subject_contact_phone_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&subject_contact_phone,
                           subject_contact_phone_buf,
                           subject_contact_phone_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "+1 45 678 45 56"
                          );

    /*----------------
     * Translations
     *----------------*/

#if defined(LV_USE_XML) && LV_USE_XML
    /* Register widgets */

    /* Check all fonts / default if needed. This prevents fonts that are used in one target but
       defined in another from causing assertion failures during rendering of the Preview. */
    check_font(&font_xs, "font_xs");
    check_font(&font_sm, "font_sm");
    check_font(&font_md, "font_md");
    check_font(&font_lg, "font_lg");
    check_font(&font_xl, "font_xl");
    check_font(&font_xxl, "font_xxl");
    check_font(&font_xxxl, "font_xxxl");

    /* Register fonts */
    lv_xml_register_font(NULL, "font_xs", font_xs);
    lv_xml_register_font(NULL, "font_sm", font_sm);
    lv_xml_register_font(NULL, "font_md", font_md);
    lv_xml_register_font(NULL, "font_lg", font_lg);
    lv_xml_register_font(NULL, "font_xl", font_xl);
    lv_xml_register_font(NULL, "font_xxl", font_xxl);
    lv_xml_register_font(NULL, "font_xxxl", font_xxxl);

    /* Register subjects */
    lv_xml_register_subject(NULL, "subject_dark_mode", &subject_dark_mode);
    lv_xml_register_subject(NULL, "subject_nav_index", &subject_nav_index);
    lv_xml_register_subject(NULL, "subject_hour", &subject_hour);
    lv_xml_register_subject(NULL, "subject_minute", &subject_minute);
    lv_xml_register_subject(NULL, "subject_day_index", &subject_day_index);
    lv_xml_register_subject(NULL, "subject_day_name", &subject_day_name);
    lv_xml_register_subject(NULL, "subject_month_name", &subject_month_name);
    lv_xml_register_subject(NULL, "subject_year", &subject_year);
    lv_xml_register_subject(NULL, "subject_temp", &subject_temp);
    lv_xml_register_subject(NULL, "subject_ev_soc", &subject_ev_soc);
    lv_xml_register_subject(NULL, "subject_ev_range_km", &subject_ev_range_km);
    lv_xml_register_subject(NULL, "subject_ev_power_kw", &subject_ev_power_kw);
    lv_xml_register_subject(NULL, "subject_outside_temp_current", &subject_outside_temp_current);
    lv_xml_register_subject(NULL, "subject_outside_temp_highest", &subject_outside_temp_highest);
    lv_xml_register_subject(NULL, "subject_weather_desc", &subject_weather_desc);
    lv_xml_register_subject(NULL, "subject_album_name", &subject_album_name);
    lv_xml_register_subject(NULL, "subject_track_name", &subject_track_name);
    lv_xml_register_subject(NULL, "subject_play", &subject_play);
    lv_xml_register_subject(NULL, "subject_shuffle", &subject_shuffle);
    lv_xml_register_subject(NULL, "subject_repeat", &subject_repeat);
    lv_xml_register_subject(NULL, "subject_music_progress", &subject_music_progress);
    lv_xml_register_subject(NULL, "subject_vacuum_battery", &subject_vacuum_battery);
    lv_xml_register_subject(NULL, "subject_kettle_temp", &subject_kettle_temp);
    lv_xml_register_subject(NULL, "subject_kettle_on", &subject_kettle_on);
    lv_xml_register_subject(NULL, "subject_vacuum_on", &subject_vacuum_on);
    lv_xml_register_subject(NULL, "subject_wifi_on", &subject_wifi_on);
    lv_xml_register_subject(NULL, "subject_album_cover", &subject_album_cover);
    lv_xml_register_subject(NULL, "subject_contact_img", &subject_contact_img);
    lv_xml_register_subject(NULL, "subject_contact_name", &subject_contact_name);
    lv_xml_register_subject(NULL, "subject_contact_email", &subject_contact_email);
    lv_xml_register_subject(NULL, "subject_contact_phone", &subject_contact_phone);

    /* Register callbacks */
    lv_xml_register_event_cb(NULL, "on_contact_clicked_cb", on_contact_clicked_cb);
    lv_xml_register_event_cb(NULL, "on_music_previous_cb", on_music_previous_cb);
    lv_xml_register_event_cb(NULL, "on_music_next_cb", on_music_next_cb);
#endif

    /* Register all the global assets so that they won't be created again when globals.xml is parsed.
     * While running in the editor skip this step to update the preview when the XML changes */
#if defined(LV_USE_XML) && LV_USE_XML && !defined(LV_EDITOR_PREVIEW)
    /* Register images */
    lv_xml_register_image(NULL, "icon_check", icon_check);
    lv_xml_register_image(NULL, "icon_minus", icon_minus);
    lv_xml_register_image(NULL, "icon_plus", icon_plus);
    lv_xml_register_image(NULL, "icon_wifi", icon_wifi);
    lv_xml_register_image(NULL, "icon_charge", icon_charge);
    lv_xml_register_image(NULL, "icon_messages", icon_messages);
    lv_xml_register_image(NULL, "icon_smart_home", icon_smart_home);
    lv_xml_register_image(NULL, "icon_back", icon_back);
    lv_xml_register_image(NULL, "icon_chart", icon_chart);
    lv_xml_register_image(NULL, "icon_range", icon_range);
    lv_xml_register_image(NULL, "icon_temp", icon_temp);
    lv_xml_register_image(NULL, "icon_weather", icon_weather);
    lv_xml_register_image(NULL, "icon_shuffle", icon_shuffle);
    lv_xml_register_image(NULL, "icon_pause", icon_pause);
    lv_xml_register_image(NULL, "icon_play", icon_play);
    lv_xml_register_image(NULL, "icon_prev", icon_prev);
    lv_xml_register_image(NULL, "icon_next", icon_next);
    lv_xml_register_image(NULL, "icon_repeat", icon_repeat);
    lv_xml_register_image(NULL, "icon_spotify", icon_spotify);
    lv_xml_register_image(NULL, "icon_contacts", icon_contacts);
    lv_xml_register_image(NULL, "icon_contact", icon_contact);
    lv_xml_register_image(NULL, "icon_email", icon_email);
    lv_xml_register_image(NULL, "icon_message", icon_message);
    lv_xml_register_image(NULL, "icon_call", icon_call);
    lv_xml_register_image(NULL, "image_weather_cloudy", image_weather_cloudy);
    lv_xml_register_image(NULL, "image_home_hero", image_home_hero);
    lv_xml_register_image(NULL, "image_car_small", image_car_small);
    lv_xml_register_image(NULL, "image_car_large", image_car_large);
    lv_xml_register_image(NULL, "image_avatar_lisa", image_avatar_lisa);
    lv_xml_register_image(NULL, "image_livecam", image_livecam);
    lv_xml_register_image(NULL, "image_vacuum", image_vacuum);
    lv_xml_register_image(NULL, "image_kettle", image_kettle);
    lv_xml_register_image(NULL, "image_wifi", image_wifi);
    lv_xml_register_image(NULL, "image_album_cover", image_album_cover);
    lv_xml_register_image(NULL, "image_album_leaving", image_album_leaving);
    lv_xml_register_image(NULL, "image_album_home", image_album_home);
    lv_xml_register_image(NULL, "image_album_esja", image_album_esja);
    lv_xml_register_image(NULL, "image_album_ghosts", image_album_ghosts);
    lv_xml_register_image(NULL, "image_contact_large", image_contact_large);
    lv_xml_register_image(NULL, "image_contact_small", image_contact_small);
    lv_xml_register_image(NULL, "image_contact_alex", image_contact_alex);
    lv_xml_register_image(NULL, "image_contact_lukas", image_contact_lukas);
    lv_xml_register_image(NULL, "image_contact_marco", image_contact_marco);
    lv_xml_register_image(NULL, "image_contact_oliver", image_contact_oliver);
    lv_xml_register_image(NULL, "image_contact_emma", image_contact_emma);
    lv_xml_register_image(NULL, "image_contact_thomas", image_contact_thomas);
    lv_xml_register_image(NULL, "image_contact_sofia", image_contact_sofia);
    lv_xml_register_image(NULL, "image_contact_isabella", image_contact_isabella);
    lv_xml_register_image(NULL, "image_contact_elena", image_contact_elena);
    lv_xml_register_image(NULL, "image_contact_noah", image_contact_noah);
    lv_xml_register_image(NULL, "image_contact_lisa", image_contact_lisa);
    lv_xml_register_image(NULL, "image_avatar_lisa_small", image_avatar_lisa_small);
    lv_xml_register_image(NULL, "image_contact_alex_small", image_contact_alex_small);
    lv_xml_register_image(NULL, "image_contact_lukas_small", image_contact_lukas_small);
    lv_xml_register_image(NULL, "image_contact_marco_small", image_contact_marco_small);
    lv_xml_register_image(NULL, "image_contact_oliver_small", image_contact_oliver_small);
    lv_xml_register_image(NULL, "image_contact_emma_small", image_contact_emma_small);
    lv_xml_register_image(NULL, "image_contact_thomas_small", image_contact_thomas_small);
    lv_xml_register_image(NULL, "image_contact_sofia_small", image_contact_sofia_small);
    lv_xml_register_image(NULL, "image_contact_isabella_small", image_contact_isabella_small);
    lv_xml_register_image(NULL, "image_contact_elena_small", image_contact_elena_small);
    lv_xml_register_image(NULL, "image_contact_noah_small", image_contact_noah_small);
    lv_xml_register_image(NULL, "frame", frame);
    lv_xml_register_image(NULL, "icon_inbox", icon_inbox);
    lv_xml_register_image(NULL, "image_alarm", image_alarm);
    lv_xml_register_image(NULL, "image_contact_marta", image_contact_marta);
    lv_xml_register_image(NULL, "image_contact_marta_small", image_contact_marta_small);
    lv_xml_register_image(NULL, "logostrip_large", logostrip_large);
    lv_xml_register_image(NULL, "logostrip_small", logostrip_small);
#endif

#if !defined(LV_USE_XML) || LV_USE_XML == 0
    /*--------------------
     *  Permanent screens
     *-------------------*/
    /* If XML is enabled it's assumed that the permanent screens are created
     * manually from XML using lv_xml_create() */
#endif
}

void ti_smart_home_set_target(uint32_t target)
{
    ti_smart_home_target = target;
}

uint32_t ti_smart_home_get_target(void)
{
    return ti_smart_home_target;
}

bool ti_smart_home_check_target(uint32_t target)
{
    return (ti_smart_home_target & target) ? true : false;
}

/* Callbacks */
#if defined(LV_EDITOR_PREVIEW)
void __attribute__((weak)) on_contact_clicked_cb(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("on_contact_clicked_cb was called\n");
}
void __attribute__((weak)) on_music_previous_cb(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("on_music_previous_cb was called\n");
}
void __attribute__((weak)) on_music_next_cb(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("on_music_next_cb was called\n");
}
#endif