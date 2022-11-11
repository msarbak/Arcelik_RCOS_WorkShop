/** @file       psoc4/platform.h
 *  @brief      Platform specific modifications for PSOC4
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Mehmet Baris
 *  @date       04.05.2016
 */
#ifndef FILE_PLATFORM_H
#define FILE_PLATFORM_H

/**
 *  @defgroup   PSOC4 PSOC4
 *  @brief      CYPRESS PSOC4 family cpu devices
 *  @ingroup    PLATFORM
 *  @{
 *      @defgroup   PSOC4_DEV_IO DEV_IO
 *      @defgroup   PSOC4_DEV_COM DEV_COM
 *      @defgroup   PSOC4_DEV_MEM DEV_MEM
 *      @defgroup   PSOC4_DEV_CPX DEV_CPX
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
#define CORE_TICK_DEFAULT(_idx)                    \
    void tickStart(void)                           \
    {                                              \
        CySysTickStart();                          \
        CySysTickSetCallback((_idx), rcosTickIsr); \
    }                                              \
    CORE_TICK_CREATE(tickStart, CySysTickEnableInterrupt, CySysTickDisableInterrupt)

#if (CY_IP_SRSSV2)
/// @brief      Create a default watchdog timer
/// @warning    Timer0(WDT0) must be enabled and setup from Clocks screen under "Design Wide Resources" in PSoC Creator
/// @param      _flagCount  Number of flags
#define CORE_WDT_DEFAULT(_flagCount)                      \
    void wdtRefresh(void)                                 \
    {                                                     \
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET); \
    }                                                     \
    void wdtStart(void)                                   \
    {                                                     \
        CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);         \
    }                                                     \
    void wdtStop(void)                                    \
    {                                                     \
        CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);        \
    }                                                     \
    CORE_WDT_CREATE(wdtRefresh, wdtStart, wdtStop, (_flagCount))
#else
/// @brief  Create a default watchdog timer
/// @param  _flagCount  Number of flags
#define CORE_WDT_DEFAULT(_flagCount) \
    void wdtRefresh(void)            \
    {                                \
        CySysWdtClearInterrupt();    \
    }                                \
    void wdtStart(void)              \
    {                                \
        CySysWdtEnable();            \
    }                                \
    void wdtStop(void)               \
    {                                \
        CySysWdtDisable();           \
    }                                \
    CORE_WDT_CREATE(wdtRefresh, wdtStart, wdtStop, (_flagCount))
#endif

/*********************************************************************
 *  INTERFACES
 */
#define PLATFORM_ASM(_asm) __ASM(_asm)
#define PLATFORM_SECTION(_name) CY_SECTION(_name)
#define PLATFORM_ALIGNED(_align) CY_ALIGN(_align)
#define PLATFORM_STATIC_INLINE __STATIC_INLINE
#define PLATFORM_WEAK __attribute__((weak))
#define PLATFORM_PACKED __attribute__((packed))

#warning If Linker Time Optimization is active, \
        goto "Cm0Start.c" and change "_sbrk" function __attribute__ from "weak" to "used". \
        it is used by vsprint which is called by devComPrint

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
