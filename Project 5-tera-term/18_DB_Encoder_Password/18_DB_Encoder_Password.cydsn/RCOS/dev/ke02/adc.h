/** @file       ke02/adc.h
 *  @brief      Analog-Digital Converter device driver for NXP KE02
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Koray Kazım Menevşe
 *  @date       26.12.2013
 *  @author     Deniz Fer
 *  @date       7.1.2019
 */
#ifndef FILE_ADC_H
#define FILE_ADC_H

#include "rcos.h"
#include "fsl_adc.h"

/**
 *  @defgroup   KE02_ADC ADC
 *  @ingroup    KE02_DEV_IO
 *  @brief      NXP KE02 Analog-Digital Converter device driver
 *  @{
 */

#ifndef FILE_ADC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   KE02_ADC_CORE ADC_CORE
 *  @brief      Peripheral core access
 *  @{
 */
/// @brief  Functions for ADC_CORE devices
INTERFACE const tsDevCpxFuncs devCpxAdcCoreFuncs;

/// @brief  Device specific constant parameters
typedef struct
{
    adc_config_t config;   ///< ADC peripheral configuration
    ADC_Type *adc;         ///< ADC peripheral registry
    const uint8_t *chList; ///< List of active channels
    uint32_t *chValues;    ///< Last conversion values for active channels
    uint16_t refreshTime;  ///< Start a new conversion sequence
    uint8_t chListSize;    ///< Channel list size
} tsAdcCoreConstParams;

/// @brief  Device specific parameters
typedef struct
{
    const tsAdcCoreConstParams *constants;
    tsTimerCallBack timer; ///< Callback timer
    uint8_t index;         ///< Index of current channel processed
} tsAdcCoreParams;

/** @brief  Create a devCpx
 *  @param  _name           Name of devCpx object
 *  @param  _nxpDev         ADC_Type pointer for target peripheral
 *  @param  _refreshTime    Timeout duration for values to be refreshed
 *  @param  _resolution     Resolution of ADC in bits(8, 10 or 12)
 *  @param  ...             Channels that are going to be converted in order
 */
#define DEV_CPX_ADC_CORE_CREATE(_name, _nxpDev, _refreshTime, _resolution, ...)   \
    extern TIMER_CALLBACK_FUNC(adcCoreRefresh);                                   \
    const tsDevCpx _name;                                                         \
    void _nxpDev##_IRQHandler(void)                                               \
    {                                                                             \
        adcCoreIrqHandler(&_name);                                                \
    }                                                                             \
    const uint8_t _name##ChList[] =                                               \
        {                                                                         \
            __VA_ARGS__,                                                          \
    };                                                                            \
    uint32_t _name##ChValues[ARRAY_SIZE(_name##ChList)];                          \
    const tsAdcCoreConstParams _name##ConstParams =                               \
        {                                                                         \
            {                                                                     \
                /*config*/                                                        \
                kADC_ReferenceVoltageSourceAlt0,       /*referenceVoltageSource*/ \
                0,                                     /*enableLowPower*/         \
                0,                                     /*enableLongSampleTime*/   \
                kADC_ClockDivider1,                    /*clockDivider*/           \
                kADC_Resolution##_resolution##BitMode, /*ResolutionMode*/         \
                kADC_ClockSourceAlt0,                  /*clockSource*/            \
            },                                                                    \
            _nxpDev,                                                              \
            _name##ChList,                                                        \
            _name##ChValues,                                                      \
            _refreshTime,                                                         \
            ARRAY_SIZE(_name##ChList),                                            \
    };                                                                            \
    tsAdcCoreParams _name##Params =                                               \
        {                                                                         \
            &_name##ConstParams,                                                  \
            TIMER_CALLBACK_INIT(adcCoreRefresh, &_name),                          \
            0,                                                                    \
    };                                                                            \
    DEV_CPX_CREATE(_name, devCpxAdcCoreFuncs, &_name##Params, NULL)

/// @brief  Function to handle interrupt requests of an ADC_CORE device. DO NOT CALL OUTSIDE THE TARGET ISR
INTERFACE void adcCoreIrqHandler(const tsDevCpx *device);

/** @} */

/// @brief  Functions for ADC devices
INTERFACE const tsDevIoFuncs devIoAdcFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *adcCore; ///< Target ADC_CORE device
    uint8_t index;           ///< Index number in the list of enabled channels
} tsAdcParams;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _adcCore    Target ADC_CORE
 *  @param  _idx        Index that will be used for the list of enabled channels in target ADC_CORE
 */
#define DEV_IO_ADC_CREATE(_name, _adcCore, _idx) \
    const tsAdcParams _name##Params =            \
        {                                        \
            &(_adcCore),                         \
            (_idx),                              \
    };                                           \
    DEV_IO_CREATE(_name, devIoAdcFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_ADC_H
