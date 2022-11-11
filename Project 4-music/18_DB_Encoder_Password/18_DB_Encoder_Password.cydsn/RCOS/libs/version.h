/**
 *  @file       version.h
 *  @brief      Version control macros
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *  @author     Deniz Fer
 *  @date       12.10.2018
 */
#ifndef FILE_VERSION_H
#define FILE_VERSION_H

#include "list_sl.h"

/** @addtogroup LIBS
 *  @{
 *      @defgroup   VERSION VERSION
 *      @brief      Version check macros using compiler built-in __DATE__ and __TIME__
 *  @}
 *
 *  @addtogroup VERSION
 *  @{
 */

///@brief version struct
typedef struct
{
    const char *compName;
    const char *compVersion;
} tsVersion;

///@brief version list item
typedef struct
{
    tsLslItem _li;
    const tsVersion *versionInfo;
} tsVersionListItem;

/// @brief  Add your version information as a tsVersionListItem into a list of type tsLsl
/// @param  _list Name of tsLsl list
/// @param  _module String of module name
/// @param  _number String of version
#define ADD_VERSION(_list, _module, _number)                                                  \
    do                                                                                        \
    {                                                                                         \
        const static tsVersion version       = {.compName = _module, .compVersion = _number}; \
        static tsVersionListItem versionItem = {.versionInfo = &version};                     \
        addVersionItem(&_list, &versionItem);                                                 \
    } while (0);

/// @brief  Add your version information as a tsVersionListItem into a list of type tsLsl
/// @param  _module String of module name
/// @param  _number String of version
#define ADD_USER_VERSION(_module, _number)                                             \
    do                                                                                        \
    {                                                                                         \
        const static tsVersion version       = {.compName = _module, .compVersion = _number}; \
        static tsVersionListItem versionItem = {.versionInfo = &version};                     \
        extern tsLsl userVersionList;                                                         \
        addVersionItem(&userVersionList, &versionItem);                                       \
    } while (0);

#define ADD_RCOS_VERSION(_module, _number)              \
    do                                                  \
    {                                                   \
        extern tsLsl rcosVersionList;                   \
        ADD_VERSION(rcosVersionList, _module, _number)  \
    } while (0);

#ifndef FILE_VERSION_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

INTERFACE void addVersionItem(tsLsl *versionList, tsVersionListItem *newCompVersion);

#undef INTERFACE

/// Convert build year into integer
#define COMPUTE_BUILD_YEAR           \
    (                                \
        (__DATE__[7] - '0') * 1000 + \
        (__DATE__[8] - '0') * 100 +  \
        (__DATE__[9] - '0') * 10 +   \
        (__DATE__[10] - '0'))

