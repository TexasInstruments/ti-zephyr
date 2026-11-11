/*
 * Copyright (C) 2025 Texas Instruments Incorporated
 *
 * MotorVision Export Layer for AM13E2309 (Cortex-M33)
 *
 * This file provides packet reception and validation for the MotorVision
 * protocol. Zephyr port: the UART is a Zephyr device. TX uses uart_poll_out()
 * and the RX interrupt handler below collects EXPORT_RX_BUFFER_SIZE bytes and
 * calls EXPORT_rxCallback().
 */

#include "export.h"
#include "export_package.h"
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

/* ========================================================================== */
/*                           Global Variables                                 */
/* ========================================================================== */

/* RX buffer for assembling 16-byte packets */
static uint16_t rxBuffer[EXPORT_RX_BUFFER_SIZE];

static volatile uint16_t lastRxPacket[EXPORT_RX_BUFFER_SIZE];
static volatile uint32_t rxPacketCount = 0;

static const struct device* uartDev;
static uint16_t             rxIndex;
static uint16_t             rxIsrBuffer[EXPORT_RX_BUFFER_SIZE];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void EXPORT_uartIsr(const struct device* dev, void* userData)
{
    uint8_t byte;

    ARG_UNUSED(userData);

    uart_irq_update(dev);

    while (uart_irq_rx_ready(dev))
    {
        if (uart_fifo_read(dev, &byte, 1) != 1)
        {
            break;
        }

        rxIsrBuffer[rxIndex++] = byte;
        if (rxIndex >= EXPORT_RX_BUFFER_SIZE)
        {
            EXPORT_rxCallback(rxIsrBuffer, EXPORT_RX_BUFFER_SIZE);
            rxIndex = 0U;
        }
    }
}

void EXPORT_init(void* uartInst)
{
    /* uartInst is a const struct device * */
    uartDev = (const struct device*)uartInst;
    rxIndex = 0U;

    if ((uartDev != NULL) && device_is_ready(uartDev))
    {
        uart_irq_callback_user_data_set(uartDev, EXPORT_uartIsr, NULL);
        uart_irq_rx_enable(uartDev);
    }
    else
    {
        uartDev = NULL;
    }

    /* Reset RX buffer state */
    memset((void*)rxBuffer, 0, sizeof(rxBuffer));
    memset((void*)lastRxPacket, 0, sizeof(lastRxPacket));
    rxPacketCount = 0;
}

void EXPORT_transmitCharBlocking(uint16_t data)
{
    if (uartDev != NULL)
    {
        uart_poll_out(uartDev, (uint8_t)(data & 0xFFU));
    }
}

void EXPORT_transmitStringLengthBlocking(const char* data, uint16_t length)
{
    uint16_t sent;

    /* Nothing is sent if EXPORT_init() was not called with a ready device */
    for (sent = 0U; (uartDev != NULL) && (sent < length); sent++)
    {
        uart_poll_out(uartDev, (uint8_t)data[sent]);
    }
}

void EXPORT_receivedData(uint16_t* receivedData, uint16_t receivedDataLength)
{
    bool error = false;

    /* Validate packet length */
    if (receivedDataLength != PACKET_SIZE)
    {
        error = true;
    }

    /* Validate start byte */
    if (receivedData[0] != PACKET_START_BYTE)
    {
        error = true;
    }

    /* Validate end byte */
    if (receivedData[PACKET_SIZE - 1] != PACKET_END_BYTE)
    {
        error = true;
    }

    /* Extract command byte (index 1) */
    uint16_t cmdByte = receivedData[1];

    /* Pass pointer to payload data starting at index 2 (skip START and CMD bytes) */
    EXPORTPACKAGE_takeActionOnMessage(cmdByte, &receivedData[2], error);
}

void EXPORT_checkReceivedData(void)
{
    /* Polling RX not used — RX is interrupt driven through the UART callback */
}

uint16_t EXPORT_getRxBufferIndex(void)
{
    return (uint16_t)rxPacketCount;
}

void EXPORT_resetRxBuffer(void)
{
    memset((void*)rxBuffer, 0, sizeof(rxBuffer));
    memset((void*)lastRxPacket, 0, sizeof(lastRxPacket));
    rxPacketCount = 0;
}

void EXPORT_rxCallback(uint16_t* data, uint16_t length)
{
    uint16_t i;

    /* Save for debugging (watchpoint: lastRxPacket) */
    for (i = 0U; i < length && i < EXPORT_RX_BUFFER_SIZE; i++)
    {
        rxBuffer[i]     = data[i];
        lastRxPacket[i] = data[i];
    }
    rxPacketCount++;

    /* Invoke packet handler */
    EXPORT_receivedData(rxBuffer, EXPORT_RX_BUFFER_SIZE);
}
