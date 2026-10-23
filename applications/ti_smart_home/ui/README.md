# TI — Smart Home Dashboard (`ti_smart_home`)

A four-screen smart-home hub UI authored in LVGL Pro XML (`lv_xml` DSL, LVGL 9.5.0, `theme`
unset). `project.xml` declares two landscape targets — 1280x800 and 1024x600 — but no specific TI
part or panel is named anywhere in the source, and no XML or C in this repo conditions on either
`target=`; layout widths are hard pixel values sized for the 1280-wide canvas (e.g. the 296px
sidebar, the 320/280/480px `ev_charging` cards), so target2 is declared but untested against them.
It is UI only — no firmware, drivers, or sensor code: every value on screen comes from LVGL
subjects carrying demo defaults. Unlike the other `ti-*` demos in this repo, there is no `sim/`
folder here — no desktop-simulator entry point is checked in, so the only way to see it running is
the Editor's own live preview.

## Screens

| Screen | Purpose |
|---|---|
| `screen_home` | Landing hub: clock/date, "Morning, Gábor!" greeting, and three summary cards (Smart Home indoor/outdoor temp, EV charge %, latest message) that each `screen_create_event` into one of the other three screens. Has its own full-height background image instead of the shared `sidebar` component. |
| `screen_smart_home` | Thermostat arc with +/- stepper buttons, current-weather card, a music player (play/pause, prev/next, album art), and four device tiles (kettle, vacuum, Wi-Fi, live camera). |
| `screen_ev_charging` | Vehicle charging detail: state of charge, a static "Full in ~2 h 15 min" / "Started at 14:32" text, range, and a 7-day bar chart of consumption (static per-point values, not subject-bound). |
| `screen_messages` | Three-column layout: a message inbox list, a 12-entry contact list, and a contact-details card that fills in from whichever contact was last tapped. |

All four are created on demand: every link (`navbar` tabs, `screen_home` cards, the `header`
logo) is a `screen_create_event`, which builds the target and deletes the previous screen once it is
unloaded, so only one screen tree is in RAM at a time (two during the 200 ms fade). They are not
`permanent="true"`: that keeps every screen alive, and LVGL Pro Editor 2.0.4 also exports broken
links between permanent screens (see `EDITOR_2.0.4_SCREEN_LOAD_BUG.md`). Each screen sets
`subject_nav_index` on `screen_loaded`, and the active `navbar` tab is non-clickable so re-tapping it
cannot rebuild the current screen.

Everything a screen shows lives in a subject, so a rebuilt screen shows what the user left: the
device switches (`subject_kettle_on`/`_vacuum_on`/`_wifi_on`), the album cover
(`subject_album_cover`) and the contact avatar (`subject_contact_img`). Only scroll positions reset.

Rendering avoids ARGB8888 draw layers: the `header` logo is a pre-scaled asset (`logostrip_small`,
105x28) rather than `transform_scale`, and nothing uses `clip_corner`. The camera tile's image is
pre-scaled to the tile (324x160, RGB565) and drawn 1:1 so its own `radius` rounds it (`lv_image`
cannot round a stretched image); the message and contact lists keep padding on all sides and inset
their scrollbar by the corner radius.

Boolean attributes are written `true`/`false`, never `1`/`0`: the 2.0.4 C exporter reads anything
but `true` as false, while the Editor preview reads anything but `false` as true.

## Subjects

Declared in `globals.xml`, exported as `extern lv_subject_t` by `ti_smart_home_gen.h`. 31 total;
several are declared but never read or written by any XML or C in this repo (noted below) — they
exist as placeholders for demo values that were never wired to a widget.

