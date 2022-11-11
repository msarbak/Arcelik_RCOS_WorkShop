/** @file       sevensegmentdisplay.h
 *  @brief      Header file of SEVENSEGMENTDISPLAY device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ozkan AKGUL
 *  @date       1.06.2021
 */
#ifndef FILE_SEVENSEGMENTDISPLAY_H
#define FILE_SEVENSEGMENTDISPLAY_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_SEVENSEGMENTDISPLAY_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   SEVENSEGMENTDISPLAY SEVENSEGMENTDISPLAY
 *  @ingroup    DEV_IO
 *  @brief      Seven segment display for RCOS SDK DB Board
 *  @{
 */

/// @brief  Functions for SEVENSEGMENTDISPLAY devices
INTERFACE const tsDevIoFuncs devIoSevenSegmentDisplayFuncs;

/// @brief Seven segment display messages
typedef enum
{
    eDisplayMessages_A = 20  ,          ///< starting from 20, blow 20 it is number
    eDisplayMessages_B   ,
    eDisplayMessages_C   ,
    eDisplayMessages_Crh ,
    eDisplayMessages_Cs  ,
    eDisplayMessages_Csrh,
    eDisplayMessages_D   ,
    eDisplayMessages_E   ,
    eDisplayMessages_F   ,
    eDisplayMessages_G   ,
    eDisplayMessages_Gs  ,
    eDisplayMessages_H   ,
    eDisplayMessages_Hs  ,
    eDisplayMessages_Hsrh,
    eDisplayMessages_I   ,
    eDisplayMessages_Is  ,
    eDisplayMessages_Isrh,
    eDisplayMessages_J   ,
    eDisplayMessages_K   ,
    eDisplayMessages_L   ,
    eDisplayMessages_Lrh ,
    eDisplayMessages_M   ,
    eDisplayMessages_n   ,
    eDisplayMessages_O   ,
    eDisplayMessages_Os  ,
    eDisplayMessages_P   ,
    eDisplayMessages_Q   ,
    eDisplayMessages_R   ,
    eDisplayMessages_S   ,
    eDisplayMessages_T   ,
    eDisplayMessages_U   ,
    eDisplayMessages_Urv ,
    eDisplayMessages_Us  ,
    eDisplayMessages_Usrv,
    eDisplayMessages_V   ,
    eDisplayMessages_W   ,
    eDisplayMessages_X   ,
    eDisplayMessages_Y   ,
    eDisplayMessages_Z   ,

    eDisplayMessages_lA   ,
    eDisplayMessages_lB   ,
    eDisplayMessages_lC   ,
    eDisplayMessages_lCrh ,
    eDisplayMessages_lCs  ,
    eDisplayMessages_lCsrh,
    eDisplayMessages_lD   ,
    eDisplayMessages_lE   ,
    eDisplayMessages_lF   ,
    eDisplayMessages_lG   ,
    eDisplayMessages_lGs  ,
    eDisplayMessages_lH   ,
    eDisplayMessages_lHs  ,
    eDisplayMessages_lHsrh,
    eDisplayMessages_lI   ,
    eDisplayMessages_lIs  ,
    eDisplayMessages_lIsrh,
    eDisplayMessages_lJ   ,
    eDisplayMessages_lK   ,
    eDisplayMessages_lL   ,
    eDisplayMessages_lLrh ,
    eDisplayMessages_lM   ,
    eDisplayMessages_ln   ,
    eDisplayMessages_lO   ,
    eDisplayMessages_lOs  ,
    eDisplayMessages_lP   ,
    eDisplayMessages_lQ   ,
    eDisplayMessages_lR   ,
    eDisplayMessages_lS   ,
    eDisplayMessages_lT   ,
    eDisplayMessages_lU   ,
    eDisplayMessages_lUrv ,
    eDisplayMessages_lUs  ,
    eDisplayMessages_lUsrv,
    eDisplayMessages_lV   ,
    eDisplayMessages_lW   ,
    eDisplayMessages_lX   ,
    eDisplayMessages_lY   ,
    eDisplayMessages_lZ   ,
    eDisplayMessagesEnd
}teDisplayMessages;

/// @brief two digit sevensegment struct
typedef struct 
{
    uint8_t digit1;         ///< field for digit 1 values
    uint8_t digit2;         ///< field for digit 2 values
}tsTwoSevenSegment;

/// @brief  Device specific configuration
typedef struct
{
} tsSevenSegmentDisplayConfig;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerEvent timerSSDrive;              ///< Timer for driving seven segment digits and segments
    uint8_t driveIndex;                     ///< for digit switch
    tsTwoSevenSegment sevenSegment;         ///< for seven segment data
    tsProcess *process;                     ///< processs for device
} tsSevenSegmentDisplayParams;

/// @brief  Events of a SevenSegmentDisplay process
typedef enum
{
    eSevenSegmentEventNone,
    eSevenSegmentDriveEvent,
} teSevenSegmentDisplayEvents;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *sevenSegment;            ///< io device for seven segment outputs - iocomb
    const tsDevIo *digit1;                  ///< io device for digit1 output
    const tsDevIo *digit2;                  ///< io device for digit2 output
} tsSevenSegmentDisplayConsts;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 *  @param  _sevenSegment   Seven Segment io comb device to drive io pins all together
 *  @param  _digit1 IO device for digit 1 pin
 *  @param  _digit2 IO device for digit 2 pin
 */
#define DEV_IO_SEVENSEGMENTDISPLAY_CREATE(_name, _sevenSegment, _digit1, _digit2 )              \
    tsProcess _name##Process;                                                                   \
    tsSevenSegmentDisplayParams _name##Params =                                                 \
    {                                                                                           \
       .timerSSDrive = TIMER_EVENT_INIT(PROCESS_NONE, PROCESS_NONE, eSevenSegmentDriveEvent),   \
       .process = &_name##Process,                                                              \
    };                                                                                          \
    const tsSevenSegmentDisplayConsts _name##Consts =                                           \
    {                                                                                           \
        .sevenSegment = &_sevenSegment,                                                         \
        .digit1 = &_digit1,                                                                     \
        .digit2 = &_digit2,                                                                     \
    };                                                                                          \
    DEV_IO_CREATE(_name, devIoSevenSegmentDisplayFuncs, &_name##Params, &_name##Consts)         \
    PROCESS_CREATE(_name##Process, sevenSegmentDisplayProcessInit, sevenSegmentDisplayProcessDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_SEVENSEGMENTDISPLAY_H
