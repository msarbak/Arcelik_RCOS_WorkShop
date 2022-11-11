/** @file       avr_da/platform.h
 *  @brief      Platform specific modifications for Microchip AVR DA
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 */
#ifndef FILE_PLATFORM_H
#define FILE_PLATFORM_H

/**
 *  @defgroup   AVR_DA AVR_DA
 *  @ingroup    PLATFORM
 *  @brief      Microchip AVR DA family cpu devices
 *  @{
 *      @defgroup   AVR_DA_DEV_IO DEV_IO
 *      @defgroup   AVR_DA_DEV_COM DEV_COM
 *      @defgroup   AVR_DA_DEV_MEM DEV_MEM
 *      @defgroup   AVR_DA_DEV_CPX DEV_CPX
 */

/*********************************************************************
 *  INCLUDES
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/*********************************************************************
 *  EXPORTED MACROS
 */
#ifndef F_CPU
#error "You need to define F_CPU as the main clock frequency for the timer to work properly"
#endif

// RCoS Platform sub-macros
#define TCA_INIT(_instance, _count)                         \
    TCA##_instance.SINGLE.INTCTRL = 1 << TCA_SINGLE_OVF_bp; \
    TCA##_instance.SINGLE.PER     = _count;                 \
    TCA##_instance.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV1_gc | 1 << TCA_SINGLE_ENABLE_bp;

#define TCB_INIT(_instance, _count)            \
    TCB##_instance.INTCTRL = 1 << TCB_CAPT_bp; \
    TCB##_instance.CCMP    = _count;           \
    TCB##_instance.CTRLA   = TCB_CLKSEL_DIV1_gc | 1 << TCB_ENABLE_bp;

#define TCA_ISR(_instance) ISR(TCA##_instance##_OVF_vect)
#define TCB_ISR(_instance) ISR(TCB##_instance##_INT_vect)

#define TCA_VECNUM(_instance) TCA##_instance##_OVF_vect_num
#define TCB_VECNUM(_instance) TCB##_instance##_INT_vect_num

#define TCA_CLEAR_INT(_instance) TCA##_instance.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
#define TCB_CLEAR_INT(_instance) TCB##_instance.INTFLAGS = TCB_CAPT_bm;

#define TCA_ENABLE(_instance, _enable) TCA##_instance.SINGLE.INTCTRL = _enable << TCA_SINGLE_OVF_bp;
#define TCB_ENABLE(_instance, _enable) TCB##_instance.INTCTRL = _enable << TCB_CAPT_bp;

/// @brief  Create a default tick configuration for this platform
/// @param  _base       Base name of Timer/Counter used to generate SysTick interrupts (TCA, TCB)
/// @param  _instance   Timer/Counter instance used (0, 1, ...)
/// @param  _freq       Frequency of RCoS Tick in Hz
#define CORE_TICK_DEFAULT(_base, _instance, _freq)  \
    void tickStart(void)                            \
    {                                               \
        _base##_INIT(_instance, (F_CPU / (_freq))); \
        CPUINT.LVL1VEC = _base##_VECNUM(_instance); \
    }                                               \
    void tickEnable(void)                           \
    {                                               \
        _base##_ENABLE(_instance, 1);               \
    }                                               \
    void tickDisable(void)                          \
    {                                               \
        _base##_ENABLE(_instance, 0);               \
    }                                               \
    _base##_ISR(_instance)                          \
    {                                               \
        rcosTickIsr();                              \
        _base##_CLEAR_INT(_instance);               \
    }                                               \
    CORE_TICK_CREATE(tickStart, tickEnable, tickDisable)

/// @brief  Create a default watchdog timer
/// @param  _flagCount  Number of flags
/// @param  _timeout    Timeout duration in milliseconds (8, 16, 32, 64, 128, 256, 512, 1K, 2K, 4K, 8K)
#define CORE_WDT_DEFAULT(_flagCount, _timeout)                                                 \
    void wdtRefresh(void)                                                                      \
    {                                                                                          \
        __asm__ __volatile__("wdr");                                                           \
    }                                                                                          \
    void wdtStart(void)                                                                        \
    {                                                                                          \
        ccp_write_io((void *)&(WDT.CTRLA), WDT_PERIOD_##_timeout##CLK_gc | WDT_WINDOW_OFF_gc); \
    }                                                                                          \
    void wdtStop(void)                                                                         \
    {                                                                                          \
        ccp_write_io((void *)&(WDT.CTRLA), WDT_PERIOD_OFF_gc | WDT_WINDOW_OFF_gc);             \
    }                                                                                          \
    CORE_WDT_CREATE(wdtRefresh, wdtStart, wdtStop, _flagCount)

/*********************************************************************
 *  INTERFACES
 */
#define PLATFORM_ASM(_asm) __asm__ __volatile__(_asm)
#define PLATFORM_SECTION(_text) __attribute__((section(_text)))
#define PLATFORM_ALIGNED(_align) __attribute__((aligned(_align)))
#define PLATFORM_STATIC_INLINE (static inline)
#define PLATFORM_WEAK __attribute__((weak))
#define PLATFORM_PACKED __attribute__((packed))

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
