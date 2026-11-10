// ##############################################################################
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

//------------------------------------------------------------------------------
//!
//! MotorControl SDK
//!
//! \file   \libraries\observers\esmo\source\esmo.c
//! \brief  Portable C floating-point code.  These functions define the
//!         enhanced smo estimator
//!
//------------------------------------------------------------------------------

// **************************************************************************
// the includes
#include "esmo.h"

// **************************************************************************
// the defines

// **************************************************************************
// the globals

// **************************************************************************
// the functions

ESMO_Handle ESMO_init(void *pMemory, const size_t numBytes)
{
    ESMO_Handle handle;

    if (numBytes < sizeof(ESMO_Obj))
    {
        return ((ESMO_Handle)NULL);
    }

    // assign the handle
    handle = (ESMO_Handle)pMemory;

    return (handle);
}  // end of ESMO_init() function

//------------------------------------------------------------------------------
void ESMO_resetParams(ESMO_Handle handle)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    obj->pll_ui = 0.0f;

    obj->speedEst = 0.0f;
    obj->theta    = 0.0f;

    obj->Kslide = obj->KslideMin;
    obj->pll_Kp = obj->pll_KpMin;

    return;
}

//------------------------------------------------------------------------------
void ESMO_setPLLParams(ESMO_Handle handle, const float32_t pll_KpMax, const float32_t pll_KpMin,
                       const float32_t pll_KpSF)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    obj->pll_KpMax = pll_KpMax;
    obj->pll_KpMin = pll_KpMin;
    obj->pll_KpSF  = pll_KpSF;

    return;
}

void ESMO_setPLLKi(ESMO_Handle handle, const float32_t pll_Ki)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    obj->pll_Ki = pll_Ki;

    return;
}

//------------------------------------------------------------------------------
void ESMO_setKslideParams(ESMO_Handle handle, const float32_t KslideMax, const float32_t KslideMin)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    obj->KslideMax = KslideMax;
    obj->KslideMin = KslideMin;

    return;
}

//------------------------------------------------------------------------------
void ESMO_setParams(ESMO_Handle handle, const USER_Params *pUserParams)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    obj->scaleFreq_Hz = pUserParams->maxFrequency_Hz;

    obj->speed_sf   = 1.0f / pUserParams->maxFrequency_Hz;
    obj->voltage_sf = (MATH_ONE_OVER_THREE / 4096.0f) / pUserParams->voltage_sf;
    obj->current_sf = (1.0f / 4096.0f) / pUserParams->current_sf;

    obj->Ts         = pUserParams->ctrlPeriod_sec;
    obj->base_wTs   = obj->Ts * obj->scaleFreq_Hz;
    obj->thetaDelta = obj->scaleFreq_Hz * obj->Ts;
    obj->thetaErrSF = MATH_ONE_OVER_TWO_PI;

    obj->offsetDelta = 0.005f;  // rad

    obj->Kslide = obj->KslideMin;

    obj->pll_Kp   = obj->pll_KpMin;
    obj->pll_ui   = 0.0f;
    obj->pll_Ki   = 0.0f;
    obj->pll_Umax = 1.0f;   // The maximum value in Per-unit format, don't change
    obj->pll_Umin = -1.0f;  // The minimum value in Per-unit format, don't change

    obj->EstIalpha = 0.0f;
    obj->EstIbeta  = 0.0f;

    obj->Zalpha = 0.0f;
    obj->Zbeta  = 0.0f;

    obj->Ealpha        = 0.0f;
    obj->Ebeta         = 0.0f;
    float32_t motor_Rs = pUserParams->motor_Rs_Ohm;

    float32_t baseVIs = (pUserParams->voltage_sf * obj->Ts) / pUserParams->current_sf;

    obj->Fdsmopos = 1.0f - (motor_Rs / pUserParams->motor_Ls_d_H * obj->Ts);
    obj->Fqsmopos = 1.0f - (motor_Rs / pUserParams->motor_Ls_q_H * obj->Ts);
    obj->Gdsmopos = baseVIs / pUserParams->motor_Ls_d_H;
    obj->Gqsmopos = baseVIs / pUserParams->motor_Ls_q_H;

    obj->Ke_PU = (pUserParams->motor_ratedFlux_Wb * pUserParams->maxFrequency_Hz * MATH_TWO_PI) /
                 (pUserParams->voltage_sf * 4096);

    ESMO_updateFilterParams(handle);

    return;
}

//------------------------------------------------------------------------------
void ESMO_updateFilterParams(ESMO_Handle handle)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    // TempVarLpf = Fc *2 * PI * Ts
    float32_t tempVarLpf = obj->lpfFc_Hz * MATH_TWO_PI * obj->Ts;

    // a1 = 1/(1+ iqTempVarLpf)
    obj->lpf_a1 = 1.0f / (1.0f + tempVarLpf);

    // b0 = 1 - a1
    obj->lpf_b0 = 1.0f - obj->lpf_a1;

    //
    obj->Kslf = obj->filterFc_sf * MATH_TWO_PI * obj->base_wTs;

    return;
}

//------------------------------------------------------------------------------
void ESMO_updatePLLParams(ESMO_Handle handle)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    float32_t speedRefAbs = fabsf(obj->speedRef);

    obj->pll_Kp = obj->pll_KpMin + obj->pll_KpSF * speedRefAbs;
    obj->pll_Kp = MATH_sat(obj->pll_Kp, obj->pll_KpMax, obj->pll_KpMin);
}

