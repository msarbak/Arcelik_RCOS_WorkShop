/** @file       process_gdm.h
 *  @brief      GDM 
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     aom
 *  @date       17.09.2019
 */
#ifndef FILE_GDM_H
#define FILE_GDM_H

/**
 *  @defgroup   GDM GDM
 *  @ingroup    MW
 *  @brief      GDM 
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */
    
#define GDM_TFT_CONTENT_UPDATE_MAX_DATASIZE (params->maxFrameSize)
#define WIFI_OTA_FRAMER_OVERHEAD_BYTES_EXCEPT_DATA 9 // Preamble[1] | PackageType[1] | Command[1] | PackageDataSize[4]|  [n]  | CRC[2]
#define WIFI_OTA_FRAMER_OVERHEAD_BYTES_EXCEPT_DATA_AND_CRC 7 // Preamble[1] | PackageType[1] | Command[1] | PackageDataSize[4]|  
#define GDM_MAX_DATA_SIZE (GDM_TFT_CONTENT_UPDATE_MAX_DATASIZE + 1)                    //1 for type id
#define GDM_MAX_FRAME_SIZE (GDM_MAX_DATA_SIZE + WIFI_OTA_FRAMER_OVERHEAD_BYTES_EXCEPT_DATA)

#define CONTINUATION_MAX_DATA_SIZE (GDM_MAX_FRAME_SIZE - WIFI_OTA_FRAMER_OVERHEAD_BYTES_EXCEPT_DATA) //-1 for type id for otaContinuation
#define OTA_MAX_DATA_SIZE (GDM_MAX_FRAME_SIZE - WIFI_OTA_FRAMER_OVERHEAD_BYTES_EXCEPT_DATA)          //-1 for type id for packtype_OTA
#define DATA_START_INDEX 1 // index 0 is type id


#define GDM_FRAME_PREAMBLE 0x5A

typedef enum
{
    GDM_PREAMBLE,
    GDM_PACK_TYPE,
    GDM_COMMAND,
    GDM_DATA_LENGTH,
    GDM_DATA,
    GDM_CRC,
} enGdmFrame;

typedef struct PLATFORM_PACKED
{
    uint8_t preamble;
    uint8_t packType;
    uint8_t command;
    uint32_t packDataSize;
    uint8_t *data;
//    uint8_t data[GDM_MAX_FRAME_SIZE];
    uint16_t crc;
} stGdmFrame;

typedef struct PLATFORM_PACKED
{
    uint8_t preamble;
    uint8_t packType;
    uint16_t crc;
} stGdmTransfer;

typedef struct
{
    uint8_t packType;
    uint8_t command;
    tProcessEnum process;
    tEventEnum event;
} tsGdmMapItem;

typedef struct PLATFORM_PACKED //TODO: !!!!
{
    uint8_t packType;
    uint8_t command;
    uint32_t length;
} tsGdmtQueueItem;

/// @brief  Parameters of a ProcessGdm process
typedef struct
{
    const tsDevCom      *com;
    tsTimerEvent        framerLowLevelTimer;
    tsTimerEvent        framerTimerTx;
    stGdmFrame          rxBufGdm;
    stGdmFrame          txBufGdm;
    stGdmTransfer       txTransfer;
    uint32_t            dataLength;
    const tsGdmMapItem  *mapArr;
    uint8_t             mapsize;
    tsDeque             *queue;
    tsTimerEvent        framerLowLevelPackTimeOut;
    stGdmFrame          txBufGdmResend;
    tsTimerEvent        tevGetData;
    uint16_t              maxFrameSize;
    tProcessEnum        maxFrameProcess;
    tEventEnum          maxFrameEvent;
    tsTimerEvent        tevMaxFrameSize;
    tsTimerEvent        tevCommCheck;
    tProcessEnum        commStatProcess;
    tEventEnum          commStatEvent;
} tsGdmParams;

/// @brief  Events of a ProcessGdm process
typedef enum
{
    eLowLevel,
    eGetPreamble,
    eGetPackType,
    eGetCommand,
    eGetDataLength,
    eGetData,
    eGetCrc,
    eFrameRooter,
    eLowLevelPackTimeOut,
    eControlPackSafir,
    eSendResume,
    eGDMDTx,
    eGdmSendMaxFrameSize,
    eGdmCheckCommStarted,
    eGdmCheckCommStatus,
} teGdmEvents;

/*********************************************************************
 *  EXPORTED MACROS
 */
/** @brief  ProcessGdm process object creation macro
 *  @param  _name   Name of process object
 *  @param  _enum   Process enumeration for this object
 */
#define PROCESS_GDM_CREATE(_name, _enum, _map, _devCom, _queueSize,_maxFrameSize,_maxFrameProcess,_maxFrameEvent,   \
                           _commStatProcess, _commStatEvent)                                                        \
    uint8_t _name##rxBuffer[_maxFrameSize];                                                                         \
    uint8_t _name##txBuffer[_maxFrameSize];                                                                         \
    uint8_t _name##txResendBuffer[_maxFrameSize];                                                                   \
    DEQUE_CREATE(_name##Que, _queueSize)                                                                            \
    tsGdmParams _name##Params =                                                                                     \
    {                                                                                                               \
        .com                        =   &_devCom,                                                                   \
        .framerLowLevelTimer        =   TIMER_EVENT_INIT(_enum, _enum, eLowLevel),                                  \
        .framerTimerTx              =   TIMER_EVENT_INIT(_enum, _enum, eGDMDTx),                                    \
        .rxBufGdm                   =   {0, 0, 0, 0, _name##rxBuffer, 0},                                           \
        .txBufGdm                   =   {0, 0, 0, 0, _name##txBuffer, 0},                                           \
        .txTransfer                 =   {0, 0, 0},                                                                  \
        .dataLength                 =   0,                                                                          \
        .mapArr                     =   _map,                                                                       \
        .mapsize                    =   ARRAY_SIZE(_map),                                                           \
        .queue                      =   &_name##Que,                                                                \
        .framerLowLevelPackTimeOut  =   TIMER_EVENT_INIT(_enum, _enum, eLowLevelPackTimeOut),                       \
        .txBufGdmResend             =   {0, 0, 0, 0, _name##txResendBuffer, 0},                                     \
        .tevGetData                 =   TIMER_EVENT_INIT(_enum, _enum, eGetData),                                   \
        .maxFrameSize               =   _maxFrameSize,                                                              \
        .maxFrameProcess            =   _maxFrameProcess,                                                           \
        .maxFrameEvent              =   _maxFrameEvent,                                                             \
        .tevMaxFrameSize            =   TIMER_EVENT_INIT(_enum, _enum, eGdmSendMaxFrameSize),                       \
        .tevCommCheck               =   TIMER_EVENT_INIT(_enum, _enum, eGdmCheckCommStarted),                       \
        .commStatProcess            =   _commStatProcess,                                                           \
        .commStatEvent              =   _commStatEvent,                                                             \
    };                                                                                                              \
    PROCESS_CREATE(_name, gdmInit, gdmDeinit, _enum, &_name##Params, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_GDM_C
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

#endif // FILE_GDM_H
