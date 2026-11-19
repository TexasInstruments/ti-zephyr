// #############################################################################
//
//  FILE:   flux_obs.h
//
//  TITLE:  ARM M33 Flux Observer library - PUBLIC API
//          (floating point)
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
 * sell ("Utilize") this software subject to the terms herein.
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

#ifndef FLUX_OBS_H
#define FLUX_OBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "types.h"

//*****************************************************************************
//
//! \defgroup FLUX_OBS_API Flux Observer API
//! \brief Public API for Flux Observer Library
//! @{
//
//*****************************************************************************

/* ============================================================================
 * MEMORY SIZE CONSTANT
 * ============================================================================ */

//! \brief Compile-time memory size for one FLUX_OBS instance (bytes).
//!
//! Use this to declare a static buffer per motor instance:
//! \code
//!   static uint8_t fluxObs_M1_mem[FLUX_OBS_MEM_SIZE_BYTES];
//!   static uint8_t fluxObs_M2_mem[FLUX_OBS_MEM_SIZE_BYTES];
//!   obj->fluxObsHandle = FLUX_OBS_init(fluxObs_M1_mem, FLUX_OBS_MEM_SIZE_BYTES);
//! \endcode
//!
//! If the buffer is too small, FLUX_OBS_init() returns NULL.
//! Call FLUX_OBS_getMemorySize() at runtime to get the required size.
//!
#define FLUX_OBS_MEM_SIZE_BYTES (164U)

/* ============================================================================
 * OPAQUE HANDLE - Implementation hidden from user
 * ============================================================================ */

//! \brief Opaque handle to flux observer instance
//!
typedef struct _FLUX_OBS_Obj_ *FLUX_OBS_Handle;

/* ============================================================================
 * INPUT STRUCTURES - User provides these
 * ============================================================================ */

//! \brief Alpha-beta voltage input structure
//!
typedef struct
{
    float32_t alpha_V;  //!< Alpha-axis voltage (V)
    float32_t beta_V;   //!< Beta-axis voltage (V)
} FLUX_OBS_VoltageInput_t;

//! \brief Alpha-beta current input structure
//!
typedef struct
{
    float32_t alpha_A;  //!< Alpha-axis current (A)
    float32_t beta_A;   //!< Beta-axis current (A)
} FLUX_OBS_CurrentInput_t;

/* ============================================================================
 * OUTPUT STRUCTURES - Library provides these
 * ============================================================================ */

//! \brief Flux observer output structure
//!
typedef struct
{
    float32_t angle_rad;        //!< Estimated flux angle (rad, [-pi, pi])
    float32_t speed_Hz;         //!< Doubly-filtered speed (Hz)
    float32_t speed_rad_per_s;  //!< Doubly-filtered speed (rad/s)
} FLUX_OBS_Output_t;

/* ============================================================================
 * INITIALIZATION PARAMETERS
 * ============================================================================ */

//! \brief Flux observer initialization parameters
//!
typedef struct
{
    float32_t ctrlPeriod_sec;      //!< Control period (s)
    float32_t motor_Rs_Ohm;        //!< Stator resistance (Ohm)
    float32_t motor_Ls_d_H;        //!< Stator inductance (H)
    float32_t motor_ratedFlux_Wb;  //!< Rated flux linkage (Wb)
} FLUX_OBS_InitParams_t;

/* ============================================================================
 * TUNING PARAMETERS
 * ============================================================================ */

//! \brief Flux observer tuning parameters
//!
typedef struct
{
    float32_t invKe_max;           //!< Max InvKe limit
    float32_t invKe_min;           //!< Min InvKe limit
    float32_t bemfFiltFactor;      //!< BEMF filter tuning (default 0.1)
    float32_t speedFiltFactor;     //!< Speed filter tuning (default 0.1)
    float32_t lpfCutoff_Hz;        //!< Speed LPF cutoff (Hz)
    bool      inductorFiltEnable;  //!< Enable DQ inductive drop filter
    bool      trapIntDisable;      //!< Use rectangular integration
} FLUX_OBS_TuningParams_t;

/* ============================================================================
 * PUBLIC API FUNCTIONS
 * ============================================================================ */

