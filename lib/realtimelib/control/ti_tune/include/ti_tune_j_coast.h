// #############################################################################
//
//! @file   ti_tune_j_coast.h
//! @brief  ARM M33 J_B identification library private header
//!         (floating point)
//!
//! This header contains the internal structure definitions for the titune
//! library. This file should NOT be included by user applications.
//! It is only for internal library use.
//
// #############################################################################
/*
 * Copyright (C) Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free,
 * non-exclusive license under copyrights and patents it now or hereafter
 * owns or controls to make, have made, use, import, offer to sell and
 * sell ("Utilize") this software subject to the terms herein. With
 * respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the
 * software alone. The patent license shall not apply to any combinations
 * which include this software, other than combinations with devices
 * manufactured by or for TI ("TI Devices"). No hardware patent is
 * licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce
 * this license (including the above copyright notice and the disclaimer
 * and (if applicable) source code license limitations below) in the
 * documentation and/or other materials provided with the distribution.
 *
 * Redistribution and use in binary form, without modification, are
 * permitted provided that the following conditions are met:
 *   - No reverse engineering, decompilation, or disassembly of this
 *     software is permitted with respect to any software provided in
 *     binary form.
 *   - Any redistribution and use are licensed by TI for use only with
 *     TI Devices.
 *   - Nothing shall obligate TI to provide you with source code for the
 *     software licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and
 * redistribution of the source code are permitted provided that the
 * following conditions are met:
 *   - any redistribution and use of the source code, including any
 *     resulting derivative works, are licensed by TI for use only with
 *     TI Devices.
 *   - any redistribution and use of any object code compiled from the
 *     source code and any resulting derivative works, are licensed by TI
 *     for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its suppliers may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI'S LICENSORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// #############################################################################

/**
 * \file ti_tune_j_coast.h
 * \brief Inertia and damping estimation using steady-state + coast-down method
 *
 * \defgroup TITUNE_J_COAST TI Tune J Coast Module
 * \brief Robust inertia (J) and damping (B) identification for high-damping systems
 *
 * This module implements a two-step identification method:
 * 1. Steady-state torque-speed curve → estimates damping coefficient B
 * 2. Coast-down exponential decay → estimates inertia J
 *
 * Advantages over frequency sweep:
 * - Works with high-damping systems (fans, pumps)
 * - Robust to noise and weak speed response
 * - Adaptive current ramping (safe for unknown loads)
 * - Separates friction and damping
 *
 * @{
 */

#ifndef TI_TUNE_J_COAST_H_
#define TI_TUNE_J_COAST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include "math_types.h"

//*****************************************************************************
//
//! \name Configuration Parameters
//! @{
//
//*****************************************************************************

//! \brief Maximum coast-down duration (seconds) - safety timeout if speed never reaches stop threshold
#define TI_TUNE_J_COAST_MAX_COASTDOWN_SEC (30.0f)

//! \brief Maximum coast-down regression duration (seconds) - caps the number of regression samples
#define TI_TUNE_J_COAST_MAX_COASTDOWN_REGRESSION_SEC (10.0f)
//! @}

//*****************************************************************************
//
//! \name State Machine States
//! @{
//
//*****************************************************************************

/**
 * \brief State machine states for J and B estimation
 */
typedef enum
{
    TI_TUNE_J_COAST_STATE_IDLE = 0,            //!< Idle state
    TI_TUNE_J_COAST_STATE_INITIAL_SETTLE,      //!< Settle IIR filters at CL entry current before first capture
    TI_TUNE_J_COAST_STATE_INITIAL_CAPTURE,     //!< Capture point 0: CL entry operating point (Iq, omega_e, Vq)
    TI_TUNE_J_COAST_STATE_RAMP_CURRENT,        //!< Ramp Iq upward; monitor filtered speed for step trigger
    TI_TUNE_J_COAST_STATE_SPEED_STEP_SETTLE,   //!< Hold current constant; run IIR filters for settleTarget ticks
    TI_TUNE_J_COAST_STATE_SPEED_STEP_CAPTURE,  //!< Accumulate G_DC and Ke regression data at current speed step
    TI_TUNE_J_COAST_STATE_SS_CALC,             //!< Compute G_DC and Ke from accumulated regression sums
    TI_TUNE_J_COAST_STATE_COASTDOWN_SAMPLE,    //!< Coast-down: sample speed decay (iqRef = 0)
    TI_TUNE_J_COAST_STATE_COASTDOWN_CALC,      //!< Coast-down: calculate tau from exponential fit
    TI_TUNE_J_COAST_STATE_CALC_PI_GAINS,       //!< Calculate speed PI gains from G_DC and tau
    TI_TUNE_J_COAST_STATE_DONE,                //!< Estimation complete
    TI_TUNE_J_COAST_STATE_ERROR                //!< Error state
} TiTune_J_Coast_State_e;

//! @}

//*****************************************************************************
//
//! \name Data Structures
//! @{
//
//*****************************************************************************

/**
 * \brief Inertia and damping estimation object
 */
