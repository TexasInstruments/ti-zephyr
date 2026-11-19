// #############################################################################
//
//  FILE:   volt_calc.h
//
//  TITLE:  ARM M33 Proportional-Integral (PI) controller library
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

#ifndef VOLT_CAL_H
#define VOLT_CAL_H

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
//! \defgroup VOLT_CALC VOLT_CALC
//! @{
//
//*****************************************************************************

#include "types.h"
#include "math_types.h"

//*****************************************************************************
//
//! \brief Defines the PHVOLC controller object
//
//*****************************************************************************
typedef struct _PHVOLC_Obj_
{
    float32_t VphaseA;         // Output: Phase voltage phase A (pu)
    float32_t VphaseB;         // Output: Phase voltage phase B (pu)
    float32_t VphaseC;         // Output: Phase voltage phase C (pu)
    float32_t Valpha;          // Output: Stationary d-axis phase voltage (pu)
    float32_t Vbeta;           // Output: Stationary q-axis phase voltage (pu)
    uint16_t  flagOutOfPhase;  // Out of Phase adjustment (0 or 1)
} PHVOLC_Obj;

//*****************************************************************************
//
//! \brief Defines the PHVOLC handle
//
//*****************************************************************************
typedef struct _PHVOLC_Obj_ *PHVOLC_Handle;

//-----------------------------------------------------------------------------
// OutOfPhase = 0 for the out of phase correction if
// MfuncV1 is out of phase with PWM1,
// MfuncV2 is out of phase with PWM3,
// MfuncV3 is out of phase with PWM5,
// otherwise, set 0 if their phases are correct.

#define VC_ONE_THIRD 0.33333333333333f
#define VC_TWO_THIRD 0.66666666666667f
#define VC_INV_SQRT3 0.57735026918963f

// Phase Voltage Calculation
static inline void PHVOLC_run(PHVOLC_Handle handle, float32_t Vdcbus, MATH_Vec3 *pVabc_pu, MATH_Vec2 *pVab_V)
{
    PHVOLC_Obj *obj = (PHVOLC_Obj *)handle;
    float32_t   Vtemp;

    // Scale the incomming Modulation functions with the DC bus voltage value
    // and calculate the 3 Phase voltages
    Vtemp = Vdcbus * VC_ONE_THIRD;

    obj->VphaseA = Vtemp * (pVabc_pu->value[0] * 2.0f - pVabc_pu->value[1] - pVabc_pu->value[2]);

    obj->VphaseB = Vtemp * (pVabc_pu->value[1] * 2.0f - pVabc_pu->value[0] - pVabc_pu->value[2]);

    obj->VphaseC = Vtemp * (pVabc_pu->value[2] * 2.0f - pVabc_pu->value[1] - pVabc_pu->value[0]);

    if (obj->flagOutOfPhase == 1)
    {
        obj->VphaseA = -obj->VphaseA;
        obj->VphaseB = -obj->VphaseB;
        obj->VphaseC = -obj->VphaseC;
    }

    // Voltage transformation (a,b,c)  ->  (Alpha,Beta)
    obj->Valpha = obj->VphaseA;
    obj->Vbeta  = (obj->VphaseA + obj->VphaseB * 2.0f) * VC_INV_SQRT3;

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

#endif  // end of VOLT_CAL_H defines
