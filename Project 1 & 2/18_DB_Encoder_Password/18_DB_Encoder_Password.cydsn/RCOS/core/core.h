/** @file       core.h
 *  @brief      RCoS+ main functions
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Mehmet Baris
 *  @author     Deniz Fer
 *  @date       08.03.2016
 */
#ifndef FILE_CORE_H
#define FILE_CORE_H

/**
 *  @defgroup   RCoS RCoS
 *  @brief      Reusable, Connected, Secure Software Platform
 * 
 *  @{
 *      @defgroup   CORE CORE
 *      @brief      Core functionalities
 *      @details    RCoS+ composed of a cooperative multi-tasking OS with abstract device drivers.
 * 
 *      @{
 *
 *          @defgroup   WDT WDT
 *          @brief      Watchdog control mechanisms
 *
 *          @defgroup   EVENTS EVENTS
 *          @brief      Signals posted for different processes to run event specific functions.
 *
 *          @defgroup   PROCESS PROCESS
 *          @brief      Applications that decide how the system should react to each event.
 * 
 *          @{
 * 
 *              @defgroup   PROTOTHREADS PROTOTHREADS
 *              @brief      This is an implementation of protothreads invented by Adam Dunkels into RCOS/PROCESS
 * 
 *          @}
 * 
 *          @defgroup   TIMERS TIMERS
 *          @brief      Software timers to call a function or post an event after a timeout.
 *
 *          @{
 *
 *              @defgroup   TIMER_GENERIC TIMER_GENERIC
 *              @brief      Simple counter to calculate passage of time
 * 
 *              @defgroup   TIMER_CALLBACK TIMER_CALLBACK
 *              @brief      Calls a specific function with a period for a number of times. If callCount is entered as 0, it runs forever
 *              @warning    TIMER_CALLBACK calls the target functions in rcosTickIsr hence these functions must be SHORT! to prevent timing errors.
 *
 *              @defgroup   TIMER_EVENT TIMER_EVENT
 *              @brief      Posts an event for target event handler after a period of time.
 * 
 *          @}
 * 
 *          @defgroup   DEBUGGING DEBUGGING
 *          @brief      Debug function defined to be used for libs/debug.h and target device creation
 * 
 *      @}
 *
 *      @defgroup   MW MW
 *      @brief      Middlewares developed on RCoS+
 * 
 *  @}
 */

#include "libs/libs.h"
#include "libs/deque.h"
#include "libs/list_dl.h"
#include "dev/dev.h"

#ifndef FILE_CORE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief      Event generic type
/// @ingroup    EVENTS
typedef uint8_t tEventEnum;

/// @brief      Process number used for defining a process throughout the system
/// @ingroup    PROCESS
typedef uint8_t tProcessEnum;

#define PROCESS_NONE ((tProcessEnum)-1ul)
#define EVENT_NONE ((tEventEnum)0ul)
#define EVENT_PT ((tEventEnum)-1ul)

#define CORE_INTERCOM DISABLE

/** @addtogroup DEBUGGING
 * @{
 */

/** @brief  Setup a device as the debug logging target
 *  @param  _devName Name of debug devCom
 */
#define CORE_DEBUG_DEV(_devName)    \
    extern const tsDevCom _devName; \
    const tsDevCom *rcosDebugDev = &_devName;

INTERFACE const tsDevCom *rcosDebugDev __attribute__((weak));

/** @brief  Debug printing function for RCOS
 *  @param  format Pointer to a format text
 *  @param  ... Arguments that will placed into formatted text
 *  @return length Length of data send
 */
INTERFACE int rcosDebugPrint(const char *format, ...);

/** @} */

/** @addtogroup EVENTS
 *  @{
 */

/// @brief  Event information stored in queue
typedef struct
{
    tProcessEnum source;      ///< Event source enumeration(another event processor or unknown)
    tProcessEnum destination; ///< Target event processor enumeration
    tEventEnum event;         ///< Event enumeration dependant on target
    uint8_t length;           ///< Length of data that accompanies the event
} tsEventPost;

/// @brief  Used for creating eventQueue object in main.c
#define CORE_EVENTQUEUE_SIZE(_size)  \
    uint8_t eventQueueBuffer[_size]; \
    volatile tsDeque eventQueue = DEQUE_INIT(eventQueueBuffer);

