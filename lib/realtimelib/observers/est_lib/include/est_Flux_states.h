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

#ifndef EST_FLUX_STATES_H
#define EST_FLUX_STATES_H

//! \file   libraries/observers/est/include/est_Flux_states.h
//! \brief  Contains the states for the flux
//!         estimator (EST_Flux) module routines
//!

// **************************************************************************
// the includes

//!
//!
//! \addtogroup EST_LIB
//!
//! @{

#ifdef __cplusplus
extern "C" {
#endif

// **************************************************************************
// the defines

// **************************************************************************
// the typedefs

//! \brief Enumeration for the estimator error codes
//!
typedef enum
{
    EST_FLUX_ERRORCODE_NOERROR = 0,    //!< no error error code
    EST_FLUX_ERRORCODE_SHIFTOVERFLOW,  //!< flux shift overflow error code
    EST_FLUX_ERRORCODE_CLIP,           //!< flux clip error code
    EST_FLUX_NUMERRORCODES             //!< the number of estimator error codes
} EST_Flux_ErrorCode_e;

//! \brief Enumeration for the estimator states
//!
typedef enum
{
    EST_FLUX_STATE_ERROR = 0,  //!< error state
    EST_FLUX_STATE_IDLE  = 1,  //!< idle state
    EST_FLUX_STATE_CL1   = 2,  //!< closed loop control stage 1
    EST_FLUX_STATE_CL2   = 3,  //!< closed loop control stage 2
    EST_FLUX_STATE_FINE  = 4,  //!< fine estimate of flux
    EST_FLUX_STATE_DONE  = 5,  //!< done state
    EST_FLUX_NUMSTATES   = 6   //!< the number of flux estimator states
} EST_Flux_State_e;

// **************************************************************************
// the globals

// **************************************************************************
// the function prototypes

#ifdef __cplusplus
}
#endif  // extern "C"

//! @} // ingroup
#endif  // end of EST_FLUX_STATES_H definition
