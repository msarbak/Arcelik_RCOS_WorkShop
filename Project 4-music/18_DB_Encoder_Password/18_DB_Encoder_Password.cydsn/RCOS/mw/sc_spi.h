/** @file       sc_spi.h
 *  @brief      Serial Communication Protocol over SPI Master
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author
 *  @date       7.02.2019
 */
#ifndef FILE_SC_SPI_H
#define FILE_SC_SPI_H

/**
 *  @defgroup   SC_SPI SC_SPI
 *  @ingroup    MW
 *  @brief      Serial Communication Protocol over SPI Master
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

#define START_OF_FRAME 0xAA
#define PREAMBLE_TIMEOUT 5
#define SC_OLD_BUG_FIX DISABLE

// Set commands requires response except NACK, ASK, BUSY 7th bit indicates response
//#define SC_FREE 0x00
//#define SC_GET_ID               0x01
//#define SC_SET_ID               0x81
#define SC_SET_NACK 0x02
#define SC_SET_ACK 0x03
//#define SC_GET_ASK              0x04
#define SC_SET_BUSY 0x05
#define SC_GET_DATA_PACKAGE 0x06
#define SC_SET_DATA_PACKAGE 0x86
//#define SC_GET_DATA_COMMON      0x07
//#define SC_SET_DATA_COMMON      0x87
//#define SC_GET_DATA_RAM         0x08
//#define SC_SET_DATA_RAM         0x88
#define SC_GET_DATA_ROM 0x09
#define SC_SET_DATA_ROM 0x89
//#define SC_GET_DATA_EEPROM      0x0A
//#define SC_SET_DATA_EEPROM      0x8A
//#define SC_SET_RESPONSE         0x1B
//#define SC_GET_WAIT             0x0C

#define SC_SET_OTA_READ 0x91
#define SC_SET_OTA_WRITE 0x92
#define SC_SET_OTA_ERASE 0x93
#define SC_SET_OTA_QUIT 0x94
#define SC_SET_OTA_ENTER 0x95

/// @brief  SerialCommunication mapping item used for making conversions between internal events and communication frame identifiers
typedef struct
{
    uint8_t identifier;   ///< Target frame identifier
    tEventEnum event;     ///< Target event number
    tProcessEnum process; ///< Target process number
} tsScMapItem;

/// @brief  Frame structure that is used to exchange data over the communication channel
typedef struct PLATFORM_PACKED
{
    uint8_t preamble;   ///< Start of frame must be @ref START_OF_FRAME
    uint8_t identifier; ///< Identifier for this frame
    uint8_t length;     ///< Length of data
    uint8_t data[255];  ///< current data
    uint8_t crc;        ///< CRC-8-MAXIM
} tsFrame;

/// @brief  Used for storing commands for sending in a queue
typedef struct
{
    uint8_t identifier; ///< Serial communication frame identifier
    uint8_t length;     ///< Length of data following this queue item that will be send as frame data
} tsScQueueItem;

typedef enum
{
    eScSpiHeader,
    eScSpiData,
    eScSpiCrc,
} teScSpiReceiveState;

/// @brief  Serial Communication process parameters
typedef struct
{
    tsTimerEvent tev; ///< Event timer used for process follow

    const tsDevCom *com;          ///< Communication device that will be used
    tsTarget *comTarget;          ///< Target information for the communication device
    tsFrame sending;              ///< Frame being send
    tsFrame receiving;            ///< Frame being received
    tTicket comTicket;            ///< Ticket for handling communication device timing
    teScSpiReceiveState recState; ///< Receveing state

    const tsScMapItem *mapArr; ///< Identifier mapper list
    tsDeque *queue;            ///< Outgoing queue
    uint8_t mapSize;           ///< Identifier mapper list size
    uint8_t recDataIdx;        ///< Received data index for handling a devCom with small buffers
} tsScSpiParams;

typedef enum
{
    eScSpiLoop = 0, // Rest of the events are coming from other processes
} teScSpiEvents;

/*********************************************************************
 *  EXPORTED MACROS
 */
/** @brief  ScSpi process object creation macro
 *  @param  _name           Name of process object
 *  @param  _enum           Process enumeration for this object
 *  @param  _devCom         SPI slave communication device that will be used
 *  @param  _devComTarget   Target information for the devCom
 *  @param  _mapName        Name of an array of tsScMapItem objects
 *  @param  _queueSize      Internal queue size in bytes
 */
#define PROCESS_SC_SPI_CREATE(_name, _enum, _devCom, _devComTarget, _mapName, _queueSize) \
    DEQUE_CREATE(_name##Que, _queueSize)                                                  \
    tsScSpiParams _name##Params =                                                         \
        {                                                                                 \
            TIMER_EVENT_INIT(_enum, _enum, eScSpiLoop), /* tev */                         \
            &_devCom,                                   /* com */                         \
            &_devComTarget,                             /* comTarget */                   \
            {START_OF_FRAME, 0, 0, {0}, 0},             /* sending */                     \
            {START_OF_FRAME, 0, 0, {0}, 0},             /* receiving */                   \
            -1,                                         /* comTicket */                   \
            eScSpiHeader,                               /* recState */                    \
            _mapName,                                   /* mapArr */                      \
            &_name##Que,                                /* queue */                       \
            ARRAY_SIZE(_mapName),                       /* mapSize */                     \
            0,                                          /* recDataIdx */                  \
    };                                                                                    \
    PROCESS_CREATE(_name, scSpiInit, scSpiDeinit, _enum, &_name##Params, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_SC_SPI_C
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

#endif // FILE_SC_SPI_H
