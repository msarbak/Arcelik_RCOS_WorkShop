/** @file       kbi.h
 *  @brief      NXP KE04 Keyboard Interrupt
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     
 *  @date       8/26/2019
 */
#ifndef FILE_KBI_H
#define FILE_KBI_H

#include "rcos.h"
#include "fsl_kbi.h"

/**
 *  @defgroup   KBI KBI
 *  @ingroup    DEV_CPX
 *  @brief      NXP KE04 Keyboard Interrupt
 *  @{
 */

#ifndef FILE_KBI_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for KBI Core devices
INTERFACE const tsDevCpxFuncs devCpxKbiCoreFuncs;
INTERFACE void kbiCoreIsr(const tsDevCpx *device);
typedef void (*kbiSubIsr)(void);

/// @brief  Device specific parameters
typedef struct
{
    kbi_config_t kbiConfig; ///< Configuration structure for KBI core
    KBI_Type *base;         ///< Registry set base address
    kbiSubIsr *list;        ///< List of kbiSubIsr functions
} tsKbiCoreParams;

/** @brief  Create a devCpx
 *  @param  _name   Name of devCpx object
 *  @param  _kbiNo  Enumeration number of target KBI peripheral
 *  @param  _mode   Operation mode
 *  @param  _pins   Mask of pins that will trigger an interrupt
 *  @param  _edges  Edge trigger selection mask for each pin 0: falling, 1: rising
 */
#define DEV_CPX_KBI_CORE_CREATE(_name, _kbiNo, _mode, _pins, _edges) \
    kbiSubIsr _name##List[32] = {NULL};                              \
    const tsDevCpx _name;                                            \
    void KBI##_kbiNo##_IRQHandler(void)                              \
    {                                                                \
        kbiCoreIsr(&_name);                                          \
    }                                                                \
    const tsKbiCoreParams _name##Params =                            \
        {                                                            \
            {                                                        \
                (_pins),                                             \
                (_edges) & (_pins),                                  \
                _mode,                                               \
            },                                                       \
            KBI##_kbiNo,                                             \
            _name##List,                                             \
    };                                                               \
    DEV_CPX_CREATE(_name, devCpxKbiCoreFuncs, &_name##Params, NULL)

/// @brief  Functions for KBI sub devices
INTERFACE const tsDevCpxFuncs devCpxKbiSubFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *kbiCore; ///< Target KBI_CORE device
    uint32_t kbiPinNo;       ///< Pin number on target KBI_CORE device
} tsKbiSubParams;

/** @brief  Create a devCpx for KBI sub device
 *  @param  _name   Name of devCpx object
 *  @param  _core   Target KBI_CORE device name
 *  @param  _pinNo  Index number of target pin
 */
#define DEV_CPX_KBI_SUB_CREATE(_name, _core, _pinNo) \
    const tsKbiSubParams _name##Params =             \
        {                                            \
            &_core,                                  \
            (_pinNo),                                \
    };                                               \
    DEV_CPX_CREATE(_name, devCpxKbiSubFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_KBI_H