typedef struct _TiTune_J_Coast_Obj_
{
    /* State machine */
    TiTune_J_Coast_State_e state;         //!< Current state
    bool                   variableInit;  //!< Variable initialization flag
    bool                   finish;        //!< Estimation complete flag
    bool                   error;         //!< Error flag

    /* Configuration parameters - populated from user macros in TiTune_J_Coast_setParams() */
    float32_t samplePeriod_sec;       //!< Sample period in seconds
    float32_t currentRampRate_pu;     //!< Current ramp rate (per-unit/sample) during RAMP_CURRENT
    float32_t coastdownStopFraction;  //!< Coast-down stop threshold (fraction of initial speed, e.g. 0.2 = 20%)
    uint32_t  maxSteadyStatePoints;   //!< Maximum SS data points (point 0 + numSteps ramp captures)
    float32_t crossoverFreqFactor;    //!< Crossover frequency factor (fraction of plant pole)
    float32_t targetSpeed_pu;         //!< Target speed for ramp exit (per-unit of F_Base)
    float32_t maxCurrent_pu;          //!< Maximum current limit for ramp exit (per-unit of I_Base)
    float32_t speedStepThreshold_pu;  //!< Computed speed step size (per-unit): (targetSpeed - initialSpeed) / numSteps
    uint32_t  numSteps;               //!< Number of speed steps to capture during upward ramp

    /* IIR filter coefficients for INITIAL_SETTLE and SPEED_STEP_SETTLE phases (computed from user cutoff frequencies)
     */
    float32_t lpfGain_Iq;     //!< IIR coefficient α for Iq and Vq filter (electrical domain)
    float32_t lpfGain_Speed;  //!< IIR coefficient α for speed filter (mechanical domain)

    /* IIR filter states - initialized bumplessly at variableInit, run continuously during ramp */
    float32_t iqFiltered_pu;     //!< IIR-filtered Iq reference (per-unit)
    float32_t speedFiltered_pu;  //!< IIR-filtered speed feedback (per-unit)
    float32_t vqFiltered_pu;     //!< IIR-filtered Vq feedback (per-unit)

    /* Base values for per-unit conversion */
    float32_t I_Base;               //!< Current base value (A)
    float32_t F_Base;               //!< Frequency base value (Hz, electrical)
    float32_t Vph_Base;             //!< Phase voltage base value (V)
    float32_t Rs_ohm;               //!< Stator resistance (Ohms) from Rs estimation
    float32_t speedCtrlPeriod_sec;  //!< Speed control loop period (sec)

    /* Steady-state measurement - online regression accumulators (no sample arrays needed) */
    uint32_t numSteadyStatePoints;  //!< Number of valid SS data points accumulated
    uint32_t settleCounter;         //!< Settling time counter
    uint32_t settleTarget;          //!< Settling time target (samples)

    /* G_DC regression accumulators: ωe = G_DC·Iq + offset */
    float32_t ssSum_Iq_A;        //!< Σ Iq_k  (A)
    float32_t ssSum_OmegaE_rps;  //!< Σ ωe_k  (rad_e/s)
    float32_t ssSum_IqOmegaE;    //!< Σ Iq_k · ωe_k  (A · rad_e/s)
    float32_t ssSum_Iq2;         //!< Σ Iq_k²  (A²)

    /* Ke (λ_PM) accumulators: λ_PM = (Vq - Rs·Iq) / ωe */
    float32_t ssSum_LambdaPM;  //!< Σ λ_PM_k  (V·s/rad_e)
    uint32_t  ssCount_Ke;      //!< Number of valid Ke samples (ωe > 1 rad_e/s)

    /* Upward ramp tracking variables */
    float32_t initialSpeed_pu;        //!< CL entry speed (per-unit) - used to compute speedStepThreshold_pu
    float32_t previousValidSpeed_pu;  //!< Speed at last captured data point (per-unit)
    uint32_t  validPointsCollected;   //!< Number of valid data points collected (including point 0)

    /* Coast-down measurement - online regression accumulators (no sample buffer needed) */
    uint32_t  maxCoastdownTicks;              //!< Safety timeout (ticks): computed from samplePeriod_sec at init
    uint32_t  maxCoastdownRegressionSamples;  //!< Max regression samples cap: computed from samplePeriod_sec at init
    uint32_t  coastdownSampleCounter;         //!< Sub-sample decimation counter
    uint32_t  coastdownSamplePeriod;          //!< Decimation period (samples between regression updates)
    uint32_t  coastdownTickCounter;           //!< Total ticks elapsed since coast-down start (safety timeout)
    uint32_t  numCoastdownSamples;            //!< Number of regression samples accumulated (for validity check)
    float32_t coastdownSumT;                  //!< Σ t_k  (sec)
    float32_t coastdownSumLnW;                //!< Σ ln(ω_k)
    float32_t coastdownSumTLnW;               //!< Σ t_k · ln(ω_k)
    float32_t coastdownSumT2;                 //!< Σ t_k²
    float32_t coastdownInitialSpeed_pu;       //!< Initial speed for coast-down (recorded in SS_CALC)
    float32_t coastdownStopSpeed_pu;          //!< Stop speed threshold (coastdownInitialSpeed * coastdownStopFraction)

    /* Estimation results */
    float32_t G_DC;           //!< DC plant gain: Δωe/ΔIq (rad_e/s per A)
    float32_t G_DC_pu;        //!< DC plant gain in per-unit
    float32_t tau_estimated;  //!< Estimated time constant (sec) from coast-down
    float32_t omega_p_rps;    //!< Plant pole frequency B/J (rad/sec)
    float32_t lambda_PM;      //!< Estimated back-EMF constant λ_PM (V·s/rad_e) - diagnostic
    float32_t Kp_spd;         //!< Speed PI proportional gain
    float32_t Ki_spd;         //!< Speed PI integral gain

    /* Runtime inputs/outputs */
    float32_t speedFbk_pu;  //!< Speed feedback input (per-unit)
    float32_t vqFbk_pu;     //!< Vq feedback input (per-unit of Vph_Base)
    float32_t iqRef_pu;     //!< Iq reference output (per-unit)

} TiTune_J_Coast_Obj;

