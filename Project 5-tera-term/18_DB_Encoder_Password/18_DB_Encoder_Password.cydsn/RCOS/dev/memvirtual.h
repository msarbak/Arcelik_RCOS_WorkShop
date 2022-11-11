/** @file       memvirtual.h
 *  @brief      Memory device that uses a RAM area
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       1.08.2019
 */
#ifndef FILE_MEMVIRTUAL_H
#define FILE_MEMVIRTUAL_H

#include "rcos.h"

/**
 *  @defgroup   MEMVIRTUAL MEMVIRTUAL
 *  @ingroup    DEV_MEM
 *  @brief      Memory device that uses a RAM area
 *  @{
 */

#ifndef FILE_MEMVIRTUAL_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for MEMVIRTUAL devices
INTERFACE const tsDevMemFuncs devMemMemvirtualFuncs;

/// @brief  Device specific constants
typedef struct
{
    uint8_t *area; ///< Memory area that will be used as devMem
    uint16_t size; ///< Total size of memory area
} tsMemvirtualConsts;

/** @brief  Create a virtual devMem device
 *  @param  _name   Name of devMem object
 *  @param  _size   Size of area that will be used(must not exceed uint16_t)
 */
#define DEV_MEM_MEMVIRTUAL_CREATE(_name, _size) \
    uint8_t _name##Area[_size];                 \
    const tsMemvirtualConsts _name##Consts =    \
        {                                       \
            .area = _name##Area,                \
            .size = ARRAY_SIZE(_name##Area),    \
    };                                          \
    DEV_MEM_CREATE(_name, devMemMemvirtualFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_MEMVIRTUAL_H
