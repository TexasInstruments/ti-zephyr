/* wash_logic - derives what the XML cannot: phase, remaining time, readiness.
 * These enums are mirrored by hand as ref_* consts in globals.xml. */
#ifndef WASH_LOGIC_H
#define WASH_LOGIC_H

#define WASH_STATE_IDLE      0
#define WASH_STATE_RUNNING   1
#define WASH_STATE_PAUSED    2
#define WASH_STATE_COMPLETE  3

#define WASH_PROG_COTTON     0
#define WASH_PROG_ECO        1
#define WASH_PROG_SYNTH      2
#define WASH_PROG_QUICK      3

#define WASH_DOOR_OPEN       0
#define WASH_DOOR_LOCKED     1

#define WASH_PHASE_PREWASH   0
#define WASH_PHASE_WASH      1
#define WASH_PHASE_RINSE     2
#define WASH_PHASE_SPIN      3

/** Register the observers. Call after washing_machine_init_gen(). */
void wash_logic_init(void);

#endif /* WASH_LOGIC_H */