/** @brief  Insert an event into queue with FIFO
 *  @param  destination Enumeration of target event process
 *  @param  event       Enumeration of event
 *  @param  data        Pointer to location of data that will accompany event
 *  @param  length      Length of data
 *  @retval EXIT_FAILURE
 *  @retval EXIT_SUCCESS
 */
INTERFACE uint8_t eventPost(tProcessEnum destination, tEventEnum event, const void *data, uint8_t length);

/** @brief  Reinsert current event into queue with FIFO
 *  @retval EXIT_FAILURE
 *  @retval EXIT_SUCCESS
 */
INTERFACE uint8_t eventPostPone(void);

/** @brief  Insert an event into queue with LIFO in ISR does not work outside of ISR
 *  @param  destination Enumeration of target event process
 *  @param  event       Enumeration of event
 *  @retval EXIT_FAILURE
 *  @retval EXIT_SUCCESS
 */
INTERFACE uint8_t eventPostInIsr(tProcessEnum destination, tEventEnum event);

/** @brief  Reply an event post with another event
 *  @param  event   Enumeration of event
 *  @param  data    Pointer to the location of data that will accompany event
 *  @param  length  Length of data
 *  @retval EXIT_FAILURE
 *  @retval EXIT_SUCCESS
 */
INTERFACE uint8_t eventReply(tEventEnum event, const void *data, uint8_t length);

INTERFACE tsEventPost eventCurrent; ///< Current event information
INTERFACE uint8_t eventData[256];   ///< Data associated with current event

#if 0 // NOT NOW
/// @brief  Structure to define an event queue for a process
typedef struct
{
    tsLdl _li;               ///< List connection
    tsDeque queue;            ///< Double Ended Queue to hold queue information
    tsProcess *process;       ///< Target process
} tsEventQueue;

#define EVENT_QUEUE_INIT(_processPtr, _buffer) \
    {                                          \
        ._li     = LIST_DL_ITEM_INIT(),        \
        .queue   = DEQUE_INIT(_buffer),        \
        .process = (_processPtr),              \
    }

#define EVENT_QUEUE_CREATE(_name, _processPtr, _size) \
    uint8_t _name##EvQueBuff[_size];                  \
    tsEventQueue _name##EvQue = EVENT_QUEUE_INIT(_processPtr, _name##EvQueBuff);
#endif

/** @} */

/** @addtogroup TIMERS
 *  @{
 */

/// @brief  Macro to be used for a tick isr function prototype
#define CORE_TICK_PROTO(_name) void _name(void)

/// @brief  Interrupt subroutine to be called every 1ms for TIMERS to work
INTERFACE CORE_TICK_PROTO(rcosTickIsr);

/// @brief  Tick functions structure
typedef struct
{
    void (*start)(void);   ///< Function to start tick timer
    void (*enable)(void);  ///< Function to enable tick interrupt
    void (*disable)(void); ///< Function to disable tick interrupt
} tsTick;

/** @brief  Creates rcosTick object.
 *          RCoS+ timer feature runs with this externally created object
 *  @param  _start      Function to be called to start a 1ms timer interrupt to call rcosTickIsr
 *  @param  _enable     Function to be called to enable timer interrupt
 *  @param  _disable    Function to be called to disable timer interrupt
 */
#define CORE_TICK_CREATE(_start, _enable, _disable) \
    const tsTick rcosTick = {&(_start), &(_enable), &(_disable)};

/// @brief  This object must be created in main.c with CORE_TICK_CREATE macro
extern const tsTick rcosTick;

/** @} */

/** @addtogroup TIMER_GENERIC
 *  @{
 */

/// @brief  Timer generic object structure
typedef struct
{
    tsLdlItem _li; /// @warning Used internally, do not modify!
    uint32_t _cnt; /// @warning Used internally, do not modify!
} tsTimerGeneric;

/// @brief  Initialized timer generic structure
#define TIMER_GENERIC_INIT()         \
    {                                \
        ._li  = LIST_DL_ITEM_INIT(), \
        ._cnt = 0,                   \
    }

/** @brief  Creates a generic timer object.
 *          Object with _name can be used to start and stop a generic timer
 *  @param  _name of the generic timer object
 */
#define TIMER_GENERIC_CREATE(_name) \
    tsTimerGeneric _name = TIMER_GENERIC_INIT();

