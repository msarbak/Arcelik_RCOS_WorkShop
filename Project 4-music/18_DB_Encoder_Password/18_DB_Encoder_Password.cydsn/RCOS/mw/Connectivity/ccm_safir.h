/** @file       ccm_safir.h
 *  @brief      ccm_safir
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     mertGemalmaz
 *  @date       27.11.2019
 */
#ifndef FILE_CCMSAFIR_H
#define FILE_CCMSAFIR_H

/**
 *  @defgroup   CCM_SAFIR
 *  @ingroup    MW
 *  @brief      ccm_safir
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "rcos.h"
#include "ccm.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

typedef enum
{
    step1Broadcasting,
    step2,
    step3connecting,
    step4connected
} teOnboardingScreens;

typedef struct PLATFORM_PACKED
{
    uint8_t region;            // 0: non China 1:china
    uint8_t networkBoardSw[3]; // 0:major 1:minor 3:build
} tsNbSwVersion;

typedef enum
{
    eCommFail,
    eCommStarted,
}teGdmCommStatus;

/// @brief  Parameters of a CcmSafir process
typedef struct
{
    const tsAttrbuteRooter *attrbuteMap;
    uint8_t                 mapsize;
    uint8_t                 carrierProcess;
    tProcessEnum            uiProcess;
    tEventEnum              uiEvent;
    tsTimerEvent            tevframeSizeAtt;
    tsTimerEvent            tevConnStatWifiAttr;
    tsTimerEvent            tevConnSubState;
    tsTimerEvent            tevNwBoardSwVerAttr;
    uint16_t                maxFrameSize;
    uint8_t                 connStateWifi;
    uint8_t                 connSubState;
    uint16_t                suppportConnTypes;
    tsTimerEvent            tevBarisTest;
    tsTimerEvent            tevCheckOnboarding;
    tsTimerEvent            tevCommCheck;
    tProcessEnum            commStatProcess;
    tEventEnum              commStatEvent;
} tsCcmSafirParams;

/// @brief  Events of a CcmSafir process
typedef enum
{
    eCcmSafirRxHandler,
    maxFramesizeHandler,
    connStateWifiHandler,
    connSubStateHandler,
    supportedConnTypeHandler,

    eCcmSendWriteAttMaxFrameSize,  //once
    eCcmGetUartRxBufCheck,         //1000 ms period
    eCcmGetConnectivityStatus,     //1000 ms period
    eCcmGetConnectionSubStateWifi, //3000 ms period
    eCcmGetNetworkBoardSwVersion,  //once

    AttrHandlerMaxFrameSize,
    AttrHandlerDeviceDescription,
    AttrHandlerSupportedConnTypes,
    AttrHandlerCommMode,
    AttrHandlerMacAddress,
    AttrHandlerCommProtocolVersion,
    AttrHandlerConnStateWifi,
    AttrHandlerStoredSSID,
    AttrHandlerConnSubStateWifi,
    AttrHandlerOnboardingTimeOutDurationSec,
    AttrHandlerCustomizedName,
    AttrHandlerConnBoardId,
    AttrHandlerOtaState,
    AttrHandlerNetworkCardSwVersion,

    eCcmwriteAttMaxFrameSizeOta,
    eCcmSafirTest_sendconnStateWifi,
    eCcmStartOnboarding,
    eCcmCheckOnboarding,
    eCcmCheckHomewhizConnected,
    eCcmHomewhizStopBroadcasting,
    eCcmArrangeMaxFrameSize,

    eCcmSafirCheckCommStarted,
    eCcmSafirGetCommStatus,
    eCcmSafirCheckCommStatus,
} teCcmSafirEvents;

/*********************************************************************
 *  EXPORTED MACROS
 */
/** @brief  CcmSafir process object creation macro
 *  @param  _name   Name of process object
 *  @param  _enum   Process enumeration for this object
 */
#define PROCESS_CCMSAFIR_CREATE(_name, _enum, _map, _carrierProcess, _uiProcess, _uiEvent,_commStatProcess, _commStatEvent) \
    tsCcmSafirParams _name##Params =                                                                                        \
        {                                                                                                                   \
            .attrbuteMap            = _map,                                                                                 \
            .mapsize                = ARRAY_SIZE(_map),                                                                     \
            .carrierProcess         = _carrierProcess,                                                                      \
            .uiProcess              = _uiProcess,                                                                           \
            .uiEvent                = _uiEvent,                                                                             \
            .tevframeSizeAtt        = TIMER_EVENT_INIT(_enum, _enum, eCcmSendWriteAttMaxFrameSize),                         \
            .tevConnStatWifiAttr    = TIMER_EVENT_INIT(_enum, _enum, eCcmGetConnectivityStatus),                            \
            .tevConnSubState        = TIMER_EVENT_INIT(_enum, _enum, eCcmGetConnectionSubStateWifi),                        \
            .tevNwBoardSwVerAttr    = TIMER_EVENT_INIT(_enum, _enum, eCcmGetNetworkBoardSwVersion),                         \
            .maxFrameSize           = 0,                                                                                    \
            .connStateWifi          = 0,                                                                                    \
            .connSubState           = 0,                                                                                    \
            .suppportConnTypes      = 0,                                                                                    \
            .tevBarisTest           = TIMER_EVENT_INIT(_enum, _enum, eCcmSafirTest_sendconnStateWifi),                      \
            .tevCheckOnboarding     = TIMER_EVENT_INIT(_enum, _enum, eCcmCheckOnboarding),                                  \
            .tevCommCheck           = TIMER_EVENT_INIT(_enum, _enum, eCcmSafirCheckCommStatus),                             \
            .commStatProcess        = _commStatProcess,                                                                     \
            .commStatEvent          = _commStatEvent,                                                                       \
    };                                                                                                                      \
    PROCESS_CREATE(_name, ccmSafirInit, ccmSafirDeinit, _enum, &_name##Params, NULL)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_CCMSAFIR_C
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
extern uint8_t dummyAttribute; //TODO: Never used in db side
extern uint8_t connStateWifi;
extern uint8_t tempConnStateWifi;
extern uint8_t connSubStateWifi;
extern uint16_t maxFrameSizeApplience;
extern uint16_t suppConnTypes;
extern uint8_t storedSSID[9];
extern tsNbSwVersion nbSwVersion;

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_CCMSAFIR_H
