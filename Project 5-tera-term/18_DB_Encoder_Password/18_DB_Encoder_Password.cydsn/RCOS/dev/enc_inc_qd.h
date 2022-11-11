/** @file       enc_inc_qd.h
 *  @brief      Header file of ENC_INC_QD device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Atilla Osman Mert Gemalmaz
 *  @date       01.07.2019
 */
#ifndef FILE_ENC_INC_QD_H
#define FILE_ENC_INC_QD_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_ENC_INC_QD_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   ENC_INC_QD ENC_INC_QD
 *  @ingroup    DEV_IO
 *  @brief      Incremental Encoder device driver using a Quadrature Decoder
 *  @{
 */

/// @brief  Functions for ENC_INC_QD devices
INTERFACE const tsDevIoFuncs devIoEncIncQdFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevIo *sigA;      ///< GPIO for reading encoder's signal A
    const tsDevIo *sigB;      ///< GPIO for reading encoder's signal B
    const tsDevIo *quadDec;   ///< Quadrature decoder device
    tsTimerCallBack timerEnc; ///< Periodicly checks encoder state
    uint32_t resetValue;      ///< Counter reset value
    uint32_t noise;           ///< Noise value
    int32_t gEncoderCounter;
    uint8_t encoderTimerCallback;
    uint8_t lastSignal;
} tsEncIncQdParams;

/** @brief  Create an incremental encoder reader using a quadrature decoder
 *  @param  _name       Name of devIo object
 *  @param  _sigA       Encoder's signal A
 *  @param  _sigB       Encoder's signal B
 *  @param  _quadDec    Quadrature decoder device
 *  @param  _quadDecBit Quadrature decoder value bit count
 *  @param  _noiseLimit Value to check before declaring a count
 *  @param  _time       Period of time for signal integrity check(a single count is generated with each check)
 */
#define DEV_IO_ENC_INC_QD_CREATE(_name, _sigA, _sigB, _quadDec, _quadDecBit, _noiseLimit, _time) \
    extern TIMER_CALLBACK_FUNC(encIncQdDecoder);                                                 \
    const tsDevIo _name;                                                                         \
    tsEncIncQdParams _name##Params =                                                             \
        {                                                                                        \
            .sigA                 = &_sigA,                                                      \
            .sigB                 = &_sigB,                                                      \
            .quadDec              = &_quadDec,                                                   \
            .timerEnc             = TIMER_CALLBACK_INIT(encIncQdDecoder, &_name),                \
            .resetValue           = BIT((_quadDecBit)-1),                                        \
            .noise                = (_noiseLimit),                                               \
            .gEncoderCounter      = 0,                                                           \
            .encoderTimerCallback = _time,                                                       \
            .lastSignal           = 0,                                                           \
    };                                                                                           \
    DEV_IO_CREATE(_name, devIoEncIncQdFuncs, &_name##Params, NULL)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_ENC_INC_QD_H
