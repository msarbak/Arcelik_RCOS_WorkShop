/** @file       m41t00.h
 *  @brief      Header file of M41T00 device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Furkan Gullu
 *  @date       2019
 */
#ifndef FILE_M41T00_H
#define FILE_M41T00_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_M41T00_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   M41T00 M41T00
 *  @ingroup    DEV_IO
 *  @brief      ST M41T00 Serial Real-Time Clock
 *  @{
 */

/// @brief  Registry structures of M41T00
typedef struct PLATFORM_PACKED
{
    uint8_t secondsBcd : 7; ///< Seconds value in BCD
    uint8_t st : 1;         ///< Stop Bit: stop clock operation when set to '1'

    uint8_t minutesBcd : 7; ///< Minutes value in BCD
    uint8_t : 1;            ///< empty

    uint8_t hoursBcd : 6; ///< Hours value in BCD
    uint8_t cb : 1;       ///< Century Bit: toggles with passing of each century
    uint8_t ceb : 1;      ///< Century Enable Bit: toggling of cb is enabled when set to '1'

    uint8_t day : 3; ///< Day of week value
    uint8_t : 5;     ///< empty

    uint8_t dateBcd : 6; ///< Day of month value in BCD
    uint8_t : 2;         ///< empty

    uint8_t monthBcd : 5; ///< Month value in BCD
    uint8_t : 3;          ///< empty

    uint8_t yearsBcd; ///< Years value in BCD

    uint8_t calibration : 5; ///< Calibration: correction value for clock error
    uint8_t sign : 1;        ///< Sign Bit: sign of calibration value; '0': negative, '1': positive
    uint8_t freqTest : 1;    ///< Frequency Test Bit: '0': out controlled by outputLevel, '1': freqTest is run
    uint8_t outputLevel : 1; ///< Output Level: level driven on FT pin when freqTest is '0'
} tsM41t00Registries;

/// @brief  Functions for M41T00 device Core
INTERFACE const tsDevCpxFuncs devCpxM41t00CoreFuncs;

/// @brief  M41T00 Core device parameters
typedef struct
{
    tsTimerEvent tev;        ///< Timer event to perform periodic updates
    tsM41t00Registries regs; ///< Last read registry values
    uint32_t time;           ///< Time value wanted to be written/read
    uint32_t date;           ///< Date value wanted to be written/read
    tTicket ticket;          ///< Communication device ticket
    uint8_t writeReq;        ///< Write requested
} tsM41t00CoreParams;

/// @brief  M41T00 Core device constants
typedef struct
{
    tsProcess *process;     ///< Background process of this device
    const tsDevCpx *device; ///< Main device pointer
    const tsDevCom *i2c;    ///< Communication device
    tsTarget *target;       ///< Communication target information
} tsM41t00CoreConsts;

/// @brief  M41T00 Sub device constants
typedef struct
{
    tsM41t00CoreParams *mainParams;
    const tsM41t00CoreConsts *mainConsts;
} tsM41t00SubConsts;

/** @brief  Create a M41T00 core device
 *  @param  _name   Name of devCpx object
 *  @param  _i2c    I2C communication device
 *  @param  _target Communication channel information
 */
#define DEV_CPX_M41T00_CORE_CREATE(_name, _i2c, _target)                          \
    tsProcess _name##Process;                                                     \
    const tsDevCpx _name;                                                         \
    tsM41t00CoreParams _name##Params =                                            \
        {                                                                         \
            .tev      = TIMER_EVENT_INIT(PROCESS_NONE, PROCESS_NONE, EVENT_NONE), \
            .regs     = {.secondsBcd = 0},                                        \
            .time     = 0,                                                        \
            .date     = 0,                                                        \
            .ticket   = 0,                                                        \
            .writeReq = 0,                                                        \
    };                                                                            \
    const tsM41t00CoreConsts _name##Consts =                                      \
        {                                                                         \
            .process = &_name##Process,                                           \
            .device  = &_name,                                                    \
            .i2c     = &_i2c,                                                     \
            .target  = &_target,                                                  \
    };                                                                            \
    DEV_CPX_CREATE(_name, devCpxM41t00CoreFuncs, &_name##Params, &_name##Consts)  \
    PROCESS_CREATE(_name##Process, m41t00ProcInit, m41t00ProcDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/** @} */

/** @defgroup   M41T00_TIME TIME
 *  @ingroup    M41T00
 *  @brief      Time of day value in seconds
 *  @{
 */

/// @brief  Functions for M41T00 device Time
INTERFACE const tsDevIoFuncs devIoM41t00TimeFuncs;

/** @brief  Create a M41T00 time device
 *  @param  _name   Name of devCpx object
 *  @param  _core   Core device name
 */
#define DEV_IO_M41T00_TIME_CREATE(_name, _core) \
    const tsM41t00SubConsts _name##Consts =     \
        {                                       \
            .mainParams = &_core##Params,       \
            .mainConsts = &_core##Consts,       \
    };                                          \
    DEV_IO_CREATE(_name, devIoM41t00TimeFuncs, NULL, &_name##Consts)

/** @} */

/** @defgroup   M41T00_DATE DATE
 *  @ingroup    M41T00
 *  @brief      Date value
 *  @details    Least significant byte is day of month, second to LSByte is month of year and two MSBytes are year value
 *  @{
 */

/// @brief  Functions for M41T00 device Date
INTERFACE const tsDevIoFuncs devIoM41t00DateFuncs;

/** @brief  Create a M41T00 date device
 *  @param  _name   Name of devCpx object
 *  @param  _core   Core device name
 */
#define DEV_IO_M41T00_DATE_CREATE(_name, _core) \
    const tsM41t00SubConsts _name##Consts =     \
        {                                       \
            .mainParams = &_core##Params,       \
            .mainConsts = &_core##Consts,       \
    };                                          \
    DEV_IO_CREATE(_name, devIoM41t00DateFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_M41T00_H