| Subject | Type | Default | Meaning |
|---|---|---|---|
| `subject_dark_mode` | int | 0 | Declared but not referenced by any XML or C in this repo. |
| `subject_hour` | int | 9 | Clock hour, shown in `header` and the `screen_home` clock. Nothing writes it. |
| `subject_minute` | int | 43 | Clock minute, same widgets. Nothing writes it. |
| `subject_day_index` | int | 12 | Day of month in the `date` component. Nothing writes it. |
| `subject_day_name` | string | `Wed` | Day-of-week text in `date` and `header`/`screen_home`. Nothing writes it. |
| `subject_month_name` | string | `Jan` | Month text in `date`. Nothing writes it. |
| `subject_year` | int | 2026 | Year in `date`. Nothing writes it. |
| `subject_temp` | int | 23 | Indoor target temperature, 16-32. Shown on the `screen_home` card and the `screen_smart_home` thermostat arc; written by the arc's +/- `button_circle`s via `subject_increment_event`. |
| `subject_ev_soc` | int | 78 | EV battery state of charge, percent. Shown on `screen_home` and `screen_ev_charging` (value + `lv_bar`). Nothing writes it. |
| `subject_ev_range_km` | int | 312 | EV range, km, on `screen_ev_charging`. Nothing writes it. |
| `subject_ev_power_kw` | int | 11 | Declared but not referenced by any XML or C in this repo. |
| `subject_outside_temp_current` | int | 6 | Outdoor temperature, °C, on `screen_home` and the `screen_smart_home` weather card. Nothing writes it. |
| `subject_outside_temp_highest` | int | 9 | Today's high, °C, `screen_smart_home` weather card only. Nothing writes it. |
| `subject_weather_desc` | string | `Cloudy` | Weather description on the `screen_smart_home` weather card. Nothing writes it. |
| `subject_album_name` | string | `Sentimental Value` | Now-playing album, music player. Written by `music_update_ui()` in `ti_smart_home.c` when the prev/next buttons are tapped. |
| `subject_track_name` | string | `Hana Rani  —  Lighter and Lighter` | Now-playing track. Same as above. |
| `subject_play` | int | 0 | Play/pause state, two-way: the play `button_circle`'s `subject_toggle_event` writes it, `bind_state_if_eq`/`bind_style` swap its icon and style when it reads back 1. |
| `subject_shuffle` | int | 0 | Declared but not referenced — the shuffle button has no binding to it. |
| `subject_repeat` | int | 0 | Declared but not referenced — the repeat button has no binding to it. |
| `subject_music_progress` | int | 45 | Declared but not referenced — no progress bar is bound to it despite the name. |
| `subject_vacuum_battery` | int | 19 | Vacuum-cleaner battery, percent, `screen_smart_home` device tile. Nothing writes it. |
| `subject_kettle_temp` | int | 67 | Smart-kettle temperature, `screen_smart_home` device tile. Nothing writes it. |
| `subject_kettle_on` / `subject_vacuum_on` / `subject_wifi_on` | int | 0 | Device tile switches on `screen_smart_home`, two-way via `bind_checked`. |
| `subject_album_cover` | pointer | `image_album_cover` | Now-playing album art, bound with `bind_src` on `album_cover`. Written by `music_update_ui()` in `ti_smart_home.c`. |
| `subject_contact_img` | pointer | `image_contact_large` | Selected contact's avatar, bound with `bind_src` on `contact_icon`. Written by `on_contact_clicked_cb` in `ti_smart_home.c` (the XML write in `list_contact.xml` is commented out). |
| `subject_contact_name` | string | `Noah Davis` | Selected contact's name, `screen_messages` details card. Written by `list_contact`'s `subject_set_string_event` on row tap. |
| `subject_contact_email` | string | `noah.davis@gmail.com` | Selected contact's e-mail. Same trigger. |
| `subject_contact_phone` | string | `+1 45 678 45 56` | Selected contact's phone. Same trigger. |

## Custom C

