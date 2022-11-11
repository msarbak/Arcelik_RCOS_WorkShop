/** @file       ke16/uart.h
 *  @brief      KE16 Universal Asynchronous Receiver-Transmitter
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       30.07.2019
 *  @author     Özkan AKGÜL
 *  @date       18.04.2021
 */
#ifndef FILE_UART_H
#define FILE_UART_H

#include "rcos.h"
#include "fsl_lpuart.h"

/**
 *  @defgroup   KE16_UART UART
 *  @ingroup    KE16_DEV_COM
 *  @brief      KE16 Universal Asynchronous Receiver-Transmitter
 *  @{
 */

#ifndef FILE_UART_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for UART devices
INTERFACE const tsDevComFuncs devComUartFuncs;

/// @brief  Device specific parameters
typedef struct
{
    LPUART_Type *base;             ///< Peripheral base address
    clock_ip_name_t clock;         ///< Peripheral clock name
    const lpuart_config_t *config; ///< Default configuration
    uint8_t *rxBuffer;             ///< Receive ring buffer area
    uint32_t rxBufferSize;         ///< Receive ring buffer size
    tsDeque *txQue;                ///< Transmit queue
    lpuart_handle_t handle;        ///< Handle to hold peripheral information
    lpuart_transfer_t xfer;        ///< Transfer information
    uint32_t opened;               ///< Currently open target
    uint32_t closed;               ///< Target that is ready for closure
} tsUartParams;

/** @brief  Create a devCom
 *  @param  _name       Name of devCom object
 *  @param  _uart       Peripheral index
 *  @param  _baud       Baudrate
 *  @param  _parity     Even, Odd or Disabled
 *  @param  _rxQueSize  Receive queue size
 *  @param  _txQueSize  Transmit queue size
 */
#define DEV_COM_UART_CREATE(_name, _uart, _baud, _parity, _rxQueSize, _txQueSize) \
    uint8_t _name##RxBuffer[_rxQueSize] = {0};                                    \
    FIFO_CREATE(_name##TxQ, _txQueSize)                                           \
    const lpuart_config_t _name##Cfg =                                            \
        {                                                                         \
            .baudRate_Bps    = (_baud),                                           \
            .parityMode      = kLPUART_Parity##_parity,                           \
            .dataBitsCount   = kLPUART_EightDataBits,                             \
            .isMsb           = false,                                             \
            .stopBitCount    = kLPUART_OneStopBit,                                \
            .txFifoWatermark = 0,                                                 \
            .rxFifoWatermark = 0,                                                 \
            .enableRxRTS     = false,                                             \
            .enableTxCTS     = false,                                             \
            .txCtsSource     = kLPUART_CtsSourcePin,                              \
            .txCtsConfig     = kLPUART_CtsSampleAtStart,                          \
            .rxIdleType      = kLPUART_IdleTypeStartBit,                          \
            .rxIdleConfig    = kLPUART_IdleCharacter1,                            \
            .enableTx        = true,                                              \
            .enableRx        = true,                                              \
    };                                                                            \
    tsUartParams _name##Params =                                                  \
        {                                                                         \
            LPUART##_uart,               /* base */                               \
            kCLOCK_Lpuart##_uart,        /* clock */                              \
            &_name##Cfg,                 /* config */                             \
            _name##RxBuffer,             /* rxBuffer */                           \
            ARRAY_SIZE(_name##RxBuffer), /* rxBufferSize */                       \
            &_name##TxQ,                 /* txQue */                              \
            {0},                         /* handle */                             \
            {0},                         /* xfer */                               \
            0,                           /* opened */                             \
            0,                           /* closed */                             \
    };                                                                            \
    DEV_COM_CREATE(_name, devComUartFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_UART_H
