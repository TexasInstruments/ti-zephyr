// #############################################################################
//
//  FILE:   pid.c
//
//  TITLE:  ARM M33 Proportional-Integral-Derivative (PID) controller
//          library (floating point)
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

#include "pid.h"

//*****************************************************************************
//
// PID_getDerFilterParams
//
//*****************************************************************************
void PID_getDerFilterParams(PID_Handle handle, float32_t *b0, float32_t *b1, float32_t *a1, float32_t *x1,
                            float32_t *y1)
{
    PID_Obj *obj = (PID_Obj *)handle;

    *b0 = FILTER_FO_get_b0(obj->derFilterHandle);
    *b1 = FILTER_FO_get_b1(obj->derFilterHandle);
    *a1 = FILTER_FO_get_a1(obj->derFilterHandle);
    *x1 = FILTER_FO_get_x1(obj->derFilterHandle);
    *y1 = FILTER_FO_get_y1(obj->derFilterHandle);

    return;
}  // end of PID_getDerFilterParams() function

//*****************************************************************************
//
// PID_init
//
//*****************************************************************************
PID_Handle PID_init(void *pMemory, const size_t numBytes)
{
    PID_Handle handle;
    PID_Obj   *obj;

    //
    // Check to ensure memory allotted can accommodate object memory needs
    //
    if (numBytes < sizeof(PID_Obj))
    {
        return ((PID_Handle)NULL);
    }

    //
    // Assign the handle
    //
    handle = (PID_Handle)pMemory;

    //
    // Assign a pointer
    //
    obj = (PID_Obj *)handle;

    //
    // Initialize the derivative filter object
    //
    obj->derFilterHandle = FILTER_FO_init(&(obj->derFilter), sizeof(obj->derFilter));

    return (handle);
}  // end of PID_init() function

//*****************************************************************************
//
// PID_setDerFilterParams
//
//*****************************************************************************
void PID_setDerFilterParams(PID_Handle handle, const float32_t b0, const float32_t b1, const float32_t a1,
                            const float32_t x1, const float32_t y1)
{
    PID_Obj *obj = (PID_Obj *)handle;

    FILTER_FO_setNumCoeffs(obj->derFilterHandle, b0, b1);
    FILTER_FO_setDenCoeffs(obj->derFilterHandle, a1);
    FILTER_FO_setInitialConditions(obj->derFilterHandle, x1, y1);

    return;
}  // end of PID_setDerFilterParams() function

// end of file
