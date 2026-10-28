/**
 * @file phase_step_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "phase_step_gen.h"
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

lv_obj_t * phase_step_create(lv_obj_t * parent, const char * title, const void * icon, int32_t index)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_WASHING_MACHINE_CHECK_COMPILE_TARGET(TI_WASHING_MACHINE_TARGET_ALL)
    if (ti_washing_machine_check_target(TI_WASHING_MACHINE_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "phase_step_#");
        lv_obj_set_width(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_bg_opa(lv_obj_0, 0, 0);
        lv_obj_set_style_border_width(lv_obj_0, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_0, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
        lv_obj_set_style_pad_column(lv_obj_0, 7, 0);
        lv_obj_set_style_layout(lv_obj_0, LV_LAYOUT_FLEX, 0);
        lv_obj_set_style_flex_flow(lv_obj_0, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_flex_main_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_track_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);

        lv_obj_t * lv_image_0 = lv_image_create(lv_obj_0);
        lv_image_set_src(lv_image_0, icon);
        lv_obj_set_flag(lv_image_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(lv_image_0, &style_icon_3, 0);
        lv_obj_add_style(lv_image_0, &style_icon_accent, LV_STATE_CHECKED);
        lv_obj_bind_state_if_ge(lv_image_0, &subject_phase, LV_STATE_CHECKED, index);

        lv_obj_t * label_0 = label_create(lv_obj_0);
        lv_label_set_text(label_0, title);
        lv_obj_set_flag(label_0, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_add_style(label_0, &style_micro, 0);
        lv_obj_add_style(label_0, &style_ink_3, 0);
        lv_obj_add_style(label_0, &style_ink, LV_STATE_CHECKED);
        lv_obj_bind_state_if_ge(label_0, &subject_phase, LV_STATE_CHECKED, index);

        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_width(lv_obj_1, 5);
        lv_obj_set_height(lv_obj_1, 5);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_CLICKABLE, false);
        lv_obj_set_style_radius(lv_obj_1, 32767, 0);
        lv_obj_set_style_border_width(lv_obj_1, 0, 0);
        lv_obj_set_style_shadow_width(lv_obj_1, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_1, 0, 0);
        lv_obj_add_style(lv_obj_1, &style_chip_dot_accent, 0);
        lv_obj_bind_flag_if_not_eq(lv_obj_1, &subject_phase, LV_OBJ_FLAG_HIDDEN, index);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

