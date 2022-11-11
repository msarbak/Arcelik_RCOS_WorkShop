/** @file       rcos_main.h
 *  @brief      RCoS+ main include
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       1.11.2018
 */
#ifndef FILE_RCOS_MAIN_H
#define FILE_RCOS_MAIN_H

/**
 *  @mainpage   How To INSTALL
        @verbatim
             ___  ___     ___   _
            | _ \/ __|___/ __|_| |_
            |   / (__/ _ \__ \_   _|
            |_|_\\___\___/___/ |_|
        @endverbatim
 *      Reusable COnnected Software platform + security @ref RCoS "More..."
 *
 *      # How To INSTALL:
 *          1. Place "rcos" folder under your project's root folder,
 *          2. Set include folder compile option to have "rcos" folder,
 *          3. Run _rcos_config.vbs script and configure "rcos.h" file,
 *          4. Follow the created "rcos.c" and "main.c" example for a setup, initialize and start rcos
 * 
 *      # main.c Example
 *      @code
 *          #include "rcos.h"   // RCoS+ main definitions
 *          #include "board.h"  // Devices defined for target board
 *          #include "system.h" // System-wide definitions
 *
 *          CORE_EVENTQUEUE_SIZE(...)
 *          CORE_TICK_DEFAULT(...)
 *          CORE_WDT_DEFAULT(...)
 * 
 *          #include "dev/gpio.h"
 *          DEV_IO_GPIO_CREATE(gpioDevice, ...)
 * 
 *          #include "dev/uart.h"
 *          DEV_COM_UART_CREATE(comUart, ...)
 * 
 *          #include "dev/w25q.h"
 *          DEV_MEM_W25Q_CREATE(memExtFlash, ...)
 * 
 *          #include "dev/matrixout.h"
 *          DEV_CPX_MATRIXOUT_CREATE(cpxMatrix, ...)
 * 
 *          #include "app/uartecho.h"
 *          PROCESS_UARTECHO_CREATE(processUartEcho, ...)
 *
 *          int main(void)
 *          {
 *              platformInit();
 *              coreInit();
 * 
 *              devIoInit(&gpioDevice, NULL);
 *              devComInit(&comUart);
 *              devMemInit(&memExtFlash);
 *              devCpxInit(&cpxMatrix);
 * 
 *              processStart(&processUartEcho);
 * 
 *              coreRun();
 *          }
 *      @endcode
 */

/// Debug function used by RCOS
#define _DEBUG_FUNC rcosDebugPrint

#include "porting.h"   ///< Platform specific definitions and functions
#include "libs/libs.h" ///< Basic libraries
#include "core/core.h" ///< Core functionalities(SW Timers and Events)
#include "dev/dev.h"   ///< Abstract device driver structures

#endif // FILE_RCOS_MAIN_H
