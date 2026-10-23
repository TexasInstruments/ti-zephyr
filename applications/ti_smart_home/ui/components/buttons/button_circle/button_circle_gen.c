/**
 * @file button_circle_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "button_circle_gen.h"
#include "../../../ti_smart_home.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * button_circle_create(lv_obj_t * parent, const void * icon, lv_color_t bg_color, lv_opa_t bg_opa, lv_color_t color, int32_t size)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t button_pressed;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&button_pressed);

        lv_style_set_bg_opa(&button_pressed, (255 * 40 / 100));
        lv_style_set_transform_height(&button_pressed, 0);
        lv_style_set_transform_width(&button_pressed, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "button_circle_#");
        lv_obj_set_width(lv_button_0, size);
        lv_obj_set_style_bg_color(lv_button_0, bg_color, 0);
        lv_obj_set_style_bg_opa(lv_button_0, bg_opa, 0);
        lv_obj_set_height(lv_button_0, size);
        lv_obj_set_flag(lv_button_0, LV_OBJ_FLAG_CLICKABLE, true);
        lv_obj_set_style_radius(lv_button_0, 100, 0);
        lv_obj_set_style_shadow_width(lv_button_0, 0, 0);
        lv_obj_set_style_bg_image_src(lv_button_0, icon, 0);
        lv_obj_set_style_bg_image_recolor(lv_button_0, color, 0);
        lv_obj_set_style_bg_image_recolor_opa(lv_button_0, (255 * 100 / 100), 0);

        lv_obj_add_style(lv_button_0, &button_pressed, LV_STATE_PRESSED);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