/** @brief      Start a general purpose sw timer
 *  @param[in]  obj target object that will be processed inside rcosTickIsr
 *  @param[in]  period of time that the timer will run
 *  @param[out] obj target object is processed
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t timerGenericStart(tsTimerGeneric *obj, uint32_t period);

/** @brief      Remove a running timer and reset parameters of it
 *  @param[in]  obj target object that will be processed inside rcosTickIsr
 *  @param[out] obj target object is processed
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t timerGenericStop(tsTimerGeneric *obj);

/** @} */

/** @addtogroup TIMER_CALLBACK
 *  @{
 */

/// @brief  Timer callback object structure
typedef struct _tsTimerCallBack tsTimerCallBack;

/** @brief  Timer callback function template
 *  @param  _name object name
 */
#define TIMER_CALLBACK_FUNC(_name) uint32_t _name(void *parameter)

struct _tsTimerCallBack
{
    tsLdlItem _li;                    ///< @warning Used internally, do not modify!
    uint32_t _cnt;                    ///< @warning Used internally, do not modify!
    TIMER_CALLBACK_FUNC((*callBack)); ///< Function to call after timer ends
    void *parameter;                  ///< Parameter passed to function
};

/** @brief  Initialized timer callback object
 *  @param  _func Name of the function that will be called
 *  @param  _parameter Parameter that will be passed to the callback function
 */
#define TIMER_CALLBACK_INIT(_func, _parameter) \
    {                                          \
        ._li       = LIST_DL_ITEM_INIT(),      \
        ._cnt      = 0,                        \
        .callBack  = (_func),                  \
        .parameter = (void *)(_parameter),     \
    }

/** @brief  Creates a timer callback object
 *          Object with _name can be used to start and stop a callback timer
 *  @param  _name Name of the timer callback object
 *  @param  _func Name of the function that will be called
 *  @param  _parameter Parameter that will be passed to the callback function
 */
#define TIMER_CALLBACK_CREATE(_name, _func, _parameter) \
    tsTimerCallBack _name = TIMER_CALLBACK_INIT(_func, _parameter);

/** @brief      Start a timer to call a function with a period of time for a number of times.
 *  @warning    Timer with callback function is unsecure, it cannot be controled by reliability functions (system reset mechanism)!
 *              If the callback function duration is greater than wdt period, wdt reset occures. However, the reset reason won't be recorded!
 *              Do not use if you are not obliged to!
 *  @param[in]  obj         Target object that will be processed inside rcosTickIsr
 *  @param[in]  period      Period of time that the timer will run
 *  @param[out] obj target object is processed
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t timerCallBackStart(tsTimerCallBack *obj, uint32_t period);

/** @brief      Remove a running timer and reset parameters of it
 *  @param[in]  obj     Target object that will be processed inside rcosTickIsr
 *  @param[out] obj     Target object is processed
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t timerCallBackStop(tsTimerCallBack *obj);

/** @} */

/** @addtogroup TIMER_EVENT
 *  @{
 */

/// @brief  Timer event object structure
typedef struct
{
    tsLdlItem _li;    ///< @warning Used internally, do not modify!
    uint32_t _cnt;    ///< @warning Used internally, do not modify!
    tsEventPost post; ///< Event that will be posted after timeout(no data)
} tsTimerEvent;

/** @brief  Initialized timer event object
 *  @param  _src source process enumeration
 *  @param  _dst destination process enumeration
 *  @param  _event event enumeration that will be posted
 */
#define TIMER_EVENT_INIT(_src, _dst, _event)     \
    {                                            \
        ._li  = LIST_DL_ITEM_INIT(),             \
        ._cnt = 0,                               \
        .post =                                  \
        {                                        \
            .source      = (tProcessEnum)(_src), \
            .destination = (tProcessEnum)(_dst), \
            .event       = (tEventEnum)(_event), \
            .length      = 0                     \
        }                                        \
    }

/** @brief  Create an event timer object
 *  @param  _name object name
 *  @param  _src source process enumeration
 *  @param  _dst destination process enumeration
 *  @param  _event target event enumeration
 */
#define TIMER_EVENT_CREATE(_name, _src, _dst, _event) \
    tsTimerEvent _name = TIMER_EVENT_INIT(_src, _dst, _event);

