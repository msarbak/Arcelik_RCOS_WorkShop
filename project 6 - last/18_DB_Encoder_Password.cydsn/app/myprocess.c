/** @file       myprocess.c
 *  @brief      MYPROCESS application program file
 *  @copyright  (c) 2022-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     UMAMI
 *  @date       11/9/2022
 */
#define FILE_MYPROCESS_C

#include "myprocess.h"

#define DEBUG_FILE_NAME "myprocess"

// #define DEBUG_PORT_
// DEBUG_START(0)

char myMessage[] = "Hello World - RcOS+ \r\n";

/**
 *  @addtogroup MYPROCESS
 *  @{
 */

static PT_THREAD(myProcessThread)
{
    tsMyProcessParams *params       = process->parameters;
    const tsMyProcessConsts *consts = process->constants;

    PT_BEGIN();

    for (;;)
    {
        PT_YIELD();
    }

    PT_END();
}

static PROCESS_HANDLER_PROTO(myProcessHandler);

/// @brief  Initialization function of myProcess
PROCESS_INIT_PROTO(myProcessInit)
{
    tsMyProcessParams *params       = process->parameters;
    const tsMyProcessConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    PROCESS_STATE_CHANGE(process, myProcessHandler);
    devComInit(consts->uart);
    //threadStart(process, myProcessThread);
    
    process->initialized = 1; // If process needs other checks, set this another time
}

/// @brief  Deinitialization function of myProcess
PROCESS_DEINIT_PROTO(myProcessDeinit)
{
    tsMyProcessParams *params       = process->parameters;
    const tsMyProcessConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    PROCESS_STATE_CHANGE(process, NULL);
    threadStop(process, process->threadFunction);
    
    process->initialized = 0; // If process needs other checks, clear this another time
}

/// @brief  Event handler function of myProcess
static PROCESS_HANDLER_PROTO(myProcessHandler)
{
    tsMyProcessParams *params       = process->parameters;
    const tsMyProcessConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    switch (eventCurrent.event)
    {
        case eMPEventsMessageSend: 
        {
            devComSend(consts->uart, myMessage, sizeof(myMessage));
        }
            break;

        default:
            break;
    }
}

/** @} */
