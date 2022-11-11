/** @file       ke02/i2c.h
 *  @brief      NXP KE02 Inter-Integrated Circuit
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       2.04.2019
 */
#ifndef FILE_I2C_H
#define FILE_I2C_H

#include "rcos.h"
#include "fsl_i2c.h"

/**
 *  @defgroup   KE02_I2C I2C
 *  @ingroup    KE02_DEV_COM
 *  @brief      NXP KE02 Inter-Integrated Circuit
 *  @{
 */

#ifndef FILE_I2C_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for I2C devices
INTERFACE const tsDevComFuncs devComI2cFuncs;

/// @brief  Device specific parameters
typedef struct
{
    I2C_Type *base;             ///< Peripheral registers base address
    i2c_master_config_t config; ///< Configuration structure
    i2c_master_handle_t handle; ///< Handle structure
    i2c_master_transfer_t xfer; ///< Transfer information
    clock_name_t clock;         ///< Clock information
} tsI2cParams;

/** @brief  Create a devCom
 *  @param  _name       Name of devCom object
 *  @param  _no         Peripheral number
 *  @param  _baud       Baudrate in Hz
 *  @param  _filter     Glitch filter width in ns
 */
#define DEV_COM_I2C_CREATE(_name, _no, _baud, _filter) \
    tsI2cParams _name##Params =                        \
        {                                              \
            I2C##_no,                                  \
            {                                          \
                .enableMaster      = TRUE,             \
                .baudRate_Bps      = (_baud),          \
                .glitchFilterWidth = (_filter),        \
            },     /* config */                        \
            {{0}}, /* handle */                        \
            {0},   /* xfer */                          \
            kCLOCK_BusClk,                             \
    };                                                 \
    DEV_COM_CREATE(_name, devComI2cFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_I2C_H
