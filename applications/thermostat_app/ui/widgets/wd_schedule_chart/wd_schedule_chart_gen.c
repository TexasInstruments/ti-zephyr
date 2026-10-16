/**
 * @file wd_schedule_chart_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "wd_schedule_chart_private_gen.h"
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
#include "../../ti_thermostat.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

void wd_schedule_chart_constructor_hook(lv_obj_t * obj);
void wd_schedule_chart_destructor_hook(lv_obj_t * obj);
void wd_schedule_chart_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void wd_schedule_chart_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void wd_schedule_chart_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void wd_schedule_chart_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t wd_schedule_chart_class = {
    .base_class = &lv_chart_class,
    .constructor_cb = wd_schedule_chart_constructor,
    .destructor_cb = wd_schedule_chart_destructor,
    .event_cb = wd_schedule_chart_event,
    .instance_size = sizeof(wd_schedule_chart_t),
    .editable = 1,
    .name = "wd_schedule_chart"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * wd_schedule_chart_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&wd_schedule_chart_class, parent);
    lv_obj_class_init_obj(obj);

    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void wd_schedule_chart_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    wd_schedule_chart_t * widget = (wd_schedule_chart_t *)obj;
    static lv_style_t style_chart;
    static lv_style_t style_series;
    static lv_style_t style_points;
    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_chart);
        lv_style_init(&style_series);
        lv_style_init(&style_points);

        lv_style_set_width(&style_chart, 264);
        lv_style_set_height(&style_chart, 104);
        lv_style_set_bg_opa(&style_chart, 0);
        lv_style_set_border_width(&style_chart, 0);
        lv_style_set_pad_all(&style_chart, 0);
        lv_style_set_line_width(&style_chart, 1);
        lv_style_set_line_color(&style_chart, CONST_BORDER);
        lv_style_set_line_opa(&style_chart, (255 * 60 / 100));
        lv_style_set_line_width(&style_series, 3);
        lv_style_set_line_rounded(&style_series, false);
        lv_style_set_radius(&style_series, 0);
        lv_style_set_width(&style_points, 0);
        lv_style_set_height(&style_points, 0);

        style_inited = true;
    }

    lv_obj_t * the_root = NULL;
    #if TI_THERMOSTAT_CHECK_COMPILE_TARGET(TI_THERMOSTAT_TARGET_ALL)
    if (ti_thermostat_check_target(TI_THERMOSTAT_TARGET_ALL)) {
        lv_chart_set_type(obj, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(obj, 24);
        lv_chart_set_hor_div_line_count(obj, 3);
        lv_chart_set_ver_div_line_count(obj, 0);

        lv_obj_remove_style_all(obj);
        lv_obj_add_style(obj, &style_chart, 0);
        lv_obj_add_style(obj, &style_series, LV_PART_ITEMS);
        lv_obj_add_style(obj, &style_points, LV_PART_INDICATOR);
        lv_chart_series_t * lv_chart_series_0 = lv_chart_add_series(obj, lv_color_hex(0xE8630A), LV_CHART_AXIS_PRIMARY_Y);
        widget->lv_chart_series_0 = lv_chart_series_0;
        static const int32_t obj_values_0[] = {17, 17, 17, 17, 17, 17, 21, 21, 21, 18, 18, 18, 18, 18, 18, 18, 21, 21, 21, 21, 21, 21, 18, 18};
        lv_chart_set_series_values(obj, lv_chart_series_0, obj_values_0, 24);
        lv_chart_set_axis_min_value(obj, LV_CHART_AXIS_PRIMARY_Y, 14);
        lv_chart_set_axis_max_value(obj, LV_CHART_AXIS_PRIMARY_Y, 24);

        the_root = obj;
    }
    #endif
    wd_schedule_chart_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void wd_schedule_chart_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    wd_schedule_chart_destructor_hook(obj);
}

static void wd_schedule_chart_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&wd_schedule_chart_class, e);
    if(res != LV_RESULT_OK) return;

    wd_schedule_chart_event_hook(e);
}