/** @brief      Add new sw timer with event
 *  @param[in]  obj         Target object that will be processed inside rcosTickIsr
 *  @param[in]  duration    Timeout to send event in milliseconds
 *  @param[out] obj         Target object is processed
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t timerEventStart(tsTimerEvent *obj, uint32_t duration);

/** @brief      Remove sw timer from list
 *  @param[in]  obj     Target object that will be processed inside rcosTickIsr
 *  @param[out] obj     Target object is processed
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t timerEventStop(tsTimerEvent *obj);

/** @} */

/// @brief      Process object structure
/// @ingroup    PROCESS
typedef struct _tsProcess tsProcess;

/** @addtogroup PROTOTHREADS
 *  @details    http://dunkels.com/adam/pt/index.html.
 *  @warning    lc-switch method is used. meaning; switch/case cannot be used inside a protothread funciton.
 *  @warning    No Hierarchical protothreads. Child protothreads cannot be started.
 *  @warning    Thread variables are hidden inside tsProcess and THERE CAN BE ONLY ONE.
 *  @code
 *      PT_THREAD(myThread)
 *      {
 *          tsParams *params = process->parameters;
 * 
 *          PT_BEGIN();
 * 
 *          for (;;)
 *          {
 *              params->ticket = devComTicketGet(params->com);                          // Get ticket
 *              PT_WAIT_UNTIL(devComTicketValid(params->com, params->ticket)); // Wait until ticket is validated
 * 
 *              devComOpen(params->com, &params->target);
 *              devComReceive(params->com, params->buffer, sizeof(params->buffer)); // Do some work
 *              devComClose(params->com, &params->target);
 * 
 *              PT_WAIT_WHILE(devComTicketValid(params->com, params->ticket)); // Wait while ticket is valid
 * 
 *              PT_YIELD();             // Leave thread voluntarily
 *              PT_WAIT_TIMEOUT(10); // Come back after 10ms
 *          }
 * 
 *          PT_END();
 *      }
 *  @endcode
    @verbatim
        Copyright (c) 2004-2005, Swedish Institute of Computer Science.
        All rights reserved.

        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        3. Neither the name of the Institute nor the names of its contributors
        may be used to endorse or promote products derived from this software
        without specific prior written permission.

        THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS `AS IS' AND
        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
        IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
        ARE DISCLAIMED. IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
        FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
        DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
        OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
        HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
        LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
        OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
        SUCH DAMAGE.

        Author: Adam Dunkels
    @endverbatim
 *  @{
 */

/// @brief      Declaration of a protothread.
/// @details    We're using our process as the protothread structure, hence no need to declare separately
/// @warning    name_args should have only the name
/// @param      name_args Name of the task function for this thread
#define PT_THREAD(name_args) uint32_t name_args(tsProcess *process, struct _tsThread *thread)

/// @brief  Thread structure
typedef struct _tsThread
{
    int16_t lc; ///< Protothread, Local continuations variable
} tsThread;

/// @brief  Initialize a tsThread structure.
#define THREAD_INIT() \
    {                 \
        .lc = 0,      \
    }

/// @brief  Local continuation initialization macro
/// @param  s Variable that keeps local continuation value
#define LC_INIT(s) s = 0; // Initial case value

/// @brief  Local continuation resume macro
/// @param  s Variable that keeps local continuation value
#define LC_RESUME(s) \
    switch (s)       \
    {                \
        case 0: // Initial case

/// @brief      Local continuation setup macro
/// @warning    Different than the original macro! A "return" keyword is placed inside the macro for this implementation
/// @param      s Variable that keeps local continuation value
#define LC_SET(s) \
    s = __LINE__; \
    case __LINE__:

/// @brief      Local continuation ending macro
/// @warning    Different than the original macro! To prevent warnings, a default case with a break is added which also acts as the exit
/// @param      s Variable that keeps local continuation value
#define LC_END(s) \
    default:      \
        break;    \
        }

#define PT_WAITING (1) // 0
#define PT_YIELDED (1) // 1
#define PT_EXITED (0)  // 2
#define PT_ENDED (0)   // 3

/// @brief  Initialize a protothread.
/// @param  pt Pointer to a tsThread
#define PT_INIT(pt)      \
    do                   \
    {                    \
        LC_INIT(pt->lc); \
    } while (0)

