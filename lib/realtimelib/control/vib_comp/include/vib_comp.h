// #############################################################################
//
//! @file   vib_comp.h
//! @brief  ARM M33 Vibration Compensation (VIB_COMP) library
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

#ifndef VIB_COMP_H
#define VIB_COMP_H

// the includes

#include "math_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \defgroup VIB_COMP VIB_COMP
//! @{
//
//*****************************************************************************

// **************************************************************************
// the defines

//! \brief  Defines the maximum number of points in the vibration compensation table
//!
#define VIB_COMP_BUF_SIZE (360)  // 360/3=120, every 3 degree

// **************************************************************************
// the typedefs

//! \brief Defines the Vibration Compensation object
//!
//! \details Implements adaptive vibration compensation using a feed-forward
//!          table indexed by mechanical angle to reduce torque ripple and
//!          mechanical vibrations in motor operation.
//!
typedef struct _VIB_COMP_Obj_
{
    float32_t FF_table[VIB_COMP_BUF_SIZE + 1];  //!< Feed-forward compensation table (360 samples + 1)
    float32_t alpha;                            //!< IIR filter alpha coefficient for table update
    float32_t beta;                             //!< IIR filter beta coefficient (1-alpha) with gain

    float32_t anglePairsMax;   //!< Maximum mechanical angle range (pole pairs * 2π), rad
    float32_t angleMechInvSf;  //!< Inverse scaling factor for mechanical angle
    float32_t indexSf;         //!< Scaling factor for table index calculation

    float32_t angleElecPrev_rad;   //!< Previous electrical angle, rad
    float32_t angleMechPoles_rad;  //!< Mechanical angle considering pole pairs, rad
    float32_t angleMech_rad;       //!< Current mechanical angle (0 to 2π), rad
    float32_t Iq_outFF_A;          //!< Feed-forward compensation output current, A

    float32_t Iq_comp_A;  //!< Compensation current value (for debugging), A

    int16_t index;       //!< Current table index based on mechanical angle
    int16_t indexDelta;  //!< Phase advance index offset for compensation timing

    bool flagEnableFF;  //!< Enable flag for feed-forward compensation output
} VIB_COMP_Obj;

//! \brief Defines the VIB_COMP handle
//!
typedef struct _VIB_COMP_Obj_ *VIB_COMP_Handle;

// the function prototypes

//! \brief     Get the index delta of vibration compensation buffer
//! \param[in] handle         The vibration compensation handle
static inline int16_t VIB_COMP_getAdvIndexDelta(VIB_COMP_Handle handle)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    return (obj->indexDelta);
}  // end of VIB_COMP_getAdvIndexDelta() function

//! \brief     Gets the Alpha coefficient of the vibration compensation
//! \param[in] handle         The vibration compensation handle
static inline float32_t VIB_COMP_getAlpha(VIB_COMP_Handle handle)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    return (obj->alpha);
}  // end of VIB_COMP_getAlpha() function

//! \brief     Gets the flag of vibration compensation algorithm enable
//! \param[in] handle         The vibration compensation handle
static inline bool VIB_COMP_getFlag_enableOutput(VIB_COMP_Handle handle)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    return (obj->flagEnableFF);
}  // end of VIB_COMP_getFlag_enableOutput() function

//! \brief     Gets the index of vibration compensation buffer
//! \param[in] handle         The vibration compensation handle
static inline int16_t VIB_COMP_getIndex(VIB_COMP_Handle handle)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    return (obj->index);
}  // end of VIB_COMP_getIndex() function

//! \brief     Initializes the vibration compensation module
//! \param[in] pMemory   A pointer to the vibration compensation object memory
//! \param[in] numBytes  The number of bytes allocated for the vibration compensation object, bytes
//! \return    The vibration compensation (VIB_COMP) object handle
extern VIB_COMP_Handle VIB_COMP_init(void *pMemory, const size_t numBytes);

//! \brief     Resets the vibration compensation module
//! \param[in] handle  The vibration compensation handle
extern void VIB_COMP_reset(VIB_COMP_Handle handle);

//! \brief     Runs the vibration compensation algorithm
//! \param[in] handle         The vibration compensation handle
//! \param[in] angle_mech_pu  The mechanical angle in per units from _IQ(0.0) to _IQ(1.0)
//! \param[in] Iq_in_pu       The measured Iq in per units
//! \return    The value to be used as a feed forward term in the speed controller

