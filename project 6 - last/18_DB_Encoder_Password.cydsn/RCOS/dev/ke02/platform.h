/** @file       ke02/platform.h
 *  @brief      Platform specific modifications for NXP KE02
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 */
#ifndef FILE_PLATFORM_H
#define FILE_PLATFORM_H

/**
 *  @defgroup   KE02 KE02
 *  @ingroup    PLATFORM
 *  @brief      NXP KE02 family cpu devices
 *  @{
 *      @defgroup   KE02_DEV_IO DEV_IO
 *      @defgroup   KE02_DEV_COM DEV_COM
 *      @defgroup   KE02_DEV_MEM DEV_MEM
 *      @defgroup   KE02_DEV_CPX DEV_CPX
 */

/*********************************************************************
 *  INCLUDES
 */
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_wdog8.h"
#include "MKE02Z4.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/*********************************************************************
 *  EXPORTED MACROS
 */
/// @brief  Create a default tick configuration for this platform
/// @param  _freq   Frequency of RCoS Tick in Hz
#define CORE_TICK_DEFAULT(_freq)                                 \
    void tickStart(void)                                         \
    {                                                            \
        SysTick_Config(BOARD_BOOTCLOCKRUN_CORE_CLOCK / (_freq)); \
    }                                                            \
    void tickEnable(void)                                        \
    {                                                            \
        EnableIRQ(SysTick_IRQn);                                 \
    }                                                            \
    void tickDisable(void)                                       \
    {                                                            \
        DisableIRQ(SysTick_IRQn);                                \
    }                                                            \
    void SysTick_Handler(void)                                   \
    {                                                            \
        rcosTickIsr();                                           \
    }                                                            \
    CORE_TICK_CREATE(tickStart, tickEnable, tickDisable)

/// @brief  Create a default watchdog timer
/// @param  _flagCount  Number of flags
/// @param  _timeout Timeout duration in milliseconds
#define CORE_WDT_DEFAULT(_flagCount, _timeout)       \
    void wdtRefresh(void)                            \
    {                                                \
        WDOG8_Refresh((WDOG_Type *)WDOG_BASE);       \
    }                                                \
    void wdtStart(void)                              \
    {                                                \
        wdog8_config_t config;                       \
        WDOG8_GetDefaultConfig(&config);             \
        /* kWDOG8_ClockSource1 is 1kHz by default */ \
        config.timeoutValue = (_timeout);            \
        WDOG8_Init((WDOG_Type *)WDOG_BASE, &config); \
    }                                                \
    void wdtStop(void)                               \
    {                                                \
        WDOG8_Deinit((WDOG_Type *)WDOG_BASE);        \
    }                                                \
    CORE_WDT_CREATE(wdtRefresh, wdtStart, wdtStop, _flagCount)

/*********************************************************************
 *  INTERFACES
 */
#define PLATFORM_ASM(_asm) __ASM(_asm)
#define PLATFORM_SECTION(_name) __attribute__((section(_name)))
#define PLATFORM_ALIGNED(_align) __attribute__((aligned(_align)))
#define PLATFORM_STATIC_INLINE __STATIC_INLINE
#define PLATFORM_WEAK __WEAK
#define PLATFORM_PACKED __PACKED

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */

/** @} */

#endif // FILE_PLATFORM_H
