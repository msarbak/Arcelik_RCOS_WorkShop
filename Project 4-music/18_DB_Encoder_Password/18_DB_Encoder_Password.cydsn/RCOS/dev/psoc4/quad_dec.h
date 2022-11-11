/** @file       psoc4/quad_dec.h
 *  @brief      Header file of QUAD_DEC device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Atilla Osman Mert Gemalmaz
 *  @date       12.11.2019
 */
#ifndef FILE_QUAD_DEC_H
#define FILE_QUAD_DEC_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_QUAD_DEC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   PSOC4_QUAD_DEC QUAD_DEC
 *  @ingroup    PSOC4_DEV_IO
 *  @brief      Quadrature Decoder device driver for CYPRESS PSOC4
 *  @{
 */

/// @brief  Functions for QUAD_DEC devices
INTERFACE const tsDevIoFuncs devIoQuadDecFuncs;

/// @brief  Device specific parameters
typedef struct
{
    void (*start)(void);         ///< Init function ex: Encoder_Start
    void (*stop)(void);          ///< DeInit function ex: Encoder_Stop
    uint32 (*read)(void);        ///< Read function ex: Encoder_ReadCounter
    void (*write)(uint32 count); ///< Write function ex: Encoder_WriteCounter
} tsQuadDecConsts;

/** @brief  Create a devIo for QUAD_DEC
 *  @param  _name   Name of devIo object
 *  @param  _cyName CYPRESS component name
 */
#define DEV_IO_QUAD_DEC_CREATE(_name, _cyName) \
    const tsQuadDecConsts _name##Consts =      \
        {                                      \
            &_cyName##_Start,                  \
            &_cyName##_Stop,                   \
            &_cyName##_ReadCounter,            \
            &_cyName##_WriteCounter,           \
    };                                         \
    DEV_IO_CREATE(_name, devIoQuadDecFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_QUAD_DEC_H
