/* wash_sim - runs the demo cycle on a timer and owns the drum's rotation. */
#ifndef WASH_SIM_H
#define WASH_SIM_H

#define WASH_SIM_DURATION_MS   8000

#define WASH_SIM_HOLD_MS       2000

/** Register the module. Call after washing_machine_init_gen(). */
void wash_sim_init(void);

/** Start (or restart) a cycle. Wired to the cycle screen's screen_load_start. */
void wash_sim_start(void);

/** Stop immediately and leave the machine where it is. */
void wash_sim_stop(void);

#endif /* WASH_SIM_H */
