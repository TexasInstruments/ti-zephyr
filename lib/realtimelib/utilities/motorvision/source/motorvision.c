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

#include "motorvision.h"
#include "export.h"
#include <string.h>
#include "motor_common.h"
//
// Defines
//
#define MV_NUMBER_OF_SIMULTANEOUS_STREAM_VARIABLES 4
#define MV_DEFAULT_STREAM_VAR_A                    0
#define MV_DEFAULT_STREAM_VAR_B                    1
#define MV_DEFAULT_STREAM_VAR_C                    2
#define MV_DEFAULT_STREAM_VAR_D                    3
#define MV_BUFFER_SIZE                             50
#define MV_POLLING_VARIABLE_MAX                    20
#define MV_PING_PONG_BUFFERING                     true

// Handle different number of variables in hash table
#if MVHASH_NUMBER_OF_VARIABLES == 1
#define MV_DEFAULT_STREAM_VAR_B 0
#define MV_DEFAULT_STREAM_VAR_C 0
#define MV_DEFAULT_STREAM_VAR_D 0
#elif MVHASH_NUMBER_OF_VARIABLES == 2
#define MV_DEFAULT_STREAM_VAR_B 1
#define MV_DEFAULT_STREAM_VAR_C 1
#define MV_DEFAULT_STREAM_VAR_D 1
#elif MVHASH_NUMBER_OF_VARIABLES == 3
#define MV_DEFAULT_STREAM_VAR_B 1
#define MV_DEFAULT_STREAM_VAR_C 2
#define MV_DEFAULT_STREAM_VAR_D 2
#elif MVHASH_NUMBER_OF_VARIABLES >= 4
#define MV_DEFAULT_STREAM_VAR_B 1
#define MV_DEFAULT_STREAM_VAR_C 2
#define MV_DEFAULT_STREAM_VAR_D 3
#endif

//
// Enumerations
//
enum MV_STREAMING_STATES
{
    STREAM_IDLE = 0,
    STREAM1     = 1,
    STREAM2     = 2,
    STREAM3     = 3,
    STREAM4     = 4,
};

enum MV_VAR_TYPES
{
    UINT16 = 0,
    UINT32 = 1,
    INT16  = 2,
    INT32  = 3,
    BOOL   = 4,
    FLOAT  = 5,
};

enum MV_CMDS
{
    NO_UPDATE                  = 0,  // default
    PACKET_ERROR               = 39,
    INITCMD                    = 40,
    WRITE_VAR                  = 43,
    INIT_STREAMING_VARS        = 44,
    START_STREAMING_A          = 45,
    START_STREAMING_B          = 46,
    START_STREAMING_C          = 47,
    START_STREAMING_D          = 48,
    STOP_STREAMING_A           = 49,
    STOP_STREAMING_B           = 50,
    STOP_STREAMING_C           = 51,
    STOP_STREAMING_D           = 52,
    STREAM_DATA_1_VAR          = 53,
    STREAM_DATA_2_VAR          = 54,
    STREAM_DATA_3_VAR          = 55,
    STREAM_DATA_4_VAR          = 56,
    STREAM_DATA_WRITEACK_1_VAR = 57,
    STREAM_DATA_WRITEACK_2_VAR = 58,
    STREAM_DATA_WRITEACK_3_VAR = 59,
    STREAM_DATA_WRITEACK_4_VAR = 60,
    START_POLLING_1            = 63,
    START_POLLING_2            = 64,
    START_POLLING_3            = 65,
    START_POLLING_4            = 66,
    START_POLLING_5            = 67,
    START_POLLING_6            = 68,
    START_POLLING_7            = 69,
    START_POLLING_8            = 70,
    START_POLLING_9            = 71,
    START_POLLING_10           = 72,
    START_POLLING_11           = 73,
    START_POLLING_12           = 74,
    START_POLLING_13           = 75,
    STOP_POLLING               = 76,
    POLLING_DATA               = 77,
    CLEAR_STREAMING_PLOT       = 78,
};

