/** @file       ke16/flash.h
 *  @brief      Header file of FLASH device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Vehbi Umur Cabuk
 *  @date       20.02.2020
 *  @author     Deniz Fer
 *  @date       5.03.2020
 *  @author     Özkan AKGÜL
 *  @date       18.04.2021
 */
#ifndef FILE_FLASH_H
#define FILE_FLASH_H

/// Includes
#include "rcos.h"
#include "fsl_flash.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_FLASH_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   KE16_FLASH FLASH
 *  @ingroup    KE16_DEV_CPX
 *  @brief      NXP KE16 Flash access
 *  @{
 */

/// @brief  Functions for FLASH devices
INTERFACE const tsDevCpxFuncs devCpxFlashFuncs;

/// @brief  Device specific parameters
typedef struct
{
    flash_config_t config;
} tsFlashParams;

/** @brief  Create a devCpx object to access internal flash of NXP KE16
 *  @param  _name   Name of devCpx object
 */
#define DEV_CPX_FLASH_CREATE(_name) \
    tsFlashParams _name##Params = { \
        {0},                        \
    };                              \
    DEV_CPX_CREATE(_name, devCpxFlashFuncs, &_name##Params, NULL)

/** @} */

#if FLASH_SSD_IS_EEPROM_ENABLED
/**
 *  @defgroup   KE16_EEPROM EEPROM
 *  @ingroup    KE16_DEV_MEM
 *  @ingroup    KE16_FLASH
 *  @brief      NXP KE16 internal EEPROM
 *  @{
 */
/// @brief  Functions for EEPROM devices
INTERFACE const tsDevMemFuncs devMemEepromFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *flash;
    tsFlashParams *fparams;
} tsEepromParams;

/** @brief  Create a devMem object for EEPROM of KE16
 *  @param  _name   Name of devMem object
 *  @param  _flash  Flash device
 */
#define DEV_MEM_EEPROM_CREATE(_name, _flash) \
    const tsEepromParams _name##Params =     \
        {                                    \
            &_flash,                         \
            &_flash##Params,                 \
    };                                       \
    DEV_MEM_CREATE(_name, devMemEepromFuncs, &_name##Params)

/** @} */
#endif

/**
 *  @defgroup   KE16_SECTION SECTION
 *  @ingroup    KE16_DEV_MEM
 *  @ingroup    KE16_FLASH
 *  @brief      NXP KE16 Flash section as a devMem
 *  @{
 */
/// @brief  Functions for SECTION devices
INTERFACE const tsDevMemFuncs devMemSectionFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *flash;
    tsFlashParams *fparams;
    const uint8_t *area;
    uint8_t *sectorBuffer;
    uint16_t areaSize;
    uint16_t sectorSize;
} tsSectionParams;

/** @brief      Create a devMem object for a SECTION of KE16
 *  @warning    Macro assumes MCUxpresso style section naming
 *  @param      _name           Name of devMem object
 *  @param      _sectionName    Name of target memory section(must be a "string")
 *  @param      _size           Size of section
 *  @param      _flash          Flash core device
 *  @param      _bufferSize     Size of the buffer used to write operation, must be equal to sector size if write
 *                              operation will be used...
 */
#define DEV_MEM_SECTION_CREATE(_name, _sectionName, _size, _flash, _bufferSize)             \
    PLATFORM_SECTION(".rodata.$"_sectionName)                                               \
    PLATFORM_ALIGNED(FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE)                            \
    const uint8_t _name##PhysicalArea[(FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE)];        \
    uint8_t _name##SectorBuffer[_bufferSize]; \
    tsSectionParams _name##Params =                                          \
        {                                                                    \
            &_flash,                                                         \
            &_flash##Params,                                                 \
            _name##PhysicalArea,                                             \
            _name##SectorBuffer,                                             \
            ARRAY_SIZE(_name##PhysicalArea),                                 \
            ARRAY_SIZE(_name##SectorBuffer),                                 \
    };                                                                       \
    DEV_MEM_CREATE(_name, devMemSectionFuncs, &_name##Params, NULL)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_FLASH_H
