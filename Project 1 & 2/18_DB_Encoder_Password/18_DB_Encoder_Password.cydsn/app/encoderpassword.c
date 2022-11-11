/** @file       encoderpassword.c
 *  @brief      ENCODERPASSWORD application program file
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ozkan AKGUL
 *  @date       2.06.2021
 */
#define FILE_ENCODERPASSWORD_C

#include "encoderpassword.h"
#include "dev/pattern.h"

#define DEBUG_FILE_NAME ""

// #define DEBUG_PORT_
// DEBUG_START(0)

/**
 *  @addtogroup ENCODERPASSWORD
 *  @{
 */
int i = 0;
tsPatternNode locked[] =
{
    PATTERN_NODE(NOTE_7_SOL, 2000),
    PATTERN_NODE_END(0)
};


const tsPatternNode unLocked[] =
{
    PATTERN_NODE(NOTE_7_SOL, 300),
    PATTERN_NODE(0, 50),
    PATTERN_NODE(NOTE_7_SOL, 300),
    PATTERN_NODE_END(0)
};

const tsPatternNode buttonPress[] =
{
    PATTERN_NODE(NOTE_7_SOL, 150),
    PATTERN_NODE(0, 50),
    PATTERN_NODE(NOTE_7_SOL, 100),
    PATTERN_NODE_END(0)
};


static PT_THREAD(encoderPasswordThread)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;
    
    UNUSED(consts);
    UNUSED(params);

    PT_BEGIN();

    for (;;)
    {
        PT_YIELD();
    }

    PT_END();
}

static PROCESS_HANDLER_PROTO(encoderPasswordInitialStateHandler);
static PROCESS_HANDLER_PROTO(encoderPasswordLockingStateHandler);
static PROCESS_HANDLER_PROTO(encoderPasswordUnLockingStateHandler);

/// @brief  Initialization function of encoderPassword
PROCESS_INIT_PROTO(encoderPasswordInit)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    PROCESS_STATE_CHANGE(process, encoderPasswordInitialStateHandler);
    eventPost(process->enumeration, eEPEventUIUpdate, NULL, 0);

    devIoInit(consts->redLed, NULL);
    devIoInit(consts->blueLed, NULL);
    devIoInit(consts->yellowLed, NULL);
    devIoInit(consts->sevenSegmentDisplay, NULL);
    devIoInit(consts->encoder, NULL);

    params->timerUIGeneral.post.destination = process->enumeration;
    params->timerUIGeneral.post.source = process->enumeration;

    threadStart(process, encoderPasswordThread);
    process->initialized = 1; // If process needs other checks, set this another time
}

/// @brief  Deinitialization function of encoderPassword
PROCESS_DEINIT_PROTO(encoderPasswordDeinit)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    PROCESS_STATE_CHANGE(process, NULL);
    threadStop(process, process->threadFunction);
    process->initialized = 0; // If process needs other checks, clear this another time
}

/// @brief  Event handler function of encoderPassword Initial state
static PROCESS_HANDLER_PROTO(encoderPasswordInitialStateHandler)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    switch (eventCurrent.event)
    {
        case eEPEventUIUpdate: 
        {
            devIoPut(consts->buzzer, (uint32_t)buttonPress);

            devIoPut(consts->redLed, 0);
            devIoPut(consts->blueLed, 0);
            devIoPut(consts->yellowLed, 0);

            params->timerUIGeneral.post.event = eEPEventLedsBlink;
            timerEventStart(&(params->timerUIGeneral), 100);
        }
        break;

        case eEPEventLedsBlink:
        {
            //devIoPut(consts->redLed, !devIoGet(consts->redLed));
            devIoPut(consts->redLed, 0);
            devIoPut(consts->blueLed, 0);
            devIoPut(consts->yellowLed, 0);

            
          
            
                //devIoPut(consts->redLed, 0);
                //devIoPut(consts->blueLed, 0);
                //devIoPut(consts->yellowLed, 0);
                
                //i = i%3;
                if(i == 0)  {
                    devIoPut(consts->redLed, !devIoGet(consts->redLed));
                    i++;
                }
                else if(i == 1) {
                    devIoPut(consts->blueLed, !devIoGet(consts->blueLed));
                    i++;
                }
                else if (i == 2){
                    devIoPut(consts->yellowLed, !devIoGet(consts->yellowLed));
                    i = 0;
                }
            
            
            timerEventStart(&(params->timerUIGeneral),100);
            
            
        }
        break;

        case p46Released:
        case p70Released:
        
        {
             devIoPut(consts->redLed, 0);
            devIoPut(consts->blueLed, 0);
            devIoPut(consts->yellowLed, 1);

            timerEventStop(&(params->timerUIGeneral));

        }
            break;
        
        case p71Released:
        {
            devIoPut(consts->buzzer, (uint32_t)buttonPress);
            timerEventStop(&(params->timerUIGeneral));
            
            PROCESS_STATE_CHANGE(process, encoderPasswordLockingStateHandler);

            params->lockingState = 0;       ///< clear locking state
            params->passwordIndex = 0;      ///< clear password index
            params->password = 0;           ///< clear password
            eventPost(process->enumeration, eEPEventUIUpdate, NULL, 0);
        }
        break;
        default:
            break;
    }
}


