/** @file       psoc6/platform.h
 *  @brief      Platform specific modifications for PSOC6
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergün
 *  @date       01.07.2019
 */
#ifndef FILE_PLATFORM_H
#define FILE_PLATFORM_H

/**
 *  @defgroup   PSOC6 PSOC6
 *  @brief      CYPRESS PSOC6 family cpu devices
 *  @ingroup    PLATFORM
 *  @{
 *      @defgroup   PSOC6_DEV_IO DEV_IO
 *      @defgroup   PSOC6_DEV_COM DEV_COM
 *      @defgroup   PSOC6_DEV_MEM DEV_MEM
 *      @defgroup   PSOC6_DEV_CPX DEV_CPX
 */

/*********************************************************************
 *  INCLUDES
 */
#include "project.h" ///< PSoC Creator project file

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/*********************************************************************
 *  EXPORTED MACROS
 */
/// @brief  Create a default tick configuration for this platform
/// @param  _idx    Systick callback index
// @param  _freq   Frequency of RCoS Tick in Hz
#define CORE_TICK_DEFAULT(_idx)                                                     \
    void tickStart(void)                                                            \
    {                                                                               \
        Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_IMO, CYDEV_CLK_IMO__HZ / 1000); \
        Cy_SysTick_Enable();                                                        \
        Cy_SysTick_SetCallback((_idx), rcosTickIsr);                                \
    }                                                                               \
    CORE_TICK_CREATE(tickStart, Cy_SysTick_EnableInterrupt, Cy_SysTick_DisableInterrupt)

/// @brief  Create a default watchdog timer
/// @param  _flagCount  Number of flags
#define CORE_WDT_DEFAULT(_flagCount) \
    void wdtRefresh(void)            \
    {                                \
    }                                \
    void wdtStart(void)              \
    {                                \
    }                                \
    void wdtStop(void)               \
    {                                \
    }                                \
    CORE_WDT_CREATE(wdtRefresh, wdtStart, wdtStop, (_flagCount))

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */
#define PLATFORM_ASM(_asm) __ASM(_asm)
#define PLATFORM_SECTION(_name) CY_SECTION(_name)
#define PLATFORM_ALIGNED(_align) CY_ALIGN(_align)
#define PLATFORM_STATIC_INLINE __STATIC_INLINE
#define PLATFORM_WEAK __attribute__((weak))
#define PLATFORM_PACKED __attribute__((packed))

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */

/** @} */

#endif // FILE_PLATFORM_H
