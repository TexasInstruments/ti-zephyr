/**
 * @file thermostat_logic.h
 *
 * Auto-mode changeover — the only application logic here that XML cannot
 * express, and the module is kept down to it.
 *
 * What forces C: `bind_*_if_eq/lt/ge/...` compares a subject against a LITERAL
 * ref_value, never against another subject, and nothing in XML writes a subject
 * as a consequence of another subject changing. Auto changeover is target_temp
 * vs now_temp — two subjects — so no comparator can see it.
 *
 * Text formatting does NOT force C: bind_text-fmt takes a full printf template,
 * and a float subject formats with "%.1f" natively.
 *
 * A plain C module, not a project widget: threshold logic is not widget
 * behaviour. Wired through user_config.cmake, called after the generated init.
 * It is the ONLY writer of subject_hvac_state — keep it that way.
 */

#ifndef THERMOSTAT_LOGIC_H
#define THERMOSTAT_LOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Attach the observer that keeps subject_hvac_state in step with the mode, the
 * setpoint and the measured temperature, then run it once so the UI is correct
 * on its first frame. Call after the generated init.
 */
void thermostat_logic_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*THERMOSTAT_LOGIC_H*/