/// @brief  Declare the start of a protothread inside the C function implementing the protothread.
#define PT_BEGIN()              \
    {                           \
        char PT_YIELD_FLAG = 1; \
        UNUSED(PT_YIELD_FLAG);  \
        LC_RESUME(thread->lc)

/// @brief  Declare the end of a protothread.
#define PT_END()        \
    LC_END(thread->lc); \
    PT_YIELD_FLAG = 0;  \
    PT_INIT(thread);    \
    return PT_ENDED;    \
    }

/// @brief      Cooperate until a timeout occurs.
/// @param      timeout Timeout duration in milliseconds
/// @warning    DO NOT USE a timeout smaller than 2ms
#define PT_YIELD_TIMEOUT(timeout) \
    do                            \
    {                             \
        PT_YIELD_FLAG = 0;        \
        LC_SET(thread->lc);       \
        if (0 == PT_YIELD_FLAG)   \
        {                         \
            return (timeout);     \
        }                         \
    } while (0)

/// @brief  Cooperative process releases CPU for others
#define PT_YIELD() PT_YIELD_TIMEOUT(PT_YIELDED)

/// @brief  Block and wait until condition is true.
/// @param  condition Condition for wait
#define PT_WAIT_UNTIL(condition) \
    do                           \
    {                            \
        LC_SET(thread->lc);      \
        if (!(condition))        \
        {                        \
            return PT_WAITING;   \
        }                        \
    } while (0)

/// @brief  Block and wait while condition is true.
/// @param  cond Condition for wait
#define PT_WAIT_WHILE(cond) PT_WAIT_UNTIL(!(cond))

/// @brief  Block and wait until condition is true. Check it after a period
/// @param  condition Condition for wait
/// @param  period Period duration in milliseconds
#define PT_WAIT_UNTIL_W_PERIOD(condition, period) \
    do                                            \
    {                                             \
        LC_SET(thread->lc);                       \
        if (!(condition))                         \
        {                                         \
            return (period);                      \
        }                                         \
    } while (0)

/// @brief  Block and wait while condition is true. Check it after a period
/// @param  cond Condition for wait
/// @param  period Period duration in milliseconds
#define PT_WAIT_WHILE_W_PERIOD(cond, period) PT_WAIT_UNTIL_W_PERIOD(!(cond), period)

/// @brief  Block and wait until condition is true. Check it after a period and if timeout occurs run the following code block.
/// @param  condition Condition for wait
/// @param  period Period duration in milliseconds
/// @param  timeout Timeout duration in milliseconds
#define PT_WAIT_UNTIL_W_PERIOD_IF_TIMEOUT(condition, period, timeout) \
    process->tgen = (timeout);                                        \
    do                                                                \
    {                                                                 \
        LC_SET(thread->lc);                                           \
        if (!(condition))                                             \
        {                                                             \
            if (process->tgen > (period))                             \
            {                                                         \
                process->tgen -= (period);                            \
                return (period);                                      \
            }                                                         \
            else                                                      \
            {                                                         \
                process->tgen = 0;                                    \
            }                                                         \
        }                                                             \
    } while (0);                                                      \
    if (!process->tgen)

/// @brief  Block and wait while condition is true. Check it after a period and if timeout occurs run the following code block.
/// @param  cond Condition for wait
/// @param  period Period duration in milliseconds
/// @param  timeout Timeout duration in milliseconds
#define PT_WAIT_WHILE_W_PERIOD_IF_TIMEOUT(cond, period, timeout) PT_WAIT_UNTIL_W_PERIOD_IF_TIMEOUT(!(cond), period, timeout)

/// @brief  A process can have only one protothread but that protothread can spawn a child and wait until it is finished
/// @param  child Pointer to child thread
/// @param  function Function name of the spawnling
#define PT_SPAWN(child, function)            \
    do                                       \
    {                                        \
        uint32_t childRet;                   \
        PT_INIT((child));                    \
        LC_SET(thread->lc);                  \
        childRet = function(process, child); \
        if (childRet)                        \
        {                                    \
            return childRet;                 \
        }                                    \
    } while (0)

/// @brief  Restart the protothread
#define PT_RESTART()       \
    do                     \
    {                      \
        PT_INIT(thread);   \
        return PT_YIELDED; \
    } while (0)

/// @brief  Exit the protothread
#define PT_EXIT()         \
    do                    \
    {                     \
        PT_INIT(thread);  \
        return PT_EXITED; \
    } while (0)

