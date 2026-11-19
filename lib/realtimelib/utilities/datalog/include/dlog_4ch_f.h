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

//----------------------------------------------------------------------------------
//	FILE:			dlog_4ch_f.h
//
//	Description:    header file for data logging module
//
//	Version: 		1.0
//
//----------------------------------------------------------------------------------

#ifndef DLOG_4CH_F_H
#define DLOG_4CH_F_H

#include "types.h"

//*********** Structure Definition ********//
typedef struct
{
    float32_t *input_ptr1;
    float32_t *input_ptr2;
    float32_t *input_ptr3;
    float32_t *input_ptr4;
    float32_t *output_ptr1;
    float32_t *output_ptr2;
    float32_t *output_ptr3;
    float32_t *output_ptr4;
    float32_t  prev_value;
    float32_t  trig_value;
    int16_t    status;
    int16_t    pre_scalar;
    int16_t    skip_count;
    int16_t    size;
    int16_t    count;
} DLOG_4CH_F;

//*********** Function Declarations *******//
void DLOG_4CH_F_init(DLOG_4CH_F *v);
void DLOG_4CH_F_FUNC(DLOG_4CH_F *v);

//*********** Macro Definition ***********//
#define DLOG_4CH_F_MACRO(v)                                                  \
    switch (v.status)                                                        \
    {                                                                        \
        case 1: /* wait for trigger*/                                        \
            if (*v.input_ptr1 > v.trig_value && v.prev_value < v.trig_value) \
            {                                                                \
                /* rising edge detected start logging data*/                 \
                v.status = 2;                                                \
            }                                                                \
            break;                                                           \
        case 2:                                                              \
            v.skip_count++;                                                  \
            if (v.skip_count == v.pre_scalar)                                \
            {                                                                \
                v.skip_count           = 0;                                  \
                v.output_ptr1[v.count] = *v.input_ptr1;                      \
                v.output_ptr2[v.count] = *v.input_ptr2;                      \
                v.output_ptr3[v.count] = *v.input_ptr3;                      \
                v.output_ptr4[v.count] = *v.input_ptr4;                      \
                v.count++;                                                   \
                if (v.count == v.size)                                       \
                {                                                            \
                    v.count  = 0;                                            \
                    v.status = 1;                                            \
                }                                                            \
            }                                                                \
            break;                                                           \
        default:                                                             \
            v.status = 0;                                                    \
    }                                                                        \
    v.prev_value = *v.input_ptr1;

#endif /* DLOG_4CH_F_H_ */