`ti_smart_home.c`/`.h` is the only custom source built into `lib-ui` (`file_list_gen.cmake`
references it directly; there's nothing in `user_config.cmake`). It holds:

- `ti_smart_home_init(asset_path)` — logs the LVGL version, calls the generated
  `ti_smart_home_init_gen()`, then loads `screen_home_create()` (not in the Editor preview).
- `on_music_previous_cb` / `on_music_next_cb` — step a 5-track playlist index and write
  `subject_album_name`/`subject_track_name`/`subject_album_cover`. Wired from the prev/next
  `button_circle`s in `screens/screen_smart_home.xml`.
- `on_contact_clicked_cb` — set `subject_contact_img` by matching the clicked row's e-mail
  against a hard-coded table. Wired from `components/list/list_contact/list_contact.xml`.

These three callback names are also declared (and given `__attribute__((weak))` do-nothing
fallback bodies that only `LV_LOG`) in the generated `ti_smart_home_gen.c`; the strong definitions
in `ti_smart_home.c` override them at link time, so it's the real behavior above that runs.

## Wiring it to application code

- Include `ti_smart_home.h` (pulls in `ti_smart_home_gen.h`, which declares every subject
  `extern lv_subject_t` and forward-declares the three event callbacks).
- Call `ti_smart_home_init(const char * asset_path)` once, after `lv_init()` and after the display
  exists. It creates every subject and loads `screen_home_create()` itself — no separate
  `lv_screen_load()` call needed. There are no screen globals (screens are created on demand);
  to navigate from C, use `lv_screen_load_anim(screen_messages_create(), LV_SCREEN_LOAD_ANIM_NONE,
  0, 0, true)` — the final `true` deletes the previous screen, which a plain `lv_screen_load()` keeps.
- Write: `lv_subject_set_int(&subject_temp, 24);` /
  `lv_subject_copy_string(&subject_weather_desc, "Rain");`
  Read: `lv_subject_get_int(&subject_ev_soc);`
- UI writes directly, no C involved: `subject_temp` (thermostat stepper), `subject_play` (music
  toggle), `subject_contact_name`/`_email`/`_phone` (tapping a contact row, via
  `subject_set_string_event`).
- Edit by hand: `*.xml`, `ti_smart_home.c`/`.h`, `user_config.cmake`. Regenerated, never edit:
  `*_gen.c`, `*_gen.h`, `*_gen.cmake`, `CMakeLists.txt`, `images/*_data.c`, `fonts/*_data.c`,
  `component_lib_list_gen.cmake`, `file_list_gen.cmake`.

## Layout

- `screens/` — the four screen XMLs.
- `components/` — component library by category: `buttons` (`button`, `button_circle`), `cards`
  (`card_empty`), `controls` (`arc`, `checkbox`, `slider`, `switch`, `value_setter`), `header`
  (`header`, `icon`), `headings` (`h1`-`h4`), `layout` (`column`, `container`, `panel`, `row`,
  `spacer`), `list` (`list_contact`, `list_container`, `list_item`, `list_message`), `sidebar`,
  `text`, `visualizers` (`chart`, `date`, `image`, `image_mono`, `time`, `value_large`/`medium`/
  `small`/`xlarge`). `checkbox`, `slider`, `list_item`, `value_setter` and `icon` are defined but
  not instantiated by any screen in this repo.
- `widgets/` — empty (README only); no custom C widgets in this project.
- `globals.xml` — base-unit/radius consts, colors, 26 subjects, 61 images, 7 `font_*` sizes of
  Outfit Medium/Regular, and the shared styles.
- `ti_smart_home.c`/`.h` — the hand-written entry point and event callbacks (see Custom C).
- `images/`, `fonts/` — source PNGs and the two Outfit `.ttf` faces; the `*_data.c` beside them are
  generated.
- `preview-bin/` — the Editor's wasm-based live-preview runtime; checked into git here (unlike the
  other `ti-*` demos, which gitignore all build output). `preview-build/` is the native preview
  build and is gitignored.
- No `sim/`, `tools/`, or `docs/` folder — this project ships no desktop-simulator entry point or
  icon-rasterizing script, and no rendered screenshots.
