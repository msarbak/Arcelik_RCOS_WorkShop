/** @file       lerp.h
 *  @brief      Header file of LERP device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Özkan Akgül
 *  @date       16.07.2019
 */
#ifndef FILE_LERP_H
#define FILE_LERP_H

#include "rcos.h"

/**
 *  @defgroup   LERP LERP
 *  @ingroup    DEV_IO
 *  @brief      Linear Interpolation devIo gets X values from a target devIo and makes necessary calculations from the values provided in a table.
 *  @{
 */

#ifndef FILE_LERP_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for LERP devices
INTERFACE const tsDevIoFuncs devIoLerpFuncs;

/// @brief  Object used to define a point on the lerp table
typedef struct
{
    int32_t x; ///< X value of point
    int32_t y; ///< Y value of point
} tsLerpPoint;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *devX;      ///< Target device used to get X values
    const tsLerpPoint *table; ///< Table of points
    uint16_t tableSize;       ///< Size of table
} tsLerpConsts;

/** @brief  Create a devIo
 *  @warning    Points in _table are assumed to be in ascending order for X values
 *  @param      _name   Name of devIo object
 *  @param      _devX   devIo to be used as X value
 *  @param      _table  Array of tsLerpPoint that represent points on the graph
 */
#define DEV_IO_LERP_CREATE(_name, _devX, _table) \
    const tsLerpConsts _name##Consts =           \
        {                                        \
            &_devX,                              \
            _table,                              \
            ARRAY_SIZE(_table),                  \
    };                                           \
    DEV_IO_CREATE(_name, devIoLerpFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_LERP_H
