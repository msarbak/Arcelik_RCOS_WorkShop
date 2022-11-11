/** @file       psoc6/emu_eeprom.h
 *  @brief      Emulator for an EEPROM
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       16.03.2020
 */
#ifndef FILE_EMU_EEPROM_H
#define FILE_EMU_EEPROM_H

#include "rcos.h"

#ifndef FILE_EMU_EEPROM_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   PSOC6_EMU_EEPROM EMU_EEPROM
 *  @ingroup    PSOC6_DEV_MEM
 *  @brief      Emulator for an EEPROM
 *  @warning
 *  In case when blocking write option is used, if this function is called by
 *  the CM4 the user code on CM0P and the user code on CM4 are blocked until erase
 *  flash row operation is finished. If this function is called by the CM0P the
 *  user code on CM4 is not blocked and the user code on CM0P is blocked until
 *  erase flash row operation is finished. Plan your task allocation accordingly.
 *  @warning
 *  Erase operation completely erases the eeprom area.
 *  @{
 */

/// @brief  Functions for EMU_EEPROM devices
INTERFACE const tsDevMemFuncs devMemEmuEepromFuncs;

/// @brief  Device specific constants
typedef struct
{
    cy_en_em_eeprom_status_t (*init)(uint32 startAddress); ///< Init function of specific EEPROM emulator
    cy_stc_eeprom_context_t *context;                      ///< Context information structure of specific EEPROM emulator
    uint32_t startAddress;                                 ///< Starting address of the target section
} tsEmuEepromConsts;

/** @brief  Create a devMem for EMU_EEPROM that has the option "Use Emulated EEPROM" set to "Yes"
 *  @param  _name           Name of devMem object
 *  @param  _cyName         CYPRESS PSOC Creator component name
 */
#define DEV_MEM_EMU_EEPROM_CREATE(_name, _cyName) \
    const tsEmuEepromConsts _name##Consts =       \
        {                                         \
            .init         = _cyName##_Init,       \
            .context      = &_cyName##_context,   \
            .startAddress = 0,                    \
    };                                            \
    DEV_MEM_CREATE(_name, devMemEmuEepromFuncs, NULL, &_name##Consts)

/** @} */

/**
 *  @defgroup   PSOC6_EMU_SECTION EMU_SECTION
 *  @ingroup    PSOC6_DEV_MEM
 *  @brief      Use a section of the flash memory for an EEPROM emulation
 *  @warning
 *  In case when blocking write option is used, if this function is called by
 *  the CM4 the user code on CM0P and the user code on CM4 are blocked until erase
 *  flash row operation is finished. If this function is called by the CM0P the
 *  user code on CM4 is not blocked and the user code on CM0P is blocked until
 *  erase flash row operation is finished. Plan your task allocation accordingly.
 *  @{
 */

/** @brief      Create a devMem for EMU_EEPROM that has the option "Use Emulated EEPROM" set to "No"
 *  @details    https://cypresssemiconductorco.github.io/emeeprom/em_eeprom_api_reference_manual/html/index.html#section_em_eeprom_appsflash_fixed
 *  @param      _name           Name of devMem object
 *  @param      _cyName         CYPRESS PSOC Creator component name
 *  @param      _sectionName    Section name as string
 */
#define DEV_MEM_EMU_SECTION_CREATE(_name, _cyName, _sectionName)          \
    PLATFORM_SECTION(_sectionName)                                        \
    PLATFORM_ALIGNED(CY_FLASH_SIZEOF_ROW)                                 \
    const uint8_t _name##_em_EepromStorage[_cyName##_ACTUAL_SIZE] = {0u}; \
    const tsEmuEepromConsts _name##Consts =                               \
        {                                                                 \
            .init         = _cyName##_Init,                               \
            .context      = &_cyName##_context,                           \
            .startAddress = (uint32_t)_name##_em_EepromStorage,           \
    };                                                                    \
    DEV_MEM_CREATE(_name, devMemEmuEepromFuncs, NULL, &_name##Consts)

/** @} */

/**
 *  @defgroup   PSOC6_EMU_ADDRESS EMU_ADDRESS
 *  @ingroup    PSOC6_DEV_MEM
 *  @brief      Use a target location in the flash memory for an EEPROM emulation
 *  @warning
 *  In case when blocking write option is used, if this function is called by
 *  the CM4 the user code on CM0P and the user code on CM4 are blocked until erase
 *  flash row operation is finished. If this function is called by the CM0P the
 *  user code on CM4 is not blocked and the user code on CM0P is blocked until
 *  erase flash row operation is finished. Plan your task allocation accordingly.
 *  @{
 */

/** @brief      Create a devMem for EMU_EEPROM that has the option "Use Emulated EEPROM" set to "No"
 *  @warning    Does not check target location size
 *  @param      _name       Name of devMem object
 *  @param      _cyName     CYPRESS PSOC Creator component name
 *  @param      _address    Target memory location starting address
 */
#define DEV_MEM_EMU_ADDRESS_CREATE(_name, _cyName, _address) \
    const tsEmuEepromConsts _name##Consts =                  \
        {                                                    \
            .init         = _cyName##_Init,                  \
            .context      = &_cyName##_context,              \
            .startAddress = (uint32_t)_address,              \
    };                                                       \
    DEV_MEM_CREATE(_name, devMemEmuEepromFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_EMU_EEPROM_H