#if 0
/// @brief  Get out until an event is received
/// @param  pt Protothread structure
#define PT_WAIT_FOR_EVENT(pt)                             \
    do                                                    \
    {                                                     \
        PROCESS_STATE_CHANGE(proess, pt->threadFunction); \
        PT_YIELD();                                       \
        PROCESS_STATE_RETURN(proess);                     \
    } while (0)

/// @brief  Handle event in the following code block
/// @param  pt Protothread structure
/// @param  _ev Event
#define PT_HANDLE_EVENT(pt, _ev) \
    if (eventCurrent.event == (_ev))
#endif

/** @brief      Used for starting a function inside a protothread that belongs to process
 *  @warning    THERE CAN BE ONLY ONE
 *  @param      process Pointer to process structure that will hold the protothread information
 *  @param      threadFunction Pointer to task function that will be run during thread
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t threadStart(tsProcess *process, PT_THREAD((*threadFunction)));

/** @brief      Used for stopping a function inside a protothread that belongs to process
 *  @param      process Pointer to process structure that will hold the protothread information
 *  @param      threadFunction Pointer to task function that will be run during thread
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t threadStop(tsProcess *process, PT_THREAD((*threadFunction)));

/** @} */

/** @addtogroup PROCESS
 *  @{
 */
/** @brief      Process init function prototype
 *  @param      _name   Name of the function
 *  @details    process Pointer to tsProcess structure that holds the process information
 */
#define PROCESS_INIT_PROTO(_name) void _name(tsProcess *process)
/** @brief      Process deinit function prototype
 *  @param      _name   Name of the function
 *  @details    process Pointer to tsProcess structure that holds the process information
 */
#define PROCESS_DEINIT_PROTO(_name) void _name(tsProcess *process)
/** @brief      Process event handler function prototype
 *  @details    Process can change its event handler depending on the state.
 *              process Pointer to tsProcess structure that holds the process information
 *  @param      _name           Name of the function
 */
#define PROCESS_HANDLER_PROTO(_name) void _name(tsProcess *process)

struct _tsProcess
{
    tsLdlItem _li;
    PROCESS_INIT_PROTO((*init));               ///< Initialization function
    PROCESS_DEINIT_PROTO((*deinit));           ///< DeInitialization function
    PROCESS_HANDLER_PROTO((*handlerCurrent));  ///< Current state's event handler function
    PROCESS_HANDLER_PROTO((*handlerPrevious)); ///< Previous state's event handler function
    void *parameters;                          ///< Variable parameters
    const void *constants;                     ///< Constants
    uint16_t timeMax;                          ///< Maximum time spent during one event
    uint16_t timeMaxEvent;                     ///< Event that caused maximum CPU usage
    uint16_t evCntLoad;                        ///< Current number of events in queue for this process
    uint16_t evCntMax;                         ///< Maximum number of events for one time waited in the queue
    PT_THREAD((*threadFunction));              ///< Protothread, function
    tsThread thread;                           ///< First thread of this process
    tsTimerEvent tlc;                          ///< Protothread, periodic operations
    uint32_t tgen;                             ///< Protothread, timeout operations
    tProcessEnum enumeration;                  ///< System-wide enumeration of event processor
    uint8_t initialized;                       ///< If this process finished initialization or not
};

/** @brief  Find current process's event handler
 *  @param  _processPtr Target tsProcess pointer
 */
#define PROCESS_STATE_CURRENT(_processPtr) (_processPtr->handlerCurrent)

/** @brief  Change pointed process event handler function for the new state
 *  @param  _processPtr Target tsProcess pointer
 *  @param  _handler    Target event handler for the new state
 */
#define PROCESS_STATE_CHANGE(_processPtr, _handler)                 \
    do                                                              \
    {                                                               \
        _processPtr->handlerPrevious = _processPtr->handlerCurrent; \
        _processPtr->handlerCurrent  = _handler;                    \
    } while (0)

/** @brief  Return to previous state's event handler
 *  @param  _processPtr Target tsProcess pointer
 */
#define PROCESS_STATE_RETURN(_processPtr)                            \
    do                                                               \
    {                                                                \
        PROCESS_HANDLER_PROTO((*temp));                              \
        temp                         = _processPtr->handlerCurrent;  \
        _processPtr->handlerCurrent  = _processPtr->handlerPrevious; \
        _processPtr->handlerPrevious = temp;                         \
    } while (0)

