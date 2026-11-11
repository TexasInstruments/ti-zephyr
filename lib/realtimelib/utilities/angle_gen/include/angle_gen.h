// #############################################################################
//
//  FILE:   angle_gen.h
//
//  TITLE:  ARM M33 Angle Generator (ANGLE_GEN) library
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

#ifndef ANGLE_GEN_H
#define ANGLE_GEN_H

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
//! \defgroup ANGLE_GEN ANGLE_GEN
//! @{
//
//*****************************************************************************

//
// the includes
//
#include "math_types.h"

//! \brief Defines the angle generator (ANGLE_COMP) object
//!
typedef struct _ANGLE_GEN_Obj_
{
    float32_t freq_Hz;           //!< the freq input value
    float32_t angleDeltaFactor;  //!< predetermined factor for use in angle compensation calculation
    float32_t angleDelta_rad;    //!< the angle delta value
    float32_t angle_rad;         //!< the angle output value
} ANGLE_GEN_Obj;

//! \brief Defines the ANGLE_GEN handle
//!
typedef struct _ANGLE_GEN_Obj_ *ANGLE_GEN_Handle;

//
// the function prototypes
//

//! \brief     Gets the predicted angle value
//! \param[in] handle  The angle generator (ANGLE_COMP) handle
//! \return    The predicted angle compensation value, rad
static inline float32_t ANGLE_GEN_getAngle(ANGLE_GEN_Handle handle)
{
    ANGLE_GEN_Obj *obj = (ANGLE_GEN_Obj *)handle;

    return (obj->angle_rad);
}  // end of ANGLE_GEN_getAngle_pu() function

//! \brief     Gets the predicted angle value
//! \param[in] handle  The angle generator (ANGLE_COMP) handle
//! \return    The predicted angle compensation value, rad
static inline void ANGLE_GEN_setAngle(ANGLE_GEN_Handle handle, const float32_t angle_rad)
{
    ANGLE_GEN_Obj *obj = (ANGLE_GEN_Obj *)handle;

    obj->angle_rad = angle_rad;

    return;
}  // end of ANGLE_GEN_getAngle_pu() function

//! \brief     Initializes the angle generator (ANGLE_GEN) module
//! \param[in] pMemory   A pointer to the memory for the object
//! \param[in] numBytes  The number of bytes allocated for the object, bytes
//! \return    The angle generator (ANGLE_GEN) object handle
extern ANGLE_GEN_Handle ANGLE_GEN_init(void *pMemory, const size_t numBytes);

//! \brief  Compensates for the delay introduced
//! \brief  from the time when the system inputs are sampled to when the PWM
//! \brief  voltages are applied to the motor windings.
//! \param[in] handle     The angle generator (ANGLE_COMP) handle
//! \param[in] fm_pu      The electrical speed in pu
//! \param[in] angleUncomp_pu  The uncompensated angle in pu
static inline void ANGLE_GEN_run(ANGLE_GEN_Handle handle, const float32_t freq_Hz)
{
    ANGLE_GEN_Obj *obj = (ANGLE_GEN_Obj *)handle;

    obj->freq_Hz        = freq_Hz;
    obj->angleDelta_rad = obj->freq_Hz * obj->angleDeltaFactor;

    float32_t angle_rad = obj->angle_rad;

    // increment the angle
    angle_rad += obj->angleDelta_rad;

    //
    // mask the angle for wrap around
    // note: must account for the sign of the angle
    //
    if (angle_rad > MATH_PI)
    {
        angle_rad -= MATH_TWO_PI;
    }
    else if (angle_rad < -MATH_PI)
    {
        angle_rad += MATH_TWO_PI;
    }

    obj->angle_rad = angle_rad;

    return;
}  // end of ANGLE_GEN_run()

//! \brief     Sets the parameters
//! \param[in] handle               The angle generator (ANGLE_COMP) handle
//! \param[in] iqFullScaleFreq_Hz   The frequency used to set 1 pu
//! \param[in] pwmPeriod_usec       The the pwmPeriod in usec
//! \param[in] numPWMTicksPerISRTick  The decimation between PWM cycles and the ISR cycle
extern void ANGLE_GEN_setParams(ANGLE_GEN_Handle handle, const float32_t ctrlPeriod_sec);

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
#endif  // extern "C"

#endif  // end of ANGLE_GEN_H definition