/// @brief  Event handler function of encoderPassword locking state
static PROCESS_HANDLER_PROTO(encoderPasswordLockingStateHandler)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    switch (eventCurrent.event)
    {
        case eEPEventUIUpdate: 
        {
            devIoPut(consts->blueLed, 0);
            devIoPut(consts->yellowLed, 1);

            params->timerUIGeneral.post.event = eEPEventLedsBlink;
            timerEventStart(&(params->timerUIGeneral), 100);
        }
        break;

        case eEPEventLedsBlink:
        {
            devIoPut(consts->redLed, !devIoGet(consts->redLed));
            
            devIoPut(consts->sevenSegmentDisplay, devIoGet(consts->encoder));  ///< show encoder

            timerEventStart(&(params->timerUIGeneral), 500);
        }
        break;

        case p46Released:
        {
            uint8_t readValue = devIoGet(consts->encoder);

            params->password |= (readValue << (4*params->passwordIndex)); 

            params->passwordIndex++; 
            if (params->passwordIndex >= 4)
            {
                params->lockingState = 1;
                ///< change state

                devIoPut(consts->buzzer, (uint32_t)locked);

                timerEventStop(&(params->timerUIGeneral));
            
                PROCESS_STATE_CHANGE(process, encoderPasswordUnLockingStateHandler);

                params->passwordIndex = 0;      ///< clear password index
                params->unlockValue = 0;        ///< clear unlock value

                eventPost(process->enumeration, eEPEventUIUpdate, NULL, 0);

            }
            else
            {
                devIoPut(consts->buzzer, (uint32_t)buttonPress);
            }
        }
        break;

        case p71Released:
        {
            devIoPut(consts->buzzer, (uint32_t)buttonPress);

            params->passwordIndex = 0;
            params->password = 0;
        }
        break;
        default:
            break;
    }
}


/// @brief  Event handler function of encoderPassword unlocking state
static PROCESS_HANDLER_PROTO(encoderPasswordUnLockingStateHandler)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    switch (eventCurrent.event)
    {
        case eEPEventUIUpdate: 
        {
            devIoPut(consts->redLed, 0);
            devIoPut(consts->yellowLed, 1);

            params->timerUIGeneral.post.event = eEPEventLedsBlink;
            timerEventStart(&(params->timerUIGeneral), 100);
        }
        break;

        case eEPEventLedsBlink:
        {
           
            
            
            devIoPut(consts->blueLed, !devIoGet(consts->blueLed));
            
            devIoPut(consts->sevenSegmentDisplay, devIoGet(consts->encoder));  ///< show encoder
            
            timerEventStart(&(params->timerUIGeneral), 500);
        }
        break;

        case p70Released:
        {
            uint8_t readValue = devIoGet(consts->encoder);

            params->unlockValue |= (readValue << (4*params->passwordIndex)); 

            params->passwordIndex++; 
            if (params->passwordIndex >= 4)
            {
                if (params->unlockValue == params->password)
                {
                    params->lockingState = 0;

                    timerEventStop(&(params->timerUIGeneral));
            
                    PROCESS_STATE_CHANGE(process, encoderPasswordInitialStateHandler);

                    params->passwordIndex = 0;      ///< clear password index

                    eventPost(process->enumeration, eEPEventUIUpdate, NULL, 0);
                }
                else
                {
                    params->unlockValue = 0;
                    params->passwordIndex = 0;

                    ///< change state

                    devIoPut(consts->buzzer, (uint32_t)locked);
                }
            }
            else
            {
                devIoPut(consts->buzzer, (uint32_t)buttonPress);
            }
        }
        break;

        case p71Released:
        {
            devIoPut(consts->buzzer, (uint32_t)buttonPress);

            params->passwordIndex = 0;
            params->unlockValue = 0;
        }
        break;
        default:
            break;
    }
}
/** @} */
