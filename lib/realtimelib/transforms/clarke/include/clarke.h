// #############################################################################
//
//! \file   clarke.h
//!
//! \brief  ARM M33 Clarke transform library
//!
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

#ifndef _CLARKE_H_
#define _CLARKE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \defgroup TRANSFORMS_API_MODULE APIs for Transformation Algorithms
 *  \ingroup  MOTOR_CONTROL_API
 *
 *  Here is the list of motor transformation function APIs
 *  @{
 *
 *  \file           clarke.h
 *  \brief          Contains abc to alpha/beta/0 transform implementation
 */

typedef float float32_t;

#define ONE_OVER_THREE      0.33333333333333f
#define ONE_OVER_SQRT_THREE 0.57735026918963f

//! \brief     Runs the Clarke transform module for three inputs
//!
//! \param[in] inIa     Input current value in a-axis
//! \param[in] inIb     Input current value in b-axis
//! \param[in] inIc     Input current value in c-axis
//! \param[in] pIalpha  Output pointer to current value in alpha-axis
//! \param[in] pIbeta   Output pointer to current value in beta-axis
//!
static inline __attribute__((always_inline)) void CLARKE_run_threeInput(const float32_t inIa, const float32_t inIb,
                                                                 const float32_t inIc, float32_t* pIalpha,
                                                                 float32_t* pIbeta)
{
    *pIalpha = ((2.0f * inIa) - (inIb + inIc)) * ONE_OVER_THREE;
    *pIbeta  = (inIb - inIc) * ONE_OVER_SQRT_THREE;
}

//! \brief     Runs the Clarke transform module for two inputs
//!
//! \param[in] inIa     Input current value in a-axis
//! \param[in] inIb     Input current value in b-axis
//! \param[in] pIalpha  Output pointer to current value in alpha-axis
//! \param[in] pIbeta   Output pointer to current value in beta-axis
//!
static inline __attribute__((always_inline)) void CLARKE_run_twoInput(const float32_t inIa, const float32_t inIb,
                                                               float32_t* pIalpha, float32_t* pIbeta)
{
    *pIalpha = inIa;
    *pIbeta  = (inIa + (2.0f * inIb)) * ONE_OVER_SQRT_THREE;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // _CLARKE_H_
