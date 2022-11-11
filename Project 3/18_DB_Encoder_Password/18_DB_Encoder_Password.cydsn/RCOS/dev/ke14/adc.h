/** @file       ke14/adc.h
 *  @brief      Analog-Digital Converter device driver for NXP KE14
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Özkan AKGÜL
 *  @date       14.11.2019
 */
#ifndef FILE_ADC_H
#define FILE_ADC_H

#include "rcos.h"
#include <fsl_adc12.h>

/**
 *  @defgroup   KE14_ADC ADC
 *  @ingroup    KE14_DEV_IO
 *  @brief      NXP MKE14 Analog-Digital Converter device driver
 *  @{
 */

#ifndef FILE_ADC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   KE14_ADC_CORE ADC_CORE
 *  @brief      Peripheral core access
 *  @{
 */
/// @brief  Functions for ADC_CORE devices
INTERFACE const tsDevCpxFuncs devCpxAdcCoreFuncs;

/// @brief  Information for each channel that will be converted
typedef struct
{
    uint8_t channelNo;
    uint8_t channelGroup;
} tsAdcChannelInfo;

/// @brief  Device specific constant parameters
typedef struct
{
    adc12_config_t config;          ///< ADC peripheral configuration
    ADC_Type *adc;                  ///< ADC peripheral registry
    const tsAdcChannelInfo *chList; ///< List of active channels
    uint32_t *chValues;             ///< Last conversion values for active channels
    IRQn_Type adcIrq;               ///< IRQ number of target ADC peripheral
    uint16_t refreshTime;           ///< Start a new conversion sequence
    uint8_t chListSize;             ///< Channel list size
} tsAdcCoreConstParams;

/// @brief  Device specific parameters
typedef struct
{
    const tsAdcCoreConstParams *constants;
    tsTimerCallBack timer; ///< Callback timer
    uint8_t index;         ///< Index of current channel processed
} tsAdcCoreParams;

#define ADC_CH(_no, _group) \
    {                       \
        (_no), (_group)     \
    }

/** @brief  Create a devCpx
 *  @param  _name           Name of devCpx object
 *  @param  _nxpDev         ADC_Type pointer for target peripheral
 *  @param  _refreshTime    Timeout duration for values to be refreshed
 *  @param  ...             Channels that are going to be converted in order (use ADC_CH macro for definitions)
 */
#define DEV_CPX_ADC_CORE_CREATE(_name, _nxpDev, _refreshTime, ...)                   \
    const tsDevCpx _name;                                                            \
    void _nxpDev##_IRQHandler(void)                                                  \
    {                                                                                \
        adcCoreIrqHandler(&_name);                                                   \
    }                                                                                \
    const tsAdcChannelInfo _name##ChList[] =                                         \
        {                                                                            \
            __VA_ARGS__,                                                             \
    };                                                                               \
    uint32_t _name##ChValues[ARRAY_SIZE(_name##ChList)];                             \
    const tsAdcCoreConstParams _name##ConstParams =                                  \
        {                                                                            \
            .config =                                                                \
                {                                                                    \
                    /*config*/                                                       \
                    .referenceVoltageSource     = kADC12_ReferenceVoltageSourceVref, \
                    .clockSource                = kADC12_ClockSourceAlt0,            \
                    .clockDivider               = kADC12_ClockDivider1,              \
                    .resolution                 = kADC12_Resolution10Bit,            \
                    .sampleClockCount           = 13,                                \
                    .enableContinuousConversion = false,                             \
                },                                                                   \
            _nxpDev,                                                                 \
            _name##ChList,                                                           \
            _name##ChValues,                                                         \
            _nxpDev##_IRQn,                                                          \
            _refreshTime,                                                            \
            ARRAY_SIZE(_name##ChList),                                               \
    };                                                                               \
    extern TIMER_CALLBACK_FUNC(adcCoreRefresh);                                      \
    tsAdcCoreParams _name##Params =                                                  \
        {                                                                            \
            &_name##ConstParams,                                                     \
            TIMER_CALLBACK_INIT(adcCoreRefresh, &_name),                             \
            0,                                                                       \
    };                                                                               \
    const tsDevCpx _name = {&devCpxAdcCoreFuncs, &_name##Params};

/// @brief  Function to handle interrupt requests of an ADC_CORE device. DO NOT CALL OUTSIDE THE TARGET ISR
INTERFACE void adcCoreIrqHandler(const tsDevCpx *device);

/** @} */

/// @brief  Functions for ADC devices
INTERFACE const tsDevIoFuncs devIoAdcFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *adcCore;
    uint8_t index;
} tsAdcParams;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _adcCore    Target ADC_CORE
 *  @param  _idx        Index of value in target ADC_CORE
 */
#define DEV_IO_ADC_CREATE(_name, _adcCore, _idx) \
    const tsAdcParams _name##Params =            \
        {                                        \
            &(_adcCore),                         \
            (_idx),                              \
    };                                           \
    const tsDevIo _name = {&devIoAdcFuncs, (void *)&_name##Params};

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_ADC_H