/** @brief  Macro to create a generic process object with _name
 *  @param  _name       Name of process object
 *  @param  _init       Name of initialization function
 *  @param  _deinit     Name of deinitialization function
 *  @param  _enum       System-wide process enumeration of current object
 *  @param  _params     Pointer to process specific parameters structure
 *  @param  _consts     Pointer to process specific constants structure
 */
#define PROCESS_CREATE(_name, _init, _deinit, _enum, _params, _consts)   \
    extern PROCESS_INIT_PROTO(_init);                                    \
    extern PROCESS_DEINIT_PROTO(_deinit);                                \
    tsProcess _name =                                                    \
        {                                                                \
            ._li             = LIST_DL_ITEM_INIT(),                      \
            .init            = _init,                                    \
            .deinit          = _deinit,                                  \
            .handlerCurrent  = NULL,                                     \
            .handlerPrevious = NULL,                                     \
            .parameters      = (_params),                                \
            .constants       = (_consts),                                \
            .timeMax         = 0,                                        \
            .timeMaxEvent    = 0,                                        \
            .evCntLoad       = 0,                                        \
            .evCntMax        = 0,                                        \
            .threadFunction  = NULL,                                     \
            .thread          = THREAD_INIT(),                            \
            .tlc             = TIMER_EVENT_INIT(_enum, _enum, EVENT_PT), \
            .tgen            = 0,                                        \
            .enumeration     = (tProcessEnum)(_enum),                    \
            .initialized     = 0,                                        \
    };

/** @brief      Start an event processor to handle events
 *  @param[in]  process Pointer to target object
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t processStart(tsProcess *process);

/** @brief      Stop an event processor
 *  @param[in]  process Pointer to target object
 *  @retval     EXIT_FAILURE
 *  @retval     EXIT_SUCCESS
 */
INTERFACE uint8_t processStop(tsProcess *process);

/** @brief  Find process object that has a matching enumeration
 *  @param  enumeration Enumeration of target process
 *  @return tsProcess * to target process. NULL = not present
 */
INTERFACE tsProcess *processFind(tProcessEnum enumeration);

/** @} */

/** @addtogroup WDT
 *  @{
 */

/// @brief  Watchdog timer object structure
typedef struct
{
    void (*refresh)(void); ///< Function to refresh watchdog
    void (*start)(void);   ///< Function to start watchdog
    void (*stop)(void);    ///< Funciton to stop watchdog
} tsWdt;

/// @brief  Watchdog timer variables object structure
///         RCoS+ uses this structure internally to decide when to refresh WDT
typedef struct
{
    uint32_t flags;    ///< Flags to check before allowing a refresh
    uint32_t flagMask; ///< Mask for flags that will be checked
} tsWdtVars;

/// @brief  Set a specific flag on WDT refresh
/// @param  _f          Flag index
#define CORE_WDT_SET_FLAG(_f)             \
    do                                    \
    {                                     \
        rcosWdtVars.flags |= 1ul << (_f); \
    } while (0)

/** @brief  Creates rcosWdt object
 *          RCoS+ watchdog control feature runs with this externally created object
 *  @param  _refresh    function to be called to prevent watchdog reset
 *  @param  _start      function to be called to start watchdog
 *  @param  _stop       function to be called to stop watchdog
 *  @param  _flagCount  Number of flags that will be checked to refresh the WDT
 */
#define CORE_WDT_CREATE(_refresh, _start, _stop, _flagCount) \
    const tsWdt rcosWdt   = {(_refresh), (_start), (_stop)}; \
    tsWdtVars rcosWdtVars = {0, ~(0xffffffff << (_flagCount))};

extern const tsWdt rcosWdt;   /// This object must be created in main.c with CORE_WDT_CREATE macro
extern tsWdtVars rcosWdtVars; /// This object must be created in main.c with CORE_WDT_CREATE macro

/** @} */

#if CORE_INTERCOM

/** @addtogroup RCOM
 *  @{
 */

/// @brief  Structure to map a process to the index number of a devCom pointer in rcomDevListArr
typedef struct
{
    tProcessEnum process; ///< Target process enumeration
    uint8_t idx;          ///< Index number of devCom pointer in rcomDevListArr that will be used to reach target process
} tsProcessMap;

