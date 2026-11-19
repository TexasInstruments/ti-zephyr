/*
 * Copyright (C) 2025 Texas Instruments Incorporated
 *
 * MotorVision Export Layer for AM13E2309 (Cortex-M33)
 *
 * This file provides packet reception and validation primitives
 * for the MotorVision protocol. The UART is a Zephyr device: TX uses
 * uart_poll_out() and the UART RX interrupt handler in export.c calls
 * EXPORT_rxCallback() to feed received bytes into this layer.
 */

#ifndef EXPORT_H
#define EXPORT_H

#include <stdint.h>
#include <stdbool.h>

/* ========================================================================== */
/*                           Macro Definitions                                */
/* ========================================================================== */

/* RX buffer size for packet reception (16 bytes per packet) */
#define EXPORT_RX_BUFFER_SIZE 16

/* ========================================================================== */
/*                         Function Prototypes                                */
/* ========================================================================== */

/**
 * @brief Initialize the export layer.
 *
 * Enables interrupt driven reception on the given Zephyr UART device and resets
 * the RX buffer.
 * Must be called once during system initialization before enabling interrupts.
 *
 * @param uartInst Pointer to the UART device (const struct device *).
 */
void EXPORT_init(void* uartInst);

/**
 * @brief Transmit a single character via UART (blocking at application layer).
 *
 * Calls uart_poll_out(), which waits until the byte is sent, so the
 * MotorVision packet framing is never broken.
 *
 * @param data Character to transmit (lower 8 bits used)
 */
void EXPORT_transmitCharBlocking(uint16_t data);

/**
 * @brief Transmit a string of specified length via UART (blocking at application layer).
 *
 * Calls uart_poll_out() byte by byte. Nothing is sent if EXPORT_init() was
 * not called with a ready device.
 *
 * @param data   Pointer to character array
 * @param length Number of characters to transmit
 */
void EXPORT_transmitStringLengthBlocking(const char* data, uint16_t length);

/**
 * @brief Check for received data and process complete packets.
 *
 * Stub - RX is interrupt driven through the UART callback in export.c.
 */
void EXPORT_checkReceivedData(void);

/**
 * @brief Callback invoked by the UART RX interrupt handler when a complete packet arrives.
 *
 * The handler reads EXPORT_RX_BUFFER_SIZE bytes from the UART FIFO and
 * passes them here. This function validates the packet and dispatches to
 * EXPORTPACKAGE_takeActionOnMessage().
 *
 * @param data   Pointer to received byte array (EXPORT_RX_BUFFER_SIZE bytes)
 * @param length Number of bytes received (must equal EXPORT_RX_BUFFER_SIZE)
 */
void EXPORT_rxCallback(uint16_t* data, uint16_t length);

/**
 * @brief Callback function invoked when a complete packet is received.
 *
 * Validates packet framing (start/end bytes) and dispatches to the command
 * handler. Called internally by EXPORT_rxCallback().
 *
 * @param receivedData       Pointer to 16-byte packet buffer
 * @param receivedDataLength Length of received data (always 16)
 */
void EXPORT_receivedData(uint16_t* receivedData, uint16_t receivedDataLength);

/**
 * @brief Get the total number of received packets (for debugging).
 *
 * @return Packet reception count since last reset
 */
uint16_t EXPORT_getRxBufferIndex(void);

/**
 * @brief Reset the RX buffer and packet counter (for error recovery).
 */
void EXPORT_resetRxBuffer(void);

#endif /* EXPORT_H */
