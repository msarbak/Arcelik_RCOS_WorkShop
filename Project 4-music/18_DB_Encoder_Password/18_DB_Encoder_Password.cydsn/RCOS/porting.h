/** @file       porting.h
 *  @brief      Declerations needed for platform porting
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       13.05.2020
 */
#ifndef FILE_PORTING_H
#define FILE_PORTING_H

#include <stdint.h>
#include "libs/libs.h"

/// @brief  Initialize platform specific configurations
void platformInit(void);

/// @brief  Enable global interrupt services
void globalIntEnable(void);

/// @brief  Disable global interrupt services
void globalIntDisable(void);

/// @brief  Check if we're in an ISR
/// @retval TRUE
/// @retval FALSE
/// @retval ~ not available in this platform
teBool isIsrActive(void);

/// @brief Writes unique id to specified pointer address
/// @param ptr unique id storage pointer
/// @param length unique id storage length
void getUniqId(uint8_t *ptr, uint8_t length);

/// @brief  Network-to-Host endian conversion for long int
/// @param  data Value that will be converted
/// @return Converted value
uint32_t ntohl(uint32_t data);

/// @brief  Host-to-Network endian conversion for long int
/// @param  data Value that will be converted
/// @return Converted value
uint32_t htonl(uint32_t data);

/// @brief  Network-to-Host endian conversion for short int
/// @param  data Value that will be converted
/// @return Converted value
uint16_t ntohs(uint16_t data);

/// @brief  Host-to-Network endian conversion for short int
/// @param  data Value that will be converted
/// @return Converted value
uint16_t htons(uint16_t data);

#if defined RCOS_PLATFORM_KE02
#include "dev/ke02/platform.h"
#elif defined RCOS_PLATFORM_KE04
#include "dev/ke04/platform.h"
#elif defined RCOS_PLATFORM_KE14
#include "dev/ke14/platform.h"
#elif defined RCOS_PLATFORM_KE15
#include "dev/ke15/platform.h"
#elif defined RCOS_PLATFORM_KE16
#include "dev/ke16/platform.h"
#elif defined RCOS_PLATFORM_PSOC4
#include "dev/psoc4/platform.h"
#elif defined RCOS_PLATFORM_PSOC6
#include "dev/psoc6/platform.h"
#elif defined RCOS_PLATFORM_AVR_DA
#include "dev/avr_da/platform.h"
#endif

#endif // FILE_PORTING_H
