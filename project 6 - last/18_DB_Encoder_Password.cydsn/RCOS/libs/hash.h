/** @file       hash.h
 *  @brief      Hashing functions
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     
 *  @date       27.06.2019
 */
#ifndef FILE_HASH_H
#define FILE_HASH_H

/*********************************************************************
 *  INCLUDES
 */
#include <stdint.h>

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/*********************************************************************
 *  EXPORTED MACROS
 */

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_HASH_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */
// Fowler–Noll–Vo hash functions
INTERFACE uint32_t hashFnv0(const void *key, int len);
INTERFACE uint32_t hashFnv1(const void *key, int len);
INTERFACE uint32_t hashFnv1a(const void *key, int len);

//-----------------------------------------------------------------------------
// MurmurHash2 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.
INTERFACE uint32_t murmurHash2        ( const void * key, int len, uint32_t seed );
INTERFACE uint64_t murmurHash64A      ( const void * key, int len, uint64_t seed );
INTERFACE uint64_t murmurHash64B      ( const void * key, int len, uint64_t seed );
INTERFACE uint32_t murmurHash2A       ( const void * key, int len, uint32_t seed );
INTERFACE uint32_t murmurHashNeutral2 ( const void * key, int len, uint32_t seed );
INTERFACE uint32_t murmurHashAligned2 ( const void * key, int len, uint32_t seed );
//-----------------------------------------------------------------------------

/** @brief Adding hash version to platform versioning
 */
INTERFACE void addHashVersion();

#undef INTERFACE // Should not let this roam free

#endif // FILE_HASH_H
