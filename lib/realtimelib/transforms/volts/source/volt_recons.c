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

//! \file   \libraries\transforms\volts\source\volt_recons.c
//! \brief  Portable C fixed point code.  These functions define the
//!         calculate the phase voltage
//!

#include "volt_recons.h"

//*****************************************************************************
//
// VOLREC_init
//
//*****************************************************************************
VOLREC_Handle VOLREC_init(void *pMemory, const size_t numBytes)
{
    VOLREC_Handle handle;

    if ((int16_t)numBytes < (int16_t)sizeof(VOLREC_Obj))
    {
        return ((VOLREC_Handle)NULL);
    }

    // Assign the handle
    handle = (VOLREC_Handle)pMemory;

    return (handle);
}  // end of VOLREC_init() function

//*****************************************************************************
//
// VOLREC_reset
//
//*****************************************************************************
void VOLREC_reset(VOLREC_Handle handle)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;

    obj->sf     = 1.0f;
    obj->sfCalc = 1.0f;

    obj->VaSenSum = 0.0f;
    obj->VaSum    = 0.0f;

    obj->VaSenRms = 0.0f;
    obj->VaRms    = 0.0f;

    obj->numSamples = 0;

    obj->signPrev = 0;
    obj->signCurr = 0;
}

//*****************************************************************************
//
// VOLREC_setCoeffs
//
//*****************************************************************************
void VOLREC_setParams(VOLREC_Handle handle, const float32_t filterPole_rps, const float32_t ctrlFreq_Hz)
{
    VOLREC_Obj *obj = (VOLREC_Obj *)handle;
    float32_t   beta_lp_rad;
    uint16_t    cn;

    beta_lp_rad = filterPole_rps / ctrlFreq_Hz;

    obj->a1 = (beta_lp_rad - (float_t)2.0f) / (beta_lp_rad + (float_t)2.0f);
    obj->b0 = beta_lp_rad / (beta_lp_rad + (float_t)2.0f);
    obj->b1 = obj->b0;

    for (cn = 0; cn < 3; cn++)
    {
        obj->x1.value[cn] = 0.0f;
        obj->y1.value[cn] = 0.0f;

        obj->Vin_V.value[cn] = 0.0f;
        obj->Vs_V.value[cn]  = 0.0f;
    }

    obj->sf        = 0.925f;
    obj->sfCalc    = 0.925f;
    obj->threshold = MOTOR_THRESHOLD_VOLTAGE_V;  // 1.0V

    obj->VaSenSum = 0.0f;
    obj->VaSum    = 0.0f;

    obj->numSamples = 0;
    obj->minSamples = (int32_t)(ctrlFreq_Hz / MOTOR_MAX_ELEC_FREQ_Hz);
    obj->maxSamples = (int32_t)(ctrlFreq_Hz / MOTOR_MIN_ELEC_FREQ_Hz);

    obj->signPrev = 0;
    obj->signCurr = 0;

    return;
}

// end of file
