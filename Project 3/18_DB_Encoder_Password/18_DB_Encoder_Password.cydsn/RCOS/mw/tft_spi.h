/** @file       tft_spi.h
 *  @brief      Serial Communication Protocol over SPI Master
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @author     Hamed Mohammadi
 *  @date       09.01.2020
 */
#ifndef FILE_TFT_SPI_H
#define FILE_TFT_SPI_H

/**
 *  @defgroup   TFT_SPI TFT_SPI
 *  @ingroup    APP
 *  @brief      Serial Communication Protocol over SPI Master
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED MACROS
 */
#define TFT_START_OF_FRAME 0xAA
#define TFT_HEART_BEAT 0xFC
#define TFT_PREAMBLE_TIMEOUT 5
#define TFT_SET_NACK 254
#define TFT_SET_ACK 253

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */
/// @brief  Frame structure that is used to exchange data over the communication channel
typedef struct PLATFORM_PACKED
{
    uint8_t preamble;   ///< Start of frame must be @ref TFT_START_OF_FRAME
    uint8_t identifier; ///< Identifier for this frame
    uint8_t length;     ///< Length of data
    uint8_t data[255];  ///< current data
    uint8_t crc;        ///< CRC-8-MAXIM
} tsTftFrame;

/// @brief  Used for storing commands for sending in a queue
typedef struct
{
    uint8_t identifier;
    uint8_t length;
} tsTftQueueItem;

typedef enum
{
    eTftSpiHeader,
    eTftSpiData,
    eTftSpiCrc,
} teTftSpiReceiveState;

/// @brief  Serial Communication process parameters
typedef struct
{
    tsTimerEvent tev;                       ///< Timer for loop event

    const tsDevCom *com;                    ///< Com device for communication
    tsTarget target;                        ///< Target chip select pin io device
    const tsDevIo *tftBusy;                 ///< Target busy signal input io device
    tTicket comTicket;                      ///< Ticket for com device
    tsTftFrame sending;                     ///< Send frame
    tsTftFrame receiving;                   ///< Recieve frame
    teTftSpiReceiveState recState;          ///< Recieve state

    tProcessEnum targetProcess;             ///< Target process for handling incoming data and error casees
    tEventEnum targetEvent;                 ///< Event that handles incoming data
    tEventEnum errorEvent;                  ///< Event that handles error cases
    tsDeque *queue;                         ///< Queue for holding data
    uint8_t lastCrc;                        ///< Latest crc for sync operations with tft board
} tsTftSpiParams;

typedef enum
{
    eTftSpiLoop = 0, // Rest of the events are coming from other processes
} teTftSpiEvents;

/** @brief  This process creates a communication between RCOS tft board.
 *  @param  _name Name of process object
 *  @param  _enum Enumeration of tft com process
 *  @param  _devCom Com device for sending and recieving data
 *  @param  _devComTarget CS pin io device for slave target
 *  @param  _tftBusy IO device that tft sends busy signal
 *  @param  _targetProcess The target process that takes incoming tft board response
 *  @param  _receivedEvent The event that processes incoming tft board response 
 *  @param  _errorEvent The event that processes error case in communication
 *  @param  _queueSize Queue size for communication
 */
#define PROCESS_TFT_SPI_CREATE(_name, _enum, _devCom, _devComTarget, _tftBusy, _targetProcess, _receivedEvent, _errorEvent, _queueSize) \
    DEQUE_CREATE(_name##Que, _queueSize)                                                                                   \
    tsTftSpiParams _name##Params =                                                                                         \
        {                                                                                                                  \
            TIMER_EVENT_INIT(_enum, _enum, eTftSpiLoop),                                                                   \
            &_devCom,                                                                                                      \
            {                                                                                                              \
                .config = NULL,                                                                                            \
                .cs     = &_devComTarget,                                                                                  \
            },                                                                                                             \
            &_tftBusy,                                                                                                     \
            -1,                                                                                                            \
            {TFT_START_OF_FRAME, 0, 0, {0}, 0},                                                                            \
            {TFT_START_OF_FRAME, 0, 0, {0}, 0},                                                                            \
            eTftSpiHeader,                                                                                                 \
            _targetProcess,                                                                                                \
            _receivedEvent,                                                                                                \
            _errorEvent,                                                                                                   \
            &_name##Que,                                                                                                   \
            0,                                                                                                             \
    };                                                                                                                     \
    PROCESS_CREATE(_name, tftSpiInit, tftSpiDeinit, _enum, &_name##Params, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_TFT_SPI_C
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

#endif // FILE_TFT_SPI_H
