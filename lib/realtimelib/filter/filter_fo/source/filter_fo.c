// #############################################################################
//
//  FILE:   filter_fo.c
//
//  TITLE:  ARM M33 filter library, first-order
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

#include "filter_fo.h"

//*****************************************************************************
//
// FILTER_FO_getDenCoeffs
//
//*****************************************************************************
void FILTER_FO_getDenCoeffs(FILTER_FO_Handle handle, float32_t *pa1)
{
    FILTER_FO_Obj *obj = (FILTER_FO_Obj *)handle;

    *pa1 = obj->a1;

    return;
}  // end of FILTER_FO_getDenCoeffs() function

//*****************************************************************************
//
// FILTER_FO_getInitialConditions
//
//*****************************************************************************
void FILTER_FO_getInitialConditions(FILTER_FO_Handle handle, float32_t *px1, float32_t *py1)
{
    FILTER_FO_Obj *obj = (FILTER_FO_Obj *)handle;

    *px1 = obj->x1;

    *py1 = obj->y1;

    return;
}  // end of FILTER_FO_getInitialConditions() function

//*****************************************************************************
//
// FILTER_FO_getNumCoeffs
//
//*****************************************************************************
void FILTER_FO_getNumCoeffs(FILTER_FO_Handle handle, float32_t *pb0, float32_t *pb1)
{
    FILTER_FO_Obj *obj = (FILTER_FO_Obj *)handle;

    *pb0 = obj->b0;
    *pb1 = obj->b1;

    return;
}  // end of FILTER_FO_getNumCoeffs() function

//*****************************************************************************
//
// FILTER_FO_init
//
//*****************************************************************************
FILTER_FO_Handle FILTER_FO_init(void *pMemory, const size_t numBytes)
{
    FILTER_FO_Handle handle;

    if ((int16_t)numBytes < (int16_t)sizeof(FILTER_FO_Obj))
    {
        return ((FILTER_FO_Handle)NULL);
    }

    //
    // Assign the handle
    //
    handle = (FILTER_FO_Handle)pMemory;

    return (handle);
}  // end of FILTER_FO_init() function

//*****************************************************************************
//
// FILTER_FO_setDenCoeffs
//
//*****************************************************************************
void FILTER_FO_setDenCoeffs(FILTER_FO_Handle handle, const float32_t a1)
{
    FILTER_FO_Obj *obj = (FILTER_FO_Obj *)handle;

    obj->a1 = a1;

    return;
}  // end of FILTER_FO_setDenCoeffs() function

//*****************************************************************************
//
// FILTER_FO_setInitialConditions
//
//*****************************************************************************
void FILTER_FO_setInitialConditions(FILTER_FO_Handle handle, const float32_t x1, const float32_t y1)
{
    FILTER_FO_Obj *obj = (FILTER_FO_Obj *)handle;

    obj->x1 = x1;

    obj->y1 = y1;

    return;
}  // end of FILTER_FO_setInitialConditions() function

//*****************************************************************************
//
// FILTER_FO_setNumCoeffs
//
//*****************************************************************************
void FILTER_FO_setNumCoeffs(FILTER_FO_Handle handle, const float32_t b0, const float32_t b1)
{
    FILTER_FO_Obj *obj = (FILTER_FO_Obj *)handle;

    obj->b0 = b0;
    obj->b1 = b1;

    return;
}  // end of FILTER_FO_setNumCoeffs() function

// end of file
