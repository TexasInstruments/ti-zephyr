/**
 * @file arc_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "arc_gen.h"
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

lv_obj_t * arc_create(lv_obj_t * parent, lv_subject_t * subject)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_arc;
    static lv_style_t style_arc_indicator;
    static lv_style_t style_arc_knob;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_arc);
        lv_style_init(&style_arc_indicator);
        lv_style_init(&style_arc_knob);

        lv_style_set_arc_width(&style_arc, BASE_LIB_UNIT_LG);
        lv_style_set_arc_color(&style_arc, COLOR_ACCENT);
        lv_style_set_arc_opa(&style_arc, (255 * 16 / 100));
        lv_style_set_arc_width(&style_arc_indicator, BASE_LIB_UNIT_LG);
        lv_style_set_arc_color(&style_arc_indicator, COLOR_ACCENT);
        lv_style_set_arc_opa(&style_arc_indicator, (255 * 100 / 100));
        lv_style_set_bg_color(&style_arc_knob, COLOR_ACCENT_TEXT);
        lv_style_set_pad_all(&style_arc_knob, -4);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_arc_0 = lv_arc_create(parent);
        lv_obj_set_name_static(lv_arc_0, "arc_#");
        lv_arc_bind_value(lv_arc_0, subject);

        lv_obj_add_style(lv_arc_0, &style_arc, 0);
        lv_obj_add_style(lv_arc_0, &style_arc_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(lv_arc_0, &style_arc_knob, LV_PART_KNOB);

        the_root = lv_arc_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

