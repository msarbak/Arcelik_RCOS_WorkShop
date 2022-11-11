/** @file       rcos.h
 *  @brief      RCoS+ configuration file
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Emre Karabek
 *  @date       1.06.2021
 */
#ifndef FILE_RCOS_H
#define FILE_RCOS_H

/**
 *  RCoS+ configuration
 */
#define RCOS_PLATFORM_PSOC4

#include "rcos_main.h"

#ifndef FILE_RCOS_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

INTERFACE void rcosMainLoop(void);

#undef INTERFACE // Should not let this roam free

#endif // FILE_RCOS_H
