/** @file       psoc4/i2c.h
 *  @brief      CYPRESS SCB module configured as I2C
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       4.12.2018
 */
#ifndef FILE_I2C_H
#define FILE_I2C_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_I2C I2C
 *  @ingroup    PSOC4_DEV_COM
 *  @warning    BLOCKING FUNCTIONS
 *  @brief      CYPRESS PSOC4 Inter-Integrated Circuit bus
 *  @code
 *      START SLAVE_R STOP // Probe
 *      START SLAVE_W SEND STOP // plain write
 *      START SLAVE_R RECV STOP // plain read
 *      START SLAVE_W ADDR SEND STOP // address write
 *      START SLAVE_W ADDR REST SLAVE_R RECV STOP // address read
 *      
 *      devComOpen(target) // START SLAVE_R STOP
 *      devComSend(*) // START SLAVE_W SEND
 *      devComClose(target) // STOP
 *      
 *      devComOpen(target) // START SLAVE_R STOP
 *      devComReceive(*) // START SLAVE_R RECV
 *      devComClose(target) // STOP
 *      
 *      devComOpen(target) // START SLAVE_R STOP
 *      devComSend(*) // START SLAVE_W ADDR
 *      devComSend(*) // SEND
 *      devComClose(target) // STOP
 *      
 *      devComOpen(target) // START SLAVE_R STOP
 *      devComSend(*) // START SLAVE_W ADDR
 *      devComReceive(*) // REST SLAVE_R RECV
 *      devComClose(target) // STOP
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

/// @brief  Generic interrupt subroutine function for I2C devices
// INTERFACE void i2cIsr(const tsDevCom *device);

/// @brief  I2C low level constant information
typedef struct
{
    void (*start)(void);                                                             ///< CYPRESS component function: _Start
    void (*stop)(void);                                                              ///< CYPRESS component function: _Stop
    uint32_t timeoutMs;                                                              ///< TIMEOUT for CYPRESS I2C componnt functions.
    uint32 (*writeBuf)(uint32 slaveAddress, uint8 *wrData, uint32 cnt, uint32 mode); ///< CYPRESS component function: _I2CMasterWriteBuf
    uint32 (*readBuf)(uint32 slaveAddress, uint8 *rdData, uint32 cnt, uint32 mode);  ///< CYPRESS component function: _I2CMasterReadBuf
    uint32 (*sendStart)(uint32 slaveAddress, uint32 bitRnW, uint32 timeoutMs);       ///< CYPRESS component function: _I2CMasterSendStart
    uint32 (*sendRestart)(uint32 slaveAddress, uint32 bitRnW, uint32 timeoutMs);     ///< CYPRESS component function: _I2CMasterSendRestart
    uint32 (*sendStop)(uint32 timeoutMs);                                            ///< CYPRESS component function: _I2CMasterSendStop
    uint32 (*writeByte)(uint32 wrByte, uint32 timeoutMs);                            ///< CYPRESS component function: _I2CMasterWriteByte
    uint32 (*readByte)(uint32 ackNack, uint8 *rdByte, uint32 timeoutMs);             ///< CYPRESS component function: _I2CMasterReadByte
    uint32 (*readBufSize)(void);                                                     ///< CYPRESS component function: _I2CMasterGetReadBufSize
    uint32 (*writeBufSize)(void);                                                    ///< CYPRESS component function: _I2CMasterGetWriteBufSize
    volatile uint16 *mstrStatus;                                                     ///< CYPRESS component variable: _mstrStatus
    uint16_t statusComplete;                                                         ///< CYPRESS component constant: value of transfer completed status
} tsI2cConsts;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t mode;       ///< Current mode followed here
    uint16_t regAddress; ///< Target device registry address
} tsI2cParams;

// #warning For each _cyName(I2C) used in the system place following lines in "cyapicallbacks.h", which should be in root .cydsn folder(and added to PSoC project), than rebuild all generated source files
// #define I2C_1_I2C_ISR_EXIT_CALLBACK
// void I2C_1_I2C_ISR_ExitCallback( void );

/** @brief      Create a devCom for I2C
 *  @param      _name   Name of devCom object
 *  @param      _cyName Module name in PSOC creator
 *  @param      _cyName timeout value in ms for i2c functions
 */
#define DEV_COM_I2C_CREATE(_name, _cyName, _timeout)                                \
    const tsDevCom _name;                                                  \
    /* void _cyName##_I2C_ISR_ExitCallback(void) */                        \
    /* {                                         */                        \
    /*     i2cIsr(&_name);                       */                        \
    /* }                                         */                        \
    const tsI2cConsts _name##Consts =                                      \
    {                                                                      \
            _cyName##_Start,                                               \
            _cyName##_Stop,                                                \
            _timeout,                                                      \
            _cyName##_I2CMasterWriteBuf,                                   \
            _cyName##_I2CMasterReadBuf,                                    \
            _cyName##_I2CMasterSendStart,                                  \
            _cyName##_I2CMasterSendRestart,                                \
            _cyName##_I2CMasterSendStop,                                   \
            _cyName##_I2CMasterWriteByte,                                  \
            _cyName##_I2CMasterReadByte,                                   \
            _cyName##_I2CMasterGetReadBufSize,                             \
            _cyName##_I2CMasterGetWriteBufSize,                            \
            &_cyName##_mstrStatus,                                         \
            (_cyName##_I2C_MSTAT_RD_CMPLT | _cyName##_I2C_MSTAT_WR_CMPLT), \
    };                                                                     \
    tsI2cParams _name##Params =                                            \
        {                                                                  \
            0,                                                             \
            0,                                                             \
    };                                                                     \
    DEV_COM_CREATE(_name, devComI2cFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_I2C_H
