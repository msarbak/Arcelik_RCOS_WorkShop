/** @file       morse.h
 *  @brief      Header file of MORSE device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       1.10.2019
 */
#ifndef FILE_MORSE_H
#define FILE_MORSE_H

/// Includes
#include "rcos.h"
#include "dev/pulse.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_MORSE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   DEV_MORSE MORSE
 *  @ingroup    DEV_COM
 *  @brief      Driver to output Morse Code
 *  @{
 */

/// @brief  Functions for MORSE devices
INTERFACE const tsDevComFuncs devComMorseFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack tcb; ///< Timer callback for background operations
    const char *symbol;  ///< Current symbol being transmitted
} tsMorseParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *pulse; ///< Output device
    tsDeque *fifo;        ///< Fifo to store output characters
    uint32_t unit;        ///< 1 unit of symbol time in milliseconds
} tsMorseConsts;

/** @brief  Create a MORSE devCom
 *  @param  _name       Name of devCom object
 *  @param  _output     Output devIo
 *  @param  _onValue    On value for output devIo
 *  @param  _offValue   Off value for output devIo
 *  @param  _unit       1 unit of symbol timing in milliseconds
 *  @param  _bufferSize Size of buffer for transmission
 */
#define DEV_COM_MORSE_CREATE(_name, _output, _onValue, _offValue, _unit, _bufferSize) \
    const tsDevCom _name;                                                             \
    DEV_IO_PULSE_CREATE(_name##Pulse, _output, _onValue, _offValue)                   \
    FIFO_CREATE(_name##Fifo, _bufferSize);                                            \
    extern TIMER_CALLBACK_FUNC(morseProcess);                                         \
    tsMorseParams _name##Params =                                                     \
        {                                                                             \
            .tcb    = TIMER_CALLBACK_INIT(morseProcess, &_name),                      \
            .symbol = NULL,                                                           \
    };                                                                                \
    const tsMorseConsts _name##Consts =                                               \
        {                                                                             \
            .pulse = &_name##Pulse,                                                   \
            .fifo  = &_name##Fifo,                                                    \
            .unit  = _unit,                                                           \
    };                                                                                \
    DEV_COM_CREATE(_name, devComMorseFuncs, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_MORSE_H
