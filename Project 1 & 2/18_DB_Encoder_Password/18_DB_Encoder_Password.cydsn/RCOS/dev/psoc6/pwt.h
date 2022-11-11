/** @file       psoc6/pwt.h
 *  @brief      Header file of PWT device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       15/04/2020
 */
#ifndef FILE_PWT_H
#define FILE_PWT_H

/// Includes
#include "rcos.h"
#include "dev/iolink.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_PWT_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   PSOC6_PWT PWT
 *  @ingroup    PSOC6_DEV_CPX
 *  @brief      Pulse Width Timer is a capture device for PWM signals
 *  @details    HOW TO USE:
 *              1) Place a timer component in your CYPRESS design.
 *              2) Configure timer;
 *                  - Enable capture for either edge
 *                  - Enable reload for either rising or falling edge(which will be the starting edge of your pulse)
 *                  - Enable interrupt source for capture
 *              3) Place a clock component as input to timer which will act as the resolution for calculations
 *              4) Place a digital input pin component and connect to both capture and reload inputs of timer
 *              5) Place an interrupt component to get capture data
 *  @{
 */

/// @brief  Functions for PWT devices
INTERFACE const tsDevCpxFuncs devCpxPwtFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t pulse; ///< Counter value of pulse(first part of signal after reload)
    uint32_t duty;  ///< Captured duty cycle
    uint32_t freq;  ///< Captured frequency
} tsPwtParams;

/// @brief  Device specific constants
typedef struct
{
    TCPWM_Type *cntrHw;                              ///< Counter hardware base address
    uint32_t cntrNum;                                ///< Counter index number
    const cy_stc_tcpwm_counter_config_t *cntrConfig; ///< Counter configuration
    const cy_stc_sysint_t *isrCfg;                   ///< ISR configuration structure
    void (*isrFunction)(void);                       ///< ISR function for this device
    uint32_t clock;                                  ///< Clock frequency of timer component in Hz
    uint32_t dutyMax;                                ///< Max value of duty cycle
} tsPwtConsts;

/** @brief      Create a devCpx
 *  @details    To increase frequency resolution _clockHz parameter should be increased
 *              without increasing the real hardware clock frequency.
 *  @param      _name       Name of devCpx object
 *  @param      _cyTimer    Timer component name
 *  @param      _cyIsr      Interrupt component name
 *  @param      _clockHz    Clock frequency of timer component in Hz
 *  @param      _dutyMax    Max value of duty value
 */
#define DEV_CPX_PWT_CREATE(_name, _cyTimer, _cyIsr, _clockHz, _dutyMax) \
    const tsDevCpx _name;                                               \
    extern void pwtIsr(const tsDevCpx *device);                         \
    void _name##Isr(void)                                               \
    {                                                                   \
        pwtIsr(&_name);                                                 \
    }                                                                   \
    tsPwtParams _name##Params =                                         \
        {                                                               \
            .pulse = 0,                                                 \
            .duty  = 0,                                                 \
            .freq  = 0,                                                 \
    };                                                                  \
    const tsPwtConsts _name##Consts =                                   \
        {                                                               \
            .cntrHw      = _cyTimer##_HW,                               \
            .cntrNum     = _cyTimer##_CNT_NUM,                          \
            .cntrConfig  = &_cyTimer##_config,                          \
            .isrCfg      = &_cyIsr##_cfg,                               \
            .isrFunction = _name##Isr,                                  \
            .clock       = (_clockHz),                                  \
            .dutyMax     = (_dutyMax),                                  \
    };                                                                  \
    DEV_CPX_CREATE(_name, devCpxPwtFuncs, &_name##Params, &_name##Consts)

/// @brief  Create a devIo device to get captured frequency value from a PWT devCpx
/// @param  _name   Name of devIo object
/// @param  _pwtCpx Name of PWT devCpx object
#define DEV_IO_PWT_FREQ_CREATE(_name, _pwtCpx) \
    DEV_IO_IOLINK_CREATE(_name, &_pwtCpx##Params.freq, sizeof(_pwtCpx##Params.freq))

/// @brief  Create a devIo device to get captured duty cycle value from a PWT devCpx
/// @param  _name   Name of devIo object
/// @param  _pwtCpx Name of PWT devCpx object
#define DEV_IO_PWT_DUTY_CREATE(_name, _pwtCpx) \
    DEV_IO_IOLINK_CREATE(_name, &_pwtCpx##Params.duty, sizeof(_pwtCpx##Params.duty))

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_PWT_H
