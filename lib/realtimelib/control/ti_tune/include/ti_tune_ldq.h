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

/*
 *  If building with a C++ compiler, make all of the definitions in this header
 *  have a C binding.
 */
#ifdef __cplusplus
extern "C" {
#endif

//! @file   TiTune_LDQ.h
//! @brief  TI Motor Control Library - Ld/Lq Inductance Tuning Module
//! @details Implements automatic D-axis and Q-axis inductance identification through
//!          high-frequency voltage injection and DFT analysis. Scans rotor position
//!          to find minimum/maximum inductance and initial rotor angle.

/**
 *  \defgroup TITUNE_API_MODULE APIs for TiTune
 *  \ingroup  CONTROL_MODULE
 *
 *  Here is the list of TiTune function APIs
 *  @{
 *
 *  \file           TiTune.h
 *  \brief          Contains TiTune functions implementation for self-commissioning
 */

// the includes

#include <stdlib.h>
#include "math_types.h"
#include "types.h"

//*****************************************************************************
//
//! @brief Defines the Ld/Lq Tuning object
//! @details Structure containing all parameters and state variables for automatic
//!          D-axis and Q-axis inductance identification
//
//*****************************************************************************
typedef struct _TiTune_LDQ_Obj_
{
    bool      VariableInit;             //!< Variable initialization enable flag
    float32_t VoltageFound;             //!< Suitable injection voltage found flag
    float32_t VoltageNotFound;          //!< Voltage search failure error flag
    float32_t VolMinAssigned;           //!< Min search voltage assigned flag
    float32_t VolMaxAssigned;           //!< Max search voltage assigned flag
    float32_t ExecuteEnable;            //!< Calculation execution enable flag
    float32_t InjVolHalfPrdSampleNo;    //!< Current injection half-period sample count
    float32_t InjSampleCounter;         //!< Injection sample counter
    float32_t InjPrdCounter;            //!< Completed injection period counter
    float32_t FirstLdqEstCheck;         //!< Inductance calculation execution counter
    float32_t FirstVolMaxLimHitCheck;   //!< Max voltage limit hit counter
    float32_t FirstFreqMinLimHitCheck;  //!< Min frequency limit hit counter
    float32_t InjVolAngleStep;          //!< Injection voltage angle step (Per-Unit)
    float32_t InjVolMagn;               //!< Injected voltage magnitude (Per-Unit)
    float32_t InjVolFreq;               //!< Injected voltage frequency (Per-Unit)
    float32_t InjVolAngle;              //!< Injection voltage angle (Per-Unit)
    float32_t InjVolAngle_ZOHComp;      //!< ZOH-compensated injection angle (Per-Unit)
    float32_t ElecThetaUnsat;           //!< Unsaturated electrical angle (Per-Unit)
    float32_t MinSearchVol;             //!< Minimum voltage search value (Per-Unit)
    float32_t MaxSearchVol;             //!< Maximum voltage search value (Per-Unit)
    float32_t InputGainDFT;             //!< DFT input scaling coefficient
    float32_t CosDFT;                   //!< Cosine value for DFT computation
    float32_t SinDFT;                   //!< Sine value for DFT computation
    float32_t VdRealDFT;                //!< Real part of D-axis voltage DFT (Per-Unit)
    float32_t VdImagDFT;                //!< Imaginary part of D-axis voltage DFT (Per-Unit)
    float32_t IdRealDFT;                //!< Real part of D-axis current DFT (Per-Unit)
    float32_t IdImagDFT;                //!< Imaginary part of D-axis current DFT (Per-Unit)
    float32_t IqRealDFT;                //!< Real part of Q-axis current DFT (Per-Unit)
    float32_t IqImagDFT;                //!< Imaginary part of Q-axis current DFT (Per-Unit)
    float32_t VdMagnDFT;                //!< D-axis voltage peak magnitude (Per-Unit)
    float32_t IdMagnDFT;                //!< D-axis current peak magnitude (Per-Unit)
    float32_t IqMagnDFT;                //!< Q-axis current peak magnitude (Per-Unit)
    float32_t IsMagn;                   //!< Stator current vector magnitude (Per-Unit)
    float32_t Lpu;                      //!< Calculated inductance at current angle (Per-Unit)
    float32_t VdPhaseDFT;               //!< D-axis voltage phase angle (Per-Unit)
    float32_t IdPhaseDFT;               //!< D-axis current phase angle (Per-Unit)

    float32_t InjVolHalfPrdSampleNoMax;   //!< Max injection half-period samples (min frequency)
    float32_t InjVolHalfPrdSampleNoInit;  //!< Initial injection half-period samples
    float32_t InjSettlePrd;               //!< Injection periods for settling
    float32_t AngIntgGain;                //!< Angle integration gain: Fbase*Tsample
    float32_t ElecThetaStep;              //!< Electrical angle increment (Per-Unit)
    float32_t InjVolMagnInit;             //!< Initial injection voltage magnitude (Per-Unit)
    float32_t IsMagnMinLim;               //!< Minimum current magnitude limit (Per-Unit)
    float32_t IsMagnMaxLim;               //!< Maximum current magnitude limit (Per-Unit)
    float32_t InjVolMagnMaxLim;           //!< Maximum injection voltage limit (Per-Unit)
    float32_t Vdc_Base;                   //!< DC-link voltage base value
    float32_t Vph_Base;                   //!< Phase voltage base value
    float32_t I_Base;                     //!< Current base value
    float32_t F_Base;                     //!< Frequency base value

    float32_t Udc;    //!< DC-link voltage feedback input (Per-Unit)
    float32_t IdFbk;  //!< D-axis current feedback input (Per-Unit)
    float32_t IqFbk;  //!< Q-axis current feedback input (Per-Unit)

    float32_t InjVol;             //!< Injected voltage reference output (Per-Unit)
    float32_t ElecTheta;          //!< Electrical angle output (Per-Unit)
    float32_t LminApprox;         //!< Approximate minimum inductance (Per-Unit)
    float32_t LmaxApprox;         //!< Approximate maximum inductance (Per-Unit)
    float32_t InitialRotorAngle;  //!< Approximate initial rotor angle (Per-Unit)
    float32_t Finish;             //!< Procedure completion flag

    // Below arrays removed to reduce memory usage size. Meant for debugging routine.
    //    float32_t Ind_Total[160];
    //    float32_t Ang_Total[160];
    //    uint16_t i12;

    float32_t X_base;           //!< Reactance base value
    float32_t flux_base;        //!< Flux linkage base value
    float32_t Lam_PM;           //!< PM flux linkage
    float32_t inductance_base;  //!< Inductance base value

    float32_t ld_Final;  //!< Final Ld in physical units
    float32_t lq_Final;  //!< Final Lq in physical units

} TiTune_LDQ_Obj;

//*****************************************************************************
//
//! brief Defines the TiTune handle
//
//*****************************************************************************
typedef struct _TiTune_LDQ_Obj_ *TiTune_LDQ_Handle;

//*****************************************************************************

//*****************************************************************************
// the function prototypes

//*****************************************************************************
//
//! brief     Initializes the TiTune
//!
//! param[in] pMemory   A pointer to the memory for the TiTune object
//!
//! param[in] numBytes  The number of bytes allocated for the TiTune
//!
//! return    The TiTune object handle
//
//*****************************************************************************
extern TiTune_LDQ_Handle TiTune_LDQ_init(void *pMemory, const size_t numBytes);

//*****************************************************************************
//!
//! \brief     Run the TiTune function
//!
//! \param[in] handle       The TiTune handle
//!
//! \param[in] vdc_ref      The DC link voltage reference value
//!
//! \param[in] id_fb        The feedback id value
//!
//! \param[in] iq_fb        The feedback iq value
//!
//*****************************************************************************
extern void TiTune_LDQ_run(TiTune_LDQ_Handle handle, const float32_t vdc_ref, const float32_t id_fb,
                           const float32_t iq_fb);

/**
 *  Mark the end of the C bindings section for C++ compilers.
 */

#ifdef __cplusplus
}
#endif
