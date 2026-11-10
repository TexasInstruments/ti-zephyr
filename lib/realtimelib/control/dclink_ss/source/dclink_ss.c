// #############################################################################
//
//  FILE:   dclink_ss.c
//
//  TITLE:  ARM M33 DC-Link Single-Shunt Current Reconstruction library
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

#include "dclink_ss.h"

// **************************************************************************
// the defines

// **************************************************************************
// the globals

// **************************************************************************
// the functions

//*****************************************************************************
//
// DCLINK_SS_init
//
//*****************************************************************************
DCLINK_SS_Handle DCLINK_SS_init(void *pMemory, const size_t numBytes)
{
    DCLINK_SS_Handle dclinkHandle;

    if ((int16_t)numBytes < (int16_t)sizeof(DCLINK_SS_Obj))
    {
        return ((DCLINK_SS_Handle)NULL);
    }

    //
    // Assign the handle
    //
    dclinkHandle = (DCLINK_SS_Handle)pMemory;

    return (dclinkHandle);
}  // end of DCLINK_SS_init() function

//*****************************************************************************
//
// DCLINK_SS_setInitialConditions
//
//*****************************************************************************
void DCLINK_SS_setInitialConditions(DCLINK_SS_Handle handle, const uint16_t pwmPeriod, const float32_t SSTOffThrVs_pu)
{
    DCLINK_SS_Obj *obj = (DCLINK_SS_Obj *)handle;

    obj->minAvDuration = 90;
    obj->sampleDelay   = 50;
    obj->sector_1      = 1;
    obj->sector        = 1;

    obj->I_A.value[0] = 0.0;
    obj->I_A.value[1] = 0.0;
    obj->I_A.value[2] = 0.0;

    obj->SSTOffThrVs_pu = SSTOffThrVs_pu;

    obj->pwmPeriod = pwmPeriod;

    obj->vecArea   = 0;
    obj->vecArea_1 = 0;

    obj->flag_SST   = 0;
    obj->flag_SST_1 = 0;

    // disable full sampling by default
    obj->flagEnableFullSample = false;

    // enable sequence control by default
    obj->flagEnableSequenceControl = true;

    obj->flagRunInHighModulation = false;

    return;
}  // end of DCLINK_SS_setInitialConditions() function
//
// end of file
//
