/**
 * @file screen_messages_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_messages_gen.h"
#include "../ti_smart_home.h"

/*********************
 *      DEFINES
 *********************/

#define COLOR_VIEW_BG lv_color_hex(0xE3F5DB)
#define COLOR_CARD1_BG lv_color_hex(0xFFFEF9)
#define COLOR_CARD1_TEXT lv_color_hex(0x484242)
#define COLOR_CARD2_BG lv_color_hex(0xF6EAB8)
#define COLOR_CARD2_TEXT lv_color_hex(0x564500)
#define COLOR_CARD3_BG lv_color_hex(0x98D79A)
#define COLOR_CARD3_TEXT lv_color_hex(0x0E3E02)


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

lv_obj_t * screen_messages_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if TI_SMART_HOME_CHECK_COMPILE_TARGET(TI_SMART_HOME_TARGET_ALL)
    if (ti_smart_home_check_target(TI_SMART_HOME_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "screen_messages_#");

        lv_obj_add_subject_set_int_event(lv_obj_0, &subject_nav_index, LV_EVENT_SCREEN_LOADED, REF_NAV_MESSAGES);
        lv_obj_t * root = column_create(lv_obj_0, 0, BASE_LIB_UNIT_MD, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(root, "root");
        lv_obj_set_width(root, lv_pct(100));
        lv_obj_set_height(root, lv_pct(100));
        lv_obj_set_flag(root, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_pad_top(root, 12, 0);
        lv_obj_set_style_bg_color(root, COLOR_VIEW_BG, 0);
        lv_obj_set_style_bg_opa(root, (255 * 100 / 100), 0);
        lv_obj_t * statusbar = header_create(root);
        lv_obj_set_name(statusbar, "statusbar");
        lv_obj_set_style_pad_hor(statusbar, BASE_LIB_UNIT_XL, 0);

        lv_obj_t * content = column_create(root, 0, BASE_LIB_UNIT_XL, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(content, "content");
        lv_obj_set_width(content, lv_pct(100));
        lv_obj_set_style_pad_hor(content, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_style_pad_bottom(content, BASE_LIB_UNIT_MD, 0);
        lv_obj_set_flag(content, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * column_0 = column_create(content, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_width(column_0, lv_pct(100));
        h1_create(column_0, "Messages");

        lv_obj_t * card_contact = card_empty_create(content);
        lv_obj_set_name(card_contact, "card_contact");
        lv_obj_set_flex_flow(card_contact, LV_FLEX_FLOW_ROW);
        lv_obj_set_width(card_contact, lv_pct(100));
        lv_obj_set_height(card_contact, 184);
        lv_obj_set_style_flex_cross_place(card_contact, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_color(card_contact, COLOR_CARD3_BG, 0);
        lv_obj_set_style_text_color(card_contact, COLOR_CARD3_TEXT, 0);
        lv_obj_set_flag(card_contact, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * contact_icon = image_create(card_contact, image_contact_large);
        lv_obj_set_name(contact_icon, "contact_icon");
        lv_image_bind_src(contact_icon, &subject_contact_img);

        lv_obj_t * column_1 = column_create(card_contact, 0, -4, 1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_style_pad_hor(column_1, BASE_LIB_UNIT_LG, 0);
        lv_obj_t * h2_0 = h2_create(column_1, "Heading 2");
        lv_label_bind_text(h2_0, &subject_contact_name, NULL);
        lv_obj_set_width(h2_0, lv_pct(100));
        lv_label_set_long_mode(h2_0, LV_LABEL_LONG_MODE_DOTS);

        lv_obj_t * text_0 = text_create(column_1, font_md);
        lv_label_bind_text(text_0, &subject_contact_email, NULL);
        lv_obj_set_width(text_0, lv_pct(100));
        lv_label_set_long_mode(text_0, LV_LABEL_LONG_MODE_DOTS);
        lv_obj_set_style_text_opa(text_0, (255 * 64 / 100), 0);

        lv_obj_t * text_1 = text_create(column_1, font_md);
        lv_label_bind_text(text_1, &subject_contact_phone, NULL);
        lv_obj_set_width(text_1, lv_pct(100));
        lv_obj_set_style_text_opa(text_1, (255 * 64 / 100), 0);

        lv_obj_t * row_0 = row_create(card_contact, 0, BASE_LIB_UNIT_MD, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        button_circle_create(row_0, icon_email, COLOR_ACCENT, 0, COLOR_CARD3_TEXT, 56);

        button_circle_create(row_0, icon_call, lv_color_hex3(0xfff), (255 * 100 / 100), COLOR_CARD3_TEXT, 64);

        button_circle_create(row_0, icon_message, COLOR_ACCENT, 0, COLOR_CARD3_TEXT, 56);

        lv_obj_t * message_list = list_container_create(content);
        lv_obj_set_name(message_list, "message_list");
        lv_obj_set_width(message_list, lv_pct(100));
        lv_obj_set_height(message_list, 340);
        lv_obj_set_flex_grow(message_list, 0);
        lv_obj_set_style_pad_all(message_list, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_style_pad_top(message_list, 64, 0);
        lv_obj_set_flag(message_list, LV_OBJ_FLAG_SCROLL_CHAIN_VER, false);
        list_message_create(message_list, image_avatar_lisa_small, "10:24", "Lisa", "Can you pick up milk on the way?");

        list_message_create(message_list, icon_charge, "08:15", "EV Charger", "Charging complete.");

        list_message_create(message_list, image_contact_alex_small, "Yesterday 19:44", "Alex", "Dinner at 7?");

        list_message_create(message_list, icon_messages, "Yesterday 18:23", "Security", "Front door unlocked");

        lv_obj_t * row_1 = row_create(message_list, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_y(row_1, -64);
        lv_obj_set_flag(row_1, LV_OBJ_FLAG_FLOATING, true);
        lv_obj_set_width(row_1, lv_pct(100));
        lv_obj_set_style_bg_color(row_1, COLOR_BG, 0);
        lv_obj_set_style_bg_opa(row_1, (255 * 100 / 100), 0);
        lv_obj_set_style_pad_top(row_1, 24, 0);
        lv_obj_set_style_shadow_width(row_1, 30, 0);
        lv_obj_set_style_shadow_color(row_1, COLOR_BG, 0);
        lv_obj_set_style_shadow_opa(row_1, (255 * 100 / 100), 0);
        lv_obj_set_style_shadow_spread(row_1, 10, 0);
        lv_obj_t * image_mono_0 = image_mono_create(row_1, icon_messages);
        lv_obj_set_style_image_recolor(image_mono_0, COLOR_CARD1_TEXT, 0);

        h3_create(row_1, "Inbox");

        lv_obj_t * contact_list = list_container_create(content);
        lv_obj_set_name(contact_list, "contact_list");
        lv_obj_set_width(contact_list, lv_pct(100));
        lv_obj_set_flex_grow(contact_list, 1);
        lv_obj_set_style_pad_all(contact_list, BASE_LIB_UNIT_XL, 0);
        lv_obj_set_style_pad_top(contact_list, 64, 0);
        lv_obj_set_flag(contact_list, LV_OBJ_FLAG_SCROLL_CHAIN_VER, false);
        list_contact_create(contact_list, image_contact_small, "Noah Davis", "noah.davis@gmail.com", "+1 45 678 45 56");

        list_contact_create(contact_list, image_contact_alex_small, "Alex Doe", "alexdoe@gmail.com", "+1 45 543 34 34");

        list_contact_create(contact_list, image_contact_lukas_small, "Lukas Schneider", "lukas.schneider@gmail.com", "+49 151 23456789");

        list_contact_create(contact_list, image_avatar_lisa_small, "Lisa Swift", "lisa.swift@gmail.com", "+33 6 15 34 76 78");

        list_contact_create(contact_list, image_contact_emma_small, "Emma Dubois", "emma.dubois@gmail.com", "+33 6 12 34 56 78");

        list_contact_create(contact_list, image_contact_marco_small, "Marco Rossi", "marco.rossi@gmail.com", "+39 320 123 4567");

        list_contact_create(contact_list, image_contact_sofia_small, "Sofia Andersson", "sofia.andersson@gmail.com", "+46 70 123 45 67");

        list_contact_create(contact_list, image_contact_oliver_small, "Oliver Novak", "oliver.novak@gmail.com", "+420 601 123 456");

        list_contact_create(contact_list, image_contact_thomas_small, "Thomas Müller", "thomas.mueller@gmail.com", "+49 176 98765432");

        list_contact_create(contact_list, image_contact_isabella_small, "Isabella Garcia", "isabella.garcia@gmail.com", "+34 612 345 678");

        list_contact_create(contact_list, image_contact_noah_small, "Noah Jensen", "noah.jensen@gmail.com", "+45 21 23 45 67");

        list_contact_create(contact_list, image_contact_elena_small, "Elena Petrova", "elena.petrova@gmail.com", "+359 88 123 4567");

        lv_obj_t * row_2 = row_create(contact_list, 0, BASE_LIB_UNIT_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_y(row_2, -64);
        lv_obj_set_flag(row_2, LV_OBJ_FLAG_FLOATING, true);
        lv_obj_set_width(row_2, lv_pct(100));
        lv_obj_set_style_bg_color(row_2, COLOR_BG, 0);
        lv_obj_set_style_bg_opa(row_2, (255 * 100 / 100), 0);
        lv_obj_set_style_pad_top(row_2, 24, 0);
        lv_obj_set_style_shadow_width(row_2, 30, 0);
        lv_obj_set_style_shadow_color(row_2, COLOR_BG, 0);
        lv_obj_set_style_shadow_opa(row_2, (255 * 100 / 100), 0);
        lv_obj_set_style_shadow_spread(row_2, 10, 0);
        lv_obj_t * image_mono_1 = image_mono_create(row_2, icon_contacts);
        lv_obj_set_style_image_recolor(image_mono_1, COLOR_CARD1_TEXT, 0);

        h3_create(row_2, "Contacts");

        navbar_create(root);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

