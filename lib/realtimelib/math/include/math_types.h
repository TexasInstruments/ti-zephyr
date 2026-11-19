// #############################################################################
//
//! \file   math_types.h
//!
//! \brief  ARM M33 math library (floating point)
//!
//! \details This library provides mathematical functions and constants
//!          optimized for ARM M33 processors with hardware-accelerated
//!          math operations through the fastrts_tmu library.
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

#ifndef TI_MATH_H
#define TI_MATH_H

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
//! \defgroup MATH MATH
//! @{
//
//*****************************************************************************

#include <math.h>
#include "types.h"

// Include TMU functions for hardware-accelerated math on ARM M33
#include <mathlib/fastrts_tmu.h>

// The Zephyr mathlib does not remap the C library names (the SDK header does),
// so remap the same functions here.
#define sinf   sinf_tmu
#define cosf   cosf_tmu
#define asinf  asinf_tmu
#define acosf  acosf_tmu
#define atanf  atanf_tmu
#define atan2f atan2f_tmu
#define expf   expf_tmu
#define logf   logf_tmu
#define sqrtf  sqrtf_tmu

//*****************************************************************************
//
// ARM M33 compatibility macros for C2000 intrinsics
//
//*****************************************************************************

//! \brief Saturation macro using TMU or standard math
#define __fsat(x, max, min) (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))

//! \brief Min/Max macros
#define __min(a, b) (((a) < (b)) ? (a) : (b))
#define __max(a, b) (((a) > (b)) ? (a) : (b))

//! \brief Trigonometric functions using TMU (radian input)
#define __sin(x)      sinf(x)
#define __cos(x)      cosf(x)
#define __sqrt(x)     sqrtf(x)
#define __atan2(y, x) atan2f(y, x)

//! \brief Per-unit trigonometric functions (input in per-unit: 0-1 = 0-2*pi)
static inline float __sinpuf32(float x)
{
    return sinf(x * FASTRTS_TMU_TWO_PI);
}

static inline float __cospuf32(float x)
{
    return cosf(x * FASTRTS_TMU_TWO_PI);
}

static inline float __atan2puf32(float y, float x)
{
    return atan2f(y, x) * FASTRTS_TMU_INV_2PI;
}

//*****************************************************************************
//
//! \brief Defines conversion scale factor from N*m to lb*in
//
//*****************************************************************************
#define MATH_Nm_TO_lbin_SF ((float32_t)(8.8507457913f))

//*****************************************************************************
//
//! \brief Defines 2/3
//
//*****************************************************************************
#define MATH_TWO_OVER_THREE ((float32_t)(0.6666666666666666666666666667f))

//*****************************************************************************
//
//! \brief Defines 1/3
//
//*****************************************************************************
#define MATH_ONE_OVER_THREE ((float32_t)(0.3333333333333333333333333333f))

//*****************************************************************************
//
//! \brief Defines 1/(pi)
//
//*****************************************************************************
#define MATH_ONE_OVER_PI ((float32_t)(0.318309886183791f))

//*****************************************************************************
//
//! \brief Defines sqrt(2)
//
//*****************************************************************************
#define MATH_SQRT_TWO ((float32_t)(1.414213562373095f))

//*****************************************************************************
//
//! \brief Defines sqrt(3)
//
//*****************************************************************************
#define MATH_SQRT_THREE ((float32_t)(1.73205080756887f))

//*****************************************************************************
//
//! \brief Defines 1/sqrt(3)
//
//*****************************************************************************
#define MATH_ONE_OVER_SQRT_THREE ((float32_t)(0.5773502691896257645091487805f))

//*****************************************************************************
//
//! \brief Defines 1/(4*pi)
//
//*****************************************************************************
#define MATH_ONE_OVER_FOUR_PI ((float32_t)(0.07957747154594767f))

