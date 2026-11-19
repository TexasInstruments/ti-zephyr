/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 */

#ifndef MOTORVISION_SINE_GENERATOR_H
#define MOTORVISION_SINE_GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <math.h>

// ============================================================================
// SINE WAVE PARAMETERS (Configurable via MotorVision GUI)
// ============================================================================

extern volatile float amp1;     // Amplitude (default: 30.0f)
extern volatile float offset1;  // Offset (default: 50.0f)
extern volatile float freq1;    // Frequency (default: 0.01f)

// ============================================================================
// GENERATED SINE WAVES (Read by MotorVision)
// ============================================================================

extern volatile float sine_wave1;  // Wave 1: offset1 - 2*amp1*sin(counter*freq1)
extern volatile float sine_wave2;  // Wave 2: offset1 + 2*amp1*sin(counter*freq1)
extern volatile float sine_wave3;  // Wave 3: offset1 - amp1*sin(counter*freq1)
extern volatile float sine_wave4;  // Wave 4: offset1 + amp1*sin(counter*freq1)

extern volatile float rw_var_1;

extern volatile float rw_var_2;

extern volatile uint32_t rw_var_3;

extern volatile int32_t rw_var_4;

extern volatile int16_t rw_var_5;

extern volatile uint16_t rw_var_6;

// extern volatile bool rw_var_7;
// ============================================================================
// ISR COUNTER
// ============================================================================

extern volatile uint32_t SM_isrCounter;  // ISR execution counter

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

/**
 * @brief Initialize sine wave generator
 *
 * Sets default values for amp1, offset1, freq1 and resets counter
 */
void SineGen_init(void);

/**
 * @brief Generate 4 sine waves based on current parameters
 *
 * This function should be called from the ISR to update sine wave values.
 * It increments the ISR counter and calculates 4 sine waves using the
 * current values of amp1, offset1, and freq1.
 *
 * The generated waves are stored in sine_wave1/2/3/4 variables which
 * are mapped to MotorVision channels A/B/C/D.
 */
void SineGen_update(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTORVISION_SINE_GENERATOR_H */
