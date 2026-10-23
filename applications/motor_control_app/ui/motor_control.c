/**
 * @file motor_control.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "motor_control.h"
#include "custom/motor_sim.h"

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

void motor_control_init(const char * asset_path)
{
    LV_LOG("Initializing custom C code using LVGL v%d.%d.%d", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);

    /* This is what creates and registers every subject, so nothing below may
     * run before it. */
    motor_control_init_gen(asset_path);

    /* The drive model. Starts the timer that publishes measured speed,
     * current, temperature and load. */
    motor_sim_init();

    /* Load the run screen so the application only has to call motor_control_init().
     * Screens are not permanent - each is built by its create function and freed
     * when another loads. The editor preview manages screens itself, hence the guard. */
#if !defined(LV_EDITOR_PREVIEW)
    /* <screen_create_event> deletes the screens it creates once they are unloaded,
     * but not this first one, so give it the same behaviour or it stays in RAM for
     * good. auto_del frees the display's empty default screen. */
    lv_obj_t * first = run_create();
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
