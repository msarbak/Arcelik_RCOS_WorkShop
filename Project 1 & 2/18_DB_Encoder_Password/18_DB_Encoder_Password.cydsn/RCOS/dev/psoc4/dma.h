/** @file       psoc4/dma.h
 *  @brief      CYPRESS PSOC4 Direct Memory Access
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       17.01.2019
 */
#ifndef FILE_DMA_H
#define FILE_DMA_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_DMA DMA
 *  @ingroup    PSOC4_DEV_CPX
 *  @brief      CYPRESS PSOC4 Direct Memory Access
 *  @{
 */

#ifndef FILE_DMA_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for DMA devices
INTERFACE const tsDevCpxFuncs devCpxDmaFuncs;
INTERFACE void dmaIsrCallback(const tsDevCpx *device);

/// @brief  Device specific parameters
typedef struct
{
    void (*start)(void *srcAddress, void *dstAddress); ///< CYPRESS DMA start function
    int32 channel;                                     ///< CYPRESS DMA channel number
    cydma_callback_t *callback;                        ///< Callback function after a cycle finishes
    void *src;                                         ///< Source address
    void *dst;                                         ///< Destination address
    uint8_t continuous;                                ///< 0: starts only when initialized ~: restarts after every cycle
} tsDmaConsts;

/** @brief  Create a devCpx
 *  @param  _name       Name of devCpx object
 *  @param  _src        Source address
 *  @param  _dst        Destination address
 *  @param  _continuous If DMA should repeat itself after completion
 *  @param  _cyDma      CYPRESS DMA block name
 *  @warning All settings should be done in CYPRESS DMA
 */
#define DEV_CPX_DMA_CREATE(_name, _src, _dst, _continuous, _cyDma)          \
    const tsDevCpx _name;                                                   \
    void _name##Isr(void)                                                   \
    {                                                                       \
        dmaIsrCallback(&_name);                                             \
    }                                                                       \
    const tsDmaConsts _name##Consts =                                       \
        {                                                                   \
            _cyDma##_Start,                                                 \
            _cyDma##_CHANNEL, /*(_cyDma##_DESCR0_CFG & 0x0000fffful) + 1,*/ \
            _name##Isr,                                                     \
            (_src),                                                         \
            (_dst),                                                         \
            (_continuous) ? 1 : 0,                                          \
    };                                                                      \
    DEV_CPX_CREATE(_name, devCpxDmaFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_DMA_H
