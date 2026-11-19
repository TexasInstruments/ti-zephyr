/*
 * Copyright (C) 2025 Texas Instruments Incorporated
 *
 * MotorVision Export Package Layer for AM13E2309 (Cortex-M33)
 *
 * This file provides packet encoding/decoding and type conversion
 * utilities for the MotorVision protocol.
 */

#ifndef EXPORT_PACKAGE_H
#define EXPORT_PACKAGE_H

#include <stdint.h>
#include <stdbool.h>

/* ========================================================================== */
/*                           Macro Definitions                                */
/* ========================================================================== */

/* MotorVision packet structure */
#define PACKET_START_BYTE   115 /* 0x73 */
#define PACKET_END_BYTE     101 /* 0x65 */
#define PACKET_SIZE         16  /* Total packet size in bytes */
#define PACKET_PAYLOAD_SIZE 13  /* Data bytes (excluding start, cmd, end) */

/* ========================================================================== */
/*                         Function Prototypes                                */
/* ========================================================================== */

/**
 * @brief Convert uint32_t to byte array (little-endian)
 *
 * @param value 32-bit unsigned integer
 * @param bytes Output array (must have space for 4 bytes)
 */
void u32toBytes(uint32_t value, uint16_t* bytes);

/**
 * @brief Convert int32_t to byte array (little-endian)
 *
 * @param value 32-bit signed integer
 * @param bytes Output array (must have space for 4 bytes)
 */
void i32toBytes(int32_t value, uint16_t* bytes);

/**
 * @brief Convert uint16_t to byte array (little-endian)
 *
 * @param value 16-bit unsigned integer
 * @param bytes Output array (must have space for 2 bytes)
 */
void u16toBytes(uint16_t value, uint16_t* bytes);

/**
 * @brief Convert int16_t to byte array (little-endian)
 *
 * @param value 16-bit signed integer
 * @param bytes Output array (must have space for 2 bytes)
 */
void i16toBytes(int16_t value, uint16_t* bytes);

/**
 * @brief Convert float to byte array (IEEE 754, little-endian)
 *
 * @param value 32-bit floating point
 * @param bytes Output array (must have space for 4 bytes)
 */
void f32toBytes(float value, uint16_t* bytes);

/**
 * @brief Convert bool to byte
 *
 * @param value Boolean value
 * @return 1 if true, 0 if false
 */
uint16_t boolToByte(bool value);

/**
 * @brief Convert byte array to uint32_t (little-endian)
 *
 * @param bytes Input array (4 bytes)
 * @return 32-bit unsigned integer
 */
uint32_t bytesToU32(uint16_t* bytes);

/**
 * @brief Convert byte array to int32_t (little-endian)
 *
 * @param bytes Input array (4 bytes)
 * @return 32-bit signed integer
 */
int32_t bytesToI32(uint16_t* bytes);

/**
 * @brief Convert byte array to uint16_t (little-endian)
 *
 * @param bytes Input array (2 bytes)
 * @return 16-bit unsigned integer
 */
uint16_t bytesToU16(uint16_t* bytes);

/**
 * @brief Convert byte array to int16_t (little-endian)
 *
 * @param bytes Input array (2 bytes)
 * @return 16-bit signed integer
 */
int16_t bytesToI16(uint16_t* bytes);

/**
 * @brief Convert byte array to float (IEEE 754, little-endian)
 *
 * @param bytes Input array (4 bytes)
 * @return 32-bit floating point
 */
float bytesToF32(uint16_t* bytes);

/**
 * @brief Convert byte to bool
 *
 * @param byte Input byte
 * @return true if non-zero, false if zero
 */
bool byteToBool(uint16_t byte);

/**
 * @brief Send uint32_t value as MotorVision packet
 *
 * Packet format: [START][CMD][CMD_DATA][VALUE_BYTES][PADDING][END]
 *
 * @param cmd Command bytes (2 bytes: command ID and optional data)
 * @param cmdLength Length of command bytes (1 or 2)
 * @param value Value to send
 */
void EXPORTPACKAGE_sendUInt32AsByteArray(char* cmd, uint16_t cmdLength, uint32_t value);

/**
 * @brief Send int32_t value as MotorVision packet
 *
 * @param cmd Command bytes (2 bytes)
 * @param cmdLength Length of command bytes (1 or 2)
 * @param value Value to send
 */
void EXPORTPACKAGE_sendInt32AsByteArray(char* cmd, uint16_t cmdLength, int32_t value);

/**
 * @brief Send uint16_t value as MotorVision packet
 *
 * @param cmd Command bytes (2 bytes)
 * @param cmdLength Length of command bytes (1 or 2)
 * @param value Value to send
 */
void EXPORTPACKAGE_sendUInt16AsByteArray(char* cmd, uint16_t cmdLength, uint16_t value);

/**
 * @brief Send int16_t value as MotorVision packet
 *
 * @param cmd Command bytes (2 bytes)
 * @param cmdLength Length of command bytes (1 or 2)
 * @param value Value to send
 */
void EXPORTPACKAGE_sendInt16AsByteArray(char* cmd, uint16_t cmdLength, int16_t value);

/**
 * @brief Send float value as MotorVision packet
 *
 * @param cmd Command bytes (2 bytes)
 * @param cmdLength Length of command bytes (1 or 2)
 * @param value Value to send
 */
void EXPORTPACKAGE_sendFloatAsByteArray(char* cmd, uint16_t cmdLength, float value);

/**
 * @brief Send bool value as MotorVision packet
 *
 * @param cmd Command bytes (2 bytes)
 * @param cmdLength Length of command bytes (1 or 2)
 * @param value Value to send
 */
void EXPORTPACKAGE_sendBoolAsByteArray(char* cmd, uint16_t cmdLength, bool value);

/**
 * @brief Send float array as MotorVision packet
 *
 * @param cmd Command bytes (2 bytes)
 * @param cmdLength Length of command bytes (1 or 2)
 * @param values Array of float values
 * @param numValues Number of floats to send (max 3 for 16-byte packet)
 */
void EXPORTPACKAGE_sendFloatArrayAsByteArray(char* cmd, uint16_t cmdLength, float* values, uint16_t numValues);

/**
 * @brief Callback function invoked when a valid packet is received
 *
 * This is a weak function that should be overridden by the application
 * to handle incoming commands.
 *
 * @param cmdByte Command byte from packet
 * @param msgData Pointer to 13-byte payload data
 * @param error True if packet validation failed
 */
void EXPORTPACKAGE_takeActionOnMessage(uint16_t cmdByte, uint16_t* msgData, bool error) __attribute__((weak));

/**
 * @brief Process received packet and invoke command handler
 *
 * This function validates the packet structure (start/end bytes)
 * and calls EXPORTPACKAGE_takeActionOnMessage() with the command
 * and payload data.
 *
 * @param receivedData Pointer to 16-byte packet
 * @param receivedDataLength Length of packet (should be 16)
 */
extern void EXPORT_receivedData(uint16_t* receivedData, uint16_t receivedDataLength);

#endif /* EXPORT_PACKAGE_H */
