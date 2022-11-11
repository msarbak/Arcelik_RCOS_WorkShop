/** @file       psoc4/ctrlreg.h
 *  @brief      CYPRESS Control Register Module
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       9.11.2018
 */
#ifndef FILE_CTRLREG_H
#define FILE_CTRLREG_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_CTRLREG CTRLREG
 *  @ingroup    PSOC4_DEV_IO
 *  @brief      CYPRESS PSOC4 Control Register Module
 *  @{
 */

#ifndef FILE_CTRLREG_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for CTRLREG devices
INTERFACE const tsDevIoFuncs devIoCtrlregFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint8 (*read)(void);  ///< CYPRESS module read function
    void (*write)(uint8); ///< CYPRESS module write function
    uint32_t actLevels;   ///< Activity levels of each pin
} tsCtrlregConsts;

/** @brief  Create a CTRLREG devIO
 *  @param  _name       Name of devIO object
 *  @param  _cyName     Name of CYPRESS module
 *  @param  _actLevels  Activity levels of each pin (0:low 1:high)
 */
#define DEV_IO_CTRLREG_CREATE(_name, _cyName, _actLevels) \
    const tsCtrlregConsts _name##Consts =                 \
        {                                                 \
            _cyName##_Read,                               \
            _cyName##_Write,                              \
            ~(_actLevels),                                \
    };                                                    \
    DEV_IO_CREATE(_name, devIoCtrlregFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_CTRLREG_H
