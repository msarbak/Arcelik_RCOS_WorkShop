/** @file       ccm.h
 *  @brief      ccm process
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     mertGemalmaz
 *  @date       23.10.2019
 */
#ifndef FILE_CCM_H
#define FILE_CCM_H

/**
 *  @defgroup   CCM CCM
 *  @ingroup    MW
 *  @brief      ccm process
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

#define PACK_TYPE_CONFIG 0x10

typedef enum
{
    CCM_CMD_READ_ATTRIBUTE    = 0x01,
    CCM_CMD_WRITE_ATTRIBUTE   = 0x03,
    CCM_CMD_ATTRIBUTE_READ    = 0x02,
    CCM_CMD_ATTRIBUTE_WRITTEN = 0x04,
    CCM_CMD_ATTRIBUTE_EMITTED = 0x07,

    CCM_CMD_NOT_VALID_COMMAND = 0xFE,

} teCcmCommands;

//CCM and CCMSafir comm struct
typedef struct
{
    uint8_t attrId;         // attrId
    teCcmCommands comand; // read write emit commands of ccm
    uint8_t size;           // data size
    uint8_t data[10];       // data to send
} stCcmSafir;

enum resultValues_e
{
    resultSUCCESS,
    CANNOT_EXECUTE_COMMAND,
    CANNOT_PARSE_DATA,
    NO_SUCH_ATTRIBUTE,
    ATTRIBUTE_IS_NOT_READABLE,
    ATTRIBUTE_IS_NOT_WRITABLE,
    NO_SUCH_FUNCTION,
    NO_SUCH_COMPARTMENT,
};

typedef struct PLATFORM_PACKED
{
    uint8_t cmdConf;
    uint8_t attrId;
} tsReadAttributeType;

typedef struct PLATFORM_PACKED //TODO: PACKED
{
    uint8_t cmdConf;
    uint8_t attrId;
    uint8_t dataSize;
    uint8_t *data; //TODO: what is the max data size ???
} tsWriteAttributeType;

typedef struct PLATFORM_PACKED
{
    uint8_t cmdConf;
    uint8_t attrId;
    uint8_t dataSize;
    uint8_t *data; //TODO: what is the max data size ???
} tsWriteEmitAttributeType;

typedef struct PLATFORM_PACKED //TODO: PACKED
{
    uint8_t cmdConf;
    uint8_t attrId;
    uint8_t result;
    uint8_t dataSize;
    uint8_t *data; //TODO: what is the max data size ???
} tsAttributeReadType;

typedef struct PLATFORM_PACKED
{
    uint8_t cmdConf;
    uint8_t result;

} tsAttributeNotValid;

typedef struct PLATFORM_PACKED
{
    uint8_t cmdConf;
    uint8_t attrId;
    uint8_t result;
} tsAttributeWrittenType;

typedef enum
{
    CONN_STATE_OFF = 0,
    CONN_STATE_STA,
    CONN_STATE_ONBOARDING,
    CONN_STATE_UNDEFINED // used for reset issues
} enum_connectionStateWifi;

typedef enum
{
    WIFI_STATE_NOT_ACTIVE,
    WIFI_STATE_ACTIVE,
    WIFI_STATE_CONFIG,
    WIFI_STATE_CONNECTING,
    WIFI_STATE_CONNECTED,
    WIFI_STATE_NOTCONNECTED,
    WIFI_STATE_UNDEFINED // used for reset issues
} enum_wifiStatus;

typedef enum
{
    CONN_SUB_STATE_NULL = 0,
    CONN_SUB_STATE_CONNECTING,
    CONN_SUB_STATE_CONNECTED,
    CONN_SUB_STATE_FAILED,
    CONN_SUB_STATE_SUCCESS, // used for reset issues
    CONN_SUB_STATE_MDNS_START,
    CONN_SUB_STATE_MDNS_FAIL,
    CONN_SUB_STATE_UNDEFINED
} enum_connectionSubStateWifi;

typedef enum
{
    NETWORK_CARD_INFO_MIN = 0,
    NETWORK_CARD_INFO_REGION,
    NETWORK_CARD_INFO_SW_VERSION,
    NETWORK_CARD_INFO_MAX
} enum_NetworkCardSWVersion; //for China manufactured machines

typedef struct
{
    uint8_t attrId;
    //uint8_t  command;
    teBool readable;
    teBool writetable;
    teBool emitable;
    uint8_t eventHandler;
    uint8_t length;
    void *data;
} tsAttrbuteRooter;

typedef enum
{
    ATTRIBUTE_ID_START = 0x00,     // 0
    MAX_FRAME_SIZE,                // 1
    DEVICE_DESCRIPTION,            // 2
    SUPPORTED_CONNECTION_TYPES,    // 3
    COMMUNICATION_MODE,            // 4
    MAC_ADDRESS,                   // 5
    COMM_PROTOCOL_VERSION,         // 6
    CONNECTION_STATE_WIFI,         // 7
    STORED_SSID,                   // 8
    CONNECTION_SUBSTATE,           // 9
    ONBOARDING_TIMEOUT_DUR_IN_SEC, // A
    CUSTOMIZED_NAME,               // B
    CONNECTIVITY_BOARD_ID,         // C
    OTA_STATE,                     // D
    NETWORK_CARD_SW_VERSION,       // E
    ATTRIBUTE_ID_END,
} atrributeIDs_e;

typedef enum
{
    UI_STATE_BROADCASTING,
    UI_STATE_CONNECTING,
    UI_STATE_CONNECTED,
} teUiStates;

/// @brief  Parameters of a Ccm process
typedef struct
{
    uint8_t targetSafirProcess;
    uint8_t targetFramerProcess;
    const tsAttrbuteRooter *attrbuteMap;
    uint8_t mapsize;
    tsTimerEvent writeAttributeMaxFrameSize;
    tsTimerEvent rxBufferCheck;
    tsTimerEvent getConnectivityStatus;
    tsTimerEvent getConnSubStateWifi;
    tsTimerEvent getSafirSwVersion;
    uint8_t ccmTxBuf[10];
} tsCcmParams;

/// @brief  Events of a Ccm process
typedef enum
{
    eCcmSafirTx, //ccmSafirRx messages
    eCcmRecievedReadAttribute,
    eCcmRecievedWriteAttribute,
    eCcmRecievedAttributeWritten,
    eCcmRecievedAttributeRead,
    eCcmRecievedAttributeEmitted,
    eCcmRecievedNotValidCommand,
} teCcmEvents;

/*********************************************************************
 *  EXPORTED MACROS
 */
