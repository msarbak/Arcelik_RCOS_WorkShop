/** @file       udaq.h
 *  @brief      ARCELIK Universal Data Acquisition & Control
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ovunc Doruk Izgen
 *  @author     Deniz Fer
 *  @date       14.03.2019
 */
#ifndef FILE_UDAQ_H
#define FILE_UDAQ_H

/**
 *  @defgroup   UDAQ UDAQ
 *  @ingroup    MW
 *  @brief      ARCELIK Universal Data Acquisition & Control
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/// @brief  Structure used to define an item in UDAQ array
typedef struct
{
    void *address; ///< Address of item
    uint8_t size;  ///< Size of item
} tsUdaqItem;

/// @brief  UDAQ package that is transmitted from an external source
typedef struct PLATFORM_PACKED
{
    uint8_t command;  ///< Requested operation
    uint16_t address; ///< Target byte index in array
    uint8_t data;     ///< Data required by operation
    uint8_t checksum; ///< Package checksum
} tsUdaqPkg;

/// @brief  Process specific variables
typedef struct
{
    tsUdaqPkg incoming;         ///< Incoming package
    tsLsl *verList;             ///< Temporary pointer to version list
    tsVersionListItem *verItem; ///< Temporary pointer to version item
    tsThread child;             ///< Child thread for processing the command
    uint16_t timeoutCnt;        ///< Timeout count to clear system if stuck
    uint8_t captureState;       ///< State of read loop
    uint8_t totalSize;          ///< Total byte size of itemArray
} tsUdaqParams;

/// @brief  Process specific parameters
typedef struct
{
    const tsDevCom *com;         ///< Communication device that will be used for data transfer
    tsTarget *target;            ///< Communication channel information for communication device
    const tsUdaqItem *itemArray; ///< Array of items that will be used for data acquisition
    uint8_t itemArraySize;       ///< Size of array
    uint8_t machineType;         ///< Machine type
    uint8_t modelNo;             ///< Model number
    uint8_t modelId;             ///< Model id
    uint8_t romVer;              ///< ROM version
    uint8_t eepVer;              ///< EEPROM version
    uint8_t cryptoKey;           ///< Crypto key
} tsUdaqConsts;

/*********************************************************************
 *  EXPORTED MACROS
 */

/// @brief  Macro used to define a UDAQ item in an array of tsUdaqItem
/// @param  _objPtr Pointer to target variable that will be placed into array
#define UDAQ_ITEM(_objPtr)            \
    {                                 \
        (_objPtr), sizeof(*(_objPtr)) \
    }

/** @brief  UDAQ process creation macro
 *  @param  _name           Name of process object
 *  @param  _com            Communication device that will be used
 *  @param  _target         Communication channel that will be used
 *  @param  _itemArray      Name of tsUdaqItem array
 *  @param  _machineType    Type of machine
 *  @param  _modelNo        Model number
 *  @param  _modelId        Model id
 *  @param  _romVer         ROM version
 *  @param  _eepVer         EEPROM version
 *  @param  _cryptoKey      Crypto key that will be used during UDAQ transitions
 */
#define PROCESS_UDAQ_CREATE(_name, _com, _target, _itemArray, _machineType, _modelNo, _modelId, _romVer, _eepVer, _cryptoKey) \
    tsUdaqParams _name##Params =                                                                                                        \
        {                                                                                                                               \
            .incoming     = {0, 0, 0, 0},                                                                                               \
            .timeoutCnt   = 0,                                                                                                          \
            .captureState = 0,                                                                                                          \
            .totalSize    = 0,                                                                                                          \
    };                                                                                                                                  \
    const tsUdaqConsts _name##Consts =                                                                                                  \
        {                                                                                                                               \
            .com             = &(_com),                                                                                                 \
            .target          = &_target,                                                                                                \
            .itemArray       = _itemArray,                                                                                              \
            .itemArraySize   = ARRAY_SIZE(_itemArray),                                                                                  \
            .machineType     = (_machineType),                                                                                          \
            .modelNo         = (_modelNo),                                                                                              \
            .modelId         = (_modelId),                                                                                              \
            .romVer          = (_romVer),                                                                                               \
            .eepVer          = (_eepVer),                                                                                               \
            .cryptoKey       = (_cryptoKey),                                                                                            \
    };                                                                                                                                  \
    PROCESS_CREATE(_name, udaqInit, udaqDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_UDAQ_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_UDAQ_H
