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

//------------------------------------------------------------------------------
//!
//! \file   libraries/utilities/cpu_time/source/cpu_usage.c
//! \brief  Portable C fixed point code.  These functions define the
//!         CPU usage time (CPU_TIME) module routines
//!
//------------------------------------------------------------------------------

// **************************************************************************
// the includes

#include "cpu_time.h"

// **************************************************************************
// the globals

// **************************************************************************
// the functions

// Initializes the CPU TIME (CPU_TIME) object
CPU_TIME_Handle CPU_TIME_init(void *pMemory, const size_t numBytes)
{
    CPU_TIME_Handle handle;

    if (numBytes < sizeof(CPU_TIME_Obj))
    {
        return ((CPU_TIME_Handle)NULL);
    }

    // assign the handle
    handle = (CPU_TIME_Handle)pMemory;

    return (handle);
}  // end of CPU_TIME_init() function

// Resets timer count
void CPU_TIME_reset(CPU_TIME_Handle handle)
{
    CPU_TIME_Obj *obj = (CPU_TIME_Obj *)handle;
    uint16_t      cnt;

    for (cnt = 0; cnt < MOD_INDEX_MAX; cnt++)
    {
        obj->deltaNow[cnt] = 0;
        obj->deltaMin[cnt] = 0x7FFF;
        obj->deltaMax[cnt] = 0;
    }

    obj->pwmCntADC = 0;
    obj->pwmCntISR = 0;

    obj->pwmCntADCMin = 0x3FFF;
    obj->pwmCntISRMin = 0x3FFF;

    obj->pwmCntADCMax = 0;
    obj->pwmCntISRMax = 0;

    obj->flag_resetStatus = true;

    return;
}  // end of CPU_TIME_setParams() function

// end of file
