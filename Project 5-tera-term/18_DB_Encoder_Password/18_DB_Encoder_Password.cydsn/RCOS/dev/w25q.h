/** @file       w25q.h
 *  @brief      W25Q Winbond Serial NOR Flash
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       28.11.2018
 */
#ifndef FILE_W25Q_H
#define FILE_W25Q_H

#include "rcos.h"

/**
 *  @defgroup   W25Q W25Q
 *  @ingroup    DEV_MEM
 *  @brief      W25Q Winbond Serial NOR Flash
 *  @{
 */

#ifndef FILE_W25Q_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for W25q devices
INTERFACE const tsDevMemFuncs devMemW25qFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t address; ///< Operation address
    void *buffer;     ///< Operation buffer
    uint32_t length;  ///< Operation total length
    uint32_t command; ///< Operation command
    tsThread child;   ///< Child thread holder
    tTicket ticket;   ///< Ticket that will be used for SPI process queue
    uint16_t subLen;  ///< Length for a single operation
} tsW25qParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevCom *spi; ///< Device connected communication device
    tsTarget *target;    ///< Device target information on communication device
    tsProcess *process;  ///< Process that handles the jobs for this device
} tsW25qConsts;

/** @brief  Create a W25Q devMem
 *  @param  _name       Name of devMem object
 *  @param  _spi        Name of devCom object for communication
 *  @param  _target     Name of target information for devCom object
 */
#define DEV_MEM_W25Q_CREATE(_name, _spi, _target)                          \
    tsProcess _name##Process;                                              \
    tsW25qParams _name##Params =                                           \
        {                                                                  \
            .ticket = -1,                                                  \
    };                                                                     \
    const tsW25qConsts _name##Consts =                                     \
        {                                                                  \
            .spi     = &_spi,                                              \
            .target  = &_target,                                           \
            .process = &_name##Process,                                    \
    };                                                                     \
    DEV_MEM_CREATE(_name, devMemW25qFuncs, &_name##Params, &_name##Consts) \
    PROCESS_CREATE(_name##Process, w25qProInit, w25qProDeinit, PROCESS_NONE, &_name##Params, &_name)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_W25Q_H
