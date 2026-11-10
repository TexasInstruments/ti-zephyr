/*
 * Copyright (C) 2025 Texas Instruments Incorporated
 *
 * MotorVision Export Package Layer for AM13E2309 (Cortex-M33)
 */

#include "export_package.h"
#include "export.h"
#include <string.h>

/* ========================================================================== */
/*                    Type Conversion Functions (To Bytes)                    */
/* ========================================================================== */

void u16toBytes(uint16_t u16, uint16_t* byteArray)
{
    uint16_t* bytePtr = &u16;
    byteArray[0]      = (*bytePtr & 0x00FF);
    byteArray[1]      = (*bytePtr >> 8);
}

void i16toBytes(int16_t i16, uint16_t* byteArray)
{
    uint16_t* bytePtr = (uint16_t*)&i16;
    byteArray[0]      = (*bytePtr & 0x00FF);
    byteArray[1]      = (*bytePtr >> 8);
}

void u32toBytes(uint32_t u32, uint16_t* byteArray)
{
    uint16_t* bytePtr  = (uint16_t*)&u32;
    byteArray[0]       = (*bytePtr & 0x00FF);
    byteArray[1]       = (*bytePtr >> 8);
    bytePtr           += 1;
    byteArray[2]       = (*bytePtr & 0x00FF);
    byteArray[3]       = (*bytePtr >> 8);
}

void i32toBytes(int32_t i32, uint16_t* byteArray)
{
    uint16_t* bytePtr  = (uint16_t*)&i32;
    byteArray[0]       = (*bytePtr & 0x00FF);
    byteArray[1]       = (*bytePtr >> 8);
    bytePtr           += 1;
    byteArray[2]       = (*bytePtr & 0x00FF);
    byteArray[3]       = (*bytePtr >> 8);
}

void f32toBytes(float f32, uint16_t* byteArray)
{
    uint32_t uBits;
    memcpy(&uBits, &f32, sizeof(uBits));
    byteArray[0] = (uint16_t)(uBits & 0x00FFU);
    byteArray[1] = (uint16_t)((uBits >> 8U) & 0x00FFU);
    byteArray[2] = (uint16_t)((uBits >> 16U) & 0x00FFU);
    byteArray[3] = (uint16_t)((uBits >> 24U) & 0x00FFU);
}

uint16_t boolToByte(bool value)
{
    return value ? 1 : 0;
}

/* ========================================================================== */
/*                   Type Conversion Functions (From Bytes)                   */
/* ========================================================================== */

uint32_t bytesToU32(uint16_t* bytes)
{
    /* Little-endian decoding */
    uint32_t value  = 0;
    value          |= (uint32_t)bytes[0];
    value          |= ((uint32_t)bytes[1]) << 8;
    value          |= ((uint32_t)bytes[2]) << 16;
    value          |= ((uint32_t)bytes[3]) << 24;
    return value;
}

int32_t bytesToI32(uint16_t* bytes)
{
    uint32_t uValue = bytesToU32(bytes);
    int32_t  result;
    memcpy(&result, &uValue, sizeof(result));
    return result;
}

uint16_t bytesToU16(uint16_t* bytes)
{
    /* Little-endian decoding */
    uint16_t value  = 0;
    value          |= bytes[0];
    value          |= (bytes[1] << 8);
    return value;
}

int16_t bytesToI16(uint16_t* bytes)
{
    uint16_t uValue = bytesToU16(bytes);
    int16_t  result;
    memcpy(&result, &uValue, sizeof(result));
    return result;
}

float bytesToF32(uint16_t* bytes)
{
    uint32_t uValue = bytesToU32(bytes);
    float    result;
    memcpy(&result, &uValue, sizeof(result));
    return result;
}

bool byteToBool(uint16_t byte)
{
    return (byte != 0);
}

/* ========================================================================== */
/*                      Packet Transmission Functions                         */
/* ========================================================================== */

void EXPORTPACKAGE_sendUInt32AsByteArray(char* cmd, uint16_t cmdLength, uint32_t value)
{
    uint16_t bytes[4];
    uint16_t byteIndex = 0;
    uint16_t cmdIndex;

    /* Send start byte */
    EXPORT_transmitCharBlocking(PACKET_START_BYTE);

    /* Key IDs */
    for (cmdIndex = 0; cmdIndex < cmdLength; cmdIndex++)
    {
        EXPORT_transmitCharBlocking(cmd[cmdIndex]);
    }

    /* Send data payload */
    u32toBytes(value, bytes);
    for (byteIndex = 0; byteIndex < 4; byteIndex++)
    {
        EXPORT_transmitCharBlocking(bytes[byteIndex]);
    }

    /* Send end byte */
    EXPORT_transmitCharBlocking(PACKET_END_BYTE);
}

