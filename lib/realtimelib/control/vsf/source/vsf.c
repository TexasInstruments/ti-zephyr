// #############################################################################
//
//  FILE:   vsf.c
//
//  TITLE:  variable switching frequency (VSF)
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

// **************************************************************************
// the includes
#include "vsf.h"
#include "mtr_config.h"

// **************************************************************************
// the defines

// **************************************************************************
// the globals

// **************************************************************************
// the functions

VSF_Handle VSF_init(void *pMemory, const size_t numBytes)
{
    VSF_Handle vsfHandle;

    if (numBytes < sizeof(VSF_Obj))
    {
        return ((VSF_Handle)NULL);
    }

    // assign the handle
    vsfHandle = (VSF_Handle)pMemory;

    return (vsfHandle);
}  // end of VSF_init() function

void VSF_initParams(VSF_Handle vsfHandle, USER_Params *pUserParams)
{
    VSF_Obj *vsfObj = (VSF_Obj *)vsfHandle;

    // setup the Counter-Compare Control Register (CMPCTL)
    vsfObj->cpuFreq_Hz       = (uint32_t)pUserParams->systemFreq_MHz * 1000000L;
    vsfObj->pwmFreqSet_Hz    = (uint16_t)(USER_M1_PWM_FREQ_kHz * 1000.0);
    vsfObj->pwmFreqTarget_Hz = vsfObj->pwmFreqSet_Hz;
    vsfObj->pwmFreqNow_Hz    = vsfObj->pwmFreqSet_Hz;
    vsfObj->pwmFreqDelta_Hz  = VSF_NUM_DELTA;

    vsfObj->pwmFreqMax_Hz = VSF_NUM_MAX_FREQ_HZ;
    vsfObj->pwmFreqMin_Hz = VSF_NUM_MIN_FREQ_HZ;

    vsfObj->pwmPeriod    = (vsfObj->cpuFreq_Hz >> 1) / vsfObj->pwmFreqNow_Hz;
    vsfObj->pwmPeriodNow = vsfObj->pwmPeriod;

    vsfObj->pwmWaitTime = VSF_NUM_WAIT_TIME;
    vsfObj->pwmCounter  = 0;

    vsfObj->state = VSF_STATE_IDLE;

    return;
}

void VSF_computeFreqParams(VSF_Handle vsfHandle)
{
    VSF_Obj *vsfObj = (VSF_Obj *)vsfHandle;

    // Check the setting PWM frequency if it's in the limitation range
    if (vsfObj->pwmFreqSet_Hz > vsfObj->pwmFreqMax_Hz)
    {
        vsfObj->pwmFreqTarget_Hz = vsfObj->pwmFreqMax_Hz;
    }
    else if (vsfObj->pwmFreqSet_Hz < vsfObj->pwmFreqMin_Hz)
    {
        vsfObj->pwmFreqTarget_Hz = vsfObj->pwmFreqMin_Hz;
    }
    else
    {
        vsfObj->pwmFreqTarget_Hz = vsfObj->pwmFreqSet_Hz;
    }

    // Calculate the PWM frequency and Period
    if (vsfObj->pwmFreqTarget_Hz != vsfObj->pwmFreqNow_Hz)
    {
        if (vsfObj->state == VSF_STATE_IDLE)
        {
            if (vsfObj->pwmFreqTarget_Hz > vsfObj->pwmFreqNow_Hz)
            {
                vsfObj->pwmFreqNow_Hz += vsfObj->pwmFreqDelta_Hz;

                if (vsfObj->pwmFreqNow_Hz >= vsfObj->pwmFreqTarget_Hz)
                {
                    vsfObj->pwmFreqNow_Hz = vsfObj->pwmFreqTarget_Hz;
                }
            }
            else
            {
                vsfObj->pwmFreqNow_Hz -= vsfObj->pwmFreqDelta_Hz;

                if (vsfObj->pwmFreqNow_Hz <= vsfObj->pwmFreqTarget_Hz)
                {
                    vsfObj->pwmFreqNow_Hz = vsfObj->pwmFreqTarget_Hz;
                }
            }

            vsfObj->pwmPeriod = (vsfObj->cpuFreq_Hz >> 1) / vsfObj->pwmFreqNow_Hz;
            vsfObj->state     = VSF_STATE_PERIOD_SET;
        }
    }

    return;
}

// end of file
