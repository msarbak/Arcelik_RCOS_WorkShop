/** @file       led.h
 *  @brief      LED driver with on/off/blink options
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     
 *  @date       24.04.2019
 */
#ifndef FILE_LED_H
#define FILE_LED_H

#include "rcos.h"
#include "dev/pattern.h" // WARNING! dependency on pattern device
#include "dev/fade.h"    // WARNING! dependency on fade device

/**
 *  @defgroup   LED LED
 *  @ingroup    DEV_IO
 *  @brief      LED driver with on/off/blink options
 *  @{
 */

#ifndef FILE_LED_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for LED devices
INTERFACE const tsDevIoFuncs devIoLedFuncs;

typedef enum
{
    eLdtFullOff,
    eLdtFullOn,
    eLdtFullOnOff,
    eLdtFadeInOut,
    eLdtFastOnOff,
    eLdtSlowOnOff,
    eLdtHalfOnOff,
    eLdtSoftOn,
    eLdtSoftOnOff,
} teLedDriveType;

//#define LED_FULL_ON_TIME                65535
//#define LED_FULL_OFF_TIME               65535
#define LED_FULL_ON_OFF_ON_TIME 750
#define LED_FULL_ON_OFF_OFF_TIME 750

#define LED_SOFT_ON_OFF_ON_TIME 750
#define LED_SOFT_ON_OFF_OFF_TIME 750

#define LED_HALF_ON_OFF_ON_TIME 10
#define LED_HALF_ON_OFF_OFF_TIME 10

#define LED_FAST_ON_OFF_LONG_ON_TIME 1040
#define LED_FAST_ON_OFF_SHORT_ON_TIME 210
#define LED_FAST_ON_OFF_OFF_TIME 160

#define LED_SLOW_ON_OFF_ON_TIME 1250
#define LED_SLOW_ON_OFF_OFF_TIME 1250

#define LED_FADE_IN_TIME 770
//#define LED_FADE_OUT_TIME               770
#define LED_DUTY_CHANGE_STEP_COUNT 7 // seven steps

#define LED_FADE_IN_CHECK_PERIOD (LED_FADE_IN_TIME / LED_DUTY_CHANGE_STEP_COUNT) // %100/STEP_COUNT of the FadeIn time
//#define LED_FADE_OUT_CHECK_PERIOD       (LED_FADE_OUT_TIME / LED_DUTY_CHANGE_STEP_COUNT)    // %100/STEP_COUNT of the FadOut time
#define LED_FADE_IN_WAIT_REGION (LED_FADE_IN_TIME - LED_FADE_IN_CHECK_PERIOD) // first step of the whole period -
//#define LED_FADE_OUT_WAIT_REGION        (LED_FADE_IN_TIME - LED_FADE_IN_CHECK_PERIOD)       // first of the whole period

#define LED_DUTY_INC_PERCENT 1
//#define LED_DUTY_DEC_PERCENT            1
#define LED_SOFT_DUTY 2
#define LED_MIN_DUTY 0
#define LED_FULL_ON_DUTY (LED_DUTY_CHANGE_STEP_COUNT * LED_DUTY_INC_PERCENT) // %100 full duty - led on case
#define LED_FULL_OFF_DUTY 0

/// @brief  Device specific parameters
typedef struct
{
    teLedDriveType state;          ///< Current state of LED
    tsPatternNode blinkPattern[3]; ///< Custom pattern to be used
} tsLedParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *io;      ///< Target io that controls LED
    const tsDevIo *pattern; ///< Pattern generator for target io
    const tsDevIo *fade;    ///< Fade function for target io
} tsLedConsts;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 *  @param  _devIo  Target devIo object that will be used as an LED
 */
#define DEV_IO_LED_CREATE(_name, _devIo)                              \
    DEV_IO_PATTERN_CREATE(_name##Pattern, _devIo, LED_FULL_OFF_DUTY)  \
    DEV_IO_FADE_CREATE(_name##Fade,                                   \
                       _devIo,                                        \
                       LED_DUTY_INC_PERCENT,                          \
                       LED_FADE_IN_TIME / LED_DUTY_CHANGE_STEP_COUNT) \
    tsLedParams _name##Params =                                       \
        {                                                             \
            eLdtFullOff,                                              \
            {                                                         \
                PATTERN_NODE(LED_FULL_ON_DUTY, 0),  /*on*/            \
                PATTERN_NODE(LED_FULL_OFF_DUTY, 0), /*off*/           \
                PATTERN_NODE_REPEAT(),              /*repeat*/        \
            },                                                        \
    };                                                                \
    const tsLedConsts _name##Consts =                                 \
        {                                                             \
            &_devIo,                                                  \
            &_name##Pattern,                                          \
            &_name##Fade,                                             \
    };                                                                \
    DEV_IO_CREATE(_name, devIoLedFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_LED_H
