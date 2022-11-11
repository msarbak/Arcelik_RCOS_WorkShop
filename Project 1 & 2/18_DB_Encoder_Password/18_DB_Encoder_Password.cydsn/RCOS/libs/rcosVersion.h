/**
 *  @file       rcosVersion.h
 *  @brief      Version control macros
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *  @author     Ozkan AKGUL
 *  @date       07.07.2020
 */
#ifndef FILE_RCOSVERSION_H
#define FILE_RCOSVERSION_H

// add all libs here to register
#include "version.h"
#include "bits.h"
#include "crc.h"
#include "debug.h"
#include "hamming.h"
#include "hash.h"
#include "libs.h"
#include "list_dl.h"
#include "list_sl.h"
#include "ticket.h"
#include "utf.h"


/** @addtogroup LIBS
 *  @{
 *      @defgroup   VERSION VERSION
 *      @brief      Version check macros using compiler built-in __DATE__ and __TIME__
 *  @}
 *
 *  @addtogroup RCOSVERSION
 *  @{
 */

#define ADD_RCOS_VERSION(_module, _number)   \
    const static tsVersion version = {.compName = _module, .compVersion = _number}; \
    static tsVersionListItem versionItem = {.versionInfo = &version}; \
    addRCoSVersionItem(&versionItem);

extern void addRCoSVersionItem(tsVersionListItem *newCompVersion);
extern void libVersionRegistiration();
extern tsLsl *getVersionList();
extern tsLsl *getRCoSVersionList();

#endif // FILE_RCOSVERSION_H
