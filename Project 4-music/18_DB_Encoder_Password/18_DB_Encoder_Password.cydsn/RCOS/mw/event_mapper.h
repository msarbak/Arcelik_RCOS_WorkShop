/** @file       event_mapper.h
 *  @brief      Event mapper process will translator an event from a source into another event for another destination
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       16.09.2019
 */
#ifndef FILE_EVENT_MAPPER_H
#define FILE_EVENT_MAPPER_H

/**
 *  @defgroup   EVENT_MAPPER EVENT_MAPPER
 *  @ingroup    MW
 *  @brief      Event mapper process will translator an event from a source into another event for another destination
 *  @{
 */

#include "rcos.h"

/// If enabled allows multiple posts but reduces performance
#define EVENT_MAPPER_MULTI_EVENTPOSTS DISABLE

/// @brief  Single event mapper item that is used for redirection
typedef struct
{
    tProcessEnum srcProcess; ///< Incoming process enumeration
    tEventEnum srcEvent;     ///< Incoming process event
    tProcessEnum dstProcess; ///< Outgoing process enumeration
    tEventEnum dstEvent;     ///< Outgoing process event
} tsEventMapperItem;

/// @brief  Parameters of a EventMapper process
typedef struct
{
    const tsEventMapperItem *array; ///< Array used for event mapping
    uint16_t arraySize;             ///< Item count in mapping array
} tsEventMapperParams;

/** @brief  EventMapper process object creation macro
 *  @param  _name   Name of process object
 *  @param  _enum   Process enumeration for this object
 *  @param  _array  Array name with elements of tsEventMapperItem that will be used for mapping
 */
#define PROCESS_EVENT_MAPPER_CREATE(_name, _enum, _array) \
    const tsEventMapperParams _name##Params =             \
        {                                                 \
            _array,                                       \
            ARRAY_SIZE(_array),                           \
    };                                                    \
    PROCESS_CREATE(_name, eventMapperInit, eventMapperDeinit, _enum, &_name##Params, NULL)

/** @} */

#endif // FILE_EVENT_MAPPER_H
