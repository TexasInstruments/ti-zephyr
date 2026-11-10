// #############################################################################
//
//  FILE:   ipark.h
//
//  TITLE:  ARM M33 inverse park transform library (floating point)
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

#ifndef IPARK_H
#define IPARK_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \defgroup IPARK IPARK
//! @{
//
//*****************************************************************************

#include "math_types.h"

//*****************************************************************************
//
//! \brief Defines the IPARK object
//
//*****************************************************************************
typedef struct _IPARK_Obj_
{
    float32_t sinTh;  //!< the sine of the angle between the d,q and the alpha,
                      //!< beta coordinate systems
    float32_t cosTh;  //!< the cosine of the angle between the d,q and the
                      //!< alpha, beta coordinate systems
} IPARK_Obj;

//*****************************************************************************
//
//! \brief Defines the IPARK handle
//
//*****************************************************************************
typedef struct _IPARK_Obj_ *IPARK_Handle;

//*****************************************************************************
//
//! \brief     Gets the cosine of the angle between the d,q and the alpha,
//!            beta coordinate systems
//!
//! \param[in] handle  The inverse Park transform handle
//!
//! \return    The cosine of the angle
//
//*****************************************************************************
static inline float32_t IPARK_getCosTh(IPARK_Handle handle)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    return (obj->cosTh);
}  // end of IPARK_getCosTh() function

//*****************************************************************************
//
//! \brief     Gets the cosine/sine phasor for the inverse Park transform
//!
//! \param[in] handle  The inverse Park transform handle
//!
//! \param[in] pPhasor      The pointer to the cosine/sine phasor
//!
//! \return    None
//
//*****************************************************************************
static inline void IPARK_getPhasor(IPARK_Handle handle, MATH_Vec2 *pPhasor)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    pPhasor->value[0] = obj->cosTh;
    pPhasor->value[1] = obj->sinTh;

    return;
}  // end of IPARK_getPhasor() function

//*****************************************************************************
//
//! \brief     Gets the sine of the angle between the d,q and the alpha,
//!            beta coordinate systems
//!
//! \param[in] handle  The inverse Park transform handle
//!
//! return     The sine of the angle
//
//*****************************************************************************
static inline float32_t IPARK_getSinTh(IPARK_Handle handle)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    return (obj->sinTh);
}  // end of IPARK_getSinTh() function

//*****************************************************************************
//
//! \brief     Initializes the inverse Park transform module
//!
//! \param[in] pMemory      A pointer to the memory for the inverse Park object
//!
//! \param[in] numBytes     The number of bytes allocated for the inverse Park
//!                         object, bytes
//!
//! \return The inverse Park (IPARK) object handle
//
//*****************************************************************************
extern IPARK_Handle IPARK_init(void *pMemory, const size_t numBytes);

//*****************************************************************************
//
//! \brief     Runs the inverse Park transform module
//!
//! \param[in] handle  The inverse Park transform handle
//!
//! \param[in] pInVec       The pointer to the input vector
//!
//! \param[in] pOutVec      The pointer to the output vector
//!
//! \return    None
//
//*****************************************************************************
static inline void IPARK_run(IPARK_Handle handle, const MATH_Vec2 *pInVec, MATH_Vec2 *pOutVec)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    float32_t sinTh = obj->sinTh;
    float32_t cosTh = obj->cosTh;

    float32_t value_0 = pInVec->value[0];
    float32_t value_1 = pInVec->value[1];

    pOutVec->value[0] = (value_0 * cosTh) - (value_1 * sinTh);
    pOutVec->value[1] = (value_1 * cosTh) + (value_0 * sinTh);

    return;
}  // end of IPARK_run() function

//*****************************************************************************
//
//! \brief     Sets the cosine of the angle between the d,q and the alpha,
//!            beta coordinate systems
//!
//! \param[in] handle  The inverse Park transform handle
//!
//! \param[in] cosTh   The cosine of the angle
//!
//! \return    None
//
//*****************************************************************************
static inline void IPARK_setCosTh(IPARK_Handle handle, const float32_t cosTh)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    obj->cosTh = cosTh;

    return;
}  // end of IPARK_setCosTh() function

//*****************************************************************************
//
//! \brief     Sets the cosine/sine phasor for the inverse Park transform
//!
//! \param[in] handle   The inverse Park transform handle
//!
//! \param[in] pPhasor  The pointer to the cosine/sine phasor, pu
//!
//! \return    None
//
//*****************************************************************************
static inline void IPARK_setPhasor(IPARK_Handle handle, const MATH_Vec2 *pPhasor)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    obj->cosTh = pPhasor->value[0];
    obj->sinTh = pPhasor->value[1];

    return;
}  // end of IPARK_setPhasor() function

//*****************************************************************************
//
//! \brief     Sets the sine of the angle between the d,q and the alpha, beta
//!            coordinate systems
//!
//! \param[in] handle  The inverse Park transform handle
//!
//! \param[in] sinTh   The sine of the angle
//!
//! \return    None
//
//*****************************************************************************
static inline void IPARK_setSinTh(IPARK_Handle handle, const float32_t sinTh)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    obj->sinTh = sinTh;

    return;
}  // end of IPARK_setSinTh() function

//*****************************************************************************
//
//! \brief     Sets up the inverse Park transform module
//!
//! \param[in] handle  The inverse Park transform handle
//!
//! \param[in] Th      The angle between the d,q and the alpha,beta coordinate
//!                    systems, rad
//!
//! \return    None
//
//*****************************************************************************
static inline void IPARK_setup(IPARK_Handle handle, const float32_t Th)
{
    IPARK_Obj *obj = (IPARK_Obj *)handle;

    // Use TMU-accelerated sin/cos functions
    obj->sinTh = sinf(Th);
    obj->cosTh = cosf(Th);

    return;
}  // end of IPARK_setup() function

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of IPARK_H definition