void EXPORTPACKAGE_sendInt32AsByteArray(char* cmd, uint16_t cmdLength, int32_t value)
{
    uint16_t bytes[4];
    uint16_t byteIndex = 0;
    uint16_t cmdIndex;

    /* Send start byte */
    EXPORT_transmitCharBlocking(PACKET_START_BYTE);

    /* Key IDs */
    for (cmdIndex = 0; cmdIndex < cmdLength; cmdIndex++)
    {
        EXPORT_transmitCharBlocking(cmd[cmdIndex]);
    }

    /* Send data payload */
    i32toBytes(value, bytes);
    for (byteIndex = 0; byteIndex < 4; byteIndex++)
    {
        EXPORT_transmitCharBlocking(bytes[byteIndex]);
    }

    /* Send end byte */
    EXPORT_transmitCharBlocking(PACKET_END_BYTE);
}

void EXPORTPACKAGE_sendUInt16AsByteArray(char* cmd, uint16_t cmdLength, uint16_t value)
{
    uint16_t bytes[2];
    uint16_t byteIndex = 0;
    uint16_t cmdIndex;

    /* Send start byte */
    EXPORT_transmitCharBlocking(PACKET_START_BYTE);

    /* Key IDs */
    for (cmdIndex = 0; cmdIndex < cmdLength; cmdIndex++)
    {
        EXPORT_transmitCharBlocking(cmd[cmdIndex]);
    }

    /* Send data payload */
    u16toBytes(value, bytes);
    for (byteIndex = 0; byteIndex < 2; byteIndex++)
    {
        EXPORT_transmitCharBlocking(bytes[byteIndex]);
    }

    /* Send end byte */
    EXPORT_transmitCharBlocking(PACKET_END_BYTE);
}

void EXPORTPACKAGE_sendInt16AsByteArray(char* cmd, uint16_t cmdLength, int16_t value)
{
    uint16_t bytes[2];
    uint16_t byteIndex = 0;
    uint16_t cmdIndex;

    /* Send start byte */
    EXPORT_transmitCharBlocking(PACKET_START_BYTE);

    /* Key IDs */
    for (cmdIndex = 0; cmdIndex < cmdLength; cmdIndex++)
    {
        EXPORT_transmitCharBlocking(cmd[cmdIndex]);
    }

    /* Send data payload */
    i16toBytes(value, bytes);
    for (byteIndex = 0; byteIndex < 2; byteIndex++)
    {
        EXPORT_transmitCharBlocking(bytes[byteIndex]);
    }

    /* Send end byte */
    EXPORT_transmitCharBlocking(PACKET_END_BYTE);
}

void EXPORTPACKAGE_sendFloatAsByteArray(char* cmd, uint16_t cmdLength, float value)
{
    uint16_t bytes[4];
    uint16_t byteIndex = 0;
    uint16_t cmdIndex;

    /* Send start byte */
    EXPORT_transmitCharBlocking(PACKET_START_BYTE);

    /* Key IDs */
    for (cmdIndex = 0; cmdIndex < cmdLength; cmdIndex++)
    {
        EXPORT_transmitCharBlocking(cmd[cmdIndex]);
    }

    /* Send data payload */
    f32toBytes(value, bytes);
    for (byteIndex = 0; byteIndex < 4; byteIndex++)
    {
        EXPORT_transmitCharBlocking(bytes[byteIndex]);
    }

    /* Send end byte */
    EXPORT_transmitCharBlocking(PACKET_END_BYTE);
}

void EXPORTPACKAGE_sendBoolAsByteArray(char* cmd, uint16_t cmdLength, bool value)
{
    uint16_t bytes[2];
    uint16_t byteIndex = 0;
    uint16_t cmdIndex;

    /* Send start byte */
    EXPORT_transmitCharBlocking(PACKET_START_BYTE);

    /* Key IDs */
    for (cmdIndex = 0; cmdIndex < cmdLength; cmdIndex++)
    {
        EXPORT_transmitCharBlocking(cmd[cmdIndex]);
    }

    /* Send data payload */
    u16toBytes(value, bytes);
    for (byteIndex = 0; byteIndex < 2; byteIndex++)
    {
        EXPORT_transmitCharBlocking(bytes[byteIndex]);
    }

    /* Send end byte */
    EXPORT_transmitCharBlocking(PACKET_END_BYTE);
}

void EXPORTPACKAGE_sendFloatArrayAsByteArray(char* cmd, uint16_t cmdLength, float* values, uint16_t numValues)
{
    uint32_t datai;
    uint16_t bytes[4];
    uint16_t byteIndex = 0;

    /* Send start byte */
    EXPORT_transmitCharBlocking(PACKET_START_BYTE);
    EXPORT_transmitCharBlocking(cmd[0]);

    /* Send data payload */
    for (datai = 0; datai < numValues; datai++)
    {
        f32toBytes(values[datai], bytes);
        for (byteIndex = 0; byteIndex < 4; byteIndex++)
        {
            EXPORT_transmitCharBlocking(bytes[byteIndex]);
        }
    }

    /* Send end byte */
    EXPORT_transmitCharBlocking(PACKET_END_BYTE);
}
