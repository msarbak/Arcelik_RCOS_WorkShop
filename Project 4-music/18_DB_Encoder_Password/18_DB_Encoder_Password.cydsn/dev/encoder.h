/** @file       encoder.h
 *  @brief      Header file of ENCODER device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergun
 *  @date       2.06.2021
 */
#ifndef FILE_ENCODER_H
#define FILE_ENCODER_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_ENCODER_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   ENCODER ENCODER
 *  @ingroup    DEV_IO
 *  @brief      4 bit encoder device
 *  @{
 */

/// @brief  Functions for ENCODER devices
INTERFACE const tsDevIoFuncs devIoEncoderFuncs;

/// @brief  Device specific configuration
typedef struct
{
} tsEncoderConfig;

/// @brief  Device specific parameters
typedef struct
{
    uint8_t value;
} tsEncoderParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *COM1;
    const tsDevIo *COM2;
    const tsDevIo *COM3;
    const tsDevIo *COM4;
    
} tsEncoderConsts;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 */
#define DEV_IO_ENCODER_CREATE(_name ,_COM1 ,_COM2 ,_COM3 ,_COM4 )      \
    tsEncoderParams _name##Params =         \
    {                                           \
        0                                       \
    };                                          \
    const tsEncoderConsts _name##Consts =   \
    {                                           \
        .COM1 = _COM1,                           \
        .COM2 = _COM2,                           \
        .COM3 = _COM3,                           \
        .COM4 = _COM4,                           \
    };                                          \
    DEV_IO_CREATE(_name, devIoEncoderFuncs, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_ENCODER_H
