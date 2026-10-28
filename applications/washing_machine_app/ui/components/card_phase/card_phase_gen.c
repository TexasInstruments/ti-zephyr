/**
 * @file card_phase_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "card_phase_gen.h"
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

lv_obj_t * card_phase_create(lv_obj_t * parent, int32_t w)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "card_phase_#");
        lv_obj_set_width(lv_obj_0, w);
        lv_obj_set_height(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_pad_left(lv_obj_0, 20, 0);
        lv_obj_set_style_pad_right(lv_obj_0, 20, 0);
        lv_obj_set_style_pad_top(lv_obj_0, 7, 0);
        lv_obj_set_style_pad_bottom(lv_obj_0, 7, 0);
        lv_obj_set_style_layout(lv_obj_0, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(lv_obj_0, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_flex_main_place(lv_obj_0, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_track_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_shadow_width(lv_obj_0, 0, 0);
        lv_obj_set_style_outline_width(lv_obj_0, 0, 0);

        lv_obj_add_style(lv_obj_0, &style_card_surface, 0);
        phase_step_create(lv_obj_0, "Pre", icon_ph_pre, REF_PHASE_PREWASH);

        phase_step_create(lv_obj_0, "Wash", icon_ph_wash, REF_PHASE_WASH);

        phase_step_create(lv_obj_0, "Rinse", icon_ph_rinse, REF_PHASE_RINSE);

        phase_step_create(lv_obj_0, "Spin", icon_ph_spin, REF_PHASE_SPIN);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

