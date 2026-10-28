/**
 * @file ti_washing_machine.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ti_washing_machine.h"
#include "custom/wash_logic.h"
#include "custom/wash_sim.h"

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

void ti_washing_machine_init(const char * asset_path)
{
    LV_LOG("Initializing custom C code using LVGL v%d.%d.%d", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);

    ti_washing_machine_init_gen(asset_path);

    /* Add your own custom code here if needed */
    wash_logic_init();
    wash_sim_init();

    /* Screens are non-permanent and built on demand, so the generated screen globals
    * are NULL under LV_USE_XML; only a real build loads a screen here. */
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