//
// Stream State Global Variables
//
enum MV_STREAMING_STATES MV_currentStreamState                                                 = STREAM4;  // sm
int16_t                  MV_currentPlotHashIndices[MV_NUMBER_OF_SIMULTANEOUS_STREAM_VARIABLES] = {
    MV_DEFAULT_STREAM_VAR_A, MV_DEFAULT_STREAM_VAR_B, MV_DEFAULT_STREAM_VAR_C, MV_DEFAULT_STREAM_VAR_D};
bool MV_streamToggleStates[MV_NUMBER_OF_SIMULTANEOUS_STREAM_VARIABLES] = {true, true, true, true};  // sm
// bool MV_streamToggleStates[MV_NUMBER_OF_SIMULTANEOUS_STREAM_VARIABLES] = {false, false, false, false}; //sm

enum MV_CMDS MV_nextTxCMD = NO_UPDATE;

//
// Stream Buffering Global Variables - Buffer 1 (Ping)
//
float             MV_streamA_buffer[MV_BUFFER_SIZE] = {0};
float             MV_streamB_buffer[MV_BUFFER_SIZE] = {0};
float             MV_streamC_buffer[MV_BUFFER_SIZE] = {0};
float             MV_streamD_buffer[MV_BUFFER_SIZE] = {0};
volatile uint16_t MV_bufferElementIndex             = 0;
bool              MV_bufferIsFull                   = false;
uint16_t          MV_streamDataSkips                = 0;
bool              MV_initiallyClearBuffer           = true;

//
// Stream Buffering Global Variables - Buffer 2 (Pong)
//
float             MV_streamA_buffer2[MV_BUFFER_SIZE] = {0};
float             MV_streamB_buffer2[MV_BUFFER_SIZE] = {0};
float             MV_streamC_buffer2[MV_BUFFER_SIZE] = {0};
float             MV_streamD_buffer2[MV_BUFFER_SIZE] = {0};
volatile uint16_t MV_bufferElementIndex2             = 0;
bool              MV_bufferIsFull2                   = false;
bool              MV_writeBuffer1readBuffer2         = true;
uint16_t          MV_streamDataSkips2                = 0;

//
// Polling State Global Variables
//
uint16_t MV_pollingEnabledHashIndicesArray[MV_POLLING_VARIABLE_MAX] = {0};
uint32_t MV_pollingEnabledHashIndices0                              = 0;  // Up to 64 polling variables are available
uint32_t MV_pollingEnabledHashIndices1                              = 0;
int16_t  MV_pollingArraySize                                        = 0;  // total number of polling variables enabled
uint16_t MV_currentPollingIndex = 0;  // index indicating which variable is next to be sent
bool     MV_writeWhileStreaming = false;

//
// Private Function Prototypes
//
static void              MOTORVISION_resetStates(void);
static void              MOTORVISION_sendAcknowledge(enum MV_CMDS command);
static enum MV_VAR_TYPES MOTORVISION_getVariableType(int16_t hashIndex);
static void*             MOTORVISION_getVariableAddressAtHashIndex(int16_t hashIndex, enum MV_VAR_TYPES variableType);
static void              MOTORVISION_queueUpStreamData(uint16_t numChannelsON, bool buffer1);
static void              MOTORVISION_enableNewPollingVariable(uint16_t* indiceArray, uint16_t numberOfVariables);
static void              MOTORVISION_clearPollingVariables(void);
static void              MOTORVISION_writePollingVariable(uint16_t hashIndex, uint16_t* dataArray);

//
// Reset all MotorVision states
//
static void MOTORVISION_resetStates(void)
{
    // Initialize/clear buffer values
    MV_bufferElementIndex      = 0;
    MV_bufferIsFull            = false;
    MV_bufferElementIndex2     = 0;
    MV_bufferIsFull2           = false;
    MV_writeBuffer1readBuffer2 = true;

    int pollingIndex;
    for (pollingIndex = 0; pollingIndex < MV_POLLING_VARIABLE_MAX; pollingIndex++)
    {
        MV_pollingEnabledHashIndicesArray[pollingIndex] = 0xFF;
    }
}

//*****************************************************************************
//
// Motor Vision Initialization function
//
//*****************************************************************************
void MOTORVISION_init(void* uartInst)
{
    EXPORT_init(uartInst);
    MOTORVISION_resetStates();
}

