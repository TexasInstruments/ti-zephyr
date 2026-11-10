// #############################################################################
//
//! \file   speedfr.h
//!
//! \brief  ARM M33 Speed Frequency Calculation (SPEEDFR) library
//!         (floating point)
//!
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

#ifndef SPEED_FR_H
#define SPEED_FR_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \defgroup SPEED_FR SPEED_FR
//! @{
//
//*****************************************************************************

// the includes
#include "math_types.h"

// modules
#include "userParams.h"

//*****************************************************************************
//
//! \brief Defines the Speed Frequency (SPDFR) object
//
//*****************************************************************************
typedef struct _SPDFR_obj_
{
    float32_t theta;      //!< Input: reference set-point
    float32_t thetaPrev;  //!< Input: previous angle feedback
    float32_t K2;         //!< Parameter: proportional loop gain
    float32_t K3;         //!< Parameter: integral gain
    float32_t scaleFreq;  //!< Parameter: frequency scale factor
    float32_t speed_pu;   //!< Output: speed in per-unit
    float32_t speed_Hz;   //!< Output: speed in Hertz
} SPDFR_Obj;

//*****************************************************************************
//
//! \brief Defines the SPDFR handle
//
//*****************************************************************************
typedef struct _SPDFR_obj_ *SPDFR_Handle;

//*****************************************************************************
//
//! \brief     Initializes the SPDFR controller
//!
//! \param[in] pMemory   A pointer to the memory for the SPDFR object
//!
//! \param[in] numBytes  The number of bytes allocated for the SPDFR object
//!
//! \return    The SPDFR controller handle
//
//*****************************************************************************
extern SPDFR_Handle SPDFR_init(void *pMemory, const size_t numBytes);

//*****************************************************************************
//
//! \brief     Resets the SPDFR controller to initial state
//!
//! \param[in] handle   The SPDFR controller handle
//!
//! \return    None
//
//*****************************************************************************
extern void SPDFR_reset(SPDFR_Handle handle);

//*****************************************************************************
//
//! \brief     Sets the SPDFR controller parameters from user parameters
//!
//! \param[in] handle       The SPDFR controller handle
//!
//! \param[in] pUserParams  Pointer to user parameters structure
//!
//! \return    None
//
//*****************************************************************************
extern void SPDFR_setParams(SPDFR_Handle handle, const USER_Params *pUserParams);

//*****************************************************************************
//
//! \brief     Gets the speed value in Hertz from the SPDFR controller
//!
//! \param[in] handle  The SPDFR controller handle
//!
//! \return    The speed value in Hertz
//
//*****************************************************************************
static inline float32_t SPDFR_getSpeedHz(SPDFR_Handle handle)
{
    SPDFR_Obj *obj = (SPDFR_Obj *)handle;

    return (obj->speed_Hz);
}

//*****************************************************************************
//
//! \brief     Runs the SPDFR speed calculation algorithm
//!
//! \param[in] handle  The SPDFR controller handle
//!
//! \param[in] theta   The rotor angle in radians
//!
//! \return    None
//
//*****************************************************************************
static inline void SPDFR_run(SPDFR_Handle handle, float32_t theta)
{
    SPDFR_Obj *obj = (SPDFR_Obj *)handle;
    float32_t  error;

    obj->theta = theta * MATH_ONE_OVER_TWO_PI;

    // error cal
    error = obj->theta - obj->thetaPrev;

    if (error < -0.5f)
    {
        error += 1.0f;
    }
    else if (error > 0.5f)
    {
        error -= 1.0f;
    }

    // Update the electrical angle
    obj->thetaPrev = obj->theta;

    // Low-pass filter
    obj->speed_pu = (obj->K2 * obj->speed_pu) + (obj->K3 * error);

    // Saturate the output
    obj->speed_pu = MATH_sat(obj->speed_pu, 1.0f, -1.0f);

    // Change motor speed for pu to rpm value
    obj->speed_Hz = obj->scaleFreq * obj->speed_pu;

    return;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of SPEED_FR_H definition
