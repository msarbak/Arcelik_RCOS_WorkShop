/** @file       psoc4/isr.h
 *  @brief      CYPRESS PSOC4 Interrupt Sub-Routine driver
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       16.01.2019
 */
#ifndef FILE_ISR_H
#define FILE_ISR_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_ISR ISR
 *  @ingroup    PSOC4_DEV_CPX
 *  @brief      CYPRESS PSOC4 Interrupt Sub-Routine driver
 *  @{
 */

#ifndef FILE_ISR_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for ISR devices
INTERFACE const tsDevCpxFuncs devCpxIsrFuncs;

/// @brief  Device specific parameters
typedef struct
{
    void (*startEx)(cyisraddress address); ///< CYPRESS component function: _StartEx
    void (*stop)(void);                    ///< CYPRESS component function: _Stop
} tsIsrConsts;

/** @brief  Create a devCpx
 *  @param  _name   Name of devCpx object
 *  @param  _cyIsr  Target CYPRESS ISR block name
 */
#define DEV_CPX_ISR_CREATE(_name, _cyIsr) \
    const tsIsrConsts _name##Consts =     \
        {                                 \
            _cyIsr##_StartEx,             \
            _cyIsr##_Stop,                \
    };                                    \
    DEV_CPX_CREATE(_name, devCpxIsrFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_ISR_H
