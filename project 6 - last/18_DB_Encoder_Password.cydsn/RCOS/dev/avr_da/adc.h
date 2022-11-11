/** @file       adc.h
 *  @brief      Header file of ADC device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @author     Hamed Mohammadi
 *  @date       2/1/2021
 */
#ifndef FILE_ADC_H
#define FILE_ADC_H

/// Includes
#include "rcos.h"
#include <avr/io.h>

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_ADC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   ADC ADC
 *  @ingroup    DEV_CPX
 *  @brief      Analog-Digital Converter device driver for Microchip AVR DA
 *  @{
 */

//// @brief  Functions for ADC_CORE devices
INTERFACE const tsDevCpxFuncs devCpxAdcCoreFuncs;

/// @brief  ADC peripheral configuration
typedef struct
{
    ADC_PRESC_t prescaler;
    ADC_RESSEL_t resolution;
    ADC_SAMPDLY_t sampleDelay;
} tsAdcConfig;

/// @brief  Device specific constant parameters
typedef struct
{
    const tsAdcConfig config;   ///< ADC peripheral configuration
    ADC_t *adc;                 ///< ADC peripheral registry
    const uint8_t *chList;      ///< List of active channels
    const uint16_t refreshTime; ///< Start a new conversion sequence
    const uint8_t chListSize;   ///< Channel list size
} tsAdcCoreConsts;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timer; ///< Callback timer
    uint8_t index;         ///< Index of current channel processed
    uint16_t *chValues;    ///< Last conversion values for active channels
} tsAdcCoreParams;

/** @brief  Create a devCpx
 *  @param  _name           Name of devCpx object
 *  @param  _adcDev         ADC_t pointer for target peripheral
 *  @param  _refreshTime    Timeout duration for values to be refreshed
 *  @param  _resolution     Resolution of ADC in bits(10 or 12)
 *  @param  _prescaler      Clock prescaler of ADC module (2, 4, 8, 12, 16, 20, 24, 28, 32, 48, 64, 96, 128, 256)
 *  @param  _sampleDelay    Delay, in ADC clock cycles, to wait before sampling (0 - 15)
 *  @param  ...             Channels that are going to be converted in order
 */
#define DEV_CPX_ADC_CORE_CREATE(_name, _adcDev, _refreshTime, _resolution, \
                                _prescaler, _sampleDelay, ...)             \
    const tsDevCpx _name;                                                  \
    ISR(_adcDev##_RESRDY_vect)                                             \
    {                                                                      \
        adcCoreIrqHandler(&_name);                                         \
        _adcDev.INTFLAGS = ADC_RESRDY_bm;                                  \
    }                                                                      \
    const uint8_t _name##ChList[] =                                        \
        {                                                                  \
            __VA_ARGS__,                                                   \
    };                                                                     \
    uint16_t _name##ChValues[ARRAY_SIZE(_name##ChList)];                   \
    const tsAdcCoreConsts _name##Consts =                                  \
        {                                                                  \
            {                                                              \
                /*config*/                                                 \
                ADC_PRESC_DIV##_prescaler##_gc,                            \
                ADC_RESSEL_##_resolution##BIT_gc,                          \
                ADC_SAMPDLY_DLY##_sampleDelay##_gc,                        \
            },                                                             \
            &_adcDev,                                                      \
            _name##ChList,                                                 \
            _refreshTime,                                                  \
            ARRAY_SIZE(_name##ChList),                                     \
    };                                                                     \
    extern TIMER_CALLBACK_FUNC(adcCoreRefresh);                            \
    tsAdcCoreParams _name##Params =                                        \
        {                                                                  \
            TIMER_CALLBACK_INIT(adcCoreRefresh, &_name),                   \
            0,                                                             \
            _name##ChValues,                                               \
    };                                                                     \
    DEV_CPX_CREATE(_name, devCpxAdcCoreFuncs, &_name##Params, &_name##Consts)

/// @brief  Function to handle interrupt requests of an ADC_CORE device. DO NOT CALL OUTSIDE THE TARGET ISR
INTERFACE void adcCoreIrqHandler(const tsDevCpx *device);

/** @} */

/// @brief  Functions for ADC devices
INTERFACE const tsDevIoFuncs devIoAdcFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint8_t index; ///< Index for the list of enabled channels in target ADC_CORE device
} tsAdcParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevCpx *adcCore;   ///< Target ADC_CORE device
    volatile uint8_t *pinCtrl; ///< Pointer to pin control register
} tsAdcConsts;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _adcCore    Target ADC_CORE
 *  @param  _idx        Index for the list of enabled channels in target ADC_CORE device
 *  @param  _port       Port name of the selected channel (A, B, C, D, E, F)
 *  @param  _pin        Pin number of the selected channel
 */
#define DEV_IO_ADC_CREATE(_name, _adcCore, _idx, _port, _pin) \
    const tsAdcParams _name##Params =                         \
        {                                                     \
            (_idx),                                           \
    };                                                        \
    const tsAdcConsts _name##Consts =                         \
        {                                                     \
            &(_adcCore),                                      \
            &PORT##_port##_PIN##_pin##CTRL,                   \
    };                                                        \
    DEV_IO_CREATE(_name, devIoAdcFuncs, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_ADC_H
