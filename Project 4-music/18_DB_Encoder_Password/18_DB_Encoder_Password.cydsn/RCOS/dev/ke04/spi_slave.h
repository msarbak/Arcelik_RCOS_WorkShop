/** @file       ke04/spi_slave.h
 *  @brief      NXP KE04 SPI slave device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       22.07.2019
 */
#ifndef FILE_SPI_SLAVE_H
#define FILE_SPI_SLAVE_H

#include "rcos.h"
#include "fsl_spi.h"

/**
 *  @defgroup   KE04_SPI_SLAVE SPI_SLAVE
 *  @ingroup    KE04_DEV_COM
 *  @brief      NXP KE04 SPI slave device
 *  @{
 */

#ifndef FILE_SPI_SLAVE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for SPI_SLAVE devices
INTERFACE const tsDevComFuncs devComSpiSlaveFuncs;

INTERFACE void spiSlaveIsr(const tsDevCom *device);

/// @brief  Device specific parameters
typedef struct
{
    tsDeque *rxQue;                   ///< Received data queue
    tsDeque *txQue;                   ///< Transmit data queue
    SPI_Type *base;                   ///< SPI peripheral base address
    IRQn_Type irqn;                   ///< SPI irq number
    const spi_slave_config_t *config; ///< SPI configuration
} tsSpiSlaveParams;

/** @brief  Create a devCom
 *  @param  _name       Name of devCom object
 *  @param  _spiNo      SPI peripheral number
 *  @param  _pol        Clock polarity
 *  @param  _pha        Clock phase
 *  @param  _rxBuffSize Receive buffer size
 *  @param  _txBuffSize Transmit buffer size
 */
#define DEV_COM_SPI_SLAVE_CREATE(_name, _spiNo, _pol, _pha, _rxBuffSize, _txBuffSize)                  \
    const tsDevCom _name;                                                                              \
    void SPI##_spiNo##_IRQHandler(void)                                                                \
    {                                                                                                  \
        spiSlaveIsr(&_name);                                                                           \
    }                                                                                                  \
    DEQUE_CREATE(_name##RxBuff, _rxBuffSize)                                                           \
    DEQUE_CREATE(_name##TxBuff, _txBuffSize)                                                           \
    const spi_slave_config_t _name##Config =                                                           \
        {                                                                                              \
            .enableSlave          = true,                                                              \
            .enableStopInWaitMode = false,                                                             \
            .polarity             = _pol ? kSPI_ClockPolarityActiveLow : kSPI_ClockPolarityActiveHigh, \
            .phase                = _pha ? kSPI_ClockPhaseSecondEdge : kSPI_ClockPhaseFirstEdge,       \
            .direction            = kSPI_MsbFirst,                                                     \
            .pinMode              = kSPI_PinModeNormal,                                                \
    };                                                                                                 \
    const tsSpiSlaveParams _name##Params =                                                             \
        {                                                                                              \
            &_name##RxBuff,                                                                            \
            &_name##TxBuff,                                                                            \
            SPI##_spiNo,                                                                               \
            SPI##_spiNo##_IRQn,                                                                        \
            &_name##Config,                                                                            \
    };                                                                                                 \
    DEV_COM_CREATE(_name, devComSpiSlaveFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_SPI_SLAVE_H
