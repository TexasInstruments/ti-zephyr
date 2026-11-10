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

//******************************************************************************
//
// FILE:   \libraries\control\vib_comp\source\vib_comp.c
//
// TITLE:  define the Vibration Compensation (VIB_COMP) module routines
//
//******************************************************************************

// **************************************************************************
// the includes

#include "vib_comp.h"

// **************************************************************************
// the defines

// **************************************************************************
// the globals

// **************************************************************************
// the functions

VIB_COMP_Handle VIB_COMP_init(void *pMemory, const size_t numBytes)
{
    VIB_COMP_Handle vib_compHandle;

    if (numBytes < sizeof(VIB_COMP_Obj))
    {
        return ((VIB_COMP_Handle)NULL);
    }

    // assign the handle
    vib_compHandle = (VIB_COMP_Handle)pMemory;

    return (vib_compHandle);
}  // end of VIB_COMP_init() function

void VIB_COMP_reset(VIB_COMP_Handle handle)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;
    int16_t       cnt;

    obj->flagEnableFF = false;

    obj->index = 0;

    for (cnt = 0; cnt < (int16_t)VIB_COMP_BUF_SIZE; cnt++)
    {
        obj->FF_table[cnt] = 0.0f;
    }

    return;
}  // end of VIB_COMP_reset() function

void VIB_COMPA_setParams(VIB_COMP_Handle handle, const float32_t alpha, const float32_t gain, const int16_t indexDelta,
                         const uint16_t numPolePairs)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    obj->alpha = alpha * gain;
    obj->beta  = (1.0f - alpha) * gain;

    obj->anglePairsMax  = ((float32_t)numPolePairs) * MATH_TWO_PI;
    obj->angleMechInvSf = 1.0f / ((float32_t)numPolePairs);
    obj->indexSf        = (float32_t)VIB_COMP_BUF_SIZE / MATH_TWO_PI;

    obj->angleElecPrev_rad  = 0.0f;
    obj->angleMechPoles_rad = 0.0f;

    obj->indexDelta = indexDelta;

    return;
}  // end of VIB_COMPA_setParams() function

void VIB_COMPT_setParams(VIB_COMP_Handle handle, const uint16_t numPolePairs)
{
    VIB_COMP_Obj *obj = (VIB_COMP_Obj *)handle;

    obj->anglePairsMax  = ((float32_t)numPolePairs) * MATH_TWO_PI;
    obj->angleMechInvSf = 1.0f / ((float32_t)numPolePairs);

    obj->angleElecPrev_rad  = 0.0f;
    obj->angleMechPoles_rad = 0.0f;

    return;
}  // end of VIB_COMPT_setParams() function

// end of file
