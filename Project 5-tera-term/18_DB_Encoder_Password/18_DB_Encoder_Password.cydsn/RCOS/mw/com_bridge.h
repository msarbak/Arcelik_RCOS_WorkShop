/** @file       com_bridge.h
 *  @brief      COM_BRIDGE middleware header file
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       09/06/2020
 */
#ifndef FILE_COM_BRIDGE_H
#define FILE_COM_BRIDGE_H

/** INCLUDES ******************************************************************/
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_COM_BRIDGE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   COM_BRIDGE COM_BRIDGE
 *  @ingroup    MW
 *  @brief      Com-Bridge used to connect 2 devCom devices to each other to transfer data from the one to the other
 *  @{
 */

/// @brief  Parameters of a ComBridge process
typedef struct
{
    tTicket ticket1;
    tTicket ticket2;
} tsComBridgeParams;

/// @brief  Constants of a ComBridge process
typedef struct
{
    const tsDevCom *com1;
    tsTarget *target1;
    const tsDevCom *com2;
    tsTarget *target2;
} tsComBridgeConsts;

/** @brief  ComBridge process object creation macro
 *  @param  _name Name of process object
 *  @param  _com1 First devCom
 *  @param  _target1 First target
 *  @param  _com2 Second devCom
 *  @param  _target2 Second target
 */
#define PROCESS_COM_BRIDGE_CREATE(_name, _com1, _target1, _com2, _target2) \
    tsComBridgeParams _name##Params =                                      \
        {                                                                  \
            .ticket1 = -1,                                                 \
            .ticket2 = -1,                                                 \
    };                                                                     \
    const tsComBridgeConsts _name##Consts =                                \
        {                                                                  \
            .com1    = _com1,                                              \
            .target1 = _target1,                                           \
            .com2    = _com2,                                              \
            .target2 = _target2,                                           \
    };                                                                     \
    PROCESS_CREATE(_name, comBridgeInit, comBridgeDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_COM_BRIDGE_H
