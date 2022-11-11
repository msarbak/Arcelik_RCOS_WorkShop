/** @file       enc_inc.h
 *  @brief      Header file of ENC_INC device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       15.01.2019
 */
#ifndef FILE_ENC_INC_H
#define FILE_ENC_INC_H

#include "rcos.h"

/**
 *  @defgroup   ENC_INC ENC_INC
 *  @ingroup    DEV_IO
 *  @brief      Incremental encoder device driver with ISR
 *  @details    Assumes that signal A changes prior to signal B during clock-wise rotation.
 *  @code
 *              (A) _---___---__
 *              (B) __---___---_
 *  @endcode
 *  @{
 */

#ifndef FILE_ENC_INC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for ENCINC devices
INTERFACE const tsDevIoFuncs devIoEncincFuncs;

/// @brief  Generic function called to determine which enc_inc device had a signal
INTERFACE void encincCheckIsr(const tsDevIo *device);

/// @brief  Device specific parameters
typedef struct
{
    int32_t count; ///< +: clock-wise rotation
                   ///< -: counter-clock-wise rotation
} tsEncIncParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *signalA; ///< Signal A
    const tsDevIo *signalB; ///< Signal B
    const tsDevCpx *isrA;   ///< Isr for signal A
    void (*isrFunc)(void);  ///< This device isr function
} tsEncIncConsts;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _signalA    devIo for signal A input
 *  @param  _signalB    devIo for signal B input
 *  @param  _isrA       devCpx for ISR of signal A
 */
#define DEV_IO_ENC_INC_CREATE(_name, _signalA, _signalB, _isrA) \
    const tsDevIo _name;                                        \
    void _name##Isr(void)                                       \
    {                                                           \
        encincCheckIsr(&_name);                                 \
    }                                                           \
    tsEncIncParams _name##Params = {.count = 0};                \
    const tsEncIncConsts _name##Consts =                        \
        {                                                       \
            &_signalA,                                          \
            &_signalB,                                          \
            _isrA,                                              \
            _name##Isr,                                         \
    };                                                          \
    DEV_IO_CREATE(_name, devIoEncincFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_ENC_INC_H
