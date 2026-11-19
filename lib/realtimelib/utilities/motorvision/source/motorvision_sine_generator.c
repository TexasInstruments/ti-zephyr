/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 */

/**
 * @file motorvision_sine_generator.c
 * @brief Sine wave generator implementation for MotorVision streaming demo
 */
#include "motorvision_sine_generator.h"

// ============================================================================
// GLOBAL VARIABLE DEFINITIONS
// ============================================================================

// Sine wave parameters (configurable via MotorVision GUI)
volatile float amp1    = 30.0f;
volatile float offset1 = 50.0f;
volatile float freq1   = 0.01f;

// Generated sine waves (read by MotorVision)
volatile float sine_wave1 = 0.0f;
volatile float sine_wave2 = 0.0f;
volatile float sine_wave3 = 0.0f;
volatile float sine_wave4 = 0.0f;

volatile float rw_var_1 = 0.0f;
volatile float rw_var_2 = 0.0f;

volatile uint32_t rw_var_3 = 0;

volatile int32_t rw_var_4 = 0;

volatile int16_t rw_var_5 = 0;

volatile uint16_t rw_var_6 = 0;

// volatile bool rw_var_7=0;

// ISR counter
volatile uint32_t SM_isrCounter = 0;

// ============================================================================
// FUNCTION IMPLEMENTATIONS
// ============================================================================

/**
 * @brief Initialize sine wave generator
 */
void SineGen_init(void)
{
    // Set default parameters
    amp1    = 30.0f;
    offset1 = 50.0f;
    freq1   = 0.01f;

    // Reset counter and waves
    SM_isrCounter = 0;
    sine_wave1    = 0.0f;
    sine_wave2    = 0.0f;
    sine_wave3    = 0.0f;
    sine_wave4    = 0.0f;

    rw_var_1 = 0.0f;
    rw_var_2 = 0.0f;

    rw_var_3 = 0;

    rw_var_4 = 0;

    rw_var_5 = -10;

    rw_var_6 = 0;

    // rw_var_7=0;
}

/**
 * @brief Generate 4 sine waves based on current parameters
 *
 * Call this from ISR to update sine wave values every sample.
 */
void SineGen_update()
{
    // Increment ISR counter and wrap at 10,000,000
    SM_isrCounter++;
    if (SM_isrCounter >= 10000000)
    {
        SM_isrCounter = 0;
    }

    // Calculate sine value once (optimization)
    float sine_val = sinf((float)SM_isrCounter * freq1);

    // Generate 4 sine waves with different amplitudes and phases
    // Wave 1: offset1 - 2*amp1*sin(counter*freq1)
    sine_wave1 = (offset1 - 2.0f * amp1 * sine_val) / 100.0f;

    // sine_wave1=systemVars.speedRef_Hz+100.0f;

    // sine_wave1 = (float)SM_isrCounter;

    // Wave 2: offset1 + 2*amp1*sin(counter*freq1)
    sine_wave2 = (offset1 + 2.0f * amp1 * sine_val) / 100.0f;

    // sine_wave2 = (float)SM_isrCounter;

    // Wave 3: offset1 - amp1*sin(counter*freq1)
    sine_wave3 = (offset1 - amp1 * sine_val) / 100.0f;

    // sine_wave3 = (float)SM_isrCounter;

    // Wave 4: offset1 + amp1*sin(counter*freq1)
    sine_wave4 = (offset1 + amp1 * sine_val) / 100.0f;

    sine_wave4 = (float)SM_isrCounter;

    // rw_var_1=100.0f;

    // rw_var_2=200.0f;

    // rw_var_3=200;
}
