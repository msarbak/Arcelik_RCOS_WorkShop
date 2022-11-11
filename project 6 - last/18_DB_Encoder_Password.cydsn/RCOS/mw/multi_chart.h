/** @file       multi_chart.h
 *  @brief      Send data over serial channel to be logged by MultiChart program
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     
 *  @date       21.03.2019
 */
#ifndef FILE_MULTI_CHART_H
#define FILE_MULTI_CHART_H

/**
 *  @defgroup   MULTI_CHART MULTI_CHART
 *  @ingroup    MW
 *  @brief      Send data over serial channel to be logged by MultiChart program
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/// @brief  DataSource definition for a MultiChart array
typedef struct
{
    uint16_t *raw;      ///< Raw data of a sensor
    uint16_t *baseline; ///< Baseline value of a sensor
    uint16_t *signal;   ///< Current signal state of a sensor
} tsMultiChartDataSource;

/** @brief  MultiChart DataSource taken from a CSD component in CYPRESS PSoC Creator
 *  @param  _cyName Name of CDS component
 *  @param  _no     Index number of sensor
 */
#define MULTI_CHART_DS_CSD(_cyName, _no)            \
    {                                               \
        .raw      = &_cyName##_sensorRaw[_no],      \
        .baseline = &_cyName##_sensorBaseline[_no], \
        .signal   = &_cyName##_sensorSignal[_no],   \
    }

/** @brief  MultiChart DataSource taken from a CapSense component in CYPRESS PSoC Creator
 *  @param  _cyName Name of CapSense component
 *  @param  _no     Index number of sensor
 */
#define MULTI_CHART_DS_CAPSENSE(_cyName, _no)                        \
    {                                                                \
        .raw      = &_cyName##_dsRam.snsList.button##_no[0].raw[0],  \
        .baseline = &_cyName##_dsRam.snsList.button##_no[0].bsln[0], \
        .signal   = &_cyName##_dsRam.snsList.button##_no[0].diff,    \
    }

/// @brief  Parameters of a MultiChart process
typedef struct
{
    tsTimerEvent tev; ///< tsTimerEvent structure for periodic operations
} tsMultiChartParams;

/// @brief  Constants of a MultiChart process
typedef struct
{
    const tsDevCom *uart;                ///< Target UART communication device
    tsTarget *target;                    ///< Target communication channel information
    const tsMultiChartDataSource *array; ///< Data source array
    uint16_t arraySize;                  ///< Item count of array member
    uint16_t period;                     ///< Data refresh period
} tsMultiChartConsts;

/*********************************************************************
 *  EXPORTED MACROS
 */
/** @brief  Creates a MultiChart process
 *  @param  _name   Name of the process
 *  @param  _uart   UART devCom device that will be used
 *  @param  _target Name of tsTarget object that will be used with _uart
 *  @param  _array  Name of an array of const tsMultiChartDataSource which denotes the data sources
 *  @param  _period Data refresh period
 */
#define PROCESS_MULTI_CHART_CREATE(_name, _uart, _target, _array, _period) \
    tsMultiChartParams _name##Params =                                     \
        {                                                                  \
            .tev = TIMER_EVENT_INIT(PROCESS_NONE, PROCESS_NONE, 0),        \
    };                                                                     \
    const tsMultiChartConsts _name##Consts =                               \
        {                                                                  \
            .uart      = &_uart,                                           \
            .target    = &_target,                                         \
            .array     = _array,                                           \
            .arraySize = ARRAY_SIZE(_array),                               \
            .period    = _period,                                          \
    };                                                                     \
    PROCESS_CREATE(_name, multiChartInit, multiChartDeinit, PROCESS_NONE, &_name##Params, &_name##Consts)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_MULTI_CHART_C
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

#endif // FILE_MULTI_CHART_H
