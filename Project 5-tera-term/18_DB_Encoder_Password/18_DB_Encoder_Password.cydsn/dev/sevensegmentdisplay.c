/** @file       sevenSegmentdisplay.c
 *  @brief      C file of SEVENSegmentDISPLAY device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ozkan AKGUL
 *  @date       1.06.2021
 */
#define FILE_SEVENSegmentDISPLAY_C

#include "sevenSegmentdisplay.h"

/**
 *  @addtogroup SEVENSEGMENTDISPLAY
 *  @{
 */
#define SEVSEG_NULL SEVSEG(0,0,0,0,0,0,0)

const tsTwoSevenSegment displayMessages[] = 
{
    ///     Segment1                    ,               ///     Segment2
    {       SEVSEG_NULL                 ,               SEVSEG_0        },
    {       SEVSEG_NULL                 ,               SEVSEG_1        },
    {       SEVSEG_NULL                 ,               SEVSEG_2        },
    {       SEVSEG_NULL                 ,               SEVSEG_3        },
    {       SEVSEG_NULL                 ,               SEVSEG_4        },
    {       SEVSEG_NULL                 ,               SEVSEG_5        },
    {       SEVSEG_NULL                 ,               SEVSEG_6        },
    {       SEVSEG_NULL                 ,               SEVSEG_7        },
    {       SEVSEG_NULL                 ,               SEVSEG_8        },
    {       SEVSEG_NULL                 ,               SEVSEG_9        },
    {       SEVSEG_1                    ,               SEVSEG_0        },
    {       SEVSEG_1                    ,               SEVSEG_1        },
    {       SEVSEG_1                    ,               SEVSEG_2        },
    {       SEVSEG_1                    ,               SEVSEG_3        },
    {       SEVSEG_1                    ,               SEVSEG_4        },
    {       SEVSEG_1                    ,               SEVSEG_5        },
    {       SEVSEG_1                    ,               SEVSEG_6        },
    {       SEVSEG_1                    ,               SEVSEG_7        },
    {       SEVSEG_1                    ,               SEVSEG_8        },
    {       SEVSEG_1                    ,               SEVSEG_9        },
    {       SEVSEG_NULL                 ,               SEVSEG_A        },
    {       SEVSEG_NULL                 ,               SEVSEG_B        },
    {       SEVSEG_NULL                 ,               SEVSEG_C        },
    {       SEVSEG_NULL                 ,               SEVSEG_Crh      },
    {       SEVSEG_NULL                 ,               SEVSEG_Cs       },
    {       SEVSEG_NULL                 ,               SEVSEG_Csrh     },
    {       SEVSEG_NULL                 ,               SEVSEG_D        },
    {       SEVSEG_NULL                 ,               SEVSEG_E        },
    {       SEVSEG_NULL                 ,               SEVSEG_F        },
    {       SEVSEG_NULL                 ,               SEVSEG_G        },
    {       SEVSEG_NULL                 ,               SEVSEG_Gs       },
    {       SEVSEG_NULL                 ,               SEVSEG_H        },
    {       SEVSEG_NULL                 ,               SEVSEG_Hs       },
    {       SEVSEG_NULL                 ,               SEVSEG_Hsrh     },
    {       SEVSEG_NULL                 ,               SEVSEG_I        },
    {       SEVSEG_NULL                 ,               SEVSEG_Is       },
    {       SEVSEG_NULL                 ,               SEVSEG_Isrh     },
    {       SEVSEG_NULL                 ,               SEVSEG_J        },
    {       SEVSEG_NULL                 ,               SEVSEG_K        },
    {       SEVSEG_NULL                 ,               SEVSEG_L        },
    {       SEVSEG_NULL                 ,               SEVSEG_Lrh      },
    {       SEVSEG_NULL                 ,               SEVSEG_M        },
    {       SEVSEG_NULL                 ,               SEVSEG_n        },
    {       SEVSEG_NULL                 ,               SEVSEG_O        },
    {       SEVSEG_NULL                 ,               SEVSEG_Os       },
    {       SEVSEG_NULL                 ,               SEVSEG_P        },
    {       SEVSEG_NULL                 ,               SEVSEG_Q        },
    {       SEVSEG_NULL                 ,               SEVSEG_R        },
    {       SEVSEG_NULL                 ,               SEVSEG_S        },
    {       SEVSEG_NULL                 ,               SEVSEG_T        },
    {       SEVSEG_NULL                 ,               SEVSEG_U        },
    {       SEVSEG_NULL                 ,               SEVSEG_Urv      },
    {       SEVSEG_NULL                 ,               SEVSEG_Us       },
    {       SEVSEG_NULL                 ,               SEVSEG_Usrv     },
    {       SEVSEG_NULL                 ,               SEVSEG_V        },
    {       SEVSEG_NULL                 ,               SEVSEG_W        },
    {       SEVSEG_NULL                 ,               SEVSEG_X        },
    {       SEVSEG_NULL                 ,               SEVSEG_Y        },
    {       SEVSEG_NULL                 ,               SEVSEG_Z        },
    {       SEVSEG_1                    ,               SEVSEG_A        },
    {       SEVSEG_1                    ,               SEVSEG_B        },
    {       SEVSEG_1                    ,               SEVSEG_C        },
    {       SEVSEG_1                    ,               SEVSEG_Crh      },
    {       SEVSEG_1                    ,               SEVSEG_Cs       },
    {       SEVSEG_1                    ,               SEVSEG_Csrh     },
    {       SEVSEG_1                    ,               SEVSEG_D        },
    {       SEVSEG_1                    ,               SEVSEG_E        },
    {       SEVSEG_1                    ,               SEVSEG_F        },
    {       SEVSEG_1                    ,               SEVSEG_G        },
    {       SEVSEG_1                    ,               SEVSEG_Gs       },
    {       SEVSEG_1                    ,               SEVSEG_H        },
    {       SEVSEG_1                    ,               SEVSEG_Hs       },
    {       SEVSEG_1                    ,               SEVSEG_Hsrh     },
    {       SEVSEG_1                    ,               SEVSEG_I        },
    {       SEVSEG_1                    ,               SEVSEG_Is       },
    {       SEVSEG_1                    ,               SEVSEG_Isrh     },
    {       SEVSEG_1                    ,               SEVSEG_J        },
    {       SEVSEG_1                    ,               SEVSEG_K        },
    {       SEVSEG_1                    ,               SEVSEG_L        },
    {       SEVSEG_1                    ,               SEVSEG_Lrh      },
    {       SEVSEG_1                    ,               SEVSEG_M        },
    {       SEVSEG_1                    ,               SEVSEG_n        },
    {       SEVSEG_1                    ,               SEVSEG_O        },
    {       SEVSEG_1                    ,               SEVSEG_Os       },
    {       SEVSEG_1                    ,               SEVSEG_P        },
    {       SEVSEG_1                    ,               SEVSEG_Q        },
    {       SEVSEG_1                    ,               SEVSEG_R        },
    {       SEVSEG_1                    ,               SEVSEG_S        },
    {       SEVSEG_1                    ,               SEVSEG_T        },
    {       SEVSEG_1                    ,               SEVSEG_U        },
    {       SEVSEG_1                    ,               SEVSEG_Urv      },
    {       SEVSEG_1                    ,               SEVSEG_Us       },
    {       SEVSEG_1                    ,               SEVSEG_Usrv     },
    {       SEVSEG_1                    ,               SEVSEG_V        },
    {       SEVSEG_1                    ,               SEVSEG_W        },
    {       SEVSEG_1                    ,               SEVSEG_X        },
    {       SEVSEG_1                    ,               SEVSEG_Y        },
    {       SEVSEG_1                    ,               SEVSEG_Z        },
    {       SEVSEG_NULL                 ,               SEVSEG_NULL     }, ///< End message - Null
};

