/** @file       psoc4/scb.h
 *  @brief      CYPRESS Serial Communication Block
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       12.11.2018
 */
#ifndef FILE_SCB_H
#define FILE_SCB_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_SCB SCB
 *  @ingroup    PSOC4_DEV_COM
 *  @brief      CYPRESS PSOC4 SCB module Unconfigured
 *  @{
 */

#ifndef FILE_SCB_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for SCB devices
INTERFACE const tsDevComFuncs devComScbFuncs;

/// @brief  Generic interrupt subroutine function for SCB devices
INTERFACE void scbSpiIsr(const tsDevCom *device);

/// @brief  SCB low level constant information
typedef struct
{
    void (*init)(const /*SCB_1_SPI_INIT_STRUCT*/ void *config); ///< CYPRESS function eg: SCB_SpiInit
    void (*start)(void);                                        ///< Stub to call low level functions to start the SCB
    void (*stop)(void);                                         ///< Stub to call low level functions to stop the SCB
    void (*put)(uint32 txData);                                 ///< CYPRESS function eg: SCB_SpiUartWriteTxData
    void (*write)(const uint8 wrBuf[], uint32 count);           ///< CYPRESS function eg: SCB_SpiUartPutArray
    uint32 (*rxSize)(void);                                     ///< CYPRESS function eg: SCB_SpiUartGetRxBufferSize
    uint32 (*rxDataGet)(void);                                  ///< CYPRESS function eg: SCB_SpiUartReadRxData
    reg32 *statusReg;                                           ///< CYPRESS function eg: SCB_SPI_STATUS_PTR
    uint32_t statusBusy;                                        ///< CYPRESS function eg: SCB_SPI_STATUS_BUS_BUSY
    reg32 *intrMasterFlags;                                     ///< CYPRESS function eg: SCB_INTR_MASTER_PTR
    uint32_t intrMasterDone;                                    ///< CYPRESS function eg: SCB_INTR_MASTER_SPI_DONE
} tsScbConsts;

/// @brief  Device specific parameters
typedef struct
{
    const /*SCB_1_SPI_INIT_STRUCT*/ void *currentCfg; ///< Current fonfiguration location ! Say hi to system developers of PSOC4 library
    uint16_t sendSize;                                ///< Used to be sure of completion of last transmission
} tsScbParams;

/** @brief      Create an SPI configuration for an unconfigured SCB
 *  @param      _confName   name of configuration
 *  @param      _cyName     SCB module name in PSOC creator design
 *  @param      _master     slave/master selection
 *  @param      _cpha       clock phase
 *  @param      _cpol       clock polarity
 *  @param      _msb        MSB or LSB send first
 *  @param      _rxBuffer255 receive buffer size(Max. value can be 255)
 *  @param      _txBuffer255 transmit buffer size(Max. value can be 255)
 */
#define DEV_COM_SCB_SPI_CONFIG(_confName, _cyName, _master, _cpha, _cpol, _msb, _rxBuffer255, _txBuffer255)                        \
    uint8_t _confName##RxBuffer[_rxBuffer255];                                                                                     \
    uint8_t _confName##TxBuffer[_txBuffer255];                                                                                     \
    const _cyName##_SPI_INIT_STRUCT _confName =                                                                                    \
        {                                                                                                                          \
            .mode               = ((_master) ? _cyName##_SPI_MASTER : _cyName##_SPI_SLAVE),                                        \
            .submode            = _cyName##_SPI_MODE_MOTOROLA,                                                                     \
            .sclkMode           = ((_cpha) ? _cyName##_SPI_SCLK_CPHA1_CPOL0 : 0) | ((_cpol) ? _cyName##_SPI_SCLK_CPHA0_CPOL1 : 0), \
            .oversample         = 8,                                                                                               \
            .enableMedianFilter = 0,                                                                                               \
            .enableLateSampling = 0,                                                                                               \
            .enableWake         = 0,                                                                                               \
            .rxDataBits         = 8,                                                                                               \
            .txDataBits         = 8,                                                                                               \
            .bitOrder           = ((_msb) ? _cyName##_BITS_ORDER_MSB_FIRST : _cyName##_BITS_ORDER_LSB_FIRST),                      \
            .transferSeperation = _cyName##_SPI_TRANSFER_CONTINUOUS,                                                               \
            .rxBufferSize       = sizeof(_confName##RxBuffer),                                                                     \
            .rxBuffer           = _confName##RxBuffer,                                                                             \
            .txBufferSize       = sizeof(_confName##TxBuffer),                                                                     \
            .txBuffer           = _confName##TxBuffer,                                                                             \
            .enableInterrupt    = 1, /*_cyName##_SCB_IRQ_INTERNAL*/                                                                \
            .rxInterruptMask    = _cyName##_INTR_RX_NOT_EMPTY,                                                                     \
            .rxTriggerLevel     = 7,                                                                                               \
            .txInterruptMask    = _cyName##_INTR_TX_NOT_FULL | _cyName##_INTR_MASTER_SPI_DONE,                                     \
            .txTriggerLevel     = 0,                                                                                               \
            .enableByteMode     = 1,                                                                                               \
            .enableFreeRunSclk  = 0,                                                                                               \
            .polaritySs         = _cyName##_SPI_SS_ACTIVE_LOW,                                                                     \
    };

/** @brief  Create a devCom for an unconfigured SCB to be used as SPI
 *  @param  _name       name of devCom object
 *  @param  _cyName     SCB module name in PSOC creator design
 *  @param  _clkName    Clock module name in PSOC creator design that supplies the SCB
 *  @param  _div        division value
 *  @param  _defCfg     Default configuration for devCom
 */
#define DEV_COM_SCB_SPI_CREATE(_name, _cyName, _clkName, _div, _defCfg) \
    const tsDevCom _name;                                               \
    void _name##Isr(void)                                               \
    {                                                                   \
        scbSpiIsr(&(_name));                                            \
    }                                                                   \
    void _name##Start(void)                                             \
    {                                                                   \
        _cyName##_SetCustomInterruptHandler(_name##Isr);                \
        _clkName##_SetDividerValue(_div);                               \
        _clkName##_Start();                                             \
        _cyName##_Start();                                              \
    }                                                                   \
    void _name##Stop(void)                                              \
    {                                                                   \
        _cyName##_Stop();                                               \
        _clkName##_Stop();                                              \
    }                                                                   \
    const tsScbConsts _name##Consts =                                   \
        {                                                               \
            (void (*)(const void *))_cyName##_SpiInit,                  \
            _name##Start,                                               \
            _name##Stop,                                                \
            _cyName##_SpiUartWriteTxData,                               \
            _cyName##_SpiUartPutArray,                                  \
            _cyName##_SpiUartGetRxBufferSize,                           \
            _cyName##_SpiUartReadRxData,                                \
            _cyName##_SPI_STATUS_PTR,                                   \
            _cyName##_SPI_STATUS_BUS_BUSY,                              \
            _cyName##_INTR_MASTER_PTR,                                  \
            _cyName##_INTR_MASTER_SPI_DONE,                             \
    };                                                                  \
    tsScbParams _name##Params =                                         \
        {                                                               \
            &_defCfg,                                                   \
            0,                                                          \
    };                                                                  \
    DEV_COM_CREATE(_name, devComScbFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_SCB_H
