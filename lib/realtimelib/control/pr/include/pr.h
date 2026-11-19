// #############################################################################
//
//! @file   pr.h
//! @brief  ARM M33 Proportional-Resonant (PR) controller library
//!         (floating point)
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

#ifndef PR_H
#define PR_H

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
//! \defgroup PR PR
//! @{
//
//*****************************************************************************

#include "types.h"
#include "math_types.h"

//*****************************************************************************
//
//! \brief Defines the PR controller object
//!
//! \details The PR controller implements a proportional-resonant control
//!          algorithm commonly used for AC current control and harmonic
//!          compensation in motor control applications.
//
//*****************************************************************************
typedef struct _PR_Obj_
{
    float32_t Feedback;       //!< Feedback signal input
    float32_t prevError;      //!< Previous error value (n-1)
    float32_t prevPrevError;  //!< Previous previous error value (n-2)
    float32_t out;            //!< Controller output value
    float32_t prevOut;        //!< Previous output value (n-1)
    float32_t prevPrevOut;    //!< Previous previous output value (n-2)
    float32_t kr;             //!< Resonant gain coefficient
    float32_t outMin;         //!< Minimum output saturation limit
    float32_t outMax;         //!< Maximum output saturation limit
} PR_Obj;

//*****************************************************************************
//
//! \brief Defines the PR handle
//
//*****************************************************************************
typedef struct _PR_Obj_ *PR_Handle;

//*****************************************************************************
//
//! \brief     Initializes the PR controller
//!
//! \param[in] pMemory   A pointer to the memory for the PR controller object
//!
//! \param[in] numBytes  The number of bytes allocated for the PR controller
//!                      object, bytes
//!
//! \return    The PR controller (PR) object handle
//
//*****************************************************************************
extern PR_Handle PR_init(void *pMemory, const size_t numBytes);

//*****************************************************************************
//
//! \brief                 Runs the PR controller
//!
//! \param[in] handle      The PR controller handle
//!
//! \param[in] refValue    The invDenominator value to the controller
//!
//! \param[in] fbackValue  The prevOutCoeff value to the controller
//!
//! \param[in] ffwdValue   The prevPrevOutCoeff value to the controller
//!
//! \return    None
//
//*****************************************************************************
static inline void PR_run(PR_Handle handle, const float32_t invDenominator, const float32_t prevOutCoeff,
                          const float32_t prevPrevOutCoeff)
{
    PR_Obj *obj = (PR_Obj *)handle;

    float32_t numerator;
    float32_t temp;
    float32_t errorTerm;
    float32_t prevOutTerm;
    float32_t prevPrevOutTerm;
    float32_t Error;

    Error = -obj->Feedback;

    errorTerm = obj->kr * (Error - obj->prevPrevError);

    prevOutTerm = obj->prevOut * prevOutCoeff;

    prevPrevOutTerm = obj->prevPrevOut * prevPrevOutCoeff;

    numerator = errorTerm + prevOutTerm + prevPrevOutTerm;

    temp = numerator * invDenominator;

    // Saturate the output
    obj->out = MATH_sat(temp, obj->outMax, obj->outMin);

    obj->prevPrevOut = obj->prevOut;
    obj->prevOut     = obj->out;

    obj->prevPrevError = obj->prevError;
    obj->prevError     = Error;

    return;
}  // end of PR_run() function

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

#endif  // end of PR_H defines
