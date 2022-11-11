/** @file       hpo.h
 *  @brief      Header file of HPO device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       30.01.2020
 */
#ifndef FILE_HPO_H
#define FILE_HPO_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_HPO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   HPO HPO
 *  @ingroup    DEV_IO
 *  @brief      High-powered outputs changes are handled with a separation.
 *  @{
 */

/// @brief  Functions for HPO devices
INTERFACE const tsDevIoFuncs devIoHpoFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevIo *target; ///< Target devIo that will be drived
    uint32_t expected;     ///< Expected value of target
    uint32_t current;      ///< Current value of target
    uint32_t mask;         ///< Last mask used for change
    union {
        struct // TimerCallback solution
        {
            tsTimerCallBack tcb; ///< Timer callback used to call hpioHandler
        };
        struct // Zero Cross solution
        {
            const tsDevCpx *isr;   ///< Zero cross interrupt
            void (*selfIsr)(void); ///< Function that will be connected to ISR
            uint16_t counter;      ///< Zero cross counter
        };
    };
    uint16_t timeout; ///< TCB: Time between each change
    uint16_t zcCnt;   ///< ZC: Count between each change
} tsHpoParams;

/** @brief  Create a devIo for an HPO with timerCallBack solution
 *  @param  _name       Name of devIo object
 *  @param  _target     Target devIo that will be drived
 *  @param  _timeout    Time between each change
 */
#define DEV_IO_HPO_TCB_CREATE(_name, _target, _timeout)             \
    extern TIMER_CALLBACK_FUNC(hpioWrapperTcb);                     \
    const tsDevIo _name;                                            \
    tsHpoParams _name##Params =                                     \
        {                                                           \
            .target  = &_target,                                    \
            .timeout = (_timeout),                                  \
            .zcCnt   = 0,                                           \
            .tcb     = TIMER_CALLBACK_INIT(hpioWrapperTcb, &_name), \
    };                                                              \
    DEV_IO_CREATE(_name, devIoHpoFuncs, &_name##Params, NULL)

INTERFACE void hpioWrapperZc(const tsDevIo *device);

/** @brief  Create a devIo for an HPO with zero cross interrupt
 *  @param  _name   Name of devIo object
 *  @param  _target Target devIo that will be drived
 *  @param  _zcCnt  Zero Cross passed between each change
 *  @param  _zcIsr  Zero Cross interrupt devCpx
 */
#define DEV_IO_HPO_ZC_CREATE(_name, _target, _zcCnt, _zcIsr) \
    const tsDevIo _name;                                     \
    void _name##Isr(void)                                    \
    {                                                        \
        hpioWrapperZc(&_name);                               \
    }                                                        \
    tsHpoParams _name##Params =                              \
        {                                                    \
            .target  = &_target,                             \
            .timeout = 0,                                    \
            .zcCnt   = (_zcCnt),                             \
            .isr     = &_zcIsr,                              \
            .selfIsr = _name##Isr,                           \
    };                                                       \
    DEV_IO_CREATE(_name, devIoHpoFuncs, &_name##Params, NULL)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_HPO_H
