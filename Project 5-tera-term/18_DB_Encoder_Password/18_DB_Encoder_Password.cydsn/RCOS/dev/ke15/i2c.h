/** @file       ke15/i2c.h
 *  @brief      NXP KE15 Inter-Integrated Circuit
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
#include <fsl_lpi2c.h>

/**
 *  @defgroup   KE15_I2C I2C
 *  @ingroup    KE15_DEV_COM
 *  @brief      NXP KE15 Inter-Integrated Circuit
 *  @code
 *      START SLAVE_R STOP // Probe
 *      START SLAVE_W SEND STOP // plain write
 *      START SLAVE_R RECV STOP // plain read
 *      START SLAVE_W ADDR SEND STOP // address write
 *      START SLAVE_W ADDR REST SLAVE_R RECV STOP // address read
 *      
 *      devComOpen(target) // 
 *      devComSend(*) // START SLAVE_W SEND STOP
 *      devComClose(target) // 
 *      
 *      devComOpen(target) // 
 *      devComReceive(*) // START SLAVE_R RECV STOP
 *      devComClose(target) // 
 *      
 *      devComOpen(target) // 
 *      devComSend(*) // regAdrRcv = true
 *      devComSend(*) // START SLAVE_W ADDR SEND STOP
 *      devComClose(target) // 
 *      
 *      devComOpen(target) // 
 *      devComSend(*) // regAdrRcv = true
 *      devComReceive(*) // START SLAVE_W ADDR REST SLAVE_R RECV STOP
 *      devComClose(target) // 
 *  @endcode
 *  @{
 */

#ifndef FILE_I2C_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for I2C devices
INTERFACE const tsDevComFuncs devComI2cFuncs;

/// @brief  Constant values of an I2C device
typedef struct
{
    LPI2C_Type *base;                  ///< I2C peripheral base address
    lpi2c_master_config_t *config;     ///< I2C master configuration
    lpi2c_master_transfer_t *transfer; ///< I2C master transfer details information
    lpi2c_master_handle_t *handle;     ///< I2C master middleware handle
} tsI2cConst;

/// @brief  Device specific parameters
typedef struct
{
    const tsI2cConst cns; ///< Constants for this device
    uint8_t regAdrRcv;    ///< Registry address is valid
} tsI2cParams;

/** @brief  Create a devCom
 *  @param  _name   Name of devCom object
 *  @param  _nxpDev Name of NXP SDK component name
 */
#define DEV_COM_I2C_CREATE(_name, _nxpDev) \
    const tsI2cConst _name##Const =        \
        {                                  \
            _nxpDev##_PERIPHERAL,          \
            &_nxpDev##_masterConfig,       \
            &_nxpDev##_masterTransfer,     \
            &_nxpDev##_masterHandle,       \
    };                                     \
    tsI2cParams _name##Params =            \
        {                                  \
            &_name##Const,                 \
    };                                     \
    DEV_COM_CREATE(_name, devComI2cFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_I2C_H
