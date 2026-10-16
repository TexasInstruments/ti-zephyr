/**
 * @file ti_thermostat.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ti_thermostat.h"
#include "custom/thermostat_logic.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
#if !defined(LV_EDITOR_PREVIEW)
static void delete_on_unload_cb(lv_event_t * e);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ti_thermostat_init(const char * asset_path)
{
    LV_LOG("Initializing custom C code using LVGL v%d.%d.%d", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);

    ti_thermostat_init_gen(asset_path);

    /* Must run after the generated init — that is what creates and registers
     * the subjects this observes. */
    thermostat_logic_init();

    /* Screens are created on demand (<screen_create_event> in tab_bar builds the
     * target, and every screen deletes itself once it is unloaded), so only one
     * screen tree is in RAM at a time. The editor preview manages screens
     * itself, hence the guard. */
#if !defined(LV_EDITOR_PREVIEW)
    /* <screen_create_event> deletes the screens it creates once they are unloaded,
     * but not this first one, so give it the same behaviour or it stays in RAM for
     * good. auto_del frees the display's empty default screen. */
    lv_obj_t * first = home_create();
    lv_obj_add_event_cb(first, delete_on_unload_cb, LV_EVENT_SCREEN_UNLOADED, NULL);
    lv_screen_load_anim(first, LV_SCREEN_LOAD_ANIM_NONE, 0, 0, true);
#endif
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
