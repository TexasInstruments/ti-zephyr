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

#ifndef VOLT_RECONS_H
#define VOLT_RECONS_H

//! \file   \libraries\transforms\volts\include\volt_recons.h
//! \brief  Contains the public interface to the
//!         calculate the phase voltage
//!

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
//! \defgroup VOLT_RECONS VOLT_RECONS
//! @{
//
//*****************************************************************************

#include "types.h"
#include "math_types.h"

#define MOTOR_MIN_ELEC_FREQ_Hz    5.0f
#define MOTOR_MAX_ELEC_FREQ_Hz    800.0f
#define MOTOR_THRESHOLD_VOLTAGE_V 0.20f

//*****************************************************************************
//
//! \brief Defines the VOLREC controller object
//
//*****************************************************************************
typedef struct _VOLREC_Obj_
{
    float32_t a1;     //!< the denominator filter coefficient value for z^(-1)
    float32_t b0;     //!< the numerator filter coefficient value for z^0
    float32_t b1;     //!< the numerator filter coefficient value for z^(-1)
    MATH_Vec3 x1;     //!< the input value at time sample n=-1
    MATH_Vec3 y1;     //!< the output value at time sample n=-1
    MATH_Vec3 Vin_V;  //!< the input Phase voltage phase (V)
    MATH_Vec3 Vs_V;   //!< the input Phase voltage phase (V)

    float32_t sf;         //!< the scale factor of phase voltage
    float32_t sfCalc;     //!< the scale factor of phase voltage
    float32_t threshold;  //!< the voltage level corresponding to zero i/p

    float32_t VaSen;     //!< the input Phase voltage phase (V)
    float32_t VaSenSum;  //!< the square calculation over one sine cycle
    float32_t VaSum;     //!< the square calculation over one sine cycle

    float32_t VaSenRms;  //!< the RMS Value
    float32_t VaRms;     //!< the RMS Value

    int32_t numSamples;  //!< the sampling points
    int32_t minSamples;  //!< the sampling points
    int32_t maxSamples;  //!< the sampling points

    int16_t signPrev;     //!< the flag to detect ZCD
    int16_t signCurr;     //!< the flag to detect ZCD
    int16_t jitterCount;  //!< the jitter count due to noise on input

    bool flagCalSf;  //!<
} VOLREC_Obj;

//*****************************************************************************
//
//! \brief Defines the VOLREC_Handle
//
//*****************************************************************************
typedef struct _VOLREC_Obj_ *VOLREC_Handle;

//*****************************************************************************
//
//! \brief     Initializes the Voltage reconstruct module
//!
//! \param[in] pMemory   A pointer to the memory for the Voltage reconstruct object
//!
//! \param[in] numBytes  The number of bytes allocated for the Voltage reconstruct object
//!
//! \return The Voltage reconstruct (VOLREC) object handle
//
//*****************************************************************************
extern VOLREC_Handle VOLREC_init(void *pMemory, const size_t numBytes);

//*****************************************************************************
//
//! \brief     Runs the Phase Voltage reconstruction
//!
//! \param[in] handle  The Voltage reconstruct handle
//!
//! \param[in] Vdcbus  The DC bus voltage
//!
//! \param[in] pVin    The pointer to the input vector
//!
//! \param[in] pVab    The pointer to the output vector
//!
//! \return    None
//
//*****************************************************************************
static inline void VOLREC_calcVolSF(VOLREC_Handle handle, float32_t VaSen)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;

    float32_t VaNorm = fabsf(obj->Vin_V.value[0]);
    obj->VaSen       = VaSen;

    obj->signCurr = (VaNorm > obj->threshold) ? 1 : 0;
    obj->numSamples++;

    obj->VaSenSum = obj->VaSenSum + (obj->VaSen * obj->VaSen);
    obj->VaSum    = obj->VaSum + (obj->Vin_V.value[0] * obj->Vin_V.value[0]);

    if ((obj->signPrev != obj->signCurr) && (obj->signCurr == 1))
    {
        if (obj->numSamples > obj->minSamples)
        {
            float32_t invSamplesSqrt = sqrtf(1.0f / obj->numSamples);

            obj->VaSenRms = sqrtf(obj->VaSenSum) * invSamplesSqrt;
            obj->VaRms    = sqrtf(obj->VaSum) * invSamplesSqrt;

            obj->sfCalc = obj->sf * 0.8f + (obj->VaSenRms / obj->VaRms) * 0.2f;

            obj->sf = MATH_sat(obj->sfCalc, 0.95f, 0.85f);

            obj->VaSenSum = 0.0f;
            obj->VaSum    = 0.0f;

            obj->numSamples  = 0;
            obj->jitterCount = 0;
        }
        else
        {
            obj->numSamples = 0;

            if (obj->jitterCount < 25)
            {
                obj->jitterCount++;
            }
        }
    }

    if ((obj->numSamples > obj->maxSamples) || (obj->jitterCount >= 20))
    {
        obj->VaSenSum = 0.0f;
        obj->VaSum    = 0.0f;

        obj->jitterCount = 0;
        obj->numSamples  = 0;
    }

    obj->signPrev = obj->signCurr;

    return;
}

