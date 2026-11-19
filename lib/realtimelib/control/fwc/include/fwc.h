// #############################################################################
//
//! @file   fwc.h
//! @brief  ARM M33 Field Weakening Control (FWC) library (floating point)
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

#ifndef FWC_H
#define FWC_H

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
//! \defgroup FWC FWC
//! @{
//
//*****************************************************************************

#include "math_types.h"
#include "pi.h"

//*****************************************************************************
//
//! \brief Defines the Field Weakening Control (FWC) object
//!
//! \details The FWC module implements field weakening control for extending
//!          the motor speed range beyond the rated speed by reducing the
//!          d-axis current component (flux weakening).
//
//*****************************************************************************
typedef struct _FWC_Obj_
{
    PI_Handle piHandle;  //!< Handle for the FWC angle PI controller
    PI_Obj    pi;        //!< FWC angle PI controller object instance

    float32_t angleCurrent_rad;  //!< Stator current phase angle, rad
    bool      flagEnable;        //!< Enable flag for the FWC controller
} FWC_Obj;

//*****************************************************************************
//
//! \brief Defines the FWC handle
//
//*****************************************************************************
typedef struct _FWC_Obj_ *FWC_Handle;

//*****************************************************************************
//
// Prototypes for the APIs
//
//*****************************************************************************
//! \brief     Disables the FWC function
//! \param[in] handle  The Field Weakening Control (FWC) handle
static inline void FWC_disable(FWC_Handle handle)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    obj->flagEnable = false;

    return;
}  // end of FWC_disable() function

//! \brief     Enables the FWC function
//! \param[in] handle  The Field Weakening Control (FWC) handle
static inline void FWC_enable(FWC_Handle handle)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    obj->flagEnable = true;

    return;
}  // end of FWC_enable() function

//! \brief     Gets the stator current phase angle memory address
//! \param[in] handle  The Field Weakening Control (FWC) handle
//! \return    The stator current phase angle memory address
static inline float32_t *FWC_getCurrentAngle_rad_addr(FWC_Handle handle)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    return (&(obj->angleCurrent_rad));
}  // end of FWC_getCurrentAngle_rad_addr() function

//! \brief     Gets the stator current phase angle value (angleCurrent_rad)
//! \param[in] handle  The Field Weakening Control (FWC) handle
//! \return    The stator current phase angle value, rad
static inline float32_t FWC_getCurrentAngle_rad(FWC_Handle handle)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    return (obj->angleCurrent_rad);
}  // end of FWC_getAnglePhase_rad() function

//! \brief     Gets the enable controller flag value from the FWC
//! \param[in] handle  The Field Weakening Control (FWC) handle
//! \return    The enable FWC flag value
static inline bool FWC_getFlagEnable(FWC_Handle handle)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    return (obj->flagEnable);
}  // end of FWC_getFlagEnable() function

//! \brief     Sets the stator current phase angle value
//! \param[in] handle      The Field Weakening Control (FWC) handle
//! \param[in] angleCurrent_rad  The stator current phase angle value, rad
static inline void FWC_setCurrentAngle_rad(FWC_Handle handle, const float32_t angleCurrent_rad)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    obj->angleCurrent_rad = angleCurrent_rad;

    return;
}  // end of FWC_setCurrentAngle_rad() function

//! \brief     Sets the enable flag
//! \param[in] handle      The Field Weakening Control (FWC) handle
//! \param[in] flagEnable  The enable flag
static inline void FWC_setFlagEnable(FWC_Handle handle, const bool flagEnable)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    obj->flagEnable = flagEnable;

    return;
}  // end of FWC_setFlagEnable() function

//! \brief     Sets the integral gain (Ki) value
//! \param[in] handle    The Field Weakening Control (FWC) handle
//! \param[in] Ki        The Ki value
static inline void FWC_setKi(FWC_Handle handle, const float32_t Ki)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    PI_setKi(obj->piHandle, Ki);

    return;
}  // end of FWC_setKi() function

//! \brief     Sets the proportional gain (Kp) value
//! \param[in] handle    The Field Weakening Control (FWC) handle
//! \param[in] Kp        The Kp value
static inline void FWC_setKp(FWC_Handle handle, const float32_t Kp)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    PI_setKp(obj->piHandle, Kp);

    return;
}  // end of FWC_setKp() function