//
// Send acknowledgement command
//
static void MOTORVISION_sendAcknowledge(enum MV_CMDS command)
{
    enum MV_CMDS* cmdPtr = &command;
    EXPORTPACKAGE_sendFloatArrayAsByteArray((char*)cmdPtr, 1, NULL, 0);
    MV_nextTxCMD = NO_UPDATE;
}

//
// Get variable type from hash index
//

static enum MV_VAR_TYPES MOTORVISION_getVariableType(int16_t hashIndex)
{
    enum MV_VAR_TYPES varType                             = UINT16;  // default
    int16_t startIndices[MVHASH_AMOUNT_OF_VARIABLE_TYPES] = {MVHASH_uint16_t_startIndex, MVHASH_uint32_t_startIndex,
                                                             MVHASH_int16_t_startIndex,  MVHASH_int32_t_startIndex,
                                                             MVHASH_bool_startIndex,     MVHASH_float_startIndex};
    int     typeIndex;
    for (typeIndex = 0; typeIndex < MVHASH_AMOUNT_OF_VARIABLE_TYPES; typeIndex++)
    {
        if ((startIndices[typeIndex] != -1) && (hashIndex >= startIndices[typeIndex]))
        {
            varType = (enum MV_VAR_TYPES)typeIndex;
        }
    }
    return varType;
}

//
// Get variable address from hash table
//
static void* MOTORVISION_getVariableAddressAtHashIndex(int16_t hashIndex, enum MV_VAR_TYPES variableType)
{
    volatile void* variableAddress = MVHASH_hash_table[hashIndex];

    if (variableType == BOOL)
    {
        return (bool*)variableAddress;
    }
    else if (variableType == UINT16)
    {
        return (uint16_t*)variableAddress;
    }
    else if (variableType == UINT32)
    {
        return (uint32_t*)variableAddress;
    }
    else if (variableType == INT16)
    {
        return (int16_t*)variableAddress;
    }
    else if (variableType == INT32)
    {
        return (int32_t*)variableAddress;
    }
    else if (variableType == FLOAT)
    {
        return (float*)variableAddress;
    }
    return 0;
}

//
// Queue up stream data to buffer
//
static void MOTORVISION_queueUpStreamData(uint16_t numChannelsON, bool buffer1)
{
    // Read current stream toggle states into temp array
    bool MV_streamToggleStatesTemp[4] = {false, false, false, false};
    MV_streamToggleStatesTemp[0]      = MV_streamToggleStates[0];
    MV_streamToggleStatesTemp[1]      = MV_streamToggleStates[1];
    MV_streamToggleStatesTemp[2]      = MV_streamToggleStates[2];
    MV_streamToggleStatesTemp[3]      = MV_streamToggleStates[3];

    // uint16_t channelsToStream = 0;  // Unused variable

    if (buffer1)
    {
        // Capture data and write to buffer 1
        if (MV_streamToggleStatesTemp[0])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[0], FLOAT);
            MV_streamA_buffer[MV_bufferElementIndex] = *dataPtr;
        }
        if (MV_streamToggleStatesTemp[1])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[1], FLOAT);
            MV_streamB_buffer[MV_bufferElementIndex] = *dataPtr;
        }
        if (MV_streamToggleStatesTemp[2])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[2], FLOAT);
            MV_streamC_buffer[MV_bufferElementIndex] = *dataPtr;
        }
        if (MV_streamToggleStatesTemp[3])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[3], FLOAT);
            MV_streamD_buffer[MV_bufferElementIndex] = *dataPtr;
        }
        MV_bufferElementIndex++;
        if (MV_bufferElementIndex == MV_BUFFER_SIZE)
        {
            MV_bufferIsFull       = true;
            MV_bufferElementIndex = 0;
        }
    }
    else
    {
        // Capture data and write to buffer 2
        if (MV_streamToggleStatesTemp[0])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[0], FLOAT);
            MV_streamA_buffer2[MV_bufferElementIndex2] = *dataPtr;
        }
        if (MV_streamToggleStatesTemp[1])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[1], FLOAT);
            MV_streamB_buffer2[MV_bufferElementIndex2] = *dataPtr;
        }
        if (MV_streamToggleStatesTemp[2])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[2], FLOAT);
            MV_streamC_buffer2[MV_bufferElementIndex2] = *dataPtr;
        }
        if (MV_streamToggleStatesTemp[3])
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(MV_currentPlotHashIndices[3], FLOAT);
            MV_streamD_buffer2[MV_bufferElementIndex2] = *dataPtr;
        }
        MV_bufferElementIndex2++;
        if (MV_bufferElementIndex2 == MV_BUFFER_SIZE)
        {
            MV_bufferIsFull2       = true;
            MV_bufferElementIndex2 = 0;
        }
    }
}

