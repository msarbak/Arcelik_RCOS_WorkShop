/** @file       uartecho.h
 *  @brief      Get data from uart and echo back
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     DF
 *  @date       25.01.2019
 */
#ifndef FILE_UARTECHO_H
#define FILE_UARTECHO_H

#include "rcos.h"

#ifndef FILE_UARTECHO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   UARTECHO UARTECHO
 *  @ingroup    MW
 *  @brief      Get data from uart and echo back
 *  @{
 */

/// @brief  Parameters of a UartEcho process
typedef struct
{
    const tsDevCom *uart; ///< devCom that will be used
} tsUartechoParams;

/** @brief  Create an echo process working for a target uart devCom
 *  @param  _name Name of process object
 *  @param  _uart Target uart devCom device
 */
#define PROCESS_UARTECHO_CREATE(_name, _uart) \
    tsUartechoParams _name##Params =          \
        {                                     \
            &_uart,                           \
    };                                        \
    PROCESS_CREATE(_name, uartechoInit, uartechoDeinit, PROCESS_NONE, &_name##Params, NULL)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_UARTECHO_H