//! \brief     Sets the gain values
//! \param[in] handle    The Field Weakening Control (FWC) handle
//! \param[in] Kp        The Kp gain value
//! \param[in] Ki        The Ki gain value
static inline void FWC_setGains(FWC_Handle handle, const float32_t Kp, const float32_t Ki)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    PI_setKp(obj->piHandle, Kp);
    PI_setKi(obj->piHandle, Ki);

    return;
}  // end of FWC_setGains() function

//! \brief     Sets the minimum and maximum output values of FWC
//! \param[in] handle        The Field Weakening Control (FWC) handle
//! \param[in] angleMin_rad  The angle out minimum value
//! \param[in] angleMax_rad  The angle out maximum value
static inline void FWC_setAngleMinMax(FWC_Handle handle, const float32_t angleMin_rad, const float32_t angleMax_rad)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    // set the minimum and maximum values of PI controller
    PI_setMinMax(obj->piHandle, angleMax_rad, angleMin_rad);

    return;
}  // end of FWC_setAngleMinMax() function

//! \brief     Sets the maximum output values of FWC
//! \param[in] handle        The Field Weakening Control (FWC) handle
//! \param[in] angleMax_rad  The angle out maximum value
static inline void FWC_setAngleMax(FWC_Handle handle, const float32_t angleMax_rad)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    // set the minimum values of PI controller
    // convert the maximum angle to negative value in PI controller
    // the maximum angle = MATH_PI_OVER_TWO - minimum value of PI output
    PI_setOutMin(obj->piHandle, angleMax_rad);

    return;
}  // end of FWC_setAngleMax() function

//! \brief     Resets the integrator start value (Ui)
//! \param[in] handle    The Field Weakening Control (FWC) handle
static inline void FWC_resetUi(FWC_Handle handle)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    // Sets the integrator start value in the PI controller
    // reset the new integrator value to Zero
    PI_setUi(obj->piHandle, 0.0);

    return;
}  // end of FWC_setKi() function

//! \brief     Sets the integrator start value (Ui)
//! \param[in] handle    The Field Weakening Control (FWC) handle
//! \param[in] Ui        The integrator start value
static inline void FWC_setUi(FWC_Handle handle, const float32_t Ui)
{
    FWC_Obj *obj = (FWC_Obj *)handle;

    // set the new integrator value
    PI_setUi(obj->piHandle, Ui);

    return;
}  // end of FWC_setKi() function

//! \brief     Initializes the Field Weakening Control (FWC) module
//! \param[in] pMemory   A pointer to the memory for the FWC object
//! \param[in] numBytes  The number of bytes allocated for the FWC object
//! \return The Field Weakening Control (FWC) object handle
extern FWC_Handle FWC_init(void *pMemory, const size_t numBytes);

//! \brief     Sets the Field Weakening Control (FWC) module parmaeters
//! \param[in] handle        The Field Weakening Control (FWC) handle
//! \param[in] Kp            The Kp gain value
//! \param[in] Ki            The Ki gain value
//! \param[in] angleMin_rad  The angle out minimum value
//! \param[in] angleMax_rad  The angle out maximum value
extern void FWC_setParams(FWC_Handle handle, const float32_t Kp, const float32_t Ki, const float32_t angleMin_rad,
                          const float32_t angleMax_rad);

//! \brief     Runs the Field Weakening Control (FWC)
//! \param[in] handle   The Field Weakening Control (FWC) handle
//! \param[in] Vs_V   The output voltage vector, V
//! \param[in] VsRef_V  The reference voltage vector, V
//! \return    None
static inline void FWC_computeCurrentAngle(FWC_Handle handle, const float32_t Vs_V, const float32_t VsRef_V)
{
    FWC_Obj  *obj       = (FWC_Obj *)handle;
    float32_t angle_rad = 0.0;

    float32_t Vs_in_V    = Vs_V;
    float32_t VsRef_in_V = VsRef_V;

    if (FWC_getFlagEnable(handle) == true)
    {
        // Perform the Field Weakening Control (FWC)

        PI_run_series(obj->piHandle, VsRef_in_V, Vs_in_V, 0.0, &angle_rad);

        obj->angleCurrent_rad = MATH_PI_OVER_TWO - angle_rad;
    }
    else
    {
        obj->angleCurrent_rad = MATH_PI_OVER_TWO;
    }

    return;
}  // end of FWC_computeCurrentAngle() function

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

#endif  // FWC_H
