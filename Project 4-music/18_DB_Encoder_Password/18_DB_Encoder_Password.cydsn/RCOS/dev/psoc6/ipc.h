/** @file       psoc6/ipc.h
 *  @brief      Header file of IPC device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergun
 *  @date       15.04.2020
 */
#ifndef FILE_IPC_H
#define FILE_IPC_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_IPC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   PSOC6_IPC IPC
 *  @ingroup    PSOC6_DEV_COM
 *  @brief      Inter-Processor Communication
 *  @todo       When there's more than 2 cores, the overall architecture should be changed
 *  @{
 */

/// @brief  Functions for IPC devices
INTERFACE const tsDevComFuncs devComIpcFuncs;

/// @brief  IPC frame that is transmitted between cpu cores
typedef struct
{
    uint8_t clientId;   ///< IPC required data area, check cy_stc_flash_notify_t and "cy_ipc_drv.h"
    uint8_t userCode;   ///< IPC required data area
    uint16_t intrMask;  ///< IPC required data area
    uint16_t rxFree;    ///< Sender's receive buffer free space
    uint16_t txLoad;    ///< Sender's transmit data amount
    uint32_t length;    ///< Length of data in frame buffer
    uint8_t buffer[16]; ///< Frame data buffer
} tsIpcFrame;

/// @brief  Device specific parameters
typedef struct
{
    tsIpcFrame sendFrame;  ///< Frame in the process of transmiting
    uint16_t rxFree;       ///< Target receive buffer free space
    uint16_t txLoad;       ///< Target transmit data amount
    tsTimerCallBack delay; ///< Delayed operation
} tsIpcParams;

/// @brief  Device specific constants
typedef struct
{
    cy_ipc_pipe_callback_ptr_t callback; ///< Called when received
    tsDeque *rxFifo;                     ///< Receive FIFO
    tsDeque *txFifo;                     ///< Transmit FIFO
} tsIpcConsts;

/** @brief      Create a devCom of type IPC
 *  @warning    _clientId = 2 is reserved for CY_FLASH_IPC_CLIENT_ID
 *  @param      _name Name of devCom object
 *  @param      _rxSize Size of receive buffer
 *  @param      _txSize Size of transmit buffer
 *  @param      _clientId IPC client ID must match target CPU IPC com device
 */
#define DEV_COM_IPC_CREATE(_name, _rxSize, _txSize, _clientId)        \
    const tsDevCom _name;                                             \
    extern void ipcRecvIsr(const tsDevCom *device, uint32_t *msgPtr); \
    extern TIMER_CALLBACK_FUNC(ipcDelay);                             \
    void _name##Recv(uint32_t *msgPtr)                                \
    {                                                                 \
        ipcRecvIsr(&_name, msgPtr);                                   \
    }                                                                 \
    FIFO_CREATE(_name##RxFifo, _rxSize)                               \
    FIFO_CREATE(_name##TxFifo, _txSize)                               \
    tsIpcParams _name##Params =                                       \
        {                                                             \
            .sendFrame =                                              \
                {                                                     \
                    .clientId = (_clientId),                          \
                    .intrMask = CY_SYS_CYPIPE_INTR_MASK,              \
                },                                                    \
            .delay = TIMER_CALLBACK_INIT(ipcDelay, &_name),           \
    };                                                                \
    const tsIpcConsts _name##Consts =                                 \
        {                                                             \
            .callback = _name##Recv,                                  \
            .rxFifo   = &_name##RxFifo,                               \
            .txFifo   = &_name##TxFifo,                               \
    };                                                                \
    DEV_COM_CREATE(_name, devComIpcFuncs, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_IPC_H
