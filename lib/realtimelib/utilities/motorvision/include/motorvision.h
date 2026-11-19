/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef MOTORVISION_H_
#define MOTORVISION_H_

#include "export_package.h"
#include "motorvision_hash.h"
#include <stdbool.h>
#include <stdint.h>

//
// Stream Skip Variable Externs
// Watch MV_streamDataSkips and MV_streamDataSkips2 in the debug view to see how many samples are being skipped.
// Ideally MV_streamDataSkips and MV_streamDataSkips2 will be zero. If non-zero, consider:
// 1. Lowering the sampling frequency
// 2. Increasing the buffer size
// 3. Increasing the baud rate
//
extern uint16_t MV_streamDataSkips;
extern uint16_t MV_streamDataSkips2;

//
// Polling State Variable Externs
//
extern uint32_t MV_pollingEnabledHashIndices0;  // Up to 64 polling variables are available
extern uint32_t MV_pollingEnabledHashIndices1;
extern int16_t  MV_pollingArraySize;     // total number of polling variables enabled
extern uint16_t MV_currentPollingIndex;  // index indicating which variable is next to be sent
extern bool     MV_writeWhileStreaming;

//
// Function Prototypes - Call the below in your application code
//

//*****************************************************************************
//
// Signal Sight Initialization function
//
// Call this function once in your initialization code. Call after
// device initialization functions, interrupt initialization functions and
// Board_init() but before enabling interrupts globally.
//
// @param uartInst  Pointer to the UART peripheral registers used for
//                  MotorVision communication (e.g. APP_UART_0_INST).
//
//*****************************************************************************
void MOTORVISION_init(void *uartInst);

//*****************************************************************************
//
// Signal Sight Capture Plot Data function
//
// Call this function where data should be sampled in your application code.
// A snapshot of the value of your plot variable will be captured and saved
// for transmission in a future call of MOTORVISION_sendPlotData().
// (example: in a CPU timer or ePWM ISR).
//
//*****************************************************************************
void MOTORVISION_capturePlotData(void);

//*****************************************************************************
//
// Signal Sight Send Data function
//
// Call this function in your application where you want to send data to the
// GUI via a communication peripheral. It is recommended to place this function
// call in a low priority location of your application, (in the main loop for
// example) since it could be blocked waiting.
//
//*****************************************************************************
void MOTORVISION_sendPlotData(void);

//*****************************************************************************
//
// Signal Sight Capture and Send Polling Data function
//
// Call this function in your application where you want to capture and send
// data from a low-priority/not time-critical variable to the  GUI via
// a communication peripheral. The function will find the next polling variable
// that is enabled (round robin), capture the variables current data, and send
// it to the GUI to be displayed by UI elements. It is recommended to place
// this function call in a low priority location of your application, (in the
// main loop for example) since it could be blocked waiting.
//
//*****************************************************************************
void MOTORVISION_captureAndSendPollingData(void);

#endif /* MOTORVISION_H_ */
