/**
 *  @file       json.h
 *  @brief      Functions to print-out data in json format
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       06.07.2020
 */
#ifndef FILE_JSON_H
#define FILE_JSON_H

#include <stdint.h>

#ifndef FILE_JSON_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   JSON JSON
 *  @ingroup    LIBS
 *  @brief      Setup a print function and stream-out in json format
 *  @details    Usage:
 *              @code
 *                  main()
 *                  {
 *                      ...
 *                      JINIT(printf);
 *                      ...
 *                      JOBJ("objKey",
 *                           JSTR("strKey", "string");
 *                           JARR("array",
 *                                JNUM(NULL, 0);
 *                                JNUM(NULL, 1);
 *                                JNUM(NULL, 2);););
 *                  }
 *              @endcode
 *  @{
 */

/** EXPORTED TYPEDEFS *********************************************************/

/** EXPORTED MACROS ***********************************************************/

/** @brief  Initialize JSON structures to use a specific print function
 *  @param  _print Name of printf like function that will be used to stream
 */
#define JINIT(_print) \
    jsonInit(_print);

/** @brief  Create a JSON OBJECT
 *  @param  _key Key text of item. Leave NULL if not used
 *  @param  _items Other JSON items separated with ';'
 */
#define JOBJ(_key, _items) \
    jsonObjOpen(_key);     \
    _items                 \
    jsonObjClose();

/** @brief  Create a JSON ARRAY
 *  @param  _key Key text of item. Leave NULL if not used
 *  @param  _values Other JSON items separated with ';'. Accepts only values without a key.
 */
#define JARR(_key, _values) \
    jsonArrOpen(_key);      \
    _values                 \
    jsonArrClose();

/** @brief  Create a JSON STRING
 *  @param  _key Key text of item. Leave NULL if not used
 *  @param  _value String
 */
#define JSTR(_key, _value) \
    jsonString(_key, _value)

/** @brief  Create a JSON TEXT
 *  @param  _key Key text of item. Leave NULL if not used
 *  @param  _format Formatted text
 *  @param  ... Values following the format
 */
#define JTXT(_key, _format, ...) \
    jsonText(_key, _format, ##__VA_ARGS__)

/** @brief  Create a JSON NUMBER
 *  @param  _key Key text of item. Leave NULL if not used
 *  @param  _value Numerical value
 */
#define JNUM(_key, _value) \
    jsonNumber(_key, _value)

/** @brief  Create a JSON NUMBER with a float type
 *  @param  _key Key text of item. Leave NULL if not used
 *  @param  _value Numerical value
 *  @param  _precision Number of digits to be printed after the decimal point
 */
#define JFLT(_key, _value, _precision) \
    jsonFloat(_key, _value, _precision)

/** EXPORTED VARIABLES ********************************************************/

/** EXPORTED FUNCTION PROTOTYPES **********************************************/

/** @brief  Initialize JSON streamer
 *  @param  printf Function name that will act as the printf output
 */
INTERFACE void jsonInit(int (*printf)(const char *format, ...));

/** @brief  Open a JSON OBJECT
 *  @param  key Null terminated string for "key". If not used, place NULL
 */
INTERFACE void jsonObjOpen(const char *key);

/// @brief  Close a JSON OBJECT
INTERFACE void jsonObjClose(void);

/** @brief  Open a JSON ARRAY
 *  @param  key Null terminated string for "key". If not used, place NULL
 */
INTERFACE void jsonArrOpen(const char *key);

/// @brief  Close a JSON ARRAY
INTERFACE void jsonArrClose(void);

/** @brief  Insert a JSON STRING
 *  @param  key Null terminated string for "key". If not used, place NULL
 *  @param  string Pointer to string value of variable
 */
INTERFACE void jsonString(const char *key, const char *string);

/** @brief  Insert a JSON TEXT
 *  @param  key Null terminated string for "key". If not used, place NULL
 *  @param  format Formatted text
 *  @param  ... Parameters for the formatted text
 */
INTERFACE void jsonText(const char *key, const char *format, ...);

/** @brief  Insert a JSON NUMBER
 *  @param  key Null terminated string for "key". If not used, place NULL
 *  @param  number Value of variable
 */
INTERFACE void jsonNumber(const char *key, uint32_t number);

/** @brief  Insert a JSON NUMBER for a float type
 *  @param  key Null terminated string for "key". If not used, place NULL
 *  @param  number Value of variable
 *  @param  precision Number of digits to be printed after the decimal point
 */
INTERFACE void jsonFloat(const char *key, float number, uint8_t precision);

/** @brief  Insert a JSON BOOL
 *  @param  key Null terminated string for "key". If not used, place NULL
 *  @param  logic Boolean value of variable
 */
INTERFACE void jsonBool(const char *key, uint8_t logic);

/** @brief  Insert a JSON NULL
 *  @param  key Null terminated string for "key". If not used, place NULL
 */
INTERFACE void jsonNull(const char *key);

/** @} */

#undef INTERFACE

#endif // FILE_DEQUE_H