//*****************************************************************************
//
// Motor Vision Capture Plot Data function
//
//*****************************************************************************
// int ctr=0;
void MOTORVISION_capturePlotData(void)
{
    // Sample streaming data
    if (MV_writeBuffer1readBuffer2)
    {
        // Write sampled data to buffer 1 (if not full)
        if (MV_bufferIsFull)
        {
            MV_streamDataSkips++;
            return;
        }
        else
        {
            if (MV_currentStreamState != STREAM_IDLE)
            {
                if (MV_currentStreamState == STREAM1)
                {
                    MOTORVISION_queueUpStreamData(1, true);
                }
                else if (MV_currentStreamState == STREAM2)
                {
                    MOTORVISION_queueUpStreamData(2, true);
                }
                else if (MV_currentStreamState == STREAM3)
                {
                    MOTORVISION_queueUpStreamData(3, true);
                }
                else if (MV_currentStreamState == STREAM4)
                {
                    MOTORVISION_queueUpStreamData(4, true);
                }
            }
            else
            {
                __asm("NOP");
            }
        }
    }
    else
    {
        // Write sampled data to buffer 2 (if not full)
        if (MV_bufferIsFull2)
        {
            MV_streamDataSkips2++;
            return;
        }
        else
        {
            if (MV_currentStreamState != STREAM_IDLE)
            {
                if (MV_currentStreamState == STREAM1)
                {
                    MOTORVISION_queueUpStreamData(1, false);
                }
                else if (MV_currentStreamState == STREAM2)
                {
                    MOTORVISION_queueUpStreamData(2, false);
                }
                else if (MV_currentStreamState == STREAM3)
                {
                    MOTORVISION_queueUpStreamData(3, false);
                }
                else if (MV_currentStreamState == STREAM4)
                {
                    MOTORVISION_queueUpStreamData(4, false);
                }
            }
        }
    }
}

