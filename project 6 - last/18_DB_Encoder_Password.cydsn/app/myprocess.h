/** @file       myprocess.h
 *  @brief      MYPROCESS application header file
 *  @copyright  (c) 2022-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     UMAMI
 *  @date       11/9/2022
 */
#ifndef FILE_MYPROCESS_H
#define FILE_MYPROCESS_H

/** INCLUDES ******************************************************************/
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_MYPROCESS_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   MYPROCESS MYPROCESS
 *  @ingroup    APP
 *  @brief      test
 *  @{
 */

/// @brief  Events of a MyProcess process
typedef enum
{
    eMPEventsMessageSend
} teMyProcessEvents;

/// @brief  Parameters of a MyProcess process
typedef struct
{

} tsMyProcessParams;

/// @brief  Constants of a MyProcess process
typedef struct
{
     const tsDevCom *uart;
} tsMyProcessConsts;

/** @brief  MyProcess process object creation macro
 *  @param  _name   Name of process object
 *  @param  _enum   Process enumeration for this object
 */
#define PROCESS_MYPROCESS_CREATE(_name, _enum, _uart) \
    tsMyProcessParams _name##Params =           \
        {                                         \
        /* DONT FORGET TO FILL INITAL VALUES */   \
    };                                            \
    const tsMyProcessConsts _name##Consts =     \
        {           \
                    .uart = &_uart,\
         \
    };                                            \
    PROCESS_CREATE(_name, myProcessInit, myProcessDeinit, _enum, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_MYPROCESS_H