//*****************************************************************************
//
//! \brief     Runs the Phase Voltage reconstruction
//!
//! \param[in] handle  The Voltage reconstruct handle
//!
//! \param[in] Vdcbus  The DC bus voltage
//!
//! \param[in] pVin    The pointer to the input vector
//!
//! \param[in] pVab    The pointer to the output vector
//!
//! \return    None
//
//*****************************************************************************
static inline void VOLREC_run(VOLREC_Handle handle, float32_t Vdcbus, MATH_Vec3 *pVin, MATH_Vec2 *pVab)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;
    float32_t   Vtemp;
    MATH_Vec3   Vin;
    uint16_t    cn;

    // Scale the input Modulation functions with the DC bus voltage value
    // and calculate the 3 Phase voltages
    Vtemp = Vdcbus * MATH_ONE_OVER_THREE;

    Vin.value[0] = Vtemp * (pVin->value[0] * 2.0f - pVin->value[1] - pVin->value[2]);
    Vin.value[1] = Vtemp * (pVin->value[1] * 2.0f - pVin->value[0] - pVin->value[2]);
    Vin.value[2] = Vtemp * (pVin->value[2] * 2.0f - pVin->value[1] - pVin->value[0]);

    for (cn = 0; cn < 3; cn++)
    {
        // Compute the output
        // y0 = (b0 * inputValue) + (a1 * y1);
        obj->Vin_V.value[cn] =
            (obj->b0 * Vin.value[cn]) + (obj->b1 * obj->x1.value[cn]) - (obj->a1 * obj->y1.value[cn]);

        obj->x1.value[cn] = Vin.value[cn];

        obj->y1.value[cn] = obj->Vin_V.value[cn];

        obj->Vs_V.value[cn] = obj->Vin_V.value[cn] * obj->sf;
    }

    // Voltage transformation (a,b,c)  ->  (Alpha,Beta)
    pVab->value[0] = (obj->Vs_V.value[0] * 2.0f - (obj->Vs_V.value[1] + obj->Vs_V.value[2])) * MATH_ONE_OVER_THREE;

    pVab->value[1] = (obj->Vs_V.value[1] - obj->Vs_V.value[2]) * MATH_ONE_OVER_SQRT_THREE;

    return;
}

//*****************************************************************************
//
//! \brief     Gets the SF enable flag
//!
//! \param[in] handle  The Voltage reconstruct handle
//!
//! \return    flag
//
//*****************************************************************************
static inline bool VOLREC_getFlagCalSf(VOLREC_Handle handle)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;

    return (obj->flagCalSf);
}

//*****************************************************************************
//
//! \brief     Sets up the SF enable flag
//!
//! \param[in] handle  The Voltage reconstruct handle
//!
//! \return    none
//
//*****************************************************************************
static inline void VOLREC_setFlagCalSf(VOLREC_Handle handle, const bool flagCalSf)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;

    obj->flagCalSf = flagCalSf;

    return;
}

//*****************************************************************************
//
//! \brief     Enables the SF enable flag
//!
//! \param[in] handle  The Voltage reconstruct handle
//!
//! \return    none
//
//*****************************************************************************
static inline void VOLREC_enableFlagCalSf(VOLREC_Handle handle)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;

    obj->flagCalSf = true;

    return;
}

//*****************************************************************************
//
//! \brief     Disables the SF enable flag
//!
//! \param[in] handle  The Voltage reconstruct handle
//!
//! \return    none
//
//*****************************************************************************
static inline void VOLREC_disableFlagEnableSf(VOLREC_Handle handle)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;

    obj->flagCalSf = false;

    return;
}

//*****************************************************************************
//
//! \brief     reset the Phase Voltage reconstruction variables
//!
//! \param[in] handle  The Voltage reconstruction handle
//!
//! \return    None
//
//*****************************************************************************
extern void VOLREC_reset(VOLREC_Handle handle);

//*****************************************************************************
//
//! \brief     set the Phase Voltage reconstruction parameters
//!
//! \param[in] handle  The Voltage reconstruction handle
//!
//! \param[in] filterPole_rps  The voltage filter pole
//!
//! \param[in] ctrlFreq_Hz     The controller frequency
//!
//! \return    None
//
//*****************************************************************************
extern void VOLREC_setParams(VOLREC_Handle handle, const float32_t filterPole_rps, const float32_t ctrlFreq_Hz);

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

#endif  // end of VOLT_RECONS_H defines
