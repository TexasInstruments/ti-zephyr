// #############################################################################
//
//  FILE:   svgen.h
//
//  TITLE:  ARM M33 Space Vector Generator (SVGEN) library
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

#ifndef _SVGEN_H_
#define _SVGEN_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \addtogroup TRANSFORMS_API_MODULE APIs for motor transformations
 *  \@{
 *
 *  \file       svgen.h
 *  \brief      Contains svpwm generation implementation
 */

#include <stdint.h>

typedef float float32_t;

#define SQRT_THREE_OVER_TWO 0.8660254037844f
#define TWO_OVER_SQRT_THREE 1.15470053837926f

#if defined(__C29__)
#define MAXF(x, y)               __builtin_c29_i32_maxf32_m(x, y)
#define MINF(x, y)               __builtin_c29_i32_minf32_m(x, y)
#define MINMAXF(val, umax, umin) __builtin_c29_i32_minmaxf32_m(val, umax, umin)
#endif

//! \brief  Implements a SVM that subtracts common-mode term to achieve SV modulation.
//! \param[in] oneOverDcBUS_invV    The inverse dc bus voltage scale factor, used to convert inputs to pu, use 1 if
//! units are already in pu.
//! \param[in] inValpha             Input voltage value in alpha-axis, V or pu
//! \param[in] inVbeta              Input voltage value in beta-axis, V or pu
//! \param[in] pVa                  Output pointer to voltage value in a-axis, pu
//! \param[in] pVb                  Output pointer to voltage value in b-axis, pu
//! \param[in] pVc                  Output pointer to voltage value in c-axis, pu
//!
static inline __attribute__((always_inline)) void SVGEN_runCom(const float32_t oneOverDcBus_invV, const float32_t inValpha,
                                                        const float32_t inVbeta, float32_t* pVa, float32_t* pVb,
                                                        float32_t* pVc)
{
    float32_t vmax_pu, vmin_pu, vcom_pu = 0;

    float32_t va_pu    = inValpha * oneOverDcBus_invV;
    float32_t vbeta_pu = inVbeta * oneOverDcBus_invV;

    float32_t va_tmp = -0.5f * va_pu;
    float32_t vb_tmp = SQRT_THREE_OVER_TWO * vbeta_pu;

    //
    // -0.5*Valpha + sqrt(3)/2 * Vbeta
    //
    float32_t vb_pu = va_tmp + vb_tmp;

    //
    // -0.5*Valpha - sqrt(3)/2 * Vbeta
    //
    float32_t vc_pu = va_tmp - vb_tmp;

    //
    // Find Vmax and Vmin
    //
#if defined(__C29__)
    vmax_pu = MAXF(MAXF(va_pu, vb_pu), vc_pu);
    vmin_pu = MINF(MINF(va_pu, vb_pu), vc_pu);
#else
    if (va_pu > vb_pu)
    {
        vmax_pu = va_pu;
        vmin_pu = vb_pu;
    }
    else
    {
        vmax_pu = vb_pu;
        vmin_pu = va_pu;
    }
    if (vc_pu > vmax_pu)
    {
        vmax_pu = vc_pu;
    }
    else if (vc_pu < vmin_pu)
    {
        vmin_pu = vc_pu;
    }
#endif /*__C29__*/

    //
    // Compute Vcom = 0.5*(Vmax+Vmin)
    //
    vcom_pu = 0.5f * (vmax_pu + vmin_pu);

    //
    // Subtract common-mode term to achieve SV modulation
    //
    *pVa = (va_pu - vcom_pu);
    *pVb = (vb_pu - vcom_pu);
    *pVc = (vc_pu - vcom_pu);

    return;
}

//! \brief  Implements a DPWM that uses maximum modulation.
//! \param[in] oneOverDcBUS_invV    The inverse dc bus voltage scale factor, used to convert inputs to pu, use 1 if
//! units are already in pu.
//! \param[in] inValpha             Input voltage value in alpha-axis, V or pu
//! \param[in] inVbeta              Input voltage value in beta-axis, V or pu
//! \param[in] pVa                  Output pointer to voltage value in a-axis, pu
//! \param[in] pVb                  Output pointer to voltage value in b-axis, pu
//! \param[in] pVc                  Output pointer to voltage value in c-axis, pu
//!
static inline __attribute__((always_inline)) void SVGEN_runMax(const float32_t oneOverDcBus_invV, const float32_t inValpha,
                                                        const float32_t inVbeta, float32_t* pVa, float32_t* pVb,
                                                        float32_t* pVc)
{
    float32_t vmax_pu = 0;

    float32_t va_pu    = inValpha * oneOverDcBus_invV;
    float32_t vbeta_pu = inVbeta * oneOverDcBus_invV;

    float32_t va_tmp = -0.5f * va_pu;
    float32_t vb_tmp = SQRT_THREE_OVER_TWO * vbeta_pu;

    //
    // -0.5*Valpha + sqrt(3)/2 * Vbeta
    //
    float32_t vb_pu = va_tmp + vb_tmp;

    //
    // -0.5*Valpha - sqrt(3)/2 * Vbeta
    //
    float32_t vc_pu = va_tmp - vb_tmp;

    //
    // Find Vmax
    //
#if defined(__C29__)
    vmax_pu = MAXF(MAXF(va_pu, vb_pu), vc_pu);
#else
    if (va_pu > vb_pu)
    {
        vmax_pu = va_pu;
    }
    else
    {
        vmax_pu = vb_pu;
    }
    if (vc_pu > vmax_pu)
    {
        vmax_pu = vc_pu;
    }
#endif /*__C29__*/

    //
    // DPWM maximum modulation
    //
    *pVa = (va_pu - vmax_pu) + 0.5f;
    *pVb = (vb_pu - vmax_pu) + 0.5f;
    *pVc = (vc_pu - vmax_pu) + 0.5f;
}

//! \brief  Implements a DPWM that uses minimum modulation.
//! \param[in] oneOverDcBUS_invV    The inverse dc bus voltage scale factor, used to convert inputs to pu, use 1 if
//! units are already in pu.
//! \param[in] inValpha             Input voltage value in alpha-axis, V or pu
//! \param[in] inVbeta              Input voltage value in beta-axis, V or pu
//! \param[in] pVa                  Output pointer to voltage value in a-axis, pu
//! \param[in] pVb                  Output pointer to voltage value in b-axis, pu
//! \param[in] pVc                  Output pointer to voltage value in c-axis, pu
//!
static inline __attribute__((always_inline)) void SVGEN_runMin(const float32_t oneOverDcBus_invV, const float32_t inValpha,
                                                        const float32_t inVbeta, float32_t* pVa, float32_t* pVb,
                                                        float32_t* pVc)
{
    float32_t vmin_pu = 0;

    float32_t va_pu    = inValpha * oneOverDcBus_invV;
    float32_t vbeta_pu = inVbeta * oneOverDcBus_invV;

    float32_t va_tmp = -0.5f * va_pu;
    float32_t vb_tmp = SQRT_THREE_OVER_TWO * vbeta_pu;

    //
    // -0.5*Valpha + sqrt(3)/2 * Vbeta
    //
    float32_t vb_pu = va_tmp + vb_tmp;

    //
    // -0.5*Valpha - sqrt(3)/2 * Vbeta
    //
    float32_t vc_pu = va_tmp - vb_tmp;

    //
    // Find Vmin
    //
#if defined(__C29__)
    vmin_pu = MINF(MINF(va_pu, vb_pu), vc_pu);
#else
    if (va_pu < vb_pu)
    {
        vmin_pu = va_pu;
    }
    else
    {
        vmin_pu = vb_pu;
    }

    if (vc_pu < vmin_pu)
    {
        vmin_pu = vc_pu;
    }
#endif /*__C29__*/

    //
    // DPWM minimum modulation
    //
    *pVa = (va_pu - vmin_pu) - 0.5f;
    *pVb = (vb_pu - vmin_pu) - 0.5f;
    *pVc = (vc_pu - vmin_pu) - 0.5f;
}

//! \brief  Saturates the SVM variable base on modulation limits
//! \param[in] Umax     Maximum modulation limit
//! \param[in] Umin     Minimum modulation limit
//! \param[in] pVa      pointer to voltage value in a-axis
//! \param[in] pVb      pointer to voltage value in b-axis
//! \param[in] pVc      pointer to voltage value in c-axis
//!
static inline __attribute__((always_inline)) void SVGEN_clamp(const float32_t Umax, const float32_t Umin, float32_t* pVa,
                                                       float32_t* pVb, float32_t* pVc)
{
#if defined(__C29__)
    *pVa = MINMAXF(*pVa, Umax, Umin);
    *pVb = MINMAXF(*pVb, Umax, Umin);
    *pVc = MINMAXF(*pVc, Umax, Umin);
#else
    *pVa = (*pVa > Umax) ? Umax : (*pVa < Umin) ? Umin : *pVa;
    *pVb = (*pVb > Umax) ? Umax : (*pVb < Umin) ? Umin : *pVb;
    *pVc = (*pVc > Umax) ? Umax : (*pVc < Umin) ? Umin : *pVc;
#endif /*__C29__*/
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // _SVGEN_H_
