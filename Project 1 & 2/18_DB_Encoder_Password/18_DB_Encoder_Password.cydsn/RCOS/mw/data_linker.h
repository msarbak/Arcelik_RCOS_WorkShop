/** @file       data_linker.h
 *  @brief      Connects event carried data to volatile memories
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author
 *  @date       28.02.2019
 */
#ifndef FILE_DATA_LINKER_H
#define FILE_DATA_LINKER_H

/**
 *  @defgroup   DATA_LINKER DATA_LINKER
 *  @ingroup    MW
 *  @brief      Connects event carried data to volatile memories
 *  @warning    DOES NOT support multiple instances
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/// @brief  Command structure used as header while sending and getting an event
typedef struct PLATFORM_PACKED
{
    uint8_t dataSet; ///< index of target dataSet
    uint8_t offset;  ///< targeted data starting address inside dataSet
    uint8_t size;    ///< size of data that is targeted
} tsDataLinkerCommand;

/// @brief  Used for defining a DataSet
typedef struct
{
    void *address; ///< target volatile memory address
    uint8_t size;  ///< total size of dataSet
    uint8_t index; ///< index of this dataSet
} tsDataSet;

/** @brief  DataSet item initializer
 *  @param  _obj    Name of object that will be used as a DataSet
 *  @param  _idx    DataSet number of target object
 */
#define DATA_SET_INIT(_obj, _idx)     \
    {                                 \
        &(_obj), sizeof(_obj), (_idx) \
    }

typedef enum
{
    eDataLinkerEventNone,
    eDataLinkerSet,   ///< Used for setting new data for a dataSet
    eDataLinkerGet,   ///< Used for getting data from a dataSet replied with eDataLinkerSet
} teDataLinkerEvents; ///< Events used by DataLinker

/*********************************************************************
 *  EXPORTED MACROS
 */

/** @brief  DataLinker process object creation macro
 *  @param  _name       Name of process object
 *  @param  _enum       Process enumeration for this object
 *  @param  _dataSetArr DataSet array name(an array of tsDataSet items)
 */
#define PROCESS_DATA_LINKER_CREATE(_name, _enum, _dataSetArr) \
    const tsDataSet *dataSetArrPtr = _dataSetArr;             \
    const uint8_t dataSetArrSize   = ARRAY_SIZE(_dataSetArr); \
    PROCESS_CREATE(_name, dataLinkerInit, dataLinkerDeinit, _enum, NULL, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_DATA_LINKER_C
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
/** @brief  Send a dataSet to target comProcess
 *  @param  comProcess  Target process enumeration that will transfer the dataSet to another card
 *  @param  dataSetNo   Enumeration of dataSet that will be transferred
 *  @retval EXIT_SUCCESS
 *  @retval EXIT_FAILURE
 */
INTERFACE uint8_t dataSetSend(tProcessEnum comProcess, uint8_t dataSetNo);

/** @brief  Request a dataSet from target comProcess
 *  @param  comProcess  Target process enumeration that will transfer the dataSet from another card
 *  @param  dataSetNo   Enumeration of dataSet that will be transferred
 *  @retval EXIT_SUCCESS
 *  @retval EXIT_FAILURE
 */
INTERFACE uint8_t dataSetRequest(tProcessEnum comProcess, uint8_t dataSetNo);

/** @brief  Send a part of a dataSet to target comProcess
 *  @param  comProcess  Target process enumeration that will transfer the dataSet to another card
 *  @param  dataSetNo   Enumeration of dataSet that will be transferred
 *  @param  offset      Starting address of partition
 *  @param  length      Length of partition
 *  @retval EXIT_SUCCESS
 *  @retval EXIT_FAILURE
 */
INTERFACE uint8_t dataSetSendPart(tProcessEnum comProcess, uint8_t dataSetNo, uint8_t offset, uint8_t length);

/** @brief  Send a member of a DataSet to target _process
 *  @param  _process    Target process enumeration that will transfer the dataSet to another card
 *  @param  _dataSet    Enumeration of dataSet that will be transferred
 *  @param  _type       Type struct of DataSet
 *  @param  _member     Member of type struct of DataSet
 *  @retval EXIT_SUCCESS
 *  @retval EXIT_FAILURE
 */
#define DATASET_MEMBER_SEND(_process, _dataSet, _type, _member) \
    dataSetSendPart((_process), (_dataSet), offsetof(_type, _member), SIZE_OF_MEMBER(_type, _member))

/** @brief  Request a part of a dataSet from target comProcess
 *  @param  comProcess  Target process enumeration that will transfer the dataSet from another card
 *  @param  dataSetNo   Enumeration of dataSet that will be transferred
 *  @param  offset      Starting address of partition
 *  @param  length      Length of partition
 *  @retval EXIT_SUCCESS
 *  @retval EXIT_FAILURE
 */
INTERFACE uint8_t dataSetRequestPart(tProcessEnum comProcess, uint8_t dataSetNo, uint8_t offset, uint8_t length);

/** @brief  Request a member of a DataSet from target _process
 *  @param  _process    Target process enumeration that will transfer the dataSet to another card
 *  @param  _dataSet    Enumeration of dataSet that will be transferred
 *  @param  _type       Type struct of DataSet
 *  @param  _member     Member of type struct of DataSet
 *  @retval EXIT_SUCCESS
 *  @retval EXIT_FAILURE
 */
#define DATASET_MEMBER_REQ(_process, _dataSet, _type, _member) \
    dataSetRequestPart((_process), (_dataSet), offsetof(_type, _member), SIZE_OF_MEMBER(_type, _member))

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_DATA_LINKER_H
