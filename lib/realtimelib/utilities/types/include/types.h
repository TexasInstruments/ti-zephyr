// #############################################################################
//
//! \file   types.h
//!
//! \brief  ARM M33 type definitions library
//!
//! \details Common type definitions and structures for motor control
//!          applications, including complex number types and status codes
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

#ifndef TYPES_H
#define TYPES_H

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
//! \defgroup TYPES TYPES
//! @{
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

//// Include CMSIS types for ARM M33 compatibility
// #include "arm_math_types.h"

//*****************************************************************************
//
// 32-bit & 64-bit float type (use CMSIS definitions if available)
//
//*****************************************************************************
#ifndef ARM_MATH_TYPES_H_
// Only define if not already defined by arm_math_types.h
typedef float  float32_t;
typedef double float64_t;
#endif  // ARM_MATH_TYPES_H_

//*****************************************************************************
//
//! \brief Defines high
//
//*****************************************************************************
#ifndef HIGH
#define HIGH 1
#endif

//*****************************************************************************
//
//! \brief Defines low
//
//*****************************************************************************
#define LOW 0

//*****************************************************************************
//
//! \brief Defines off
//
//*****************************************************************************
#define OFF 0

//*****************************************************************************
//
//! \brief Defines ok
//
//*****************************************************************************
#define OK 0

//*****************************************************************************
//
//! \brief Defines on
//
//*****************************************************************************
#define ON 1

//*****************************************************************************
//
//! \brief Defines generic error
//
//*****************************************************************************
#define ERROR 1

//*****************************************************************************
//
//! \brief Defines pass
//
//*****************************************************************************
#define PASS 1

//*****************************************************************************
//
//! \brief Defines fails
//
//*****************************************************************************
#define FAIL 0

//*****************************************************************************
//
//! \brief Defines the portable data type for a status result
//
//*****************************************************************************
typedef unsigned int status;

//*****************************************************************************
//
//! \brief Defines the portable data type for 32 bit, signed floating-point
//!        data
//
//*****************************************************************************
typedef float float_t;

//*****************************************************************************
//
//! \brief Defines the portable data type for 64 bit, signed floating-point
//!        data
//
//*****************************************************************************
typedef double double_t;

//*****************************************************************************
//
//! \brief Define the complex data type for at least 8 bit signed real and
//!        imaginary components
//
//*****************************************************************************
typedef struct _cplx_int_least8_t
{
    int_least8_t imag;
    int_least8_t real;
} cplx_int_least8_t;

//*****************************************************************************
//
//! \brief Define the complex data type for at least 8 bit unsigned real and
//!        imaginary components
//
//*****************************************************************************
typedef struct _cplx_uint_least8_t
{
    uint_least8_t imag;
    uint_least8_t real;
} cplx_uint_least8_t;

//*****************************************************************************
//
//! \brief Define the complex data type for at least 16 bit signed real and
//!        imaginary components
//
//*****************************************************************************
typedef struct _cplx_least16_t
{
    int_least16_t imag;
    int_least16_t real;
} cplx_int_least16_t;

//*****************************************************************************
//
//! \brief Define the complex data type for at least 16 bit unsigned real and
//!        imaginary components
//
//*****************************************************************************
typedef struct _cplx_uleast16_t
{
    uint_least16_t imag;
    uint_least16_t real;
} cplx_uint_least16_t;

//*****************************************************************************
//
//! \brief Define the complex data type for at least 32 bit signed real and
//!        imaginary components
//
//*****************************************************************************
typedef struct _cplx_int_least32_t_
{
    int_least32_t imag;
    int_least32_t real;
} cplx_int_least32_t;

//*****************************************************************************
//
//! \brief Define the complex data type for at least 32 bit unsigned real and
//!        imaginary components
//
//*****************************************************************************
typedef struct _cplx_uint_least32_t_
{
    uint_least32_t imag;
    uint_least32_t real;
} cplx_uint_least32_t;

//*****************************************************************************
//
//! \brief Define the complex data type for 16 bit signed real and imaginary
//!        components
//
//*****************************************************************************
typedef struct _cplx_int16_t_
{
    int16_t imag;
    int16_t real;
} cplx_int16_t;

//*****************************************************************************
//
//! \brief Define the complex data type for 16 bit unsigned real and imaginary
//!        components
//
//*****************************************************************************
typedef struct _cplx_uint16_t_
{
    uint16_t imag;
    uint16_t real;
} cplx_uint16_t;

//*****************************************************************************
//
//! \brief Define the complex data type for 32 bit signed real and imaginary
//!        components
//
//*****************************************************************************
typedef struct _cplx_int32_t
{
    int32_t imag;
    int32_t real;
} cplx_int32_t;

//*****************************************************************************
//
//! \brief Define the complex data type for 32 bit unsigned real and imaginary
//!        components
//
//*****************************************************************************
typedef struct _cplx_uint32_t
{
    uint32_t imag;
    uint32_t real;
} cplx_uint32_t;

//-----------------------------------------------------------------------------
//*****************************************************************************
//
//! \brief Define the complex data type for float real and imaginary
//!        components
//!
//
//*****************************************************************************
typedef struct _cplx_float_t
{
    float_t real;
    float_t imag;
} cplx_float_t;

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

#endif  // TYPES_H
