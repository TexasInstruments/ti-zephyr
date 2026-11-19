// #############################################################################
//
//  FILE:   datalog.h
//
//  TITLE:  ARM M33 Data Logging (DATALOG) library
//          (floating point)
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

#ifndef DATALOG_H
#define DATALOG_H

// **************************************************************************
// the includes

#include "math_types.h"

//!
//!
//! \defgroup DATALOG DATALOG
//!
//! @{

#ifdef __cplusplus
extern "C" {
#endif

// **************************************************************************
// the defines
#define DATA_LOG_BUFF_NUM 4

#define DATA_LOG_BUFF_SIZE 400  // 20*20

#define DLOG_BURST_SIZE \
    20  // write 820 to the register for
        // a burst size of 20

#define DLOG_TRANSFER_SIZE 42  // [(MEM_BUFFER_SIZE/(BURST)]

//! \brief Defines the default initialization for the DLOG object
//!
#define DATALOG_DEFAULTS {{NULL, NULL, NULL, NULL}, true, false, 0, DATA_LOG_BUFF_SIZE}

// **************************************************************************
// the typedefs

//! \brief Defines the data logging (DATALOG) object
//!
typedef struct _DATALOG_OBJ_
{
    float32_t *iptr[DATA_LOG_BUFF_NUM];  //!< Input: First input pointer

    bool     flag_enableLogData;     //!< Parameter: Maximum data buffer
    bool     flag_enableLogOneShot;  //!< Parameter: Maximum data buffer
    uint16_t cntr;                   //!< Variable:  Data log counter
    uint16_t size;                   //!< Parameter: Maximum data buffer
} DATALOG_Obj;

//! \brief Defines the DATALOG handle
//!
typedef struct _DATALOG_Obj_ *DATALOG_Handle;

// **************************************************************************
// the globals
extern float32_t datalogBuff1[DATA_LOG_BUFF_SIZE + 2];
extern float32_t datalogBuff2[DATA_LOG_BUFF_SIZE + 2];
extern float32_t datalogBuff3[DATA_LOG_BUFF_SIZE + 2];
extern float32_t datalogBuff4[DATA_LOG_BUFF_SIZE + 2];

//! \brief Defines the DATALOG object
//!
extern DATALOG_Obj    datalog;
extern DATALOG_Handle datalogHandle;

// **************************************************************************
// the globals

// **************************************************************************
// the function prototypes

//! \brief     Initializes the data logger
//! \param[in] ptr  The pointer to memory
extern DATALOG_Handle DATALOG_init(void *pMemory, const size_t numBytes);

//! \brief     Updates the data logger
//! \param[in] ptr  The pointer to memory
static inline void DATALOG_update(DATALOG_Handle handle)
{
    DATALOG_Obj *obj = (DATALOG_Obj *)handle;

    if (obj->flag_enableLogData == true)
    {
        if (obj->cntr >= obj->size)
        {
            obj->cntr = 0;

            if (obj->flag_enableLogOneShot == true)
            {
                obj->flag_enableLogData    = false;
                obj->flag_enableLogOneShot = false;
            }
        }

        datalogBuff1[obj->cntr] = (*obj->iptr[0]);
        datalogBuff2[obj->cntr] = (*obj->iptr[1]);
        datalogBuff3[obj->cntr] = (*obj->iptr[2]);
        datalogBuff4[obj->cntr] = (*obj->iptr[3]);

        obj->cntr++;
    }
    else if (obj->flag_enableLogOneShot == true)
    {
        obj->flag_enableLogData = true;
        obj->cntr               = 0;
    }
}

//! \brief     Updates the data logger
//! \param[in] ptr  The pointer to memory
static inline void DATALOG_updateWithDMA(DATALOG_Handle handle)
{
    DATALOG_Obj *obj = (DATALOG_Obj *)handle;

    uint16_t number = obj->size - 1;

    datalogBuff1[number] = (*obj->iptr[0]);
    datalogBuff2[number] = (*obj->iptr[1]);
    datalogBuff3[number] = (*obj->iptr[2]);
    datalogBuff4[number] = (*obj->iptr[3]);
}

#ifdef __cplusplus
}
#endif  // extern "C"

//! @} // ingroup
#endif  // end of DATALOG_H definition
