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
//! MotorControl SDK
//!
//! \file   libraries/observers/est/include/est_Traj_states.h
//! \brief  Contains the states for the flux
//!         estimator (EST_Flux) module routines
//!
//------------------------------------------------------------------------------

#ifndef EST_TRAJ_STATES_H
#define EST_TRAJ_STATES_H

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

//! \brief Enumeration for the trajectory generator error codes
//!
typedef enum
{
    EST_TRAJ_ERRORCODE_NOERROR = 0,  //!< no error error code
    EST_TRAJ_ERRORCODE_IDCLIP,       //!< Id clip error code
    EST_TRAJ_NUMERRORCODES           //!< the number of trajectory generator error codes
} EST_Traj_ErrorCode_e;

//! \brief Enumeration for the trajectory generator states
//!
#ifdef CLA
typedef enum
{
    EST_TRAJ_STATE_ERROR  = 0,       //!< the trajectory generator error state
    EST_TRAJ_STATE_IDLE   = 1,       //!< the trajectory generator idle state
    EST_TRAJ_STATE_EST    = 2,       //!< the trajectory generator parameter estimation state
    EST_TRAJ_STATE_ONLINE = 3,       //!< the trajectory generator online state
    EST_TRAJ_NUMSTATES    = 4,       //!< the number of trajectory generator states
    EST_TRAJ_RESERVED     = 0x10000  //!< reserved to force 32 bit data
} EST_Traj_State_e;
#else
typedef enum
{
    EST_TRAJ_STATE_ERROR  = 0,       //!< the trajectory generator error state
    EST_TRAJ_STATE_IDLE   = 1,       //!< the trajectory generator idle state
    EST_TRAJ_STATE_EST    = 2,       //!< the trajectory generator parameter estimation state
    EST_TRAJ_STATE_ONLINE = 3,       //!< the trajectory generator online state
    EST_TRAJ_NUMSTATES    = 4,       //!< the number of trajectory generator states
    EST_TRAJ_RESERVED     = 0x10000  //!< reserved to force 32 bit data
} EST_Traj_State_e;
#endif

// **************************************************************************
// the globals

// **************************************************************************
// the function prototypes

#ifdef __cplusplus
}
#endif  // extern "C"

//! @} // ingroup
#endif  // end of EST_TRAJ_STATES_H definition
