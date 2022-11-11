/** @file       psoc4/spi.h
 *  @brief      CYPRESS PSOC4 Serial Peripheral Interface
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       21.01.2019
 */
#ifndef FILE_SPI_H
#define FILE_SPI_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_SPI SPI
 *  @ingroup    PSOC4_DEV_COM
 *  @brief      CYPRESS PSOC4 Serial Peripheral Interface
 *  @{
 */

#ifndef FILE_SPI_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for SPI devices
INTERFACE const tsDevComFuncs devComSpiFuncs;

/// @brief  Generic interrupt subroutine function for SPI devices
INTERFACE void spiIsr(const tsDevCom *device);

/// @brief  SCB low level constant information
typedef struct
{
    void (*start)(void);                              ///< CYPRESS function eg: SPIM_1_Start
    void (*stop)(void);                               ///< CYPRESS function eg: SPIM_1_Stop
    void (*setIsr)(void (*func)(void));               ///< CYPRESS function eg: SPIM_1_SetCustomInterruptHandler
    void (*put)(uint32 txData);                       ///< CYPRESS function eg: SPIM_1_SpiUartWriteTxData
    void (*write)(const uint8 wrBuf[], uint32 count); ///< CYPRESS function eg: SPIM_1_SpiUartPutArray
    uint32 (*rxSize)(void);                           ///< CYPRESS function eg: SPIM_1_SpiUartGetRxBufferSize
    uint32 (*rxDataGet)(void);                        ///< CYPRESS function eg: SPIM_1_SpiUartReadRxData
    reg32 *statusReg;                                 ///< CYPRESS function eg: SPIM_1_SPI_STATUS_PTR
    uint32_t statusBusy;                              ///< CYPRESS function eg: SPIM_1_SPI_STATUS_BUS_BUSY
    reg32 *intrMasterFlags;                           ///< CYPRESS function eg: SPIM_1_INTR_MASTER_PTR
    uint32_t intrMasterDone;                          ///< CYPRESS function eg: SPIM_1_INTR_MASTER_SPI_DONE
    void (*isr)(void);                                ///< Object specific ISR function
} tsSpiConsts;

/// @brief  Device specific parameters
typedef struct
{
    uint16_t sendSize; ///< Used to be sure of completion of last transmission
} tsSpiParams;

/** @brief      Create a devCom for an unconfigured SCB to be used as SPI
 *  @param      _name       name of devCom object
 *  @param      _cyName     SCB module name in PSOC creator design
 */
#define DEV_COM_SPI_CREATE(_name, _cyName)       \
    const tsDevCom _name;                        \
    void _name##Isr(void)                        \
    {                                            \
        spiIsr(&(_name));                        \
    }                                            \
    const tsSpiConsts _name##Consts =            \
        {                                        \
            _cyName##_Start,                     \
            _cyName##_Stop,                      \
            _cyName##_SetCustomInterruptHandler, \
            _cyName##_SpiUartWriteTxData,        \
            _cyName##_SpiUartPutArray,           \
            _cyName##_SpiUartGetRxBufferSize,    \
            _cyName##_SpiUartReadRxData,         \
            _cyName##_SPI_STATUS_PTR,            \
            _cyName##_SPI_STATUS_BUS_BUSY,       \
            _cyName##_INTR_MASTER_PTR,           \
            _cyName##_INTR_MASTER_SPI_DONE,      \
            _name##Isr,                          \
    };                                           \
    tsSpiParams _name##Params =                  \
        {                                        \
            0,                                   \
    };                                           \
    DEV_COM_CREATE(_name, devComSpiFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_SPI_H
