/** @file       24cxx.h
 *  @brief      Header file of 24CXX device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Rohat Ibishukcu
 *  @date       28.06.2020
 */
#ifndef FILE_24CXX_H
#define FILE_24CXX_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_24CXX_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   24CXX 24CXX
 *  @ingroup    DEV_MEM
 *  @brief      24cXX EEPROM device driver
 *  @{
 */

/// @brief  Functions for 24CXX devices
INTERFACE const tsDevMemFuncs devMem24cxxFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t address; ///< Operation address
    void *buffer;     ///< Operation buffer
    uint32_t length;  ///< Operation total length
    tTicket ticket;   ///< Ticket that will be used for SPI process queue
    uint16_t subLen;  ///< Length for a single operation
} ts24cxxParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevCom *i2c; ///< Device connected communication device
    tsTarget *target;    ///< Device target information on communication device
    tsProcess *process;  ///< Process that handles the jobs for this device
    uint8_t aaa;         ///< I2C slave address modifier
    uint8_t xx;          ///< XX code of EEPROM
} ts24cxxConsts;

/** @brief  Create a 24cXX EEPROM memory device
 *  @param  _name   Name of devMem object
 *  @param  _i2c    I2C communication device that will be used
 *  @param  _target tsTarget object
 *  @param  _aaa    I2C slave address modifier pins(A2A1A0) (for all pins 0x07)
 *  @param  _xx     XX code of EEPROM(omit leading zeros)
 */
#define DEV_MEM_24CXX_CREATE(_name, _i2c, _target, _aaa, _xx)               \
    tsProcess _name##Process;                                               \
    ts24cxxParams _name##Params =                                           \
        {                                                                   \
            0,                                                              \
    };                                                                      \
    const ts24cxxConsts _name##Consts =                                     \
        {                                                                   \
            /* DONT FORGET TO FILL INITAL VALUES */                         \
            .i2c     = &_i2c,                                               \
            .target  = &_target,                                            \
            .process = &_name##Process,                                     \
            .aaa     = (_aaa),                                              \
            .xx      = _xx,                                                 \
    };                                                                      \
    DEV_MEM_CREATE(_name, devMem24cxxFuncs, &_name##Params, &_name##Consts) \
    PROCESS_CREATE(_name##Process, nvm24cxxProcInit, nvm24cxxProcDeinit, PROCESS_NONE, &_name##Params, &_name)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_24CXX_H
