/** @file       psoc4/emu_eeprom.h
 *  @brief      Emulator for an EEPROM
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       21.05.2019
 */
#ifndef FILE_EMU_EEPROM_H
#define FILE_EMU_EEPROM_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_EMU_EEPROM EMU_EEPROM
 *  @ingroup    PSOC4_DEV_MEM
 *  @brief      Emulator for an EEPROM
 *  @{
 */

#ifndef FILE_EMU_EEPROM_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for EMU_EEPROM devices
INTERFACE const tsDevMemFuncs devMemEmuEepromFuncs;

/// @brief  Device specific parameters
typedef struct
{
    cy_en_em_eeprom_status_t (*init)(uint32 startAddress); ///< Init function of specific EEPROM emulator
    cy_stc_eeprom_context_t *context;                      ///< Context information structure of specific EEPROM emulator
    uint32_t startAddress;                                 ///< Starting address of area that will be used for emulation
} tsEmuEepromConsts;

/** @brief      Create a devMem for target section in memory
 *  @details    https://cypresssemiconductorco.github.io/emeeprom/em_eeprom_api_reference_manual/html/index.html#section_em_eeprom_appsflash_fixed
 *  @param      _name           Name of devMem object
 *  @param      _cyName         CYPRESS PSOC Creator component name
 *  @param      _sectionName    Section name as string
 */
#define DEV_MEM_EMU_SECTION_CREATE(_name, _cyName, _sectionName) \
    PLATFORM_SECTION(_sectionName)                               \
    PLATFORM_ALIGNED(CY_FLASH_SIZEOF_ROW)                        \
    const uint8 _name##Area[_cyName##_PHYSICAL_SIZE] = {0u};     \
    const tsEmuEepromConsts _name##Consts =                      \
        {                                                        \
            .init         = _cyName##_Init,                      \
            .context      = &_cyName##_context,                  \
            .startAddress = (uint32_t)_name##Area,               \
    };                                                           \
    DEV_MEM_CREATE(_name, devMemEmuEepromFuncs, NULL, &_name##Consts)

/** @brief      Create a devMem EMU_EEPROOM for target area in memory
 *  @warning    Does not check target area size
 *  @param      _name Name of devMem object
 *  @param      _cyName CYPRESS PSOC Creator component name
 *  @param      _address Target area address that will be used for emulation
 */
#define DEV_MEM_EMU_ADDRESS_CREATE(_name, _cyName, _address)  \
    /* sizeof(_address) must match _cyName##_PHYSICAL_SIZE */ \
    const tsEmuEepromConsts _name##Consts =                   \
        {                                                     \
            .init         = _cyName##_Init,                   \
            .context      = &_cyName##_context,               \
            .startAddress = (uint32_t)_address,               \
    };                                                        \
    DEV_MEM_CREATE(_name, devMemEmuEepromFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_EMU_EEPROM_H