//*****************************************************************************
//
// Motor Vision Send Data function
//
//*****************************************************************************
void MOTORVISION_sendPlotData(void)
{
    // Check MV_nextTxCMD - Sending ack takes priority over sending data
    if (MV_nextTxCMD != NO_UPDATE)
    {
        MOTORVISION_sendAcknowledge(MV_nextTxCMD);
    }
    else if (MV_currentStreamState != STREAM_IDLE)
    {
        if (MV_writeBuffer1readBuffer2)
        {
            // Read from buffer 2
            if (MV_bufferIsFull2)
            {
                // Read current stream toggle states into temp array
                bool MV_streamToggleStatesTemp[4] = {false, false, false, false};
                MV_streamToggleStatesTemp[0]      = MV_streamToggleStates[0];
                MV_streamToggleStatesTemp[1]      = MV_streamToggleStates[1];
                MV_streamToggleStatesTemp[2]      = MV_streamToggleStates[2];
                MV_streamToggleStatesTemp[3]      = MV_streamToggleStates[3];

                // Send clear plot command before sending first packet
                if (MV_initiallyClearBuffer)
                {
                    MOTORVISION_sendAcknowledge(CLEAR_STREAMING_PLOT);
                    MV_initiallyClearBuffer = false;
                }

                // Loop through all buffer elements
                uint16_t bufferElementIndex;
                for (bufferElementIndex = 0; bufferElementIndex < MV_BUFFER_SIZE; bufferElementIndex++)
                {
                    // Read in data from buffers at bufferElementIndex
                    uint16_t insertIndex  = 0;
                    float    dataArray[4] = {0};

                    if (MV_streamToggleStatesTemp[0])
                    {
                        dataArray[insertIndex] = MV_streamA_buffer2[bufferElementIndex];
                        insertIndex++;
                    }
                    if (MV_streamToggleStatesTemp[1])
                    {
                        dataArray[insertIndex] = MV_streamB_buffer2[bufferElementIndex];
                        insertIndex++;
                    }
                    if (MV_streamToggleStatesTemp[2])
                    {
                        dataArray[insertIndex] = MV_streamC_buffer2[bufferElementIndex];
                        insertIndex++;
                    }
                    if (MV_streamToggleStatesTemp[3])
                    {
                        dataArray[insertIndex] = MV_streamD_buffer2[bufferElementIndex];
                        insertIndex++;
                    }

                    // Package and transmit data from buffers
                    if (insertIndex == 1)
                    {
                        char cmd = (char)STREAM_DATA_1_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_1_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 1);
                    }
                    else if (insertIndex == 2)
                    {
                        char cmd = (char)STREAM_DATA_2_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_2_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 2);
                    }
                    else if (insertIndex == 3)
                    {
                        char cmd = (char)STREAM_DATA_3_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_3_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 3);
                    }
                    else if (insertIndex == 4)
                    {
                        char cmd = (char)STREAM_DATA_4_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_4_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 4);
                    }
                }
                MV_bufferIsFull2 = false;
            }
            else
            {
                if (!MV_bufferIsFull2 && MV_bufferIsFull)
                {
                    MV_writeBuffer1readBuffer2 = false;
                }
                return;
            }
        }
        else
        {
            // Read from buffer 1
            if (MV_bufferIsFull)
            {
                // Read current stream toggle states into temp array
                bool MV_streamToggleStatesTemp[4] = {false, false, false, false};
                MV_streamToggleStatesTemp[0]      = MV_streamToggleStates[0];
                MV_streamToggleStatesTemp[1]      = MV_streamToggleStates[1];
                MV_streamToggleStatesTemp[2]      = MV_streamToggleStates[2];
                MV_streamToggleStatesTemp[3]      = MV_streamToggleStates[3];

                // Send clear plot command before sending first packet
                if (MV_initiallyClearBuffer)
                {
                    MOTORVISION_sendAcknowledge(CLEAR_STREAMING_PLOT);
                    MV_initiallyClearBuffer = false;
                }

                // Loop through all buffer elements
                uint16_t bufferElementIndex;
                for (bufferElementIndex = 0; bufferElementIndex < MV_BUFFER_SIZE; bufferElementIndex++)
                {
                    // Read in data from buffers at bufferElementIndex
                    uint16_t insertIndex  = 0;
                    float    dataArray[4] = {0};

                    if (MV_streamToggleStatesTemp[0])
                    {
                        dataArray[insertIndex] = MV_streamA_buffer[bufferElementIndex];
                        insertIndex++;
                    }
                    if (MV_streamToggleStatesTemp[1])
                    {
                        dataArray[insertIndex] = MV_streamB_buffer[bufferElementIndex];
                        insertIndex++;
                    }
                    if (MV_streamToggleStatesTemp[2])
                    {
                        dataArray[insertIndex] = MV_streamC_buffer[bufferElementIndex];
                        insertIndex++;
                    }
                    if (MV_streamToggleStatesTemp[3])
                    {
                        dataArray[insertIndex] = MV_streamD_buffer[bufferElementIndex];
                        insertIndex++;
                    }

                    // Package and transmit data from buffers
                    if (insertIndex == 1)
                    {
                        char cmd = (char)STREAM_DATA_1_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_1_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 1);
                    }
                    else if (insertIndex == 2)
                    {
                        char cmd = (char)STREAM_DATA_2_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_2_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 2);
                    }
                    else if (insertIndex == 3)
                    {
                        char cmd = (char)STREAM_DATA_3_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_3_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 3);
                    }
                    else if (insertIndex == 4)
                    {
                        char cmd = (char)STREAM_DATA_4_VAR;
                        if (MV_writeWhileStreaming)
                        {
                            cmd                    = (char)STREAM_DATA_WRITEACK_4_VAR;
                            MV_writeWhileStreaming = false;
                        }
                        EXPORTPACKAGE_sendFloatArrayAsByteArray(&cmd, 1, dataArray, 4);
                    }
                }
                MV_bufferIsFull = false;
            }
            else
            {
                if (!MV_bufferIsFull && MV_bufferIsFull2)
                {
                    MV_writeBuffer1readBuffer2 = true;
                }
                return;
            }
        }
    }
}

