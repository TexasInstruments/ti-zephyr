/**
 * @file ti_smart_home.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ti_smart_home.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    const void * album_cover;
    const char * album_name;
    const char * track_name;
} track_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
#if !defined(LV_EDITOR_PREVIEW)
static void delete_on_unload_cb(lv_event_t * e);
#endif
static void music_update_ui(void);

static const void * get_avatar(const char * email);

/**********************
 *  STATIC VARIABLES
 **********************/
LV_IMAGE_DECLARE(image_album_cover_data);
LV_IMAGE_DECLARE(image_album_ghosts_data);
LV_IMAGE_DECLARE(image_album_esja_data);
LV_IMAGE_DECLARE(image_album_leaving_data);
LV_IMAGE_DECLARE(image_album_home_data);

static track_data_t music_list[] = {
    {&image_album_cover_data, "Sentimental Value", "Hana Rani  —  Lighter and Lighter"},
    {&image_album_ghosts_data, "Ghosts", "Hana Rani  —  Whispering House"},
    {&image_album_esja_data, "Esja", "Hana Rani  —  Now, Run"},
    {&image_album_leaving_data, "Home", "Hana Rani  —  Leaving"},
    {&image_album_home_data, "Home", "Hana Rani  —  Letter to Glass"}
};

static uint32_t music_index = 0;
static const uint32_t music_count = sizeof(music_list) / sizeof(music_list[0]);

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ti_smart_home_init(const char * asset_path)
{
    LV_LOG("Initializing custom C code using LVGL v%d.%d.%d", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);

    ti_smart_home_init_gen(asset_path);

    /* Load the home screen so the application only has to call
     * ti_smart_home_init(). Screens are created on demand (<screen_create_event>
     * builds the target, and every screen deletes itself once it is unloaded),
     * so only one screen tree is in RAM at a time (two during the fade). The
     * editor preview manages screens itself, hence the guard. */
#if !defined(LV_EDITOR_PREVIEW)
    /* <screen_create_event> deletes the screens it creates once they are unloaded,
     * but not this first one, so give it the same behaviour or it stays in RAM for
     * good. auto_del frees the display's empty default screen. */
    lv_obj_t * first = screen_home_create();
    lv_obj_add_event_cb(first, delete_on_unload_cb, LV_EVENT_SCREEN_UNLOADED, NULL);
    lv_screen_load_anim(first, LV_SCREEN_LOAD_ANIM_NONE, 0, 0, true);
#endif
}

void on_music_previous_cb(lv_event_t * e)
{
    LV_UNUSED(e);

    if (music_index == 0)
        music_index = music_count - 1;
    else
        music_index--;

    music_update_ui();
}

void on_music_next_cb(lv_event_t * e)
{
    LV_UNUSED(e);

    music_index++;

    if (music_index >= music_count)
        music_index = 0;

    music_update_ui();
}

void on_contact_clicked_cb(lv_event_t * e)
{
    const char * email = lv_event_get_user_data(e);

    const void * icon = get_avatar(email);

    /* contact_icon binds its src to this subject, so a rebuilt screen_messages
     * shows the selected contact too */
    if (icon) lv_subject_set_pointer(&subject_contact_img, (void *)icon);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#if !defined(LV_EDITOR_PREVIEW)
static void delete_on_unload_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_target_obj(e));
}
#endif

static void music_update_ui(void)
{
    /* album_cover binds its src to this subject, so a rebuilt screen_smart_home
     * shows the current track's cover too */
    lv_subject_set_pointer(&subject_album_cover, (void *)music_list[music_index].album_cover);
    lv_subject_copy_string(&subject_album_name, music_list[music_index].album_name);
    lv_subject_copy_string(&subject_track_name, music_list[music_index].track_name);
}

static const void * get_avatar(const char * email)
{
    if(lv_streq("noah.davis@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_large_data);
        return &image_contact_large_data;
    } else if(lv_streq("alexdoe@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_alex_data);
        return &image_contact_alex_data;
    } else if(lv_streq("lukas.schneider@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_lukas_data);
        return &image_contact_lukas_data;
    } else if(lv_streq("lisa.swift@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_lisa_data);
        return &image_contact_lisa_data;
    } else if(lv_streq("emma.dubois@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_emma_data);
        return &image_contact_emma_data;
    } else if(lv_streq("marco.rossi@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_marco_data);
        return &image_contact_marco_data;
    } else if(lv_streq("sofia.andersson@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_sofia_data);
        return &image_contact_sofia_data;
    } else if(lv_streq("oliver.novak@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_oliver_data);
        return &image_contact_oliver_data;
    } else if(lv_streq("thomas.mueller@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_thomas_data);
        return &image_contact_thomas_data;
    } else if(lv_streq("isabella.garcia@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_isabella_data);
        return &image_contact_isabella_data;
    } else if(lv_streq("noah.jensen@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_noah_data);
        return &image_contact_noah_data;
    } else if(lv_streq("elena.petrova@gmail.com", email)) {
        LV_IMAGE_DECLARE(image_contact_elena_data);
        return &image_contact_elena_data;
    } else {
        return NULL;
    }
}