/** @brief  Ccm process object creation macro
 *  @param  _name   Name of process object
 *  @param  _enum   Process enumeration for this object
 */
#define PROCESS_CCM_CREATE(_name, _enum, _map, _targetSafirProcess,_targetFramerProcess)          \
    tsCcmParams _name##Params =                                                                     \
    {                                                                                           \
            _targetSafirProcess,                                                                  \
            _targetFramerProcess,                                                                   \
            _map,                                                                                   \
            ARRAY_SIZE(_map),                                                                       \
            TIMER_EVENT_INIT(_enum, _enum, eCcmSendWriteAttMaxFrameSize),                           \
            TIMER_EVENT_INIT(_enum, _enum, eCcmGetUartRxBufCheck),                                  \
            TIMER_EVENT_INIT(_enum, _enum, eCcmGetConnectivityStatus),                              \
            TIMER_EVENT_INIT(_enum, _enum, eCcmGetConnectionSubStateWifi),                          \
            TIMER_EVENT_INIT(_enum, _enum, eCcmGetNetworkBoardSwVersion),                           \
            {0, 0},                                                                                 \
    };                                                                                              \
    PROCESS_CREATE(_name, ccmInit, ccmDeinit, _enum, &_name##Params, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_CCM_C
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

void sendAttrNotValid(void);

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_CCM_H