static inline float32_t VIB_COMP_run(VIB_COMP_Handle handle, const float32_t angleElec_rad, const float32_t Iq_in_A)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;
    //! \brief      Calculates mechanical angle from electrical angle
    float32_t     angleElecAbs_rad;
    float32_t     angleElecDelta_rad;
    float32_t     angleMechTemp_rad;  // temporary value for intermediate calculations

    int16_t tmp_adv_index;

    obj->index = (int16_t)(obj->angleMech_rad * obj->indexSf);

    obj->index = (obj->index >= (int16_t)VIB_COMP_BUF_SIZE) ? ((int16_t)VIB_COMP_BUF_SIZE - 1) : obj->index;

    obj->index = (obj->index < 0) ? 0 : obj->index;

    tmp_adv_index = obj->index + obj->indexDelta;

    tmp_adv_index =
        (tmp_adv_index >= (int16_t)VIB_COMP_BUF_SIZE) ? (tmp_adv_index - (int16_t)VIB_COMP_BUF_SIZE) : tmp_adv_index;

    tmp_adv_index = (tmp_adv_index < 0) ? 0 : tmp_adv_index;

    obj->FF_table[obj->index] = obj->alpha * obj->FF_table[obj->index] + obj->beta * Iq_in_A;

    obj->Iq_comp_A = obj->FF_table[tmp_adv_index];  // only for debugging

    if (obj->flagEnableFF == true)
    {
        obj->Iq_outFF_A = obj->FF_table[tmp_adv_index];
    }
    else
    {
        obj->Iq_outFF_A = 0.0f;
    }

    // calculates angle delta, (-pi, pi) -> (0, pi)
    if (angleElec_rad < 0.0f)
    {
        angleElecAbs_rad = angleElec_rad + MATH_TWO_PI;
    }
    else
    {
        angleElecAbs_rad = angleElec_rad;
    }

    angleElecDelta_rad = angleElecAbs_rad - obj->angleElecPrev_rad;

    // store the angle so next time this function is called we have the angle from the previous sample
    obj->angleElecPrev_rad = angleElecAbs_rad;

    // calculate new mechanical angle
    angleMechTemp_rad = obj->angleMechPoles_rad + angleElecDelta_rad;

    // take care of delta calculations when electrical angle wraps around
    // from -2*PI to 0.0 or from 0.0 to 2*PI
    if (angleElecDelta_rad < -MATH_PI)
    {
        angleMechTemp_rad = angleMechTemp_rad + MATH_TWO_PI;
    }
    else if (angleElecDelta_rad > MATH_PI)
    {
        angleMechTemp_rad = angleMechTemp_rad - MATH_TWO_PI;
    }

    // take care of wrap around of the mechanical angle,
    // so that angle_mech_poles stays within -USER_MOTOR_NUM_POLE_PAIRS*2*PI
    // to USER_MOTOR_NUM_POLE_PAIRS*2*PI
    if (angleMechTemp_rad >= obj->anglePairsMax)
    {
        angleMechTemp_rad = angleMechTemp_rad - obj->anglePairsMax;
    }
    else if (angleMechTemp_rad <= -obj->anglePairsMax)
    {
        angleMechTemp_rad = angleMechTemp_rad + obj->anglePairsMax;
    }

    // store value in angle_mech_poles
    obj->angleMechPoles_rad = angleMechTemp_rad;

    // scale the mechanical angle so that final output is from -2*PI to 2*PI
    angleMechTemp_rad = angleMechTemp_rad * obj->angleMechInvSf;

    // make the final mechanical angle a positive only values from 0.0 to 2*PI
    if (angleMechTemp_rad < 0.0f)
    {
        obj->angleMech_rad = angleMechTemp_rad + MATH_TWO_PI;
    }
    else
    {
        obj->angleMech_rad = angleMechTemp_rad;
    }

    return (obj->Iq_outFF_A);
}  // end of VIB_COMP_run() function

//! \brief     Sets AngleMechPoles for the vibration compensation
//! \param[in] handle         The vibration compensation handle
static inline void VIB_COMP_setAngleMechPoles(VIB_COMP_Handle handle, const int16_t angleMechPoles_rad)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    // calculates angle delta, (-pi, pi) -> (0, 2*pi)
    obj->angleMechPoles_rad = angleMechPoles_rad + MATH_TWO_PI;

    return;
}  // end of VIB_COMP_setAngleMechPoles() function