//! \brief     Runs the ESMO controller
//! \param[in] handle      The ESMO controller handle
//! \param[in] refValue    The reference value to the controller
//! \param[in] fbackValue  The feedback value to the controller
//! \param[in] pOutValue   The pointer to the controller output value
// static inline void ESMO_run(ESMO_Handle handle, _iq *pAngleValue, _iq *pSpeedValue)
void ESMO_full_run(ESMO_Handle handle, float32_t Vdcbus, MATH_vec3 *pVabc_pu, MATH_vec2 *pIabVec)
{
    ESMO_Obj *obj = (ESMO_Obj *)handle;

    // Scale the incomming modulation functions with the DC bus voltage value
    // and calculate the 3 Phase voltages
    float32_t Vtemp = Vdcbus * obj->voltage_sf;

    float32_t VphaseA = Vtemp * (pVabc_pu->value[0] * 2.0f - pVabc_pu->value[1] - pVabc_pu->value[2]);

    float32_t VphaseB = Vtemp * (pVabc_pu->value[1] * 2.0f - pVabc_pu->value[0] - pVabc_pu->value[2]);

    // Voltage transformation (a,b,c)  ->  (Alpha,Beta)
    obj->Valpha = VphaseA;
    obj->Vbeta  = (VphaseA + VphaseB * 2.0f) * MATH_ONE_OVER_SQRT_THREE;

    // Sliding mode current observer
    obj->EstIalpha = obj->Fdsmopos * obj->EstIalpha + obj->Gdsmopos * (obj->Valpha - obj->Ealpha - obj->Zalpha);

    obj->EstIbeta = obj->Fqsmopos * obj->EstIbeta + obj->Gqsmopos * (obj->Vbeta - obj->Ebeta - obj->Zbeta);

    //  Current errors
    float32_t IalphaError = obj->EstIalpha - pIabVec->value[0] * obj->current_sf;
    float32_t IbetaError  = obj->EstIbeta - pIabVec->value[1] * obj->current_sf;

    // Sliding control calculator
    obj->Zalpha = MATH_sat(IalphaError, obj->E0, -obj->E0) * obj->Kslide;
    obj->Zbeta  = MATH_sat(IbetaError, obj->E0, -obj->E0) * obj->Kslide;

    //  Sliding control filter -> back EMF calculator
    obj->Ealpha = obj->Ealpha + obj->Kslf * (obj->Zalpha - obj->Ealpha);
    obj->Ebeta  = obj->Ebeta + obj->Kslf * (obj->Zbeta - obj->Ebeta);

    obj->thetaOffset_rad = obj->speedRef * obj->offsetSF + obj->offsetDelta;
    obj->thetaElec_rad   = __atan2(obj->Ealpha, obj->Ebeta) + obj->thetaOffset_rad;

    if (obj->thetaElec_rad > MATH_PI)
    {
        obj->thetaElec_rad -= MATH_TWO_PI;
    }
    else if (obj->thetaElec_rad < (-MATH_PI))
    {
        obj->thetaElec_rad += MATH_TWO_PI;
    }

    // arc tangent of src radians (convert to per-unit: divide by 2*PI)
    float32_t thetaOffset = __atan2((obj->speedRef * obj->offsetSF), obj->Kslf) * MATH_ONE_OVER_TWO_PI;
    obj->thetaPll         = obj->theta - thetaOffset;

    // Convert per-unit angle to radians for trig functions
    float32_t thetaPll_rad = obj->thetaPll * MATH_TWO_PI;
    float32_t pllSine      = __sin(thetaPll_rad);
    float32_t pllCosine    = __cos(thetaPll_rad);

    obj->Ed     = obj->Ealpha * pllCosine + obj->Ebeta * pllSine;
    obj->Eq     = obj->Ebeta * pllCosine - obj->Ealpha * pllSine;
    obj->Eq_mag = sqrtf(obj->Ealpha * obj->Ealpha + obj->Ebeta * obj->Ebeta);

    // 0.1591549431 = 1/6.28 (1/(2*PI())
    float32_t thetaErrSF = obj->thetaErrSF;

    if (obj->Eq >= 0.0f)
    {
        thetaErrSF = -obj->thetaErrSF;
    }

    obj->thetaErr = obj->Ed * thetaErrSF / obj->Eq_mag;

    // integral term
    obj->pll_ui = (obj->pll_Ki * obj->thetaErr) + obj->pll_ui;

    // control output
    obj->pll_Out = MATH_sat((obj->pll_Kp * obj->thetaErr + obj->pll_ui), obj->pll_Umax, obj->pll_Umin);

    obj->speedEst = (obj->pll_Out + obj->speedEst) * 0.5f;

    // low pass filter for estimation speed
    obj->speedFlt = obj->lpf_b0 * obj->pll_Out + obj->lpf_a1 * obj->speedFlt;

    // speed integrate
    obj->theta = obj->theta + obj->speedFlt * obj->thetaDelta;

    if (obj->theta > 1.0f)
    {
        obj->theta -= 1.0f;
    }
    else if (obj->theta < -1.0f)
    {
        obj->theta += 1.0f;
    }

    obj->thetaEst = obj->theta * MATH_TWO_PI;

    if (obj->thetaEst > MATH_PI)
    {
        obj->thetaEst -= MATH_TWO_PI;
    }
    else if (obj->thetaEst < (-MATH_PI))
    {
        obj->thetaEst += MATH_TWO_PI;
    }

    return;
}  // end of ESMO_run() function
//----------------------------------------------------------------

// end of file
