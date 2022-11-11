/** @file       cli.h
 *  @brief      CLI middleware header file
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       20/05/2020
 */
#ifndef FILE_CLI_H
#define FILE_CLI_H

/** INCLUDES ******************************************************************/
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_CLI_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   CLI CLI
 *  @ingroup    MW
 *  @brief      Command Line Interface
 *  @{
 */

/** @defgroup   CLI_CMDS COMMANDS
 *  @brief      Command definitions, structures and macros
 *  @{
 */

/// @brief  Structure that holds a command information
typedef struct
{
    const char *command;    ///< Command name string
    const char *desc;       ///< Short description string
    const char *help;       ///< Full help string
    PT_THREAD((*function)); ///< Function that will called for this command
} tsCliCmd;

/// @brief  CLI command prototype
/// @param  _name Name of command object
#define CMD_PROTO(_name) const tsCliCmd _name

/// @brief  CLI command function
/// @param  _name Name of command object
/// @param  _cmd CLI command
/// @param  _desc Short description string
/// @param  _help Full help string
#define CMD_FUNC(_name, _cmd, _desc, _help) \
    PT_THREAD(_name##Func);                 \
    CMD_PROTO(_name) =                      \
        {                                   \
            .command  = _cmd,               \
            .desc     = _desc,              \
            .help     = _help,              \
            .function = _name##Func,        \
    };                                      \
    PT_THREAD(_name##Func)

/// @brief  Begining of every command
#define CMD_BEGIN()                                  \
    tsCliParams *params       = process->parameters; \
    const tsCliConsts *consts = process->constants;  \
    UNUSED(params);                                  \
    UNUSED(consts);                                  \
    PT_BEGIN()

/// @brief  End of every command
#define CMD_END() PT_END()

/// @brief  Argument count
#define CMD_ARGC() params->argc

/// @brief  Argument vector array
#define CMD_ARGV() params->argv

/// @brief  Command receive
#define CMD_RECV(_addr, _len) devComReceive(consts->uart, (_addr), (_len))

/// @brief  Command print
#define CMD_PRINT(...) devComPrint(consts->uart, __VA_ARGS__)

/** @defgroup   ESC ESC
 *  @brief      Escape sequence of keyboard keys
 *  @{
 */

/// @brief  Reads escape sequence and places into params->escXxx
/// @note   Must be called if the received byte is ASCII_ESC.
///         CMD_ESC_IF_XXX macros should be used to differentiate escape codes
#define CMD_ESC_SEQ() PT_SPAWN(&params->escThread, cliEscSeqParser)

/** @defgroup   ARROW_PAD ARROW_PAD
 *  @brief      Arrow pad keys
 *  @{
 */

/// @brief  If escape sequence is about cursor up
#define CMD_ESC_IF_APAD_UP() if ('A' == params->escOpt)

/// @brief  If escape sequence is about cursor down
#define CMD_ESC_IF_APAD_DWN() if ('B' == params->escOpt)

/// @brief  If escape sequence is about cursor forward
#define CMD_ESC_IF_APAD_FWD() if ('C' == params->escOpt)

/// @brief  If escape sequence is about cursor backward
#define CMD_ESC_IF_APAD_BWD() if ('D' == params->escOpt)

/** @} */

/** @defgroup   FUNCTION FUNCTION
 *  @brief      Function keys
 *  @{
 */

/// @brief  If escape sequence is about a function key
/// @param  _no Number of the function key
#define CMD_ESC_IF_FUNCTION(_no) if (('~' == params->escOpt) && ((10 + (_no)) == params->escVal[0]))

/** @} */

/** @defgroup   CONTROL_PAD CONTROL_PAD
 *  @brief      Control pad keys
 *  @{
 */

/// @brief  If escape sequence is about a control pad key
#define CMD_ESC_IF_CPAD() if ('~' == params->escOpt)

/// @brief  If escape sequence is about control pad Home key
#define CMD_ESC_IF_CPAD_HOME() if (('~' == params->escOpt) && (1 == params->escVal[0]))

/// @brief  If escape sequence is about control pad Insert key
#define CMD_ESC_IF_CPAD_INS() if (('~' == params->escOpt) && (2 == params->escVal[0]))

/// @brief  If escape sequence is about control pad Delete key
#define CMD_ESC_IF_CPAD_DEL() if (('~' == params->escOpt) && (3 == params->escVal[0]))

/// @brief  If escape sequence is about control pad End key
#define CMD_ESC_IF_CPAD_END() if (('~' == params->escOpt) && (4 == params->escVal[0]))

/// @brief  If escape sequence is about control pad PgUp key
#define CMD_ESC_IF_CPAD_PGUP() if (('~' == params->escOpt) && (5 == params->escVal[0]))

/// @brief  If escape sequence is about control pad PgDn key
#define CMD_ESC_IF_CPAD_PGDN() if (('~' == params->escOpt) && (6 == params->escVal[0]))

/** @} */

/** @} ESC */

/** @} CLI_CMDS */

/// @brief  Parameters of a Cli process
typedef struct
{
    char *argv[16];     ///< Argument vector array
    tsThread cmdThread; ///< Current command thread
    tsThread escThread; ///< Escape command thread
    tTicket uartTicket; ///< UART ticket
    uint16_t bufferIdx; ///< Stack index
    uint16_t escCnt;    ///< Escaped values count
    uint16_t escVal[3]; ///< Escaped values
    uint8_t argc;       ///< Argument count
    uint8_t cmdIdx;     ///< Command index
    uint8_t escOpt;     ///< Escape option code
    uint8_t readByte;   ///< Single byte read from terminal
    char buffer[256];   ///< Stack for CLI input
} tsCliParams;

/// @brief  Constants of a Cli process
typedef struct
{
    const tsDevCom *uart;     ///< UART device
    tsTarget *target;         ///< UART target
    const tsCliCmd **cmdList; ///< Command list
    uint8_t cmdCnt;           ///< Command list count
} tsCliConsts;

INTERFACE PT_THREAD(cliEscSeqParser); ///< Escape sequence parser

/** @brief  Cli process object creation macro
 *  @param  _name Name of process object
 *  @param  _uart UART device to be used
 *  @param  _target Target object
 *  @param  ... Address of available command objects (const tsCliCmd *)
 */
#define PROCESS_CLI_CREATE(_name, _uart, _target, ...) \
    extern CMD_PROTO(cmdHelp);                         \
    const tsCliCmd *_name##List[] =                    \
        {                                              \
            &cmdHelp,                                  \
            ##__VA_ARGS__};                            \
    tsCliParams _name##Params =                        \
        {                                              \
            .uartTicket = -1,                          \
            .cmdThread  = THREAD_INIT(),               \
    };                                                 \
    const tsCliConsts _name##Consts =                  \
        {                                              \
            .uart    = &_uart,                         \
            .target  = &_target,                       \
            .cmdList = _name##List,                    \
            .cmdCnt  = ARRAY_SIZE(_name##List),        \
    };                                                 \
    PROCESS_CREATE(_name, cliInit, cliDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_CLI_H
