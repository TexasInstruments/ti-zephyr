// #############################################################################
//
//  FILE:   filter_notch.c
//
//  TITLE:  second-order notch filter
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

#include "filter_notch.h"

//*****************************************************************************
//
// FILTER_NOTCH_init
//
//*****************************************************************************
FILTER_NOTCH_Handle FILTER_NOTCH_init(void *pMemory, const size_t numBytes)
{
    FILTER_NOTCH_Handle handle;

    if ((int16_t)numBytes < (int16_t)sizeof(FILTER_NOTCH_Obj))
    {
        return ((FILTER_NOTCH_Handle)NULL);
    }

    handle = (FILTER_NOTCH_Handle)pMemory;

    return (handle);
}  // end of FILTER_NOTCH_init() function

//*****************************************************************************
//
// FILTER_NOTCH_init
//
//*****************************************************************************
FILTER_NOTCH_Coeff_Handle FILTER_COEFF_init(void *pMemory, const size_t numBytes)
{
    FILTER_NOTCH_Coeff_Handle handle;

    if ((int16_t)numBytes < (int16_t)sizeof(FILTER_NOTCH_CoeffObj))
    {
        return ((FILTER_NOTCH_Coeff_Handle)NULL);
    }

    handle = (FILTER_NOTCH_Coeff_Handle)pMemory;

    return (handle);
}  // end of FILTER_COEFF_init() function

//*****************************************************************************
//
// FILTER_COEFF_update
//
//*****************************************************************************
void FILTER_NOTCH_reset(FILTER_NOTCH_Handle handle)
{
    FILTER_NOTCH_Obj *obj = (FILTER_NOTCH_Obj *)handle;

    // reset the Notch filer
    obj->in   = 0.0f;
    obj->in1  = 0.0f;
    obj->in2  = 0.0f;
    obj->out  = 0.0f;
    obj->out1 = 0.0f;
    obj->out2 = 0.0f;

    return;
}  // end of FILTER_COEFF_update() function

//*****************************************************************************
//
// FILTER_COEFF_update
//
//*****************************************************************************
void FILTER_COEFF_update(FILTER_NOTCH_Coeff_Handle handle, const float32_t Ts, const float32_t freqGrid,
                         const float32_t c2, const float32_t c1)
{
    FILTER_NOTCH_CoeffObj *obj = (FILTER_NOTCH_CoeffObj *)handle;

    // Note c2<<c1 for the notch to work
    float32_t wn = 2.0f * MATH_TWO_PI * freqGrid;

    float32_t x = 2.0f * c2 * wn * Ts;
    float32_t y = 2.0f * c1 * wn * Ts;
    float32_t z = wn * Ts * wn * Ts;

    obj->b0 = (1.0f);
    obj->b1 = (x - 2.0f);
    obj->b2 = (z - x + 1.0f);
    obj->a1 = -(y - 2.0f);
    obj->a2 = -(z - y + 1.0f);

    return;
}  // end of FILTER_COEFF_update() function

// end of file
