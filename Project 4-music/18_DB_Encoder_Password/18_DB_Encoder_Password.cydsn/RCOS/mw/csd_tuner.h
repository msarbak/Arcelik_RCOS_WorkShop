/** @file       csd_tuner.h
 *  @brief      CYPRESS PSoC Creator Sense Tuner
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       29.04.2019
 */
#ifndef FILE_CSD_TUNER_H
#define FILE_CSD_TUNER_H

/**
 *  @defgroup   CSD_TUNER CSD_TUNER
 *  @ingroup    MW
 *  @brief      CYPRESS PSoC Creator Sense Tuner
 *  @details    Read "PSoC 4 Capacitive Sensing" for details
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/// @brief  Parameters of a CsdTuner process
typedef struct
{
    tsTimerEvent tev;     ///< tsTimerEvent structure used for periodic operations
    const tsDevCom *uart; ///< Target UART communication device
    tsTarget target;      ///< Target communication channel information
    void *dsRam;          ///< CYPRESS component structure pointer: CAPS_RAM_STRUCT *dsRam
    uint8_t dsRamSize;    ///< Total size of dsRam area
    uint16_t period;      ///< Broadcasting period
} tsCsdTunerParams;

typedef enum
{
    eCsdTunerLoop,
} teCsdTunerEvents; ///< Events of a CsdTuner process

/*********************************************************************
 *  EXPORTED MACROS
 */
/** @brief  CsdTuner process object creation macro
 *  @param  _name   Name of process object
 *  @param  _uart   Uart device that will be used to output data
 *  @param  _period Broadcasting period
 *  @param  _cyName CYPRESS PSoC Creator component name
 */
#define PROCESS_CSD_TUNER_CREATE(_name, _uart, _period, _cyName)         \
    tsCsdTunerParams _name##Params =                                     \
        {                                                                \
            TIMER_EVENT_INIT(PROCESS_NONE, PROCESS_NONE, eCsdTunerLoop), \
            &_uart,                                                      \
            {                                                            \
                .config = NULL,                                          \
                .cs     = NULL,                                          \
            },                                                           \
            &_cyName##_dsRam,                                            \
            sizeof(_cyName##_dsRam),                                     \
            _period,                                                     \
    };                                                                   \
    PROCESS_CREATE(_name, csdTunerInit, csdTunerDeinit, PROCESS_NONE, &_name##Params, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_CSD_TUNER_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_CSD_TUNER_H