//*****************************************************************************
//
//! \brief Initialize flux observer instance
//!
//! \param[in] pMemory   Pointer to memory for the object
//! \param[in] numBytes  Number of bytes allocated
//!
//! \return    FLUX_OBS handle, or NULL if initialization failed
//!
//*****************************************************************************
extern FLUX_OBS_Handle FLUX_OBS_init(void *pMemory, const size_t numBytes);

//*****************************************************************************
//
//! \brief Get required memory size for flux observer object
//!
//! \return    Size in bytes required for FLUX_OBS_Obj
//!
//*****************************************************************************
extern size_t FLUX_OBS_getMemorySize(void);

//*****************************************************************************
//
//! \brief Set core motor parameters
//!
//! \param[in] handle   FLUX_OBS handle
//! \param[in] pParams  Pointer to initialization parameters
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_setParams(FLUX_OBS_Handle handle, const FLUX_OBS_InitParams_t *pParams);

//*****************************************************************************
//
//! \brief Set tuning parameters
//!
//! \param[in] handle   FLUX_OBS handle
//! \param[in] pTune    Pointer to tuning parameters
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_setTuning(FLUX_OBS_Handle handle, const FLUX_OBS_TuningParams_t *pTune);

//*****************************************************************************
//
//! \brief Update filter parameters (call after changing lpfCutoff_Hz)
//!
//! \param[in] handle   FLUX_OBS handle
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_updateFilterParams(FLUX_OBS_Handle handle);

//*****************************************************************************
//
//! \brief Reset all state variables
//!
//! \param[in] handle   FLUX_OBS handle
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_resetState(FLUX_OBS_Handle handle);

//*****************************************************************************
//
//! \brief Set flux angle estimate
//!
//! \param[in] handle      FLUX_OBS handle
//! \param[in] angle_rad   Angle in radians [-pi, pi]
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_setAngle_rad(FLUX_OBS_Handle handle, float32_t angle_rad);

//*****************************************************************************
//
//! \brief Set filtered reference speed (called by FOC layer)
//!
//! \param[in] handle           FLUX_OBS handle
//! \param[in] filteredSpeed    Filtered speed in rad/s
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_setFilteredRefSpeed(FLUX_OBS_Handle handle, float32_t filteredSpeed);

//*****************************************************************************
//
//! \brief Run flux observer algorithm (non-inline version)
//!
//! \param[in]  handle   FLUX_OBS handle
//! \param[in]  pVab_V   Alpha-beta voltage input
//! \param[in]  pIab_A   Alpha-beta current input
//! \param[out] pOutput  Flux observer output
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_run(FLUX_OBS_Handle handle, const FLUX_OBS_VoltageInput_t *pVab_V,
                            const FLUX_OBS_CurrentInput_t *pIab_A, FLUX_OBS_Output_t *pOutput);

//*****************************************************************************
//
//! \brief Get current flux observer output
//!
//! \param[in]  handle   FLUX_OBS handle
//! \param[out] pOutput  Flux observer output
//!
//! \return    0 on success, negative error code on failure
//!
//*****************************************************************************
extern int32_t FLUX_OBS_getOutput(FLUX_OBS_Handle handle, FLUX_OBS_Output_t *pOutput);

//*****************************************************************************
//
//! \brief Get current estimated angle
//!
//! \param[in] handle   FLUX_OBS handle
//!
//! \return    Estimated flux angle in radians [-pi, pi]
//!
//*****************************************************************************
extern float32_t FLUX_OBS_getAngle_rad(FLUX_OBS_Handle handle);

//*****************************************************************************
//
//! \brief Get current estimated speed in rad/s
//!
//! \param[in] handle   FLUX_OBS handle
//!
//! \return    Estimated speed in rad/s
//!
//*****************************************************************************
extern float32_t FLUX_OBS_getSpeed_rad_per_s(FLUX_OBS_Handle handle);

//*****************************************************************************
//
//! \brief Get current estimated speed in Hz
//!
//! \param[in] handle   FLUX_OBS handle
//!
//! \return    Estimated speed in Hz
//!
//*****************************************************************************
extern float32_t FLUX_OBS_getSpeed_Hz(FLUX_OBS_Handle handle);

//! @}

#ifdef __cplusplus
}
#endif

#endif  // FLUX_OBS_H
