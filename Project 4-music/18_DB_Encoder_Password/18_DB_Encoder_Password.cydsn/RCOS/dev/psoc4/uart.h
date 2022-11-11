/** @file       psoc4/uart.h
 *  @brief      CYPRESS PSOC4 Universal Asynchronous Receiver-Transmitter based on SCB
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       11.01.2019
 */
#ifndef FILE_UART_H
#define FILE_UART_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_SCBUART UART
 *  @ingroup    PSOC4_DEV_COM
 *  @brief      CYPRESS PSOC4 Universal Asynchronous Receiver-Transmitter based on SCB
 *  @{
 */

#ifndef FILE_UART_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for SCBUART devices
INTERFACE const tsDevComFuncs devComUartFuncs;

/// @brief  Device specific constants
typedef struct
{
    void (*start)(void);                             ///< CYPRESS component function: _Start
    void (*stop)(void);                              ///< CYPRESS component function: _Stop
    uint32 (*getTxSize)(void);                       ///< CYPRESS component function: _SpiUartGetTxBufferSize
    void (*send)(const uint8 wrBuf[], uint32 count); ///< CYPRESS component function: _SpiUartPutArray
    uint32 (*getRxSize)(void);                       ///< CYPRESS component function: _SpiUartGetRxBufferSize
    uint32 (*getData)(void);                         ///< CYPRESS component function: _SpiUartReadRxData
} tsUartConsts;

/** @brief      Create a devCom
 *  @details    Open/Close functions are dummy
 *  @param      _name   Name of devCom object
 *  @param      _cyScb  CYPRESS SCB name
 *  @warning DMA settings for RX should be to transfer "Word to Byte" with "Increment destination address"
 */
#define DEV_COM_UART_CREATE(_name, _cyScb)   \
    const tsUartConsts _name##Consts =       \
        {                                    \
            _cyScb##_Start,                  \
            _cyScb##_Stop,                   \
            _cyScb##_SpiUartGetTxBufferSize, \
            _cyScb##_SpiUartPutArray,        \
            _cyScb##_SpiUartGetRxBufferSize, \
            _cyScb##_SpiUartReadRxData,      \
    };                                       \
    DEV_COM_CREATE(_name, devComUartFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_UART_H
