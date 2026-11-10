// #############################################################################
//
//! @file   ti_tune_ol.h
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
 * \file ti_tune_ol.h
 * \brief Open-loop startup module for TI Tune motor parameter identification
 *
 * \defgroup TITUNE_OL TI Tune Open-Loop Startup Module
 * \brief Angle blending and current profiling for OL-to-CL handoff
 *
 * This module encapsulates the proprietary open-loop startup algorithm:
 * - Phase 1: Speed ramp-up with angle integration
 * - Phase 2: Multiplicative current ramp-down with cos(angleDelta) weighting
 * - OL-to-CL handoff when angle converges or timeout expires
 *
 * @{
 */

#ifndef TI_TUNE_OL_H_
#define TI_TUNE_OL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>
#include "math_types.h"

//*****************************************************************************
//
//! \name Data Structures
//! @{
//
//*****************************************************************************

/**
 * \brief Open-loop startup object
 *
 * \details Contains all state for the two-phase open-loop startup:
 *          Phase 1 ramps speed to target; Phase 2 reduces current while
 *          waiting for the observer angle to converge to the OL angle.
 *          All fields are internal -- access via getters only.
 */
typedef struct _TiTune_OL_Obj_
{
    /* Configuration -- set by TiTune_OL_setParams() */
    float32_t ratedCurrent_A;             /*!< Motor rated current (A) */
    float32_t ratedSpeed_Hz;              /*!< Motor rated electrical speed (Hz) */
    float32_t samplePeriod_sec;           /*!< ISR sample period (s) */
    float32_t speedTargetFraction;        /*!< OL target speed as fraction of rated */
    float32_t speedRamp_HzPerSec;         /*!< Speed ramp rate (Hz/s) */
    float32_t currentFraction;            /*!< OL Iq as fraction of rated current */
    float32_t idFraction;                 /*!< OL Id injection as fraction of rated */
    float32_t rampDownInterval_sec;       /*!< Time between current ramp-down steps (s) */
    float32_t angleConvergeThreshold_rad; /*!< |angleDelta| threshold for handoff (rad) */
    uint32_t  maxRampDownSteps;           /*!< Timeout: max ramp-down steps before forced handoff */
    float32_t rampFactorOffset;           /*!< Additive offset in cos(angleDelta) ramp factor */
    float32_t rampFactorScale;            /*!< Scale on |cos(angleDelta)| in ramp factor */

    /* Runtime state -- internal */
    float32_t Angle_rad;        /*!< Current OL angle integrator state (rad) */
    float32_t angleDelta_rad;   /*!< OL angle minus observer angle (rad) */
    float32_t Speed_Hz;         /*!< Current OL speed (Hz) */
    float32_t SpeedTarget_Hz;   /*!< Computed OL target speed (Hz) */
    float32_t SpeedRamp_Hz;     /*!< Speed ramp increment per sample (Hz/sample) */
    float32_t AngleStep_rad;    /*!< Angle increment per sample (rad/sample) */
    float32_t Current_A;        /*!< Current OL Iq command (A) */
    float32_t IdRef_A;          /*!< OL Id injection (A) */
    uint32_t  count;            /*!< Interval counter for ramp-down steps */
    uint32_t  rampDownSteps;    /*!< Number of ramp-down steps applied */
    uint32_t  rampDownInterval; /*!< Ramp-down interval in samples */

    /* Outputs -- read via getters */
    float32_t outFOC_Angle_rad; /*!< FOC angle to use this cycle (rad) */
    float32_t outIdRef_A;       /*!< Id reference this cycle (A) */
    float32_t outIqRef_A;       /*!< Iq reference this cycle (A) */
    float32_t outSpeed_Hz;      /*!< Speed reference for observer (Hz) */
    bool      done;             /*!< true when OL-to-CL handoff is complete */

    /* Handoff values -- valid only when done == true */
    float32_t handoffIqRef_A; /*!< Iq at handoff moment (A) -- init CL.IqRef_A */
    float32_t handoffIdRef_A; /*!< Id at handoff moment (A) -- init CL.IdRef_A */

} TiTune_OL_Obj;

/**
 * \brief Handle to open-loop startup object
 */
typedef struct _TiTune_OL_Obj_ *TiTune_OL_Handle;

//! @}

//*****************************************************************************
//
//! \name API Functions
//! @{
//
//*****************************************************************************

/**
 * \brief Initialize the open-loop startup module
 *
 * \param[in] pMemory   Pointer to memory for object storage
 * \param[in] numBytes  Size of memory block in bytes
 *
 * \return Handle to initialized object, or NULL if insufficient memory
 */
extern TiTune_OL_Handle TiTune_OL_init(void *pMemory, size_t numBytes);

/**
 * \brief Set parameters for the open-loop startup module
 *
 * \param[in] handle              Handle to OL object
 * \param[in] ratedCurrent_A      Motor rated current (A)
 * \param[in] ratedSpeed_Hz       Motor rated electrical speed (Hz)
 * \param[in] initialAngle_rad    Starting angle (rad) -- typically from Rs IPD
 * \param[in] samplePeriod_sec    ISR sample period (s)
 */
extern void TiTune_OL_setParams(TiTune_OL_Handle handle, float32_t ratedCurrent_A, float32_t ratedSpeed_Hz,
                                float32_t initialAngle_rad, float32_t samplePeriod_sec);

/**
 * \brief Run the open-loop startup algorithm (call every ISR cycle)
 *
 * \details Phase 1: ramps speed to target, advances OL angle.
 *          Phase 2: reduces Iq multiplicatively using cos(angleDelta) weighting
 *          until angle converges or timeout expires, then sets done = true.
 *
 * \param[in] handle             Handle to OL object
 * \param[in] observerAngle_rad  Current observer estimated angle (rad)
 */
extern void TiTune_OL_run(TiTune_OL_Handle handle, float32_t observerAngle_rad);

/**
 * \brief Get the FOC angle to use this cycle
 *
 * \param[in] handle Handle to OL object
 * \return FOC angle (rad)
 */
static inline float32_t TiTune_OL_getFOCAngle(TiTune_OL_Handle handle)
{
    TiTune_OL_Obj *obj = (TiTune_OL_Obj *)handle;
    return obj->outFOC_Angle_rad;
}

/**
 * \brief Get the Id reference this cycle
 *
 * \param[in] handle Handle to OL object
 * \return Id reference (A)
 */
static inline float32_t TiTune_OL_getIdRef(TiTune_OL_Handle handle)
{
    TiTune_OL_Obj *obj = (TiTune_OL_Obj *)handle;
    return obj->outIdRef_A;
}

/**
 * \brief Get the Iq reference this cycle
 *
 * \param[in] handle Handle to OL object
 * \return Iq reference (A)
 */
static inline float32_t TiTune_OL_getIqRef(TiTune_OL_Handle handle)
{
    TiTune_OL_Obj *obj = (TiTune_OL_Obj *)handle;
    return obj->outIqRef_A;
}

/**
 * \brief Get the speed reference output for the observer
 *
 * \param[in] handle Handle to OL object
 * \return Speed reference (Hz)
 */
static inline float32_t TiTune_OL_getSpeed(TiTune_OL_Handle handle)
{
    TiTune_OL_Obj *obj = (TiTune_OL_Obj *)handle;
    return obj->outSpeed_Hz;
}

/**
 * \brief Check if OL-to-CL handoff is complete
 *
 * \param[in] handle Handle to OL object
 * \return true when handoff is complete and CL can begin
 */
static inline bool TiTune_OL_isDone(TiTune_OL_Handle handle)
{
    TiTune_OL_Obj *obj = (TiTune_OL_Obj *)handle;
    return obj->done;
}

/**
 * \brief Get the Iq reference at the OL-to-CL handoff moment
 *
 * \details Valid only after TiTune_OL_isDone() returns true.
 *          Use to initialize CL.IqRef_A for bumpless transfer.
 *
 * \param[in] handle Handle to OL object
 * \return Iq at handoff (A)
 */
static inline float32_t TiTune_OL_getHandoffIqRef(TiTune_OL_Handle handle)
{
    TiTune_OL_Obj *obj = (TiTune_OL_Obj *)handle;
    return obj->handoffIqRef_A;
}

/**
 * \brief Get the Id reference at the OL-to-CL handoff moment
 *
 * \details Valid only after TiTune_OL_isDone() returns true.
 *          Use to initialize CL.IdRef_A for bumpless transfer.
 *
 * \param[in] handle Handle to OL object
 * \return Id at handoff (A)
 */
static inline float32_t TiTune_OL_getHandoffIdRef(TiTune_OL_Handle handle)
{
    TiTune_OL_Obj *obj = (TiTune_OL_Obj *)handle;
    return obj->handoffIdRef_A;
}

//! @}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* TI_TUNE_OL_H_ */
