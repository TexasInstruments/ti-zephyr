# TI MSPM33 — Washing Machine HMI

An LVGL Pro (lv_xml) UI for a washing-machine HMI on the TI MSPM33 platform, for a 480x320
landscape touch panel (`project.xml` target1; a 3.5 inch class panel per `tools/iconset.js`).
UI only — no firmware: XML screens and components, assets, two small C modules, and a desktop
simulator. It presents a finished appliance (fabric programme, temperature, spin, water, options,
cycle progress) plus a service screen exposing motor, water, door, heater and pump.
Rendered screenshots of every screen are in [`docs/`](docs/).

## Screens

| Screen | Purpose |
|---|---|
| `home` | Landing/configurator: fabric programme keys, temperature slider, door artwork toggle, START key or blocked-hint key, links to setup and service. |
| `setup` | Options: spin-speed pills (400–1400), water-level slider, prewash/extra-rinse/delay-start toggles. |
| `cycle` | Running wash: drum with progress ring, remaining time, phase card, STOP (long press); the same screen also renders the "Cycle complete" summary, gated on `subject_state`. |
| `service` | Component test and status: motor PWM and water-level sliders, plus door-interlock / heater / drain-pump rows. |

## Subjects

Declared in `globals.xml`, defined in the generated `ti_washing_machine_gen.c/h`. Application code
drives the UI by writing these; those marked *derived* are owned by `custom/wash_logic.c` and must
not be written from anywhere else.

| Subject | Type | Default | Meaning |
|---|---|---|---|
| `subject_program` | int | -1 | Selected fabric programme: -1 none, 0 Cotton, 1 Eco 40, 2 Synthetics, 3 Quick 30 (`ref_prog_*`). |
| `subject_temp_idx` | int | 0 | Temperature slider detent, 0..4. 0 = not set; 1..4 = 20/40/60/90 °C. |
| `subject_temp_c` | int | 0 | *Derived.* Wash temperature in °C: 0 (unset) or 20/40/60/90. |
| `subject_spin_rpm` | int | 1200 | Spin speed, 400..1400 in steps of 200. Set by the `setup` pills. |
| `subject_water_target` | int | 62 | Target water level, 0..100 %. Set by the `setup` and `service` sliders; shown as the tick on the water indicator. |
| `subject_water_level` | int | 48 | Sensed water level, 0..100 %. Drives the water indicator beside the appliance. Firmware (level sensor) is the only writer; the demo does not simulate it. |
| `subject_opt_prewash` | int | 0 | Prewash option, 0/1. |
| `subject_opt_rinse` | int | 1 | Extra-rinse option, 0/1. |
| `subject_opt_delay` | int | 0 | Delay-start option, 0/1. |
| `subject_progress` | int | 0 | Cycle progress, 0..100 %. Written by `custom/wash_sim.c`; drives the ring, time, phase and completion. |
| `subject_state` | int | 1 | 0 idle, 1 running, 2 paused, 3 complete (`ref_state_*`; 2 is defined in `wash_logic.h` but unused by the XML). |
| `subject_door` | int | 1 | Door interlock: 0 open, 1 locked (`ref_door_*`). Toggled by the door artwork on `home` and the service row. |
| `subject_pwm_duty` | int | 74 | Drum-motor drive duty, 0..100 %. Service slider. |
| `subject_heater` | int | 0 | Heater element, 0 off / 1 on. Service row. |
| `subject_pump` | int | 0 | Drain pump, 0 idle / 1 running. Service row. |
| `subject_phase` | int | 0 | *Derived from progress.* 0 prewash (<12), 1 wash (<62), 2 rinse (<84), 3 spin (`ref_phase_*`). |
| `subject_ready` | int | 0 | *Derived.* 1 when a programme and temperature are chosen and the door is locked. Gates the START key. |
| `subject_time_left_str` | string | `"2:15"` | *Derived.* Remaining time, digits only (`"46"` or `"1:24"`). The hero font is subset to digits and `:` — any letter renders as a missing glyph. |
| `subject_time_unit_str` | string | `"h"` | *Derived.* Unit shown beside the time: `"min"` or `"h"`. |
| `subject_hint_str` | string | `"Select a fabric"` | *Derived.* Why the wash cannot start; empty when `subject_ready` is 1. |
| `subject_duration_str` | string | `"2h 15m"` | *Derived.* Total programme length, preformatted. |
| `subject_program_name` | string | `"Cotton"` | *Derived.* Display name of the selected programme; empty when none. |

## Custom C