//
// Enable new polling variable
//
static void MOTORVISION_enableNewPollingVariable(uint16_t* indiceArray, uint16_t numberOfVariables)
{
    int variableIndex;
    for (variableIndex = 0; variableIndex < numberOfVariables; variableIndex++)
    {
        if (MV_pollingArraySize < MV_POLLING_VARIABLE_MAX)
        {
            MV_pollingEnabledHashIndicesArray[MV_pollingArraySize] = indiceArray[variableIndex];
            MV_pollingArraySize++;
        }
        else
        {
            // out of bound
            break;
        }
    }
}

//
// Clear all polling variables
//
static void MOTORVISION_clearPollingVariables(void)
{
    MV_pollingArraySize = 0;
    int pollingIndex;
    for (pollingIndex = 0; pollingIndex < MV_POLLING_VARIABLE_MAX; pollingIndex++)
    {
        MV_pollingEnabledHashIndicesArray[pollingIndex] = 0xFF;
    }
}

//*****************************************************************************
//
// Motor Vision Capture and Send Polling Data function
//
//*****************************************************************************
void MOTORVISION_captureAndSendPollingData(void)
{
    if (MV_nextTxCMD != NO_UPDATE)
    {
        MOTORVISION_sendAcknowledge(MV_nextTxCMD);
    }
    else if (MV_pollingArraySize == 0)
    {
        return;
    }
    else
    {
        uint16_t hashIndex = 0;

        // Find next polling index
        if (MV_pollingEnabledHashIndicesArray[MV_currentPollingIndex] == 0xFF)
        {
            MV_currentPollingIndex = 0;
        }
        hashIndex = MV_pollingEnabledHashIndicesArray[MV_currentPollingIndex];
        MV_currentPollingIndex++;

        // Get polling variable data and send to packaging layer
        enum MV_VAR_TYPES variableType = MOTORVISION_getVariableType(hashIndex);

        char cmdArray[2] = {(char)POLLING_DATA, (char)hashIndex};

        if (variableType == BOOL)
        {
            bool* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(hashIndex, variableType);
            EXPORTPACKAGE_sendBoolAsByteArray(&(cmdArray[0]), 2, *dataPtr);
        }
        else if (variableType == UINT16)
        {
            uint16_t* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(hashIndex, variableType);
            EXPORTPACKAGE_sendUInt16AsByteArray(&(cmdArray[0]), 2, *dataPtr);
        }
        else if (variableType == UINT32)
        {
            uint32_t* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(hashIndex, variableType);
            EXPORTPACKAGE_sendUInt32AsByteArray(&(cmdArray[0]), 2, *dataPtr);
        }
        else if (variableType == INT16)
        {
            int16_t* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(hashIndex, variableType);
            EXPORTPACKAGE_sendInt16AsByteArray(&(cmdArray[0]), 2, *dataPtr);
        }
        else if (variableType == INT32)
        {
            int32_t* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(hashIndex, variableType);
            EXPORTPACKAGE_sendInt32AsByteArray(&(cmdArray[0]), 2, *dataPtr);
        }
        else if (variableType == FLOAT)
        {
            float* dataPtr = MOTORVISION_getVariableAddressAtHashIndex(hashIndex, variableType);
            EXPORTPACKAGE_sendFloatAsByteArray(&(cmdArray[0]), 2, *dataPtr);
        }
    }
}

