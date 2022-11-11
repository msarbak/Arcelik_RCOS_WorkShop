/** @file       bu16501ks2.h
 *  @brief      Header file of BU16501KS2 device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Furkan Gullu
 *  @author     Deniz Fer
 *  @date       4.02.2020
 */
#ifndef FILE_BU16501KS2_H
#define FILE_BU16501KS2_H

/// Includes
#include "rcos.h"
#include "dev/iolink.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_BU16501KS2_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   BU16501KS2 BU16501KS2
 *  @ingroup    DEV_CPX
 *  @brief      LED Driver with I2C Compatible and 3-wire Serial Interface for 8x16 LEDs in Dot Matrix
 *  @{
 */

#define BU_SW_CNT (8)
#define BU_LED_CNT (16)
#define BU_LED_PER_REG (2)
#define BU16501_FullDuty (0x3F)

/// @brief  Functions for BU16501KS2 devices
INTERFACE const tsDevCpxFuncs devCpxBu16501ks2Funcs;

/// @brief  Events of a BU16501KS2 background process
typedef enum
{
    eBuInit,
    eBuCombo,
    eBuUpdate,
    eBuUpdateBrightness,
    eBuUpdateStep1,
    eBuUpdateStep2,
    eBuUpdateStep3,
    eBuUpdateStep4,
} teBu16501ks2Events;

/// @brief  Registry and data combination used for consecutive writes
typedef struct
{
    uint8_t registry; ///< Target registry address
    uint8_t data;     ///< Data to be written on target registry
} tsRegData;

/// @brief  Device specific parameters
typedef struct
{
    tsTarget target;    ///< Communication target information
    tTicket ticket;     ///< Communication ticket
    uint8_t idx;        ///< General index
    uint8_t brightness; ///< Overall brightness value, Max. BU16501_FullDuty
} tsBu16501ks2CpxParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevCom *i2c;    ///< I2C communication device
    tsProcess *process;     ///< Background process pointer
    const tsDevCpx *device; ///< Device pointer
    uint8_t *map;           ///< Target ram area that holds the map
    uint8_t slave;          ///< Target device slave address
} tsBu16501ks2CpxConsts;

/** @brief  Create a devCpx
 *  @param  _name   Name of devCpx object
 *  @param  _i2c    I2C communication device
 *  @param  _config I2C configuration
 *  @param  _ceLvl  CE signal level on chip
 */
