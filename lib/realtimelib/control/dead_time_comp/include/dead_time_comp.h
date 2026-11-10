// #############################################################################
//
//! @file   dead_time_comp.h
//! @brief  ARM M33 Dead Time Compensation (DTC) library public API
//!         (floating point)
//!
//! This header provides the public interface for the Dead Time Compensation
//! library. All internal implementation details are hidden from the user.
//! Users should only include this header to use the DTC functionality.
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

#ifndef DEAD_TIME_COMP_H
#define DEAD_TIME_COMP_H

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
//! \defgroup DTC_API Dead Time Compensation API
//! \brief Public API for Dead Time Compensation Library
//! @{
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//*****************************************************************************
//
//! \brief Opaque handle type for DTC object
//!
//! \details The internal structure is hidden from the user. Users should
//!          only use this handle type to interact with the DTC module.
//
//*****************************************************************************
typedef struct _DTC_Obj_ *DTC_Handle;

//*****************************************************************************
//
//! \brief Motor instance index enumeration
//!
//! \details Used to identify which motor instance to get the DTC handle for.
//!          The library internally allocates memory for each motor instance.
//
//*****************************************************************************
typedef enum _DTC_MotorInstance_
{
    DTC_MOTOR_1 = 0,   //!< Motor 1 instance
    DTC_NUM_INSTANCES  //!< Total number of supported instances
} DTC_MotorInstance_e;

//*****************************************************************************
//
//! \brief DTC initialization parameters structure
//!
//! \details This structure contains parameters needed to initialize
//!          the Dead Time Compensation module. Note: Compensation gains
//!          are set separately using DTC_updateGains() API.
//
//*****************************************************************************
typedef struct _DTC_InitParams_
{
    float maxOutFactor;          //!< Maximum output factor (0.0 to 1.0)
    float speedEnableLimit_Hz;   //!< Speed threshold to enable DTC, Hz
    float speedDisableLimit_Hz;  //!< Speed threshold to disable DTC, Hz
    bool  enableDTC;             //!< Initial enable state for DTC
} DTC_InitParams_t;

//*****************************************************************************
//
//! \brief DTC output structure
//!
//! \details Contains the compensation voltage outputs from the DTC module.
//
//*****************************************************************************
typedef struct _DTC_Output_
{
    float Vd_V;  //!< D-axis compensation voltage, V
    float Vq_V;  //!< Q-axis compensation voltage, V
} DTC_Output_t;

//*****************************************************************************
//
//! \brief Default values for DTC initialization parameters
//
//*****************************************************************************

//! \brief Default compensation gain scale factor for d-axis
#define DTC_DEFAULT_GAIN_ID_SF (0.5f)

//! \brief Default compensation gain scale factor for q-axis
#define DTC_DEFAULT_GAIN_IQ_SF (0.5f)

//! \brief Default maximum output factor (percentage of DC bus voltage)
#define DTC_DEFAULT_MAX_OUT_FACTOR (0.05f)

//! \brief Default speed threshold to enable DTC, Hz
#define DTC_DEFAULT_SPEED_ENABLE_LIMIT_HZ (10.0f)

//! \brief Default speed threshold to disable DTC, Hz
#define DTC_DEFAULT_SPEED_DISABLE_LIMIT_HZ (5.0f)

//*****************************************************************************
//
// API Function Prototypes
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief     Gets the DTC handle for a specific motor instance
//!
//! \param[in] motorInstance  The motor instance index (DTC_MOTOR_1, DTC_MOTOR_2)
//!
//! \return    The DTC object handle, or NULL if invalid motor instance
//!
//! \details   This function returns a handle to the internally allocated
//!            DTC object for the specified motor instance. The library
//!            manages all memory allocation internally.
//!
//! \note      Call DTC_initParams() after getting the handle to configure
//!            the DTC module parameters.
//
//*****************************************************************************
extern DTC_Handle DTC_getInstance(DTC_MotorInstance_e motorInstance);

//*****************************************************************************
//
//! \brief     Sets default values for DTC initialization parameters
//!
//! \param[out] pParams  Pointer to the DTC initialization parameters structure
//!
//! \return    None
//!
//! \details   This function initializes the DTC_InitParams_t structure with
//!            default values. Users can then modify specific parameters as
//!            needed before calling DTC_initParams().
//
//*****************************************************************************
extern void DTC_setDefaultParams(DTC_InitParams_t *pParams);

//*****************************************************************************
//
//! \brief     Configures the DTC module with specified parameters
//!
//! \param[in] handle   The DTC handle (from DTC_getInstance())
//! \param[in] pParams  Pointer to the DTC initialization parameters
//!
//! \return    None
//!
//! \details   This function configures the DTC module with the provided
//!            parameters. It sets up the internal controllers for both
//!            d-axis and q-axis compensation.
//!
//! \note      DTC_getInstance() must be called before this function.
//
//*****************************************************************************
extern void DTC_initParams(DTC_Handle handle, const DTC_InitParams_t *pParams);