//
// Write polling variable from GUI
//
int         vart = 0;
static void MOTORVISION_writePollingVariable(uint16_t hashIndex, uint16_t* dataArray)
{
    uint16_t data3 = 0;
    uint16_t data2 = 0;
    uint16_t data1 = 0;
    uint16_t data0 = 0;

    enum MV_VAR_TYPES variableType = MOTORVISION_getVariableType(hashIndex);

    if (variableType == BOOL)
    {
        data0             = *(dataArray + 1);
        bool* ptr         = (bool*)MVHASH_hash_table[hashIndex];
        bool  dataToWrite = (bool)data0;
        *ptr              = dataToWrite;

        vart = 1;
    }
    else if (variableType == UINT16)
    {
        data1                 = *(dataArray);
        data0                 = *(dataArray + 1);
        uint16_t* ptr         = (uint16_t*)MVHASH_hash_table[hashIndex];
        uint16_t  dataToWrite = (((uint16_t)data1 << 8) & 0xFF00) | (((uint16_t)data0) & 0xFF);
        *ptr                  = dataToWrite;

        vart = 2;
    }
    else if (variableType == UINT32)
    {
        data3                 = *(dataArray);
        data2                 = *(dataArray + 1);
        data1                 = *(dataArray + 2);
        data0                 = *(dataArray + 3);
        uint32_t* ptr         = (uint32_t*)MVHASH_hash_table[hashIndex];
        uint32_t  dataToWrite = (((uint32_t)data3 << 24) & 0xFF000000) | (((uint32_t)data2 << 16) & 0xFF0000) |
                               (((uint32_t)data1 << 8) & 0xFF00) | (((uint32_t)data0) & 0xFF);
        *ptr = dataToWrite;

        vart = 3;
    }
    else if (variableType == INT16)
    {
        data1                = *(dataArray);
        data0                = *(dataArray + 1);
        int16_t* ptr         = (int16_t*)MVHASH_hash_table[hashIndex];
        int16_t  dataToWrite = (((int16_t)data1 << 8) & 0xFF00) | (((int16_t)data0) & 0xFF);
        *ptr                 = dataToWrite;

        vart = 4;
    }
    else if (variableType == INT32)
    {
        data3                = *(dataArray);
        data2                = *(dataArray + 1);
        data1                = *(dataArray + 2);
        data0                = *(dataArray + 3);
        int32_t* ptr         = (int32_t*)MVHASH_hash_table[hashIndex];
        int32_t  dataToWrite = (((int32_t)data3 << 24) & 0xFF000000) | (((int32_t)data2 << 16) & 0xFF0000) |
                              (((int32_t)data1 << 8) & 0xFF00) | (((int32_t)data0) & 0xFF);
        *ptr = dataToWrite;

        vart = 5;
    }
    else if (variableType == FLOAT)
    {
        data3                = *(dataArray);
        data2                = *(dataArray + 1);
        data1                = *(dataArray + 2);
        data0                = *(dataArray + 3);
        float*   ptr         = (float*)MVHASH_hash_table[hashIndex];
        // float floatValue;
        uint32_t dataToWrite = (((uint32_t)data3 << 24) & 0xFF000000) | (((uint32_t)data2 << 16) & 0xFF0000) |
                               (((uint32_t)data1 << 8) & 0xFF00) | (((uint32_t)data0) & 0xFF);
        memcpy(ptr, &dataToWrite, sizeof(float));

        vart = 6;
    }

    if (MV_currentStreamState != STREAM_IDLE)
    {
        MV_writeWhileStreaming = true;
    }
}

