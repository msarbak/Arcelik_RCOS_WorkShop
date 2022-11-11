/** @file       ke15/spi.h
 *  @brief      NXP KE15 Serial Peripheral Interface
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       1.04.2019
 */
#ifndef FILE_SPI_H
#define FILE_SPI_H

#include "rcos.h"

/**
 *  @defgroup   KE15_SPI SPI
 *  @ingroup    KE15_DEV_COM
 *  @brief      NXP KE15 Serial Peripheral Interface
 *  @{
 */

#ifndef FILE_SPI_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for SPI devices
INTERFACE const tsDevComFuncs devComSpiFuncs;

/// @brief  Device specific parameters
typedef struct
{
    LPSPI_Type *peripheral;              ///< Related peripheral pointer
    const lpspi_master_config_t *config; ///< Related peripheral configuration
    IRQn_Type irq;                       ///< Related peripheral interrupt request number
    lpspi_master_handle_t handleStruct;  ///< Structure used for master transactions
    lpspi_transfer_t transferStruct;     ///< Transfer information
} tsSpiParams;

/** @brief  Create a devCom
 *  @param  _name   Name of devCom object
 *  @param  _module Name of target NXP module
 */
#define DEV_COM_SPI_CREATE(_name, _module) \
    tsSpiParams _name##Params =            \
        {                                  \
            _module##_PERIPHERAL,          \
            _module##_config,              \
            _module##_IRQN,                \
            {0},                           \
            {0},                           \
            NULL,                          \
    };                                     \
    DEV_COM_CREATE(_name, devComSpiFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_SPI_H
