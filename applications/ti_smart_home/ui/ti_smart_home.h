/**
 * @file ti_smart_home.h
 */

#ifndef LVGL_PRO_TI_SMART_HOME_H
#define LVGL_PRO_TI_SMART_HOME_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "ti_smart_home_gen.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the component library
 */
void ti_smart_home_init(const char * asset_path);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_TI_SMART_HOME_H*/