/**
 * \brief Handle to inertia and damping estimation object
 */
typedef struct _TiTune_J_Coast_Obj_ *TiTune_J_Coast_Handle;

//! @}

//*****************************************************************************
//
//! \name API Functions
//! @{
//
//*****************************************************************************

/**
 * \brief Initialize the inertia and damping estimation module
 *
 * \param[in] pMemory      Pointer to memory for object storage
 * \param[in] numBytes     Size of memory block in bytes
 *
 * \return Handle to initialized object, or NULL if insufficient memory
 */
extern TiTune_J_Coast_Handle TiTune_J_Coast_init(void *pMemory, const size_t numBytes);

/**
 * \brief Run the estimation state machine (call every sample period)
 *
 * \param[in] handle           Handle to estimation object
 * \param[in] speedFbk_pu      Speed feedback in per-unit (electrical)
 * \param[in] initialCurrent_pu Initial Iq current for bumpless transfer (per-unit)
 * \param[in] vqFbk_pu         Vq voltage feedback in per-unit (of Vph_Base)
 */
extern void TiTune_J_Coast_run(TiTune_J_Coast_Handle handle, const float32_t speedFbk_pu,
                               const float32_t initialCurrent_pu, const float32_t vqFbk_pu);

/**
 * \brief Get Iq reference output
 *
 * \param[in] handle Handle to estimation object
 *
 * \return Iq reference in per-unit
 */
static inline float32_t TiTune_J_Coast_getIqRef(TiTune_J_Coast_Handle handle)
{
    TiTune_J_Coast_Obj *obj = (TiTune_J_Coast_Obj *)handle;
    return obj->iqRef_pu;
}

/**
 * \brief Check if estimation is complete
 *
 * \param[in] handle Handle to estimation object
 *
 * \return true if estimation complete, false otherwise
 */
static inline bool TiTune_J_Coast_isFinished(TiTune_J_Coast_Handle handle)
{
    TiTune_J_Coast_Obj *obj = (TiTune_J_Coast_Obj *)handle;
    return obj->finish;
}

/**
 * \brief Get DC plant gain G_DC
 *
 * \param[in] handle Handle to estimation object
 *
 * \return DC plant gain in (rad_e/s) / A
 */
static inline float32_t TiTune_J_Coast_getGDC(TiTune_J_Coast_Handle handle)
{
    TiTune_J_Coast_Obj *obj = (TiTune_J_Coast_Obj *)handle;
    return obj->G_DC;
}

/**
 * \brief Get estimated time constant τ
 *
 * \param[in] handle Handle to estimation object
 *
 * \return Time constant τ in seconds
 */
static inline float32_t TiTune_J_Coast_getTau(TiTune_J_Coast_Handle handle)
{
    TiTune_J_Coast_Obj *obj = (TiTune_J_Coast_Obj *)handle;
    return obj->tau_estimated;
}

/**
 * \brief Get estimated back-EMF constant λ_PM (diagnostic)
 *
 * \param[in] handle Handle to estimation object
 *
 * \return λ_PM in V·s/rad_e
 */
static inline float32_t TiTune_J_Coast_getLambdaPM(TiTune_J_Coast_Handle handle)
{
    TiTune_J_Coast_Obj *obj = (TiTune_J_Coast_Obj *)handle;
    return obj->lambda_PM;
}

/**
 * \brief Get speed PI proportional gain
 *
 * \param[in] handle Handle to estimation object
 *
 * \return Kp gain (dimensionless in per-unit)
 */
static inline float32_t TiTune_J_Coast_getKp(TiTune_J_Coast_Handle handle)
{
    TiTune_J_Coast_Obj *obj = (TiTune_J_Coast_Obj *)handle;
    return obj->Kp_spd;
}

/**
 * \brief Get speed PI integral gain
 *
 * \param[in] handle Handle to estimation object
 *
 * \return Ki gain (dimensionless in per-unit)
 */
static inline float32_t TiTune_J_Coast_getKi(TiTune_J_Coast_Handle handle)
{
    TiTune_J_Coast_Obj *obj = (TiTune_J_Coast_Obj *)handle;
    return obj->Ki_spd;
}

//! @}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* TI_TUNE_J_COAST_H_ */
