/*******************************************************************************
 * Size: 24 px
 * Bpp: 2
 * Opts: --font /fonts/Gabarito-Medium.ttf -o /fonts/font_medium_24_data.c --size 24 --bpp 2 --format lvgl --no-compress --range 0x20,0x68,0x69,0x6D,0x6E
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef FONT_MEDIUM_24_DATA
#define FONT_MEDIUM_24_DATA 1
#endif

#if FONT_MEDIUM_24_DATA

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0068 "h" */
    0xbc, 0x00, 0x02, 0xf0, 0x00, 0x0b, 0xc0, 0x00,
    0x2f, 0x00, 0x00, 0xbc, 0x2f, 0xd2, 0xff, 0xff,
    0xdb, 0xff, 0xff, 0xaf, 0xd0, 0x3f, 0xbd, 0x00,
    0xfe, 0xf0, 0x03, 0xfb, 0xc0, 0x0f, 0xef, 0x00,
    0x3f, 0xbc, 0x00, 0xfe, 0xf0, 0x03, 0xfb, 0xc0,
    0x0f, 0xef, 0x00, 0x3f,

    /* U+0069 "i" */
    0x7c, 0xfe, 0xbd, 0x14, 0x00, 0xbd, 0xbd, 0xbd,
    0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd,
    0xbd,

    /* U+006D "m" */
    0xb8, 0x7f, 0xc0, 0xbf, 0x4b, 0xff, 0xff, 0xbf,
    0xfd, 0xbf, 0xff, 0xff, 0xff, 0xeb, 0xf0, 0x1f,
    0xd0, 0x3f, 0xbd, 0x00, 0xfc, 0x03, 0xfb, 0xc0,
    0x0f, 0x80, 0x3f, 0xbc, 0x00, 0xf8, 0x03, 0xfb,
    0xc0, 0x0f, 0x80, 0x3f, 0xbc, 0x00, 0xf8, 0x03,
    0xfb, 0xc0, 0x0f, 0x80, 0x3f, 0xbc, 0x00, 0xf8,
    0x03, 0xfb, 0xc0, 0x0f, 0x80, 0x3f,

    /* U+006E "n" */
    0xb8, 0x6f, 0xd2, 0xff, 0xff, 0xdb, 0xff, 0xff,
    0xaf, 0xd0, 0x3f, 0xbd, 0x00, 0xfe, 0xf0, 0x03,
    0xfb, 0xc0, 0x0f, 0xef, 0x00, 0x3f, 0xbc, 0x00,
    0xfe, 0xf0, 0x03, 0xfb, 0xc0, 0x0f, 0xef, 0x00,
    0x3f
};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 100, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 212, .box_w = 11, .box_h = 16, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 92, .box_w = 4, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 61, .adv_w = 324, .box_w = 18, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 115, .adv_w = 212, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x00, 0x48, 0x49, 0x4d, 0x4e
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 79, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 5, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t font_medium_24_data = {
#else
lv_font_t font_medium_24_data = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if LV_VERSION_CHECK(9, 6, 0) || LVGL_VERSION_MAJOR >= 10
    .cap_height = 16,           /*Cap height of the font*/
    .x_height = 12,               /*x-height of the font*/
#endif
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -5,
    .underline_thickness = 2,
#endif

#if LV_VERSION_CHECK(9, 3, 0)
    .static_bitmap = 1,    /*Bitmaps are stored as const so they are always static if not compressed */
#endif

    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if FONT_MEDIUM_24_DATA*/