//
// Command handler - called by export layer when packet received
//
void EXPORTPACKAGE_takeActionOnMessage(uint16_t cmdByte, uint16_t* msgData, bool error)
{
    if (error)
    {
        MV_nextTxCMD = PACKET_ERROR;
    }
    else
    {
        bool streamVarAdd      = false;
        bool streamVarSubtract = false;

        // Set up ack of CMD
        MV_nextTxCMD = (enum MV_CMDS)cmdByte;

        // Get CMD byte and perform action
        if (cmdByte == INITCMD)
        {
            // Reinitialize all states
            MV_currentPlotHashIndices[0] = MV_DEFAULT_STREAM_VAR_A;
            MV_currentPlotHashIndices[1] = MV_DEFAULT_STREAM_VAR_B;
            MV_currentPlotHashIndices[2] = MV_DEFAULT_STREAM_VAR_C;
            MV_currentPlotHashIndices[3] = MV_DEFAULT_STREAM_VAR_D;
            MV_currentStreamState        = STREAM_IDLE;
            int i;
            for (i = 0; i < MV_NUMBER_OF_SIMULTANEOUS_STREAM_VARIABLES; i++)
            {
                MV_streamToggleStates[i] = false;
            }
            MOTORVISION_resetStates();
        }
        else if (cmdByte == WRITE_VAR)
        {
            MOTORVISION_writePollingVariable(msgData[0], &msgData[1]);
        }
        else if (cmdByte == INIT_STREAMING_VARS)
        {
            MV_currentPlotHashIndices[0] = (int16_t)msgData[0];
            MV_currentPlotHashIndices[1] = (int16_t)msgData[1];
            MV_currentPlotHashIndices[2] = (int16_t)msgData[2];
            MV_currentPlotHashIndices[3] = (int16_t)msgData[3];
        }
        else if (cmdByte == START_STREAMING_A)
        {
            MV_streamToggleStates[0] = true;
            streamVarAdd             = true;
        }
        else if (cmdByte == START_STREAMING_B)
        {
            MV_streamToggleStates[1] = true;
            streamVarAdd             = true;
        }
        else if (cmdByte == START_STREAMING_C)
        {
            MV_streamToggleStates[2] = true;
            streamVarAdd             = true;
        }
        else if (cmdByte == START_STREAMING_D)
        {
            MV_streamToggleStates[3] = true;
            streamVarAdd             = true;
        }
        else if (cmdByte == STOP_STREAMING_A)
        {
            MV_streamToggleStates[0] = false;
            streamVarSubtract        = true;
        }
        else if (cmdByte == STOP_STREAMING_B)
        {
            MV_streamToggleStates[1] = false;
            streamVarSubtract        = true;
        }
        else if (cmdByte == STOP_STREAMING_C)
        {
            MV_streamToggleStates[2] = false;
            streamVarSubtract        = true;
        }
        else if (cmdByte == STOP_STREAMING_D)
        {
            MV_streamToggleStates[3] = false;
            streamVarSubtract        = true;
        }
        else if (cmdByte == START_POLLING_1)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 1);
        }
        else if (cmdByte == START_POLLING_2)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 2);
        }
        else if (cmdByte == START_POLLING_3)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 3);
        }
        else if (cmdByte == START_POLLING_4)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 4);
        }
        else if (cmdByte == START_POLLING_5)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 5);
        }
        else if (cmdByte == START_POLLING_6)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 6);
        }
        else if (cmdByte == START_POLLING_7)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 7);
        }
        else if (cmdByte == START_POLLING_8)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 8);
        }
        else if (cmdByte == START_POLLING_9)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 9);
        }
        else if (cmdByte == START_POLLING_10)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 10);
        }
        else if (cmdByte == START_POLLING_11)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 11);
        }
        else if (cmdByte == START_POLLING_12)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 12);
        }
        else if (cmdByte == START_POLLING_13)
        {
            MOTORVISION_enableNewPollingVariable(msgData, 13);
        }
        else if (cmdByte == STOP_POLLING)
        {
            MOTORVISION_clearPollingVariables();
        }
        else
        {
            MV_nextTxCMD = PACKET_ERROR;
        }

        // Update MV_currentStreamState
        if (streamVarAdd)
        {
            switch (MV_currentStreamState)
            {
                case STREAM_IDLE:
                    MV_currentStreamState = STREAM1;
                    break;
                case STREAM1:
                    MV_currentStreamState = STREAM2;
                    break;
                case STREAM2:
                    MV_currentStreamState = STREAM3;
                    break;
                case STREAM3:
                    MV_currentStreamState = STREAM4;
                    break;
                case STREAM4:
                    MV_currentStreamState = STREAM4;
                    MV_nextTxCMD          = PACKET_ERROR;
                    break;
                default:
                    MV_currentStreamState = STREAM_IDLE;
            }
        }
        else if (streamVarSubtract)
        {
            switch (MV_currentStreamState)
            {
                case STREAM_IDLE:
                    MV_currentStreamState = STREAM_IDLE;
                    MV_nextTxCMD          = PACKET_ERROR;
                    break;
                case STREAM1:
                    MV_currentStreamState   = STREAM_IDLE;
                    MV_initiallyClearBuffer = true;
                    break;
                case STREAM2:
                    MV_currentStreamState = STREAM1;
                    break;
                case STREAM3:
                    MV_currentStreamState = STREAM2;
                    break;
                case STREAM4:
                    MV_currentStreamState = STREAM3;
                    break;
                default:
                    MV_currentStreamState = STREAM_IDLE;
            }
        }
    }
}