#define DEV_CPX_BU16501KS2_CREATE(_name, _i2c, _config, _ceLvl)                  \
    uint8_t _name##Map[BU_SW_CNT * BU_LED_CNT / BU_LED_PER_REG] = {0};           \
    tsProcess _name##Process;                                                    \
    const tsDevCpx _name;                                                        \
    tsBu16501ks2CpxParams _name##Params =                                        \
        {                                                                        \
            .target = {                                                          \
                .config     = _config,                                           \
                .regAdrSize = 1,                                                 \
                .slave      = 0x74 | ((_ceLvl) ? 1 : 0),                         \
            },                                                                   \
            .ticket     = 0,                                                     \
            .brightness = BU16501_FullDuty,                                      \
    };                                                                           \
    const tsBu16501ks2CpxConsts _name##Consts =                                  \
        {                                                                        \
            .i2c     = &_i2c,                                                    \
            .process = &_name##Process,                                          \
            .device  = &_name,                                                   \
            .map     = _name##Map,                                               \
            .slave   = 0x74 | ((_ceLvl) ? 1 : 0),                                \
    };                                                                           \
    DEV_CPX_CREATE(_name, devCpxBu16501ks2Funcs, &_name##Params, &_name##Consts) \
    PROCESS_CREATE(_name##Process, bu16501ks2ProcInit, bu16501ks2ProcDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/** @defgroup   BU16501KS2_BR BRIGHTNESS
 *  @brief      Subdevice for a BU16501KS2 that controls the overall brightness
 *  @{
 */

/** @brief  Create a devIo on a BU16501KS2 devCpx to control overall brightness of the device
 *  @param  _name   Name of devIo object
 *  @param  _devCpx devCpx device name that is created by DEV_CPX_BU16501KS2_CREATE
 */
#define DEV_IO_BU16501KS2_BRIGTHNESS_CREATE(_name, _devCpx) \
    DEV_IO_IOLINK_CREATE(_name, &_devCpx##Params.brightness, sizeof(_devCpx##Params.brightness))

/** @} */

/** @defgroup   BU16501KS2_PX PIXELS
 *  @brief      Subdevice for a BU16501KS2 that controls multiple pixels on maindevice
 *  @{
 */

/// @brief  Functions for BU16501ks2 devIo devices
INTERFACE const tsDevIoFuncs devIoBu16501ks2PxFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t value;       ///< devIo current value
    const tsDevCpx *core; ///< target BU16501KS2 device
    const uint8_t *list;  ///< List of pixel coordinates
    uint8_t listCnt;      ///< Item count of list
    uint8_t current;      ///< Current value to specify LED brightness
} tsBu16501ks2PxParams;

/// @brief  Macro used to define a led coordinate
/// @param  _l LED_X index number of led pins
/// @param  _s SW_Y index number of switch pins
#define BU16_COORD(_l, _s) ((((_l)&0xf) << 4) | ((_s)&0x7))

/** @brief      Create a devIo for multiple pixels on BU16501KS2 devCpx
 *  @details    LED coordinates are as LEDm and SWn where m = [0..15], n = [0..7] presented as a hex value 0xMN
 *              0x00, 0x01, ... 0x07, 0x10, 0x11, ... 0xf7
 *  @param      _name       Name of devIo object
 *  @param      _core       Core complex device
 *  @param      _current    Current drive value for defined coordinates 0..15
 *  @param      ...         List of LED coordinates use BU16_COORD macro
 */
#define DEV_IO_BU16501KS2_PIXEL_CREATE(_name, _core, _current, ...) \
    const uint8_t _name##List[] =                                   \
        {                                                           \
            __VA_ARGS__,                                            \
    };                                                              \
    tsBu16501ks2PxParams _name##Params =                            \
        {                                                           \
            0, /* value */                                          \
            &_core,                                                 \
            _name##List,                                            \
            ARRAY_SIZE(_name##List),                                \
            (_current) & (BIT(8 / BU_LED_PER_REG) - 1),             \
    };                                                              \
    DEV_IO_CREATE(_name, devIoBu16501ks2PxFuncs, &_name##Params, NULL)

/** @} */

/** @defgroup   BU16501KS2_PX_BR BRIGHTNESS
 *  @ingroup    BU16501KS2_PX
 *  @brief      Subdevice for a BU16501KS2 Pixels that controls the brightness of the maindevice
 *  @{
 */

/// @brief  Functions for BU16501ks2 devIo devices
INTERFACE const tsDevIoFuncs devIoBu16501ks2PxBrFuncs;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *mainDev;
    tsBu16501ks2PxParams *mainDevParams;
} tsBu16501ks2PxBrConsts;

/** @brief  Create a devIo on a BU16501KS2 devIo to control its brightness
 *  @param  _name   Name of devIo object
 *  @param  _pixel  A pixel devIo created by DEV_IO_BU16501KS2_CREATE
 */
#define DEV_IO_BU16501KS2_PIXEL_BRIGTHNESS_CREATE(_name, _pixel) \
    const tsBu16501ks2PxBrConsts _name##Consts =                 \
        {                                                        \
            &_pixel,                                             \
            &_pixel##Params,                                     \
    };                                                           \
    DEV_IO_CREATE(_name, devIoBu16501ks2PxBrFuncs, NULL, &_name##Consts)

/** @} */

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_BU16501KS2_H