| File | Responsibility |
|---|---|
| `custom/wash_logic.c` | Observer group on `subject_progress`, `subject_program`, `subject_temp_idx`, `subject_door`; recomputes and writes `subject_temp_c`, `subject_phase`, `subject_ready`, `subject_time_left_str`, `subject_time_unit_str`, `subject_hint_str`, `subject_duration_str`, `subject_program_name`. Holds the programme-duration and temperature tables. |
| `custom/wash_logic.h` | `wash_logic_init()` plus `WASH_STATE_*` / `WASH_PROG_*` / `WASH_PHASE_*` / `WASH_DOOR_*` enums, mirrored by hand as `ref_*` consts in `globals.xml`. |
| `custom/wash_sim.c` | Demo cycle: an `lv_timer` at 33 ms that ramps `subject_progress` 0→100 over 8 s (after a 2 s hold), spins the `drum_load` image via `lv_image_set_rotation` (not the `transform_rotation` style, which would redraw it through a ~51 KB ARGB8888 layer every frame), then sets `subject_state` to complete. Exports `wash_sim_screen_loaded_cb`, referenced by name from `screens/cycle.xml`. |
| `custom/wash_sim.h` | `wash_sim_init()` / `wash_sim_start()` / `wash_sim_stop()` and the cycle timing constants. |
| `ti_washing_machine.c` (project root) | Entry point: calls `ti_washing_machine_init_gen()`, then `wash_logic_init()` and `wash_sim_init()`, then loads the `home` screen. |

The XML handles visibility, styling, text formatting and all direct user writes
(`subject_set_int_event`, `subject_toggle_event`, two-way `bind_value`). C is needed only for what
the DSL cannot do: strings richer than `%d`/`%s`, deriving one subject from another, and anything
timed. When firmware replaces the demo cycle, drop `custom/wash_sim.c`; `wash_logic.c` stays.

## Wiring it to application code

- Include `ti_washing_machine.h`; it pulls in the generated `ti_washing_machine_gen.h`, which declares
  every subject as `extern lv_subject_t`. Add this project's `CMakeLists.txt` as a subdirectory —
  it builds a `lib-ui` target.
- Call `ti_washing_machine_init(const char * asset_path)` once, after `lv_init()` and after the
  display exists. `asset_path` is the LVGL filesystem drive prefix (`"A:"` in the simulator). It
  calls the generated `ti_washing_machine_init_gen()` first, which creates all subjects.
- Write: `lv_subject_set_int(&subject_progress, 42);` /
  `lv_subject_copy_string(&subject_hint_str, "Close the door");`
- Read: `lv_subject_get_int(&subject_door);`
- React: `lv_subject_add_observer(&subject_x, cb, NULL)`, or group several with
  `lv_subject_init_group()` — see `wash_logic_init()`.
- Firmware is expected to own: door GPIO → `subject_door`, water level sensor → `subject_water_level`,
  motor PWM → `subject_pwm_duty` / `subject_spin_rpm`, heater and pump outputs →
  `subject_heater` / `subject_pump`, cycle progress → `subject_progress`.
- Edit: `screens/*.xml`, `components/**/*.xml`, `globals.xml`, `custom/*`, `ti_washing_machine.c`,
  `user_config.cmake`, `sim/main.c`. Never edit: `*_gen.c/h`, `file_list_gen.cmake`,
  `CMakeLists.txt`, `images/*_data.c`, `fonts/*_data.c`.

## Layout

- `screens/` — the four screen XML files.
- `components/` — 23 reusable components; `components/base/` holds the thin wrappers (row, column, label, container, slider).
- `globals.xml` — colours, spacing consts, `ref_*` enums, fonts, images, subjects, styles.
- `custom/` — hand-written C the UI depends on.
- `images/` — PNG sources. The rotating drum load (`load.png`, 114x114) must stay circular and inscribed in a square canvas: LVGL has no arbitrary masking, so the round artwork is its own mask and a spinning circle can never spill past the door opening. Drum images are used at native size and must never be scaled.
- `fonts/` — Gabarito `.ttf` faces; sizes and glyph ranges declared in `globals.xml`.
- `sim/` — desktop simulator (SDL2 on Linux/macOS, Win32 on Windows). See `sim/README.md`.
- `tools/` — `iconset.js`, the icon manifest for the generator in `../tools/`.
- `docs/` — rendered screenshots of each screen.

The panel is dense (480x320 across roughly 3.5 inches): `const_touch` fixes a 58px minimum touch
target and no type is under 13px — respect both when adding controls.
