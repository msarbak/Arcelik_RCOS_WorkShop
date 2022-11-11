/** @file       wfa.h
 *  @brief      wfa
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     mertGemalmaz
 *  @date       24.09.2019
 */
#ifndef FILE_WFA_H
#define FILE_WFA_H

/**
 *  @defgroup   WFA WFA
 *  @ingroup    MW
 *  @brief      wfa
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */
#define PACK_TYPE_WFA 0xF0

typedef enum
{
    WFA_READ_BYTE           = 0x32,
    WFA_WRITE_BYTE          = 0x33,
    WFA_READ_DYNAMIC_BYTES  = 0x34,
    WFA_READ_ALL_BYTES      = 0x35,
    WFA_READ_MERCEK_ARRAY   = 0x36,
    WFA_READ_MERCEK_VERSION = 0x37,
    WFA_BATCH_WRITE_BYTE    = 0x46,
    WFA_BYTE_READ           = 0x96,
    WFA_BYTE_WRITTEN        = 0x97,
    WFA_DYNAMIC_BYTES_READ  = 0x98,
    WFA_ALL_BYTES_READ      = 0x99,
    WFA_MERCEK_ARRAY_READ   = 0x9A,
    WFA_MERCEK_VERSION_READ = 0x9B,
    WFA_BATCH_BYTES_WRITTEN = 0xAA,
} teWfaCommands;

#define WFA_DEFAULT_DATA_SIZE 3

typedef struct PLATFORM_PACKED //TODO:
{
    uint8_t *address; //target data address
    uint8_t length;   //target data lenght
    uint8_t process;  //target machine process
    uint8_t event;    //target machine event
} tsWfaMap;

typedef struct PLATFORM_PACKED
{
    uint8_t *address;
    uint8_t lenght;
} tsMercekMap;

/// @brief  Parameters of a AppWfa process
typedef struct
{
    const tsWfaMap *wfaMap;
    uint8_t mapsize;
    const tsMercekMap *mercekArrayMap;
    uint8_t mercekSize;
    uint8_t txDataBuf[255]; 
    uint8_t mercekVersionMajor;
    uint8_t mercekVersionMinor;
    tProcessEnum mercekSentProcess;
    tEventEnum mercekSentEvent;
} tsWfaParams;

/// @brief  Events of a AppWfa process
typedef enum
{
    eWfaReadByte,
    eWfaWriteByte,
    eWfaReadDynamicBytes,
    eWfaReadAllBytes,
    eWfaReadMercek,
    eWfaReadMercekVersion,
 
    eWfaByteRead,
    eWfaByteWritten,
    eWfaDynamicBytesRead,
    eWfaAllBytesRead,
    eWfaMercekRead,
    eWfaMercekVersionRead,
 
    eFindTargetEvent,
} teAppWfaEvents;

/*********************************************************************
 *  EXPORTED MACROS
 */
/** @brief  AppWfa process object creation macro
 *  @param  _name   Name of process object
 *  @param  _enum   Process enumeration for this object
 */
#define PROCESS_APPWFA_CREATE(_name, _enum, _mapWfa, _mapMercek, _mercekVersionMajor, _mercekVersionMinor,_mercekSentProcess, _mercekSentEvent) \
    tsWfaParams _name##Params =                               \
        {                                                        \
            _mapWfa,                                             \
            ARRAY_SIZE(_mapWfa),                                 \
            _mapMercek,                                          \
            ARRAY_SIZE(_mapMercek),                              \
            {0, 0},                                              \
            _mercekVersionMajor,                                 \
            _mercekVersionMinor,                                 \
            _mercekSentProcess,                                  \
            _mercekSentEvent,                                    \
    };                                                           \
    PROCESS_CREATE(_name, wfaInit, wfaDeinit, _enum, &_name##Params, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_WFA_C
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

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_WFA_H
