/**
 * @file drum_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "drum_gen.h"
#include "../../ti_washing_machine.h"

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

lv_obj_t * drum_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_arc;
    static lv_style_t style_ring_stroke;
    static lv_style_t style_ring_knob;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_arc);
        lv_style_init(&style_ring_stroke);
        lv_style_init(&style_ring_knob);

        lv_style_set_width(&style_arc, 180);
        lv_style_set_height(&style_arc, 180);
        lv_style_set_align(&style_arc, LV_ALIGN_CENTER);
        lv_style_set_arc_width(&style_arc, 6);
        lv_style_set_pad_all(&style_arc, 7);
        lv_style_set_arc_width(&style_ring_stroke, 6);
        lv_style_set_bg_opa(&style_ring_knob, (255 * 0 / 100));
        lv_style_set_border_width(&style_ring_knob, 0);
        lv_style_set_shadow_width(&style_ring_knob, 0);
        lv_style_set_pad_all(&style_ring_knob, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "drum_#");
        lv_obj_set_width(lv_obj_0, 200);
        lv_obj_set_height(lv_obj_0, 200);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_bg_opa(lv_obj_0, 0, 0);
        lv_obj_set_style_border_width(lv_obj_0, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_0, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);

        lv_obj_t * ring = lv_arc_create(lv_obj_0);
        lv_obj_set_name(ring, "ring");
        lv_arc_set_min_value(ring, 0);
        lv_arc_set_max_value(ring, 100);
        lv_arc_set_rotation(ring, 270);
        lv_arc_set_bg_start_angle(ring, 0);
        lv_arc_set_bg_end_angle(ring, 360);
        lv_arc_set_mode(ring, LV_ARC_MODE_NORMAL);
        lv_arc_bind_value(ring, &subject_progress);
        lv_obj_set_flag(ring, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_bind_flag_if_not_eq(ring, &subject_state, LV_OBJ_FLAG_HIDDEN, REF_STATE_RUNNING);
        lv_obj_remove_style_all(ring);
        lv_obj_add_style(ring, &style_arc, 0);
        lv_obj_add_style(ring, &style_arc_track, 0);
        lv_obj_add_style(ring, &style_arc_ind, LV_PART_INDICATOR);
        lv_obj_add_style(ring, &style_ring_stroke, 0);
        lv_obj_add_style(ring, &style_ring_stroke, LV_PART_INDICATOR);
        lv_obj_add_style(ring, &style_ring_knob, LV_PART_KNOB);

        lv_obj_t * drum_load = lv_image_create(lv_obj_0);
        lv_obj_set_name(drum_load, "drum_load");
        lv_image_set_src(drum_load, img_clothes);
        lv_obj_set_align(drum_load, LV_ALIGN_CENTER);
        lv_obj_set_flag(drum_load, LV_OBJ_FLAG_CLICKABLE, false);

        lv_obj_t * lv_image_0 = lv_image_create(lv_obj_0);
        lv_image_set_src(lv_image_0, img_door);
        lv_obj_set_align(lv_image_0, LV_ALIGN_CENTER);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

