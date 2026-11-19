// #############################################################################
//
//! \file   rampgen.h
//!
//! \brief  ARM M33 Ramp Generator (RAMPGEN) library
//!         (floating point)
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

#ifndef RAMPGEN_H
#define RAMPGEN_H

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \addtogroup RAMPGEN
//! @{
//
//*****************************************************************************

//
// Included Files
//
#include <stdint.h>
#include <math.h>
#include "types.h"

//*****************************************************************************
//
//! \brief Defines the Ramp Signal Generator (RAMPGEN) structure
//!
//! \details The rampgen can be used to generate a ramp signal
//!          of a desired frequency
//
//*****************************************************************************
typedef volatile struct
{
    float32_t freq;          //!< Input: Ramp frequency (pu)
    float32_t stepAngleMax;  //!< Parameter: Maximum step angle (pu)
    float32_t out;           //!< Output: Ramp signal (pu)
} RAMPGEN;

//*****************************************************************************
//
//! \brief     Resets RAMPGEN internal storage data and outputs
//!
//! \param[in] v  A pointer to a RAMPGEN structure
//!
//! \return    None
//
//*****************************************************************************
static inline void RAMPGEN_reset(RAMPGEN *v)
{
    v->out = 0;
}

//*****************************************************************************
//
//! \brief     Configures RAMPGEN module
//!
//! \param[in] v             A pointer to a RAMPGEN structure
//!
//! \param[in] isrFrequency  Frequency at which the RAMPGEN is called in ISR
//!
//! \param[in] rampFrequency Frequency of the RAMP generated by RAMPGEN module
//!
//! \return    None
//
//*****************************************************************************
static inline void RAMPGEN_config(RAMPGEN *v, float32_t isrFrequency, float32_t rampFrequency)
{
    v->freq         = rampFrequency;
    v->stepAngleMax = ((float32_t)1.0) / isrFrequency;
}

//*****************************************************************************
//
//! \brief     Runs the RAMPGEN module
//!
//! \param[in] v  A pointer to a RAMPGEN structure
//!
//! \return    None
//
//*****************************************************************************
static inline void RAMPGEN_run(RAMPGEN *v)
{
    //
    // Compute the angle rate
    //
    v->out += (v->stepAngleMax * v->freq);

    //
    // Saturate the angle rate within (0,1)
    //
    if (v->out > (1.0f))
    {
        v->out -= (1.0f);
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif  // extern "C"

#endif  // end of  _RAMPGEN_H_ definition

//
// End of File
//
