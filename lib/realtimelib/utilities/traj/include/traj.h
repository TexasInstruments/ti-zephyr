// #############################################################################
//
//  FILE:   traj.h
//
//  TITLE:  ARM M33 trajectory library (floating point)
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

#ifndef TRAJ_H
#define TRAJ_H

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
//! \defgroup TRAJ TRAJ
//! @{
//
//*****************************************************************************

#include "types.h"
#include "math_types.h"

//*****************************************************************************
//
//! \brief Defines the trajectory (TRAJ) object
//
//*****************************************************************************
typedef struct _TRAJ_Obj_
{
    float32_t targetValue;  //!< the target value for the trajectory
    float32_t intValue;     //!< the intermediate value along the trajectory
    float32_t minValue;     //!< the minimum value for the trajectory generator
    float32_t maxValue;     //!< the maximum value for the trajectory generator
    float32_t maxDelta;     //!< the maximum delta value for the trajectory
                            //!< generator
} TRAJ_Obj;

//*****************************************************************************
//
//! \brief Defines the TRAJ handle
//
//*****************************************************************************
typedef struct _TRAJ_Obj_ *TRAJ_Handle;

//*****************************************************************************
//
//! \brief     Gets the intermediate value for the trajectory
//!
//! \param[in] handle  The trajectory (TRAJ) handle
//!
//! \return    The intermediate value
//
//*****************************************************************************
static inline float32_t TRAJ_getIntValue(TRAJ_Handle handle)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    return (obj->intValue);
}  // end of TRAJ_getIntValue() function

//*****************************************************************************
//
//! \brief     Gets the maximum delta value for the trajectory
//!
//! \param[in] handle  The trajectory (TRAJ) handle
//!
//! \return    The maximum delta value
//
//*****************************************************************************
static inline float32_t TRAJ_getMaxDelta(TRAJ_Handle handle)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    return (obj->maxDelta);
}  // end of TRAJ_getMaxDelta() function

//*****************************************************************************
//
//! \brief     Gets the maximum value for the trajectory
//!
//! \param[in] handle  The trajectory (TRAJ) handle
//!
//! \return    The maximum value
//
//*****************************************************************************
static inline float32_t TRAJ_getMaxValue(TRAJ_Handle handle)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    return (obj->maxValue);
}  // end of TRAJ_getMaxValue() function

//*****************************************************************************
//
//! \brief     Gets the minimum value for the trajectory
//!
//! \param[in] handle  The trajectory (TRAJ) handle
//!
//! \return    The minimum value
//
//*****************************************************************************
static inline float32_t TRAJ_getMinValue(TRAJ_Handle handle)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    return (obj->minValue);
}  // end of TRAJ_getMinValue() function

//*****************************************************************************
//
//! \brief     Gets the target value for the trajectory
//!
//! \param[in] handle  The trajectory (TRAJ) handle
//!
//! \return    The target value
//
//*****************************************************************************
static inline float32_t TRAJ_getTargetValue(TRAJ_Handle handle)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    return (obj->targetValue);
}  // end of TRAJ_getTargetValue() function

//*****************************************************************************
//
//! \brief     Initializes the trajectory (TRAJ) object
//!
//! \param[in] pMemory   A pointer to the memory for the trajectory (TRAJ)
//!                      object
//!
//! \param[in] numBytes  The number of bytes allocated for the trajectory
//!                      object, bytes
//!
//! \return    The trajectory (TRAJ) object handle
//
//*****************************************************************************
extern TRAJ_Handle TRAJ_init(void *pMemory, const size_t numBytes);

//*****************************************************************************
//
//! \brief     Sets the intermediate value for the trajectory
//!
//! \param[in] handle    The trajectory (TRAJ) handle
//!
//! \param[in] intValue  The intermediate value
//!
//! \return    None
//
//*****************************************************************************
static inline void TRAJ_setIntValue(TRAJ_Handle handle, const float32_t intValue)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    obj->intValue = intValue;

    return;
}  // end of TRAJ_setIntValue() function

//*****************************************************************************
//
//! \brief     Sets the maximum delta value for the trajectory
//!
//! \param[in] handle    The trajectory (TRAJ) handle
//!
//! \param[in] maxDelta  The maximum delta value
//!
//! \return    None
//
//*****************************************************************************
static inline void TRAJ_setMaxDelta(TRAJ_Handle handle, const float32_t maxDelta)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    obj->maxDelta = maxDelta;

    return;
}  // end of TRAJ_setMaxDelta() function

//*****************************************************************************
//
//! \brief     Sets the maximum value for the trajectory
//!
//! \param[in] handle    The trajectory (TRAJ) handle
//!
//! \param[in] maxValue  The maximum value
//!
//! \return    None
//
//*****************************************************************************
static inline void TRAJ_setMaxValue(TRAJ_Handle handle, const float32_t maxValue)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    obj->maxValue = maxValue;

    return;
}  // end of TRAJ_setMaxValue() function

//*****************************************************************************
//
//! \brief     Sets the minimum value for the trajectory
//!
//! \param[in] handle    The trajectory (TRAJ) handle
//!
//! \param[in] minValue  The minimum value
//!
//! \return    None
//
//*****************************************************************************
static inline void TRAJ_setMinValue(TRAJ_Handle handle, const float32_t minValue)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    obj->minValue = minValue;

    return;
}  // end of TRAJ_setMinValue() function

//*****************************************************************************
//
//! \brief     Sets the target value for the trajectory
//!
//! \param[in] handle       The trajectory (TRAJ) handle
//!
//! \param[in] targetValue  The target value
//!
//! \return    None
//
//*****************************************************************************
static inline void TRAJ_setTargetValue(TRAJ_Handle handle, const float32_t targetValue)
{
    TRAJ_Obj *obj = (TRAJ_Obj *)handle;

    obj->targetValue = targetValue;

    return;
}  // end of TRAJ_setTargetValue() function

//*****************************************************************************
//
//! \brief     Runs the trajectory (TRAJ) object
//!
//! \param[in] handle  The trajectory (TRAJ) handle
//!
//! \return    None
//
//*****************************************************************************
static inline void TRAJ_run(TRAJ_Handle handle)
{
    float32_t targetValue = TRAJ_getTargetValue(handle);
    float32_t intValue    = TRAJ_getIntValue(handle);
    float32_t error       = targetValue - intValue;
    float32_t maxDelta    = TRAJ_getMaxDelta(handle);
    float32_t minValue    = TRAJ_getMinValue(handle);
    float32_t maxValue    = TRAJ_getMaxValue(handle);

    //
    // Increment the value
    //
    intValue += MATH_sat(error, maxDelta, -maxDelta);

    //
    // Bound the value
    //
    intValue = MATH_sat(intValue, maxValue, minValue);

    //
    // Store the value
    //
    TRAJ_setIntValue(handle, intValue);

    return;
}  // end of TRAJ_run() function

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

#endif  // TRAJ_H