static DEV_IO_FUNC_INIT(sevenSegmentDisplayInit);
static DEV_IO_FUNC_DEINIT(sevenSegmentDisplayDeinit);
static DEV_IO_FUNC_GET(sevenSegmentDisplayGet);
static DEV_IO_FUNC_PUT(sevenSegmentDisplayPut);

static PT_THREAD(sevenSegmentDisplayThread)
{
    tsSevenSegmentDisplayParams *params       = process->parameters;
    const tsSevenSegmentDisplayConsts *consts = process->constants;
    
    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED
    
    PT_BEGIN();

    for (;;)
    {
        PT_YIELD();
    }

    PT_END();
}

static PROCESS_HANDLER_PROTO(sevenSegmentDisplayHandler);

/// @brief  Initialization function of sevenSegmentDisplay
PROCESS_INIT_PROTO(sevenSegmentDisplayProcessInit)
{
    tsSevenSegmentDisplayParams *params       = process->parameters;
    const tsSevenSegmentDisplayConsts *consts = process->constants;

    UNUSED(consts); // REMOVE IF USED

    params->timerSSDrive.post.source = process->enumeration;
    params->timerSSDrive.post.destination = process->enumeration;

    timerEventStart(&(params->timerSSDrive), 5);

    PROCESS_STATE_CHANGE(process, sevenSegmentDisplayHandler);
    threadStart(process, sevenSegmentDisplayThread);
    process->initialized = 1; // If process needs other checks, set this another time
}

