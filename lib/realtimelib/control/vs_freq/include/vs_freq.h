// #############################################################################
//
//! @file   vs_freq.h
//! @brief  ARM M33 Volts per Frequency (VS_FREQ) library
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

#ifndef VS_FREQ_H
#define VS_FREQ_H

// **************************************************************************
// the includes
#include "math_types.h"

//!
//! \defgroup VS_FREQ VS_FREQ
//! @{

#ifdef __cplusplus
extern "C" {
#endif

//! \brief Defines the Volts per Frequency (V/F) control object
//!
//! \details Implements V/F profile control for open-loop motor operation,
//!          maintaining voltage-to-frequency ratio for stable motor operation
//!          across the speed range.
//!
typedef struct _VS_FREQ_Obj_
{
    float32_t maxVsMag_pu;  //!< Maximum voltage magnitude limit, pu
    float32_t Freq;         //!< Input frequency (electrical), pu
    float32_t LowFreq;      //!< Low frequency threshold for boost voltage, pu
    float32_t HighFreq;     //!< High frequency threshold for rated voltage, pu
    float32_t MaxFreq;      //!< Maximum operating frequency limit, pu
    float32_t VoltMin;      //!< Minimum voltage at low frequency (boost voltage), pu
    float32_t VoltMax;      //!< Maximum rated voltage output, pu
    float32_t VfSlope;      //!< V/F slope coefficient (calculated internally)
    float32_t Vs_out;       //!< Output voltage magnitude, pu
    MATH_Vec2 Vdq_gain;     //!< D/Q voltage gain coefficients for output scaling
    MATH_Vec2 Vdq_out;      //!< Output voltage vector [Vd, Vq], pu
} VS_FREQ_Obj;

//! \brief Defines the VS_FREQ_obj handle
//!
typedef struct _VS_FREQ_Obj_ *VS_FREQ_Handle;

// **************************************************************************
// the function prototypes

//! \brief     Gets the Vd output value in VS_FREQ
//! \param[in] handle  The volts/hertz (VS_FREQ) handle
//! \return    The Vd output value in VS_FREQ, pu
static inline float32_t VS_FREQ_getVd_out(VS_FREQ_Handle handle)
{
    VS_FREQ_Obj *obj = (VS_FREQ_Obj *)handle;

    return (obj->Vdq_out.value[0]);
}  // end of VS_FREQ_getVd_pu() function

//! \param[in] handle  The volts/hertz profile (VS_FREQ) handle
//! \return    The Vq output value in VS_FREQ, pu
static inline float32_t VS_FREQ_getVq_out(VS_FREQ_Handle handle)
{
    VS_FREQ_Obj *obj = (VS_FREQ_Obj *)handle;

    return (obj->Vdq_out.value[1]);
}  // end of VS_FREQ_getVq_pu() function

//! \brief     Initializes the angle generator (VS_FREQ) module
//! \param[in] pMemory   A pointer to the memory for the object
//! \param[in] numBytes  The number of bytes allocated for the object, bytes
//! \return    The volts/hertz profile (VS_FREQ) object handle
extern VS_FREQ_Handle VS_FREQ_init(void *pMemory, const size_t numBytes);

//! \brief     Generates an output command voltage for a specific
//!            input command frequency according to the specified
//!            volts/hertz profile.
//! \param[in] handle     The volts/hertz profile (VS_FREQ) handle
//! \param[in] fm_pu      The electrical speed in pu
static inline void VS_FREQ_run(VS_FREQ_Handle handle, const float32_t Freq_pu)
{
    VS_FREQ_Obj *obj = (VS_FREQ_Obj *)handle;

    obj->Freq = fabsf(Freq_pu);

    if (obj->Freq <= obj->LowFreq)
    {
        obj->Vs_out = obj->VoltMin;
    }
    else if (obj->Freq >= obj->HighFreq)
    {
        obj->Vs_out = obj->VoltMax;
    }
    else
    {
        obj->Vs_out = obj->VoltMin + obj->VfSlope * (obj->Freq - obj->LowFreq);
    }

    obj->Vdq_out.value[0] = obj->Vs_out * obj->Vdq_gain.value[0];

    if (obj->Freq > 0.0f)
    {
        obj->Vdq_out.value[1] = obj->Vs_out * obj->Vdq_gain.value[1];
    }
    else
    {
        obj->Vdq_out.value[1] = -obj->Vs_out * obj->Vdq_gain.value[1];
    }

    return;
}  // end of VS_FREQ_run()

//! \brief     Sets the parameters VsMag_pu
//! \param[in] handle               The volts/hertz profile (VS_FREQ) handle
//! \param[in] maxVsMag_pu          The maixmum magintude for volts/hertz profile, pu
static inline void VS_FREQ_setVsMagPu(VS_FREQ_Handle handle, float32_t maxVsMag_pu)
{
    VS_FREQ_Obj *obj = (VS_FREQ_Obj *)handle;

    obj->maxVsMag_pu = maxVsMag_pu;

    return;
}  // end of ANGLE_COMP_setParams() function

//! \brief     Sets the parameters maximum frequency
//! \param[in] handle               The volts/hertz profile (VS_FREQ) handle
//! \param[in] MaxFreq              The maixmum frequency for volts/hertz profile, Hz
static inline void VS_FREQ_setMaxFreq(VS_FREQ_Handle handle, float32_t maxFreq)
{
    VS_FREQ_Obj *obj = (VS_FREQ_Obj *)handle;

    obj->MaxFreq = maxFreq;

    return;
}  // end of ANGLE_COMP_setParams() function

//! \brief     Sets the parameters
//! \param[in] handle               The volts/hertz profile (VS_FREQ) handle
//! \param[in] LowFreq              The low frequency for volts/hertz profile, Hz
//! \param[in] HighFreq             The high frequency for volts/hertz profile, Hz
//! \param[in] VoltMin              The minimum frequency for volts/hertz profile, Volts
//! \param[in] VoltMax              The maximum frequency for volts/hertz profile, Volts
extern void VS_FREQ_setProfile(VS_FREQ_Handle handle, float32_t LowFreq, float32_t HighFreq, float32_t VoltMin,
                               float32_t VoltMax);

#ifdef __cplusplus
}
#endif  // extern "C"

//! @}  // ingroup

#endif  // end of VS_FREQ_H definition