//*****************************************************************************
//
//! \brief Defines 1/(2*pi)
//
//*****************************************************************************
#define MATH_ONE_OVER_TWO_PI ((float32_t)(0.1591549430918954f))

//*****************************************************************************
//
//! \brief Defines pi
//
//*****************************************************************************
#define MATH_PI ((float32_t)(3.1415926535897932384626433832f))

//*****************************************************************************
//
//! \brief Defines pi per unit
//
//*****************************************************************************
#define MATH_PI_PU ((float32_t)(0.5f))

//*****************************************************************************
//
//! \brief Defines 2*pi
//
//*****************************************************************************
#define MATH_TWO_PI ((float32_t)(6.283185307179586f))

//*****************************************************************************
//
//! \brief Defines 2*pi per unit
//
//*****************************************************************************
#define MATH_TWO_PI_PU ((float32_t)(1.0f))

//*****************************************************************************
//
//! \brief Defines 4*pi
//
//*****************************************************************************
#define MATH_FOUR_PI ((float32_t)(12.56637061435917f))

//*****************************************************************************
//
//! \brief Defines 4*pi per unit
//
//*****************************************************************************
#define MATH_FOUR_PI_PU ((float32_t)(2.0f))

//*****************************************************************************
//
//! \brief Defines pi/2
//
//*****************************************************************************
#define MATH_PI_OVER_TWO ((float32_t)(1.570796326794897f))

//*****************************************************************************
//
//! \brief Defines pi/2 per unit
//
//*****************************************************************************
#define MATH_PI_OVER_TWO_PU ((float32_t)(0.25f))

//*****************************************************************************
//
//! \brief Defines pi/3
//
//*****************************************************************************
#define MATH_PI_OVER_THREE ((float32_t)(1.047197551196598f))

//*****************************************************************************
//
//! \brief Defines pi/4
//
//*****************************************************************************
#define MATH_PI_OVER_FOUR ((float32_t)(0.785398163397448f))

//*****************************************************************************
//
//! \brief Defines pi/4 per unit
//
//*****************************************************************************
#define MATH_PI_OVER_FOUR_PU ((float32_t)(0.125f))

//*****************************************************************************
//
//! \brief Defines a two element vector
//!
//! \details Used for 2D coordinate representation in control algorithms
//
//*****************************************************************************
typedef struct _MATH_Vec2_
{
    float32_t value[2];  //!< Two-element array for vector components
} MATH_Vec2;

//! \brief Typedef alias for MATH_Vec2
typedef MATH_Vec2 MATH_vec2;

//*****************************************************************************
//
//! \brief Defines a three element vector
//!
//! \details Used for 3-phase quantities and 3D coordinate representation
//
//*****************************************************************************
typedef struct _MATH_Vec3_
{
    float32_t value[3];  //!< Three-element array for vector components
} MATH_Vec3;

//! \brief Typedef alias for MATH_Vec3
typedef MATH_Vec3 MATH_vec3;

//*****************************************************************************
//
//! \brief     Finds the absolute value
//!
//! \param[in] in   The input value
//!
//! \return    The absolute value
//
//*****************************************************************************
static inline float32_t MATH_abs(const float32_t in)
{
    float32_t out = in;

    if (in < (float32_t)0.0f)
    {
        out = -in;
    }

    return (out);
}  // end of MATH_abs() function

//*****************************************************************************
//
//! \brief     Finds the maximum value between the twp input values
//!
//! \param[in] in   The input values
//!
//! \return    The absolute value
//
//*****************************************************************************
static inline float32_t MATH_max(const float32_t in1, const float32_t in2)
{
    float32_t out = in1;

    if (in1 < in2)
    {
        out = in2;
    }

    return (out);
}  // end of MATH_max() function

//*****************************************************************************
//
//! \brief     Finds the minimum value between the twp input values
//!
//! \param[in] in   The input values
//!
//! \return    The absolute value
//
//*****************************************************************************
static inline float32_t MATH_min(const float32_t in1, const float32_t in2)
{
    float32_t out = in1;

    if (in1 > in2)
    {
        out = in2;
    }

    return (out);
}  // end of MATH_min() function

