/** @file       mem_hamming.h
 *  @brief      Header file of MEM_HAMMING device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       23.01.2020
 */
#ifndef FILE_MEM_HAMMING_H
#define FILE_MEM_HAMMING_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_MEM_HAMMING_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   MEM_HAMMING MEM_HAMMING
 *  @ingroup    DEV_MEM
 *  @brief      An intermediary devMem that encodes/decodes any data with [8,4]Hamming method
 *  @{
 */

/// @brief  Functions for MEM_HAMMING devices
INTERFACE const tsDevMemFuncs devMemMemHammingFuncs;

/// @brief  Device specific constants
typedef struct
{
    const tsDevMem *mem; ///< Target devMem to read/write hamming coded data
} tsMemHammingConsts;

/** @brief  Create a devMem
 *  @param  _name   Name of devMem object
 *  @param  _mem    Target memory device to hold Hamming encoded data
 */
#define DEV_MEM_MEM_HAMMING_CREATE(_name, _mem) \
    const tsMemHammingConsts _name##Consts =    \
        {                                       \
            &_mem,                              \
    };                                          \
    DEV_MEM_CREATE(_name, devMemMemHammingFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_MEM_HAMMING_H
