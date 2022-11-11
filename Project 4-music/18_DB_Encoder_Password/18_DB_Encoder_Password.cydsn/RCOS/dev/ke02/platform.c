/** @file       ke02/platform.c
 *  @brief      Platform specific modifications for KE02
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 */
#define FILE_PLATFORM_C

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"
#include <string.h>

#ifndef RCOS_PLATFORM_KE02
#error this file is prepared for KE02 only
#endif

/** @addtogroup KE02
 *  @{
 */

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/*********************************************************************
 *  EXPORTED MACROS
 */

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */
void platformInit(void)
{
    ADD_RCOS_VERSION("ke02/platform", "2.0.2")

    SIM->SOPT &= ~SIM_SOPT_NMIE_MASK;
}

volatile uint32_t globalIntPriMask;

void globalIntEnable(void)
{
    EnableGlobalIRQ(globalIntPriMask);
}

void globalIntDisable(void)
{
    globalIntPriMask = DisableGlobalIRQ();
}

teBool isIsrActive(void)
{
    return 0xff; // n/a
}

void getUniqId(uint8_t *ptr, uint8_t length)
{
    uint8_t tempBuff[8];
    ///< Returns the 64-bit unique id of the device by loading them into the integer array pointed to by uniqueId.
    // TODO: Interface to lower drivers
    memcpy(ptr, tempBuff, length);
}

uint32_t ntohl(uint32_t data)
{
    return htonl(data);
}

uint32_t htonl(uint32_t _32)
{
    return ((((_32)&0xfful) << 24) |
            (((_32)&0xff00ul) << 8) |
            (((_32)&0xff0000ul) >> 8) |
            (((_32)&0xff000000ul) >> 24));
}

uint16_t ntohs(uint16_t _16)
{
    return htons(_16);
}

uint16_t htons(uint16_t _16)
{
    return ((((_16)&0xfful) << 8) | (((_16)&0xff00ul) >> 8));
}

/** @} */