/// Convert build day into integer
#define COMPUTE_BUILD_DAY                                       \
    (                                                           \
        ((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + \
        (__DATE__[5] - '0'))

/// True if build month is January
#define BUILD_MONTH_IS_JAN (/*__DATE__[0] == 'J' */ __DATE__[1] == 'a' && __DATE__[2] == 'n')
/// True if build month is February
#define BUILD_MONTH_IS_FEB (/*__DATE__[0] == 'F' && __DATE__[1] == 'e' */ __DATE__[2] == 'b')
/// True if build month is March
#define BUILD_MONTH_IS_MAR (/*__DATE__[0] == 'M' */ __DATE__[1] == 'a' && __DATE__[2] == 'r')
/// True if build month is April
#define BUILD_MONTH_IS_APR (/*__DATE__[0] == 'A' */ __DATE__[1] == 'p' /* __DATE__[2] == 'r'*/)
/// True if build month is May
#define BUILD_MONTH_IS_MAY (/*__DATE__[0] == 'M' && __DATE__[1] == 'a' */ __DATE__[2] == 'y')
/// True if build month is June
#define BUILD_MONTH_IS_JUN (/*__DATE__[0] == 'J' */ __DATE__[1] == 'u' && __DATE__[2] == 'n')
/// True if build month is July
#define BUILD_MONTH_IS_JUL (/*__DATE__[0] == 'J' && __DATE__[1] == 'u' */ __DATE__[2] == 'l')
/// True if build month is August
#define BUILD_MONTH_IS_AUG (/*__DATE__[0] == 'A' && __DATE__[1] == 'u' */ __DATE__[2] == 'g')
/// True if build month is September
#define BUILD_MONTH_IS_SEP (/*__DATE__[0] == 'S' && __DATE__[1] == 'e' */ __DATE__[2] == 'p')
/// True if build month is October
#define BUILD_MONTH_IS_OCT (/*__DATE__[0] == 'O' && __DATE__[1] == 'c' */ __DATE__[2] == 't')
/// True if build month is November
#define BUILD_MONTH_IS_NOV (/*__DATE__[0] == 'N' && __DATE__[1] == 'o' */ __DATE__[2] == 'v')
/// True if build month is December
#define BUILD_MONTH_IS_DEC (/*__DATE__[0] == 'D' && __DATE__[1] == 'e' */ __DATE__[2] == 'c')

/// Convert build month into integer
#define COMPUTE_BUILD_MONTH \
    (                       \
        (BUILD_MONTH_IS_JAN) ? 1 : (BUILD_MONTH_IS_FEB) ? 2 : (BUILD_MONTH_IS_MAR) ? 3 : (BUILD_MONTH_IS_APR) ? 4 : (BUILD_MONTH_IS_MAY) ? 5 : (BUILD_MONTH_IS_JUN) ? 6 : (BUILD_MONTH_IS_JUL) ? 7 : (BUILD_MONTH_IS_AUG) ? 8 : (BUILD_MONTH_IS_SEP) ? 9 : (BUILD_MONTH_IS_OCT) ? 10 : (BUILD_MONTH_IS_NOV) ? 11 : (BUILD_MONTH_IS_DEC) ? 12 : /* error default */ 99)

/// Convert build hour into integer
#define COMPUTE_BUILD_HOUR ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
/// Convert build minute into integer
#define COMPUTE_BUILD_MIN ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
/// Convert build second into integer
#define COMPUTE_BUILD_SEC ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')

/// True if build date is bad
#define BUILD_DATE_IS_BAD (__DATE__[0] == '?')

/// Check if build date is OK and convert year into integer
#define BUILD_YEAR ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_YEAR)
/// Check if build date is OK and convert month into integer
#define BUILD_MONTH ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_MONTH)
/// Check if build date is OK and convert day into integer
#define BUILD_DAY ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_DAY)

/// True if build time is bad
#define BUILD_TIME_IS_BAD (__TIME__[0] == '?')

/// Check if build time is OK and convert hour into integer
#define BUILD_HOUR ((BUILD_TIME_IS_BAD) ? 99 : COMPUTE_BUILD_HOUR)
/// Check if build time is OK and convert minute into integer
#define BUILD_MIN ((BUILD_TIME_IS_BAD) ? 99 : COMPUTE_BUILD_MIN)
/// Check if build time is OK and convert second into integer
#define BUILD_SEC ((BUILD_TIME_IS_BAD) ? 99 : COMPUTE_BUILD_SEC)

/// Unique build number
#define BUILD_UNIQUE (__DATE__[0] + __DATE__[1] + __DATE__[2] + __DATE__[3] +  \
                      __DATE__[4] + __DATE__[5] + __DATE__[6] +                \
                      __DATE__[7] + __DATE__[8] + __DATE__[9] + __DATE__[10] + \
                      __TIME__[0] + __TIME__[1] + __TIME__[2] +                \
                      __TIME__[3] + __TIME__[4] + __TIME__[5] +                \
                      __TIME__[6] + __TIME__[7])

/** @} */

#endif // FILE_VERSION_H