//*****************************************************************************
//
//! \brief     Increments an angle value and handles wrap-around
//!
//! \param[in] angle_rad       The angle value, rad
//!
//! \param[in] angleDelta_rad  The angle increment value, rad
//!
//! \return    The incremented angle value, rad
//
//*****************************************************************************
static inline float32_t MATH_incrAngle(const float32_t angle_rad, const float32_t angleDelta_rad)
{
    float32_t angleNew_rad;

    //
    // Increment the angle
    //
    angleNew_rad = angle_rad + angleDelta_rad;

    //
    // Check for limits
    //
    //    if(angleNew_rad > MATH_PI)
    //    {
    //        angleNew_rad -= MATH_TWO_PI;
    //    }
    //    else if(angleNew_rad < (-MATH_PI))
    //    {
    //        angleNew_rad += MATH_TWO_PI;
    //    }

    angleNew_rad = (angleNew_rad > MATH_PI) ? angleNew_rad - MATH_TWO_PI : angleNew_rad;
    angleNew_rad = (angleNew_rad < -MATH_PI) ? angleNew_rad + MATH_TWO_PI : angleNew_rad;

    return (angleNew_rad);
}  // end of MATH_incrAngle() function

//*****************************************************************************
//
//! \brief     Saturates the input value between the minimum and maximum values
//!
//! \param[in] in   The input value
//!
//! \param[in] max  The maximum value allowed
//!
//! \param[in] min  The minimum value allowed
//!
//! \return    The saturated value
//
//*****************************************************************************
static inline float32_t MATH_sat(const float32_t in, const float32_t max, const float32_t min)
{
    float32_t out = in;

    //    if(in < min)
    //    {
    //        out = min;
    //    }
    //    else if(in > max)
    //    {
    //        out = max;
    //    }

    out = (out > max) ? max : out;
    out = (out < min) ? min : out;

    return (out);
}  // end of MATH_sat() function

//----------------------------------------------------------------------------
// For Motor Fault Diagnostic
//-----------------------------------------------------------------------------
//*****************************************************************************
//
//! \brief Defines a two element complex vector
//!
//! \details Used for complex number arrays in motor fault diagnostics
//
//*****************************************************************************
typedef struct _MATH_cplx_vec2_
{
    cplx_float_t value[2];  //!< Two-element array of complex numbers
} MATH_cplx_vec2;

//*****************************************************************************
//
//! \brief Defines a three element complex vector
//!
//! \details Used for 3-phase complex quantities in motor diagnostics
//
//*****************************************************************************
typedef struct _MATH_cplx_vec3_
{
    cplx_float_t value[3];  //!< Three-element array of complex numbers
} MATH_cplx_vec3;

//*****************************************************************************
//
//! \brief Defines 6*pi
//
//*****************************************************************************
#define MATH_SIX_PI ((float32_t)(18.8495559215f))

//*****************************************************************************
//
//! \brief Defines sqrt(3)/2
//
//*****************************************************************************
#define MATH_SQRTTHREE_OVER_TWO ((float32_t)(0.8660254038f))

//*****************************************************************************
//
//! \brief     Finds the absolute value squared of a complex number
//!
//! \param[in] x1   The input complex value
//!
//! \return    The absolute value squared
//
//*****************************************************************************
static inline float32_t MATH_cAbsSq(const cplx_float_t* x1)
{
    float32_t y;

    // Use TMU-accelerated operations
    y = (x1->real * x1->real) + (x1->imag * x1->imag);

    return y;
}

//*****************************************************************************
//
//! \brief     Returns the sign of the input value
//!
//! \param[in] in  The input value
//!
//! \return    1.0 if input is positive or zero, -1.0 if input is negative
//
//*****************************************************************************
static inline float32_t MATH_sign(const float32_t in)
{
    float32_t out = 1.0f;

    if (in < 0.0f)
    {
        out = -1.0f;
    }

    return (out);
}  // end of MATH_sign() function

