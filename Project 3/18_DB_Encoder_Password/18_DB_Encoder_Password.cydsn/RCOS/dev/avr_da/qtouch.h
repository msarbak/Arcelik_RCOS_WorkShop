/** @file       qtouch.h
 *  @brief      Header file of QTOUCH device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Hamed Mohammadi
 *  @date       07/05/2021
 */
#ifndef FILE_QTOUCH_H
#define FILE_QTOUCH_H

/// Includes
#include "rcos.h"
#include <avr/io.h>
#include "qtouch/touch.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_QTOUCH_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   QTOUCH QTOUCH
 *  @ingroup    AVR_DA_DEV_IO
 *  @brief      Microchip QTouch Modular Library connector
 *  @{
 */

/// @brief  Functions for QTOUCH devices
INTERFACE const tsDevIoFuncs devIoQtouchFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t buttons; ///< Last value of touch sensors
} tsQtouchParams;

/// @brief  Device specific constants
typedef struct
{
    tsProcess *process;        ///< Process that handles the jobs for this device
    const uint8_t buttonCount; ///< Numeber of button sensors
} tsQtouchConsts;

/** @brief  Create a devIo to read touch butons from qtouch library
 *  @param  _name       Name of devIo object
 *  @param  _buttons    Number of button sensors
 */
#define DEV_IO_QTOUCH_CREATE(_name, _buttons)                              \
    tsProcess _name##Process;                                              \
    tsQtouchParams _name##Params = {                                       \
        0,                                                                 \
    };                                                                     \
    const tsQtouchConsts _name##Consts = {                                 \
        &_name##Process,                                                   \
        _buttons,                                                          \
    };                                                                     \
    DEV_IO_CREATE(_name, devIoQtouchFuncs, &_name##Params, &_name##Consts) \
    PROCESS_CREATE(_name##Process, qtouchProInit, qtouchProDeinit, PROCESS_NONE, NULL, &_name);

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_QTOUCH_H
