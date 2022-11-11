/** @file       log_csv.h
 *  @brief      LOG_CSV middleware header file
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       20/04/2020
 */
#ifndef FILE_LOG_CSV_H
#define FILE_LOG_CSV_H

/** INCLUDES ******************************************************************/
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_LOG_CSV_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   LOG_CSV LOG_CSV
 *  @ingroup    MW
 *  @brief      Create a log output through a UART channel with CSV format
 *  @details    Can be used with Serial Plot https://hasanyavuz.ozderya.net/?p=244
 *  @{
 */

/// @brief  Structure of a log item
typedef struct
{
    const char *label;   ///< Label of item
    const char *format;  ///< Data format
    const void *address; ///< Addres of data
    uint8_t length;      ///< Length of data
} tsLogItem;

/// @brief      Create a log item
// @warning    Can only be formatted as long integer values or float
/// @param      _label  Text label for this log item
/// @param      _format Output format
/// @param      _item   Internal variable(must be a single variable with basic type)
#define LOG_ITEM(_label, _format, _item) \
    {                                    \
        .label   = _label,               \
        .format  = _format,              \
        .address = &_item,               \
        .length  = sizeof(_item),        \
    }

/// @brief  Parameters of a LogCsv process
typedef struct
{
    tTicket ticket;  ///< Ticket for communication queue
    uint8_t lineCnt; ///< Number of lines printed
} tsLogCsvParams;

/// @brief  Constants of a LogCsv process
typedef struct
{
    const tsDevCom *uart;  ///< Communication device that will be used
    tsTarget *target;      ///< Communication target that will be used
    const tsLogItem *list; ///< Address of list array
    uint32_t period;       ///< Update period for each line
    const char *delimiter; ///< Delimiter
    const char *lineBreak; ///< Line break
    uint8_t listCnt;       ///< Number of items in list
    uint8_t window;        ///< Line count of a window
} tsLogCsvConsts;

/** @brief  LogCsv process object creation macro
 *  @param  _name       Name of process object
 *  @param  _uart       Communication device that will be used
 *  @param  _target     Communication target
 *  @param  _list       Array of const tsLogItem created by LOG_ITEM macros
 *  @param  _period     Log time period
 *  @param  _window     Window line size
 */
#define PROCESS_LOG_CSV_CREATE(_name, _uart, _target, _list, _period, _window) \
    tsLogCsvParams _name##Params =                                             \
        {                                                                      \
            .ticket  = -1,                                                     \
            .lineCnt = 0,                                                      \
    };                                                                         \
    const tsLogCsvConsts _name##Consts =                                       \
        {                                                                      \
            .uart      = &_uart,                                               \
            .target    = &_target,                                             \
            .list      = _list,                                                \
            .listCnt   = ARRAY_SIZE(_list),                                    \
            .period    = (_period),                                            \
            .delimiter = ",",                                                  \
            .lineBreak = "\r\n",                                               \
            .window    = (_window),                                            \
    };                                                                         \
    PROCESS_CREATE(_name, logCsvInit, logCsvDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_LOG_CSV_H