/** @brief  Construct a map item to link an external process with a devCom in rcomDevListArr
 *  @param  _process    Target process enumeration
 *  @param  _idx        Index number of devCom pointer defined by RCOM_DEVS
 */
#define RCOM_MAP(_process, _idx) \
    {                            \
        (_process), (_idx)       \
    }

/** @brief  Macro to define devCom device list that will be used by RCOM
 *  @param  ... Each devCom device pointer that will be used in order
 */
#define RCOM_DEVS(...) \
    {                  \
        __VA_ARGS__    \
    }

/// @brief  Structure to hold RCOM information
typedef struct
{
    const tsDevCom **devList; ///< Pointer for an array of devCom pointers
    const tsProcessMap *map;  ///< Process map array
    tsEventPost *incoming;    ///< Pointer for an array of tsEventPost data
    uint8_t devCount;         ///< Number of devCom items
    uint8_t mapCount;         ///< Number of items in map
    tProcessEnum extProcess;  ///< External process enumeration
} tsRcomParams;

/** @brief      Initialize RCoS intercom structures
 *  @warning    Target on communication devices are not implemented yet!
 *  @param      _name       Name of RCOM object
 *  @param      _extProcess Target process enumeration that will handle RCOM events(if present)
 *  @param      _mapEn      The system will use a map and multiple devCom devices to communicate
 *  @param      _rcomDevs   devCom devices that will be used by RCOM, use RCOM_DEVS macro to define all
 *  @param      ...         Process map items each defined by RCOM_MAP macro
 *  @code
 *              RCOS_RCOM_CREATE(rcomOne, eProcessSc, DISABLE)
 *  @endcode
 * 
 *  @code
 *              RCOS_RCOM_CREATE(rcomTwo, PROCESS_NONE, ENABLE,
 *                  RCOM_DEVS(&comUart, &comSpi),
 *                  RCOM_MAP(eProcessExtOne, 0),
 *                  RCOM_MAP(eProcessExtTwo, 0),
 *                  RCOM_MAP(eProcessExtThree, 1))
 *  @endcode
 */
#define RCOS_RCOM_CREATE(_name, _extProcess, _mapEn, _rcomDevs, ...) _RCOS_RCOM_CREATE_SUB(_name, _extProcess, _mapEn, _rcomDevs, __VA_ARGS__)

/// @brief  SUB macro for RCOS_RCOM_CREATE to help Stringification
#define _RCOS_RCOM_CREATE_SUB(_name, _extProcess, _mapEn, _rcomDevs, ...) _RCOS_RCOM_CREATE_SUB_##_mapEn(_name, _extProcess, _rcomDevs, __VA_ARGS__)

/// @brief  SUB macro for RCOS_RCOM_CREATE to disable devCom list and map array connection
#define _RCOS_RCOM_CREATE_SUB_0(_name, _extProcess, _rcomDevs, ...) \
    const tsRcomParams _name =                                      \
        {                                                           \
            NULL,                                                   \
            NULL,                                                   \
            NULL,                                                   \
            0,                                                      \
            0,                                                      \
            _extProcess,                                            \
    };

/// @brief  SUB macro for RCOS_RCOM_CREATE to enable devCom list and map array connection
#define _RCOS_RCOM_CREATE_SUB_1(_name, _extProcess, _rcomDevs, ...)                \
    const tsDevCom const *_name##DevListArr[]                     = _rcomDevs;     \
    const tsProcessMap _name##MapArr[]                            = {__VA_ARGS__}; \
    tsEventPost _name##IncomingArr[ARRAY_SIZE(_name##DevListArr)] = {0};           \
    const tsRcomParams _name =                                                     \
        {                                                                          \
            _name##DevListArr,                                                     \
            _name##MapArr,                                                         \
            _name##IncomingArr,                                                    \
            ARRAY_SIZE(_name##DevListArr),                                         \
            ARRAY_SIZE(_name##MapArr),                                             \
            _extProcess,                                                           \
    };

/** @} */

#endif // #if CORE_INTERCOM
/** @addtogroup CORE
 *  @{
 */

/** @brief  Startup init of all hw and sw
 */
INTERFACE void coreInit(void);

/** @brief      Main loop of RCoS+ infrastructure
 *  @warning    Event calls are processed here in a blocking manner
 */
INTERFACE void coreRun(void);

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_CORE_H