//*****************************************************************************
//
//! \brief     Runs the dead time compensation algorithm
//!
//! \param[in] handle   The DTC handle
//! \param[in] Id_A     D-axis current feedback, A
//! \param[in] Iq_A     Q-axis current feedback, A
//!
//! \return    None
//!
//! \details   This function executes the dead time
//!            compensation algorithm. Call this function in the high-priority
//!            current control ISR. Use DTC_getOutput() to retrieve results.
//
//*****************************************************************************
extern void DTC_run(DTC_Handle handle, const float Id_A, const float Iq_A);

//*****************************************************************************
//
//! \brief     Updates the DTC coefficients based on operating conditions
//!
//! \param[in] handle       The DTC handle
//! \param[in] speed_Hz     Motor electrical speed, Hz
//! \param[in] deltaT_sec   Control loop period, seconds
//! \param[in] Vdc_V        DC bus voltage, V
//!
//! \return    None
//!
//! \details   This function updates the internal controller coefficients based
//!            on the current motor speed and DC bus voltage. Call this function
//!            in the low-priority background task.
//
//*****************************************************************************
extern void DTC_updateCoeffs(DTC_Handle handle, const float speed_Hz, const float deltaT_sec, const float Vdc_V);

//*****************************************************************************
//
//! \brief     Updates the DTC compensation gains
//!
//! \param[in] handle       The DTC handle
//! \param[in] Ki_Id        Integral gain for d-axis current controller
//! \param[in] Ki_Iq        Integral gain for q-axis current controller
//! \param[in] gainIdSf     Compensation gain scale factor for d-axis
//! \param[in] gainIqSf     Compensation gain scale factor for q-axis
//!
//! \return    None
//!
//! \details   This function updates the compensation gains based on the
//!            current controller integral gains and scale factors.
//!            Call this function after DTC_initParams() and whenever
//!            the current controller gains change.
//!
//! \note      The internal gain is calculated as: Ki * gainSf
//
//*****************************************************************************
extern void DTC_updateGains(DTC_Handle handle, const float Ki_Id, const float Ki_Iq, const float gainIdSf,
                            const float gainIqSf);

//*****************************************************************************
//
//! \brief     Gets the compensation voltage output
//!
//! \param[in]  handle   The DTC handle
//! \param[out] pOutput  Pointer to output structure to store results
//!
//! \return    None
//!
//! \details   Retrieves the d-axis and q-axis compensation voltages
//!            calculated by the last call to DTC_run().
//
//*****************************************************************************
extern void DTC_getOutput(DTC_Handle handle, DTC_Output_t *pOutput);

//*****************************************************************************
//
//! \brief     Gets the d-axis compensation voltage
//!
//! \param[in] handle   The DTC handle
//!
//! \return    D-axis compensation voltage, V
//
//*****************************************************************************
extern float DTC_getVd(DTC_Handle handle);

//*****************************************************************************
//
//! \brief     Gets the q-axis compensation voltage
//!
//! \param[in] handle   The DTC handle
//!
//! \return    Q-axis compensation voltage, V
//
//*****************************************************************************
extern float DTC_getVq(DTC_Handle handle);

//*****************************************************************************
//
//! \brief     Enables the dead time compensation
//!
//! \param[in] handle   The DTC handle
//!
//! \return    None
//
//*****************************************************************************
extern void DTC_enable(DTC_Handle handle);

//*****************************************************************************
//
//! \brief     Disables the dead time compensation
//!
//! \param[in] handle   The DTC handle
//!
//! \return    None
//
//*****************************************************************************
extern void DTC_disable(DTC_Handle handle);

//*****************************************************************************
//
//! \brief     Checks if dead time compensation is enabled
//!
//! \param[in] handle   The DTC handle
//!
//! \return    true if enabled, false if disabled
//
//*****************************************************************************
extern bool DTC_isEnabled(DTC_Handle handle);

//*****************************************************************************
//
//! \brief     Resets the DTC internal states
//!
//! \param[in] handle   The DTC handle
//!
//! \return    None
//!
//! \details   Resets all internal controller states to zero. Call this
//!            when restarting motor control or after a fault condition.
//
//*****************************************************************************
extern void DTC_reset(DTC_Handle handle);

//*****************************************************************************
//
//! \brief     Sets the compensation gain for d-axis
//!
//! \param[in] handle   The DTC handle
//! \param[in] gain     Compensation gain value
//!
//! \return    None
//
//*****************************************************************************
extern void DTC_setGainId(DTC_Handle handle, const float gain);

//*****************************************************************************
//
//! \brief     Sets the compensation gain for q-axis
//!
//! \param[in] handle   The DTC handle
//! \param[in] gain     Compensation gain value
//!
//! \return    None
//
//*****************************************************************************
extern void DTC_setGainIq(DTC_Handle handle, const float gain);

//*****************************************************************************
//
//! \brief     Sets the maximum output factor
//!
//! \param[in] handle   The DTC handle
//! \param[in] factor   Maximum output factor (0.0 to 1.0)
//!
//! \return    None
//
//*****************************************************************************
extern void DTC_setMaxOutFactor(DTC_Handle handle, const float factor);

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

#endif  // end of DEAD_TIME_COMP_H defines
