/** @file       debug.h
 *  @brief      Print debug string
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Rüştü Irklı
 *  @author     Deniz Fer
 *  @date       18.01.2018
 *  @date       6.12.2018
 */
#ifndef FILE_DEBUG_H
#define FILE_DEBUG_H

#ifndef FILE_DEBUG_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

#include <stddef.h>
#include <stdint.h>
#include <ctype.h>

/** @defgroup   DEBUG DEBUG
 *  @ingroup    LIBS
 *  @brief      Print debug string if DEBUG is defined
 *  @details    define _DEBUG_FUNC on global scale which will has the name of a printf-like function.<BR>
 *              define DEBUG_FILE_NAME at the top of your file.<BR>
 *              define different ports starting with DEBUG_PORT_ each with a different bit index(up to 31).<BR>
 *              use DEBUG_START macro to select available ports during compile-time(use 0 for none).<BR>
 *              use DEBUG_ENABLE and DEBUG_DISABLE to manipulate ports during run-time.<BR>
 *              use DEBUG_PRINT and DEBUG_DUMP to output logs.<BR>
 *              @code
 *                  #define DEBUG_FILE_NAME     "adc"
 *                  #define DEBUG_PORT_INIT     BIT(0)
 *                  #define DEBUG_PORT_INFO     BIT(1)
 *                  #define DEBUG_PORT_ERROR    BIT(2)
 *                  DEBUG_START(DEBUG_PORT_INIT + DEBUG_PORT_INFO + DEBUG_PORT_ERROR)
 *                  ...
 *                  // inside the functions
 *                  DEBUG_ENABLE(DEBUG_PORT_INIT + DEBUG_PORT_ERROR);
 *                  DEBUG_DISABLE(DEBUG_PORT_INFO);
 *                  DEBUG_PRINT(DEBUG_PORT_INIT, "module initialized.");
 *                  DEBUG_PRINT(DEBUG_PORT_INFO, "read value: %d", adcValue);
 *                  DEBUG_DUMP(DEBUG_PORT_ERROR, params->area, sizeof(params->area));
 *                  ...
 *              @endcode
 *  @{
 */

/*********************************************************************
 *  EXPORTED MACROS
 */

#if (defined DEBUG & defined _DEBUG_FUNC) | defined RCOS_DOXYGEN

extern int _DEBUG_FUNC(const char *format, ...); // There's a printf-like function out there for printing our logs
extern void debugDump(int (*printf)(const char *format, ...), const void *address, size_t length);

// RCOS Debug sub-macros
#define _DEBUG_LOC_FORMAT "%-6.6s:%04d: " /*TERMINAL_SET_FG_COLOR(%d)*/ "|%d|" /*TERMINAL_SET_FG_COLOR(9)*/ " "
#define _DEBUG_LOC_ARGS(_port) DEBUG_FILE_NAME, __LINE__, (_port)

// RCOS Debug active macros
#define DEBUG_START(_available)                          \
    static const uint32_t debugAvailable = (_available); \
    static uint32_t debugEnabled         = 0;

#define DEBUG_ENABLE(_name)      \
    do                           \
    {                            \
        debugEnabled |= (_name); \
    } while (0)

#define DEBUG_DISABLE(_name)      \
    do                            \
    {                             \
        debugEnabled &= ~(_name); \
    } while (0)

#define DEBUG_PRINT(_port, _format, ...)                                                                     \
    do                                                                                                       \
    {                                                                                                        \
        if (debugAvailable & (_port))                                                                        \
        {                                                                                                    \
            if (debugEnabled & (_port))                                                                      \
            {                                                                                                \
                _DEBUG_FUNC("\r\n" _DEBUG_LOC_FORMAT _format "\r\n", _DEBUG_LOC_ARGS(_port), ##__VA_ARGS__); \
            }                                                                                                \
        }                                                                                                    \
    } while (0)

#define DEBUG_DUMP(_port, _pointer, _length)               \
    do                                                     \
    {                                                      \
        if (debugAvailable & (_port))                      \
        {                                                  \
            if (debugEnabled & (_port))                    \
            {                                              \
                debugDump(_DEBUG_FUNC, _pointer, _length); \
            }                                              \
        }                                                  \
    } while (0)

#else // #if (defined DEBUG & defined _DEBUG_FUNC) | defined RCOS_DOXYGEN

// RCOS Debug deactive macros
#define DEBUG_START(_available)
#define DEBUG_ENABLE(_name)
#define DEBUG_DISABLE(_name)

#define DEBUG_PRINT(_port, _format, ...)
#define DEBUG_DUMP(_port, _pointer, _length)

#endif // #else // #if DEBUG

/** @brief Adding debug version to platform versioning
 */
INTERFACE void addDebugVersion();

#undef INTERFACE
/** @} */

#endif // FILE_DEBUG_H