//*****************************************************************************
//
//! \brief     Multiplies two complex numbers
//!
//! \param[in]  x1  Pointer to first complex number
//!
//! \param[in]  x2  Pointer to second complex number
//!
//! \param[out] y   Pointer to result complex number
//!
//! \return    None
//
//*****************************************************************************
static inline void MATH_mult_cc(cplx_float_t* x1, cplx_float_t* x2, cplx_float_t* y)
{
    y->real = x1->real * x2->real - x1->imag * x2->imag;
    y->imag = x1->real * x2->imag + x1->imag * x2->real;
    return;
}

//*****************************************************************************
//
//! \brief     Adds two complex numbers
//!
//! \param[in]  x1  Pointer to first complex number
//!
//! \param[in]  x2  Pointer to second complex number
//!
//! \param[out] y   Pointer to result complex number
//!
//! \return    None
//
//*****************************************************************************
static inline void MATH_add_cc(cplx_float_t* x1, cplx_float_t* x2, cplx_float_t* y)
{
    y->real = x1->real + x2->real;
    y->imag = x1->imag + x2->imag;
    return;
}

//*****************************************************************************
//
//! \brief     Subtracts two complex numbers
//!
//! \param[in]  x1  Pointer to first complex number
//!
//! \param[in]  x2  Pointer to second complex number
//!
//! \param[out] y   Pointer to result complex number (x1 - x2)
//!
//! \return    None
//
//*****************************************************************************
static inline void MATH_sub_cc(cplx_float_t* x1, cplx_float_t* x2, cplx_float_t* y)
{
    y->real = x1->real - x2->real;
    y->imag = x1->imag - x2->imag;
    return;
}

//*****************************************************************************
//
//! \brief     Multiplies a real number by a complex number
//!
//! \param[in]  x1  Real number
//!
//! \param[in]  x2  Pointer to complex number
//!
//! \param[out] y   Pointer to result complex number
//!
//! \return    None
//
//*****************************************************************************
static inline void MATH_mult_rc(float32_t x1, cplx_float_t* x2, cplx_float_t* y)
{
    y->real = x1 * x2->real;
    y->imag = x1 * x2->imag;
    return;
}

//*****************************************************************************
//
//! \brief     Adds a real number to a complex number
//!
//! \param[in]  x1  Real number
//!
//! \param[in]  x2  Pointer to complex number
//!
//! \param[out] y   Pointer to result complex number
//!
//! \return    None
//
//*****************************************************************************
static inline void MATH_add_rc(float32_t x1, cplx_float_t* x2, cplx_float_t* y)
{
    y->real = x1 + x2->real;
    y->imag = x2->imag;
    return;
}

//*****************************************************************************
//
//! \brief     Subtracts a complex number from a real number
//!
//! \param[in]  x1  Real number
//!
//! \param[in]  x2  Pointer to complex number
//!
//! \param[out] y   Pointer to result complex number (x1 - x2)
//!
//! \return    None
//
//*****************************************************************************
static inline void MATH_sub_rc(float32_t x1, cplx_float_t* x2, cplx_float_t* y)
{
    y->real = x1 - x2->real;
    y->imag = -x2->imag;
    return;
}

//*****************************************************************************
//
//! \brief     Subtracts a real number from a complex number
//!
//! \param[in]  x1  Pointer to complex number
//!
//! \param[in]  x2  Real number
//!
//! \param[out] y   Pointer to result complex number (x1 - x2)
//!
//! \return    None
//
//*****************************************************************************
static inline void MATH_sub_cr(cplx_float_t* x1, float32_t x2, cplx_float_t* y)
{
    y->real = x1->real - x2;
    y->imag = x1->imag;

    return;
}

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

#endif  // MATH_TYPES_H
