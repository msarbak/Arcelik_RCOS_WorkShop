/** @file       psoc6/uart.h
 *  @brief      CYPRESS PSOC6 Universal Asynchronous Receiver-Transmitter based on SCB
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Emre Karabek
 *  @author     Deniz Fer
 *  @date       20.04.2020
 */
#ifndef FILE_UART_H
#define FILE_UART_H

#include "rcos.h"

/**
 *  @defgroup   PSOC6_SCBUART UART
 *  @ingroup    PSOC6_DEV_COM
 *  @brief      CYPRESS PSOC6 Universal Asynchronous Receiver-Transmitter based on SCB
 * 
 *  @defgroup   PSOC6_SCBUART_WB WITH SW BUFFER
 *  @warning    NOT IMPLEMENTED YET!!!
 *  @ingroup    PSOC6_SCBUART
 * 
 *  @defgroup   PSOC6_SCBUART_WOB WITHOUT SW BUFFER
 *  @ingroup    PSOC6_SCBUART
 * 
 */

#ifndef FILE_UART_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @addtogroup PSOC6_SCBUART_WB
 *  @{
 */

#if defined RCOS_DOXYGEN

/// @brief  Functions for SCBUART devices
INTERFACE const tsDevComFuncs devComUartWBFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t lastTx;                         ///< Amount of data last transmited
} tsUartWBParams;

/// @brief  Device specific constants
typedef struct
{
    CySCB_Type *base;                        ///< CYPRESS uart base
    cy_stc_scb_uart_context_t *context;      ///< CYPRESS component context
    const cy_stc_scb_uart_config_t *config;  ///< CYPRESS component config
    void (*start)(void);                     ///< CYPRESS component function: _Start
    cy_cb_scb_uart_handle_events_t callback; ///< CYPRESS component callback
    uint8_t *rxBuffer;                       ///< Ringbuffer for receive
    uint32_t rxSize;                         ///< rxBuffer size
    tsDeque *txFifo;                         ///< Ringbuffer for transmit
} tsUartWBConsts;

/** @brief  Create a devCom device for UART with extra sw buffer
 *  @param  _name Name of devCom object
 *  @param  _cyUart CYPRESS UART component name
 *  @param  _rxQueSize Receive queue size
 *  @param  _txQueSize Transmit queue size
 */
#define DEV_COM_UART_WB_CREATE(_name, _cyUart, _rxQueSize, _txQueSize)  \
    extern void uartWBCallBack(const tsDevCom *device, uint32_t event); \
    uint8_t _name##RxBuffer[_rxQueSize] = {0};                          \
    FIFO_CREATE(_name##Fifo, _txQueSize);                               \
    const tsDevCom _name;                                               \
    void _name##CallBack(uint32_t event)                                \
    {                                                                   \
        return uartWBCallBack(&_name, event);                           \
    }                                                                   \
    tsUartWBParams _name##Params =                                      \
        {                                                               \
            .lastTx = 0,                                                \
    };                                                                  \
    const tsUartWBConsts _name##Consts =                                \
        {                                                               \
            .base     = _cyUart##_HW,                                   \
            .context  = &_cyUart##_context,                             \
            .config   = &_cyUart##_config,                              \
            .start    = _cyUart##_Start,                                \
            .callback = _name##CallBack,                                \
            .rxBuffer = _name##RxBuffer,                                \
            .rxSize   = ARRAY_SIZE(_name##RxBuffer),                    \
            .txFifo   = &_name##Fifo,                                   \
    };                                                                  \
    DEV_COM_CREATE(_name, devComUartWBFuncs, &_name##Params, &_name##Consts)

#endif

/** @} */

/** @addtogroup PSOC6_SCBUART_WOB
 *  @{
 */

/// @brief  Functions for SCBUART devices
INTERFACE const tsDevComFuncs devComUartWOBFuncs;

/// @brief  Device specific constants
typedef struct
{
    CySCB_Type *base;                       ///< CYPRESS uart base
    const cy_stc_scb_uart_config_t *config; ///< CYPRESS component config
    void (*start)(void);                    ///< CYPRESS component function: _Start
    // cy_stc_scb_uart_context_t *context;      ///< CYPRESS component context
    // cy_cb_scb_uart_handle_events_t callback; ///< CYPRESS component callback
} tsUartWOBConsts;

/** @brief  Create a devCom device for UART
 *  @param  _name Name of devCom object
 *  @param  _cyUart CYPRESS UART component name
 */
#define DEV_COM_UART_WOB_CREATE(_name, _cyUart)                                     \
    /*extern void uartWOBCallBack(const tsDevCom *device, uint32_t event);*/        \
    /*const tsDevCom _name;                     */                                  \
    /*void _name##CallBack(uint32_t event)      */                                  \
    /*{                                         */                                  \
    /*    return uartWOBCallBack(&_name, event);*/                                  \
    /*}                                         */                                  \
    const tsUartWOBConsts _name##Consts =                                           \
        {                                                                           \
            .base   = _cyUart##_HW,                                                 \
            .config = &_cyUart##_config,                                            \
            .start  = _cyUart##_Start,                                              \
            /*.context  = &_cyUart##_context,*/ /*.callback = _name##CallBack,   */ \
    };                                                                              \
    DEV_COM_CREATE(_name, devComUartWOBFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_UART_H