/// @brief  Deinitialization function of sevenSegmentDisplay
PROCESS_DEINIT_PROTO(sevenSegmentDisplayProcessDeinit)
{
    tsSevenSegmentDisplayParams *params       = process->parameters;
    const tsSevenSegmentDisplayConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    PROCESS_STATE_CHANGE(process, NULL);
    threadStop(process, process->threadFunction);
    process->initialized = 0; // If process needs other checks, clear this another time
}

/// @brief  Event handler function of sevenSegmentDisplay
static PROCESS_HANDLER_PROTO(sevenSegmentDisplayHandler)
{
    tsSevenSegmentDisplayParams *params       = process->parameters;
    const tsSevenSegmentDisplayConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    switch (eventCurrent.event)
    {
        case EVENT_NONE: // FILL WITH NEW EVENTS
            break;

        case eSevenSegmentDriveEvent:
        {
            devIoPut(consts->digit1, 0);  ///< deactivate digit1
            devIoPut(consts->digit2, 0);  ///< deactivate digit2

            if (params->driveIndex)
            {
                params->driveIndex = 0;         ///< for first digit
                
                devIoPut(consts->sevenSegment,params->sevenSegment.digit1);

                devIoPut(consts->digit1, 1);  ///< activate digit1
            }
            else
            {
                params->driveIndex = 1;         ///< for second digit

                devIoPut(consts->sevenSegment,params->sevenSegment.digit2);

                devIoPut(consts->digit2, 1);  ///< activate digit2
            }
            
            timerEventStart(&(params->timerSSDrive), 5);
        }
        break;

        default:
            break;
    }
}

/// @brief  Structure that defines the functions for sevenSegmentDisplay
const tsDevIoFuncs devIoSevenSegmentDisplayFuncs =
{
    sevenSegmentDisplayInit,
    sevenSegmentDisplayDeinit,
    sevenSegmentDisplayGet,
    sevenSegmentDisplayPut,
};

/// @brief  Init function for a sevenSegmentDisplay
static DEV_IO_FUNC_INIT(sevenSegmentDisplayInit)
{
    tsSevenSegmentDisplayParams *params       = device->parameters;
    const tsSevenSegmentDisplayConsts *consts = device->constants;
    
    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    if (config)
    {
        const tsSevenSegmentDisplayConfig *cfg = config;

        UNUSED(cfg);    // REMOVE IF USED
    }

    processStart(params->process);

    device->sys->initialized = 1;

    return EXIT_SUCCESS;
}

/// @brief  Deinit function for a sevenSegmentDisplay
static DEV_IO_FUNC_DEINIT(sevenSegmentDisplayDeinit)
{
    tsSevenSegmentDisplayParams *params       = device->parameters;
    const tsSevenSegmentDisplayConsts *consts = device->constants;

    device->sys->initialized = 0;
    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    return EXIT_SUCCESS;
}

/// @brief  Get function for a sevenSegmentDisplay
static DEV_IO_FUNC_GET(sevenSegmentDisplayGet)
{
    tsSevenSegmentDisplayParams *params       = device->parameters;
    const tsSevenSegmentDisplayConsts *consts = device->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    return 0;
}

/// @brief  Put function for a sevenSegmentDisplay
static DEV_IO_FUNC_PUT(sevenSegmentDisplayPut)
{
    tsSevenSegmentDisplayParams *params       = device->parameters;
    const tsSevenSegmentDisplayConsts *consts = device->constants;

    UNUSED(consts); // REMOVE IF USED

    if (data < eDisplayMessagesEnd)
    {
        params->sevenSegment.digit1 = displayMessages[data].digit1;
        params->sevenSegment.digit2 = displayMessages[data].digit2;
    }
    else
    {
        return EXIT_FAILURE;
    }
    

    return EXIT_SUCCESS;
}

/** @} */
