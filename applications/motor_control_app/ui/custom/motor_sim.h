/**
 * @file motor_sim.h
 *
 * Stand-in for the LP-AM13E230 drive. Publishes measured quantities as subjects.
 */
#ifndef MOTOR_SIM_H
#define MOTOR_SIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../motor_control.h"

/** Starts the 100 ms drive model. Call after init_gen. */
void motor_sim_init(void);

/**
 * The hardware seam. On a real board the control loop calls this instead of the
 * simulated model running - pass measured values and the UI updates itself.
 *
 * @param rpm        measured shaft speed, whole RPM
 * @param milliamps  phase current in mA (kept integer; the display string is
 *                   formatted here, since bind_text-fmt has no fixed-point form)
 * @param temp_c     power-stage temperature in whole degrees C
 * @param load_pct   torque demand, 0-100
 */
void motor_sim_publish(int32_t rpm, int32_t milliamps, int32_t temp_c, int32_t load_pct);

/** Latches a fault: fills the count, raises the flag and stops the drive. */
void motor_sim_trip(void);

#ifdef __cplusplus
}
#endif
#endif /*MOTOR_SIM_H*/
