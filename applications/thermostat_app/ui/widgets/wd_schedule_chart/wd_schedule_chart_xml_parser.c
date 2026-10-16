/**
 * @file wd_schedule_chart_xml_parser.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "wd_schedule_chart_gen.h"

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML

#include "lv_xml_private/lv_xml_private.h"

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

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void * wd_schedule_chart_xml_create(lv_xml_parser_state_t * state, const char ** attrs)
{
    LV_UNUSED(attrs);
    void * item = wd_schedule_chart_create(lv_xml_state_get_parent(state));

    if(item == NULL) {
        LV_LOG_ERROR("Failed to create wd_schedule_chart");
        return NULL;
    }

    return item;
}

void wd_schedule_chart_xml_apply(lv_xml_parser_state_t * state, const char ** attrs)
{
    void * item = lv_xml_state_get_item(state);

    lv_xml_obj_apply(state, attrs);

    /* Hand-maintained: the generator declares the setter from the <api> but does
     * not wire the attribute to it. Every prop added to wd_schedule_chart.xml
     * needs a branch here or it silently does nothing. */
    for(int i = 0; attrs[i]; i += 2) {
        const char * name = attrs[i];
        const char * value = attrs[i + 1];

        if(lv_streq(name, "bind_day")) {
            lv_subject_t * subject = lv_xml_get_subject(&state->scope, value);
            if(subject) wd_schedule_chart_bind_day(item, subject);
        }
    }
}

void wd_schedule_chart_register(void)
{
    lv_xml_register_widget("wd_schedule_chart", wd_schedule_chart_xml_create, wd_schedule_chart_xml_apply);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /* LV_USE_XML */