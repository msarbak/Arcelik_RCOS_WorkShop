/** @file       cli_cmds.h
 *  @brief      CLI_CMDS middleware header file
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       27/05/2020
 */
#ifndef FILE_CLI_CMDS_H
#define FILE_CLI_CMDS_H

/** INCLUDES ******************************************************************/
#include "cli.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_CLI_CMDS_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @addtogroup CLI_CMDS
 *  @{
 */

/// @brief  Cls command object
INTERFACE CMD_PROTO(cmdCls);

/// @brief  Echo command object
INTERFACE CMD_PROTO(cmdEcho);

/// @brief  Key test command object
INTERFACE CMD_PROTO(cmdKeyTest);

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_CLI_CMDS_H
