/** @file       buttons.h
 *  @brief      Buttons application, checks buttons and creates button pressed events
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ozkan AKGUL (ozkan.akgul@arcelik.com)
 *  @author     Deniz Fer
 *  @date       01.04.2016
 */
#ifndef FILE_BUTTONS_H
#define FILE_BUTTONS_H

#include "rcos.h"

#ifndef FILE_BUTTONS_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   BUTTONS BUTTONS
 *  @ingroup    MW
 *  @brief      Reads a devIo which holds pressed state of multiple buttons
 *              and creates corresponding events according to definitions.
 *  @{
 */

/// @brief  Button value definition and events for pressed and released actions
typedef struct
{
    uint32_t buttonsValue; ///< Button(s) pressed value
    tEventEnum pressed;    ///< Corresponding pressed action event
    tEventEnum released;   ///< Corresponding released action event
} tsButtonsMapItem;

/** @brief  Macro to define button map items
 *  @param  _val        32-bit raw value of a button(or button combination)
 *  @param  _press      Pressed moment and every time update event
 *  @param  _release    Released moment event
 */
#define BUTTONS_ITEM(_val, _press, _release) \
    {                                        \
        (_val), (_press), (_release)         \
    }

/// @brief  Buttons process parameters
typedef struct
{
    uint32_t lastValue;         ///< Last value read from buttonsDevIo
    uint32_t time;              ///< Time spent in current lastValue
    tProcessEnum targetProcess; ///< Target process enumeration to send buttons' presssed released events
    uint8_t lastIdx;            ///< Last index found on map that has buttonsValue member matching lastValue member
} tsButtonsParams;

/// @brief  Buttons process constants
typedef struct
{
    const tsDevIo *buttonsDevIo; ///< devIo for reading button pressed/released states
    const tsButtonsMapItem *map; ///< Mapping list between button actions and events
    uint32_t resolution;         ///< buttonsDevIo checking period which is also the pressed time resolution
    uint8_t mapSize;             ///< Total size of map list
} tsButtonsConsts;

/** @brief  Creates a buttons conversion process object
 *  @param  _name           Name of process object
 *  @param  _devIo          devIo device name that will be used to get buttons states
 *  @param  _targetProcess  Target process enumeration that will receive buttons
 *                          pressed and released information
 *  @param  _resolution     Resolution of pressed time information in milliseconds
 *  @param  ...             Button definitions, use BUTTONS_ITEM
 */
#define PROCESS_BUTTONS_CREATE(_name, _devIo, _targetProcess, _resolution, ...)        \
    const tsButtonsMapItem _name##Map[] =                                              \
        {                                                                              \
            __VA_ARGS__,                                                               \
    };                                                                                 \
    tsButtonsParams _name##Params =                                                    \
        {                                                                              \
            .lastValue     = 0,                                                        \
            .time          = 0,                                                        \
            .targetProcess = (_targetProcess),                                         \
            .lastIdx       = ARRAY_SIZE(_name##Map),                                   \
    };                                                                                 \
    const tsButtonsConsts _name##Consts =                                              \
        {                                                                              \
            .buttonsDevIo = &(_devIo),                                                 \
            .map          = _name##Map,                                                \
            .mapSize      = ARRAY_SIZE(_name##Map),                                    \
            .resolution   = (_resolution),                                             \
    };                                                                                 \
    PROCESS_CREATE(_name, buttonsInit, buttonsDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_BUTTONS_H
