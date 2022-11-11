/** @file       psoc4/adc.h
 *  @brief      CYPRESS PSOC4 Analog-Digital Conversion
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       11.01.2019
 */
#ifndef FILE_ADC_H
#define FILE_ADC_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_ADC ADC
 *  @ingroup    PSOC4_DEV_IO
 *  @brief      CYPRESS PSOC4 Analog-Digital Conversion
 *  @{
 */

#ifndef FILE_ADC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   PSOC4_ADC_CORE ADC_CORE
 *  @brief      Peripheral core access
 *  @{
 */
/// @brief  Functions for ADC_CORE devices
INTERFACE const tsDevCpxFuncs devCpxAdcCoreFuncs;

/// @brief  Device specific parameters
typedef struct
{
    void (*start)(void);                  ///< Component start function
    void (*stop)(void);                   ///< Component stop function
    void (*startConvert)(void);           ///< Component start conversion function
    void (*stopConvert)(void);            ///< Component stop conversion function
    int16 (*getResult16)(uint32 channel); ///< Component get result function
    uint8_t channelCount;                 ///< Total number of channels enabled on component
} tsAdcCoreConsts;

/** @brief  Create a devCpx
 *  @param  _name       Name of devCpx object
 *  @param  _cyBlock    CYPRESS ADC block name
 */
#define DEV_CPX_ADC_CORE_CREATE(_name, _cyBlock)           \
    const tsAdcCoreConsts _name##Consts =                  \
        {                                                  \
            .start        = _cyBlock##_Start,              \
            .stop         = _cyBlock##_Stop,               \
            .startConvert = _cyBlock##_StartConvert,       \
            .stopConvert  = _cyBlock##_StopConvert,        \
            .getResult16  = _cyBlock##_GetResult16,        \
            .channelCount = _cyBlock##_TOTAL_CHANNELS_NUM, \
    };                                                     \
    DEV_CPX_CREATE(_name, devCpxAdcCoreFuncs, NULL, &_name##Consts)

/** @} */

/// @brief  Functions for ADC devices
INTERFACE const tsDevIoFuncs devIoAdcFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *adcCore; ///< Target ADC_CORE device
    uint8_t channel;         ///< Channel number on target ADC_CORE device
} tsAdcConsts;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _adcCore    Target tsDevCpx used as ADC_CORE device
 *  @param  _channel    Target channel number on ADC_CORE device
 */
#define DEV_IO_ADC_CREATE(_name, _adcCore, _channel) \
    const tsAdcConsts _name##Consts =                \
        {                                            \
            .adcCore = &_adcCore,                    \
            .channel = (_channel),                   \
    };                                               \
    DEV_IO_CREATE(_name, devIoAdcFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_ADC_H
