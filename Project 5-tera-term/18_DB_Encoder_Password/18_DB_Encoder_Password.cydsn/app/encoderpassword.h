/** @file       encoderpassword.h
 *  @brief      ENCODERPASSWORD application header file
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ozkan AKGUL
 *  @date       2.06.2021
 */
#ifndef FILE_ENCODERPASSWORD_H
#define FILE_ENCODERPASSWORD_H

/** INCLUDES ******************************************************************/
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_ENCODERPASSWORD_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   ENCODERPASSWORD ENCODERPASSWORD
 *  @ingroup    APP
 *  @brief      Encoder password application for RCOS SDK DB Board
 *  @{
 */

/// @brief  Events of a EncoderPassword process
typedef enum
{
    eEncoderPasswordEventNone,
    p46Pressed,
    p46Released,
    p70Pressed,
    p70Released,
    p71Pressed,
    p71Released,
    eEPEventUIUpdate,
    eEPEventLedsBlink,
} teEncoderPasswordEvents;

/// @brief  Parameters of a EncoderPassword process
typedef struct
{
    tsTimerEvent timerUIGeneral;              ///<      Timer event for general UI operations

    uint8_t lockingState;                      ///<    Storing locking states
    uint8_t passwordIndex;                      ///<    Storing password item index
    uint16_t password;                          ///<    Password value
    uint16_t unlockValue;                       ///<    Value to unlock password
} tsEncoderPasswordParams;

/// @brief  Constants of a EncoderPassword process
typedef struct
{
    const tsDevIo *redLed;                      ///<    IO device for red led
    const tsDevIo *blueLed;                     ///<    IO device for blue led
    const tsDevIo *yellowLed;                   ///<    IO device for yellow led
    const tsDevIo *sevenSegmentDisplay;         ///<    IO device for seven segment display
    const tsDevIo *encoder;                     ///<    IO device for encoder
    const tsDevIo *buzzer;                      ///<    IO device for buzzer
    const tsDevCom *uart;
} tsEncoderPasswordConsts;

/** @brief  EncoderPassword process object creation macro
 *  @param  _name   Name of process object
 *  @param  _enum   Process enumeration for this object
 *  @param  _redLed IO device for red led
 *  @param  _blueLed IO device for blue led
 *  @param  _yellowLed IO device for yellow led
 *  @param  _sevenSegmentDisplay IO device for seven segment display
 *  @param  _encoder IO device for encoder
 *  @param  _buzzer IO device for buzzer
 */
#define PROCESS_ENCODERPASSWORD_CREATE(_name, _enum, _redLed, _blueLed, _yellowLed, _sevenSegmentDisplay, _encoder, _buzzer, _uart) \
    tsEncoderPasswordParams _name##Params =           \
    {                                         \
        /* DONT FORGET TO FILL INITAL VALUES */   \
    };                                            \
    const tsEncoderPasswordConsts _name##Consts =     \
    {                                                        \
        .redLed = &_redLed,                                                         \
        .blueLed = &_blueLed,                                                       \
        .yellowLed = &_yellowLed,                                                   \
        .sevenSegmentDisplay = &_sevenSegmentDisplay,                               \
        .encoder = &_encoder,                                                       \
        .buzzer = &_buzzer,                                                         \
.uart = &_uart   \
    };                                            \
    PROCESS_CREATE(_name, encoderPasswordInit, encoderPasswordDeinit, _enum, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_ENCODERPASSWORD_H
