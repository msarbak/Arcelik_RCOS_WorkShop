/** @file       system.h
 *  @brief      System wide definitions
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 */ 
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#define ONE_SECOND_IN_MS (1000)
#define ONE_MINUTE_IN_SEC (60)
#define ONE_MINUTE_IN_MS (ONE_MINUTE_IN_SEC * ONE_SECOND_IN_MS)
#define ONE_HOUR_IN_MINUTE (60)

#define SOUND_DUTY_LOW (25)
#define SOUND_DUTY_HIGH (50)

typedef enum
{
    eProcessButtons,
    eProcessEncoderPassword,
} teSystemProcesses;

#endif // FILE_SYSTEM_H