//! \brief     Sets the index delta for the vibration compensation
//! \param[in] handle         The vibration compensation handle
static inline void VIB_COMP_setAdvIndexDelta(VIB_COMP_Handle handle, const int16_t indexDelta)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    obj->indexDelta = indexDelta;

    return;
}  // end of VIB_COMP_setAdvIndexDelta() function

//! \brief     Sets Alpha and Gain for the vibration compensation
//! \param[in] handle         The vibration compensation handle
static inline void VIB_COMP_setAlphaGain(VIB_COMP_Handle handle, const float32_t alpha, const float32_t gain)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    obj->alpha = alpha * gain;
    obj->beta  = (1.0f - alpha) * gain;

    return;
}  // end of VIB_COMP_setAlpha() function

//! \brief     Runs the vibration compensation algorithm
//! \param[in] handle         The vibration compensation handle
static inline void VIB_COMP_setFlag_enableOutput(VIB_COMP_Handle handle, const bool state)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    obj->flagEnableFF = state;

    return;
}  // end of VIB_COMP_setFlag_enableOutput() function

//! \brief     Runs the vibration compensation algorithm
//! \param[in] handle         The vibration compensation handle
static inline void VIB_COMP_setIndex(VIB_COMP_Handle handle, const int16_t index)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    obj->index = index;

    return;
}  // end of VIB_COMP_setIndex() function

inline float32_t VIB_COMP_calcMechangle(VIB_COMP_Handle handle, const float32_t angleElec_rad)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;
    //! \brief      Calculates mechanical angle from electrical angle
    float32_t     angleElecAbs_rad;
    float32_t     angleElecDelta_rad;
    float32_t     angleMechTemp_rad;  // temporary value for intermediate calculations

    // calculates angle delta
    if (angleElec_rad < 0.0f)
    {
        angleElecAbs_rad = angleElec_rad + MATH_TWO_PI;
    }
    else
    {
        angleElecAbs_rad = angleElec_rad;
    }

    angleElecDelta_rad = angleElecAbs_rad - obj->angleElecPrev_rad;

    // store the angle so next time this function is called we have the angle from the previous sample
    obj->angleElecPrev_rad = angleElecAbs_rad;

    // calculate new mechanical angle
    angleMechTemp_rad = obj->angleMechPoles_rad + angleElecDelta_rad;

    // take care of delta calculations when electrical angle wraps around from _IQ(1.0) to _IQ(0.0) or from _IQ(0.0) to
    // _IQ(1.0)
    if (angleElecDelta_rad < -MATH_PI)
    {
        angleMechTemp_rad = angleMechTemp_rad + MATH_TWO_PI;
    }
    else if (angleElecDelta_rad > MATH_PI)
    {
        angleMechTemp_rad = angleMechTemp_rad - MATH_TWO_PI;
    }

    // take care of wrap around of the mechanical angle, so that angle_mech_poles stays within
    // _IQ(-USER_MOTOR_NUM_POLE_PAIRS) to _IQ(USER_MOTOR_NUM_POLE_PAIRS)
    if (angleMechTemp_rad >= obj->anglePairsMax)
    {
        angleMechTemp_rad = angleMechTemp_rad - obj->anglePairsMax;
    }
    else if (angleMechTemp_rad <= -obj->anglePairsMax)
    {
        angleMechTemp_rad = angleMechTemp_rad + obj->anglePairsMax;
    }

    // store value in angle_mech_poles
    obj->angleMechPoles_rad = angleMechTemp_rad;

    // scale the mechanical angle so that final output is from -1.0 to 1.0
    angleMechTemp_rad = angleMechTemp_rad * obj->angleMechInvSf;

    // make the final mechanical angle a positive only values from 0.0 to 1.0
    if (angleMechTemp_rad < 0.0f)
    {
        obj->angleMech_rad = angleMechTemp_rad + MATH_TWO_PI;
    }
    else
    {
        obj->angleMech_rad = angleMechTemp_rad;
    }

    return (obj->angleMech_rad);
}  // end of VIB_COMP_Mechangle_run() function

//! \brief     set up parameters for the automatic vibration compensation algorithm
//! \param[in] handle         The vibration compensation handle
extern void VIB_COMPA_setParams(VIB_COMP_Handle handle, const float32_t alpha, const float32_t gain,
                                const int16_t index_delta, const uint16_t numPolePairs);

//! \brief     set up parameters for the traditional vibration compensation algorithm
//! \param[in] handle         The vibration compensation handle
extern void VIB_COMPT_setParams(VIB_COMP_Handle handle, const uint16_t numPolePairs);

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

#endif  // end of VIB_COMP_H definition
