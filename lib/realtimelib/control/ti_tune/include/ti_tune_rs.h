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

//! @file   TiTune_Rs.h
//! @brief  TI Motor Control Library - Stator Resistance Tuning Module
//! @details Implements automatic stator resistance and dead-time distortion voltage
//!          identification. Applies two distinct DC currents at D-axis (zero torque)
//!          and measures voltage drop to calculate Rs and dead-time compensation.

/*
 *      Estimation of stator resistance and distortion voltage due to dead-time.
 *      The method applies two distinct and high-amplitude DC currents at d-axis, resulting zero torque.
 *      Results include motor resistance as well as cable and IGBT resistances. So, it is a good representation of real
 * system.
 *
 */

//***************************************************************************************************************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//***************************************************************************************************************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//***************************************************************************************************************************************************************************
#include <stdlib.h>
#include <stdint.h>
#include "math_types.h"

// #include "user.h"

//***************************************************************************************************************************************************************************
//! @brief Defines the Stator Resistance Tuning object
//! @details Structure containing all parameters and state variables for automatic
//!          stator resistance and dead-time distortion voltage identification
//***************************************************************************************************************************************************************************

typedef struct _TiTune_Rs_Obj_
{
    bool      VariableInit;      //!< Variable initialization enable flag
    float32_t AveragerEnable;    //!< Averaging enable flag after current stabilization
    float32_t TimeOutError;      //!< Timeout error flag - current failed to stabilize
    float32_t Mode;              //!< Test mode: 1=TestCurrent1, 2=TestCurrent2
    float32_t CtrlCycleCounter;  //!< Control cycle execution counter
    float32_t IdFbk_Average;     //!< Accumulated Id feedback sum (Per-Unit)
    float32_t VdRef_Average;     //!< Accumulated Vd reference sum (Per-Unit)
    float32_t IdFbkAve_Mode1;    //!< Averaged Id from Mode 1 (Per-Unit)
    float32_t VdRefAve_Mode1;    //!< Averaged Vd from Mode 1 (Per-Unit)
    float32_t IdErrFiltered;     //!< Filtered Id error for stabilization detection (Per-Unit)

    uint32_t  AveragingSize;       //!< Number of samples to average
    float32_t Wait4SettleCurrent;  //!< Samples to wait for current loop settling
    float32_t MaxTime4Settle;      //!< Maximum allowed settling time before timeout
    float32_t TestCurrent1;        //!< First test current amplitude (Per-Unit)
    float32_t TestCurrent2;        //!< Second test current amplitude (Per-Unit)
    float32_t IdErrLim4Settle;     //!< Id error threshold for stabilization (Per-Unit)
    float32_t LPF_IdErr_Gain;      //!< LPF gain for Id error filtering
    float32_t Vdc_Base;            //!< DC-link voltage base value
    float32_t Vph_Base;            //!< Phase voltage base value
    float32_t I_Base;              //!< Current base value
    float32_t F_Base;              //!< Frequency base value
    int32_t   i12;                 //!< Internal counter

    float32_t IdFbk;       //!< D-axis current feedback input (Per-Unit)
    float32_t VdRef;       //!< D-axis voltage reference input (Per-Unit)
    float32_t VdistGain;   //!< Angle-dependent distortion gain input
    float32_t VdistGain1;  //!< Secondary angle-dependent distortion gain

    float32_t IdRef;   //!< D-axis current reference output (Per-Unit)
    float32_t Rs;      //!< Measured stator resistance output (Per-Unit)
    float32_t Vdist;   //!< Measured distortion voltage output (Per-Unit)
    float32_t Finish;  //!< Identification completion flag

    float32_t X_base;          //!< Reactance base value
    float32_t Rs_Final;        //!< Final Rs in physical units (Ohms)
    float32_t impedance_base;  //!< Impedance base value
} TiTune_Rs_Obj;

//***************************************************************************************************************************************************************************

//***************************************************************************************************************************************************************************
//      Defines the TiTune_Rs handle
//***************************************************************************************************************************************************************************

typedef struct _TiTune_Rs_Obj_ *TiTune_Rs_Handle;

//***************************************************************************************************************************************************************************

//***************************************************************************************************************************************************************************
//      extern TiTune_Rs_Handle
//***************************************************************************************************************************************************************************
extern TiTune_Rs_Handle TiTune_Rs_init(void *pMemory, const size_t numBytes);

//***************************************************************************************************************************************************************************
//!
//! \brief     Run the TiTune Rs estimation function
//!
//! \param[in] handle       The TiTune Rs handle
//!
//! \param[in] v_dist_gain  The distortion voltage gain
//!
//! \param[in] id_fb        The feedback id value
//!
//! \param[in] vd_fb        The feedback vd value
//!
//***************************************************************************************************************************************************************************
extern void TiTune_Rs_run(TiTune_Rs_Handle handle, const float32_t v_dist_gain, const float32_t id_fb,
                          const float32_t vd_fb);

//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
