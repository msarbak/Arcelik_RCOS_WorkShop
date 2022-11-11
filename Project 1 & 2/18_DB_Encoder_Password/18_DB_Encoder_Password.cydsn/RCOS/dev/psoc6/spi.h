/** @file       psoc6/spi.h
 *  @brief      CYPRESS PSOC6 Serial Peripheral Interface
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergun
 *  @date       03.01.2020
 */
#ifndef FILE_SPI_H
#define FILE_SPI_H

#include "rcos.h"

/**
 *  @defgroup   PSOC6_SPI SPI
 *  @ingroup    PSOC6_DEV_COM
 *  @brief      CYPRESS PSOC6 Serial Peripheral Interface
 *  @{
 */

#ifndef FILE_SPI_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for SPI devices
INTERFACE const tsDevComFuncs devComSpiFuncs;

/// @brief  Generic SPI Isr
INTERFACE void spiIsr(const tsDevCom *device);

/// @brief  Device specific parameters
typedef struct
{
    tsDeque rxFifo; ///< Receive fifo
    tsDeque txFifo; ///< Transmit fifo
} tsSpiParams;

/// @brief  Device specific constants
typedef struct
{
    void (*start)(void);                   ///< Start function of the module
    CySCB_Type *base;                      ///< Peripheral registries base address
    const cy_stc_scb_spi_config_t *config; ///< Cypress block configuration
    cy_stc_scb_spi_context_t *context;     ///< Cypress block context
    const cy_stc_sysint_t *irqCfg;         ///< Cypress block irq config
    void (*isr)(void);                     ///< Object specific isr function
} tsSpiConsts;

/** @brief      Create a devCom for an unconfigured SCB to be used as SPI
 *  @details    SCB module's IRQ configuration and this object must be defined in the same core
 *  @param      _name   name of devCom object
 *  @param      _cyName SCB module name in PSOC creator design
 *  @param      _rxSize Receive soft-buffer size
 *  @param      _txSize Transmit soft-buffer size
 */
#define DEV_COM_SPI_CREATE(_name, _cyName, _rxSize, _txSize) \
    uint8_t _name##RxBuffer[(_rxSize)];                      \
    uint8_t _name##TxBuffer[(_txSize)];                      \
    const tsDevCom _name;                                    \
    void _name##Isr(void)                                    \
    {                                                        \
        spiIsr(&_name);                                      \
    }                                                        \
    tsSpiParams _name##Params =                              \
        {                                                    \
            .rxFifo = DEQUE_INIT(_name##RxBuffer),           \
            .txFifo = DEQUE_INIT(_name##TxBuffer),           \
    };                                                       \
    const tsSpiConsts _name##Consts =                        \
        {                                                    \
            .start   = _cyName##_Start,                      \
            .base    = _cyName##_HW,                         \
            .config  = &_cyName##_config,                    \
            .context = &_cyName##_context,                   \
            .irqCfg  = &_cyName##_SCB_IRQ_cfg,               \
            .isr     = _name##Isr,                           \
    };                                                       \
    DEV_COM_CREATE(_name, devComSpiFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_SPI_H
