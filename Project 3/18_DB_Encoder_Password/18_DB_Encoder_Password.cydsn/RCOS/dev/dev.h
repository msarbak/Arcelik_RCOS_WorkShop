/** @file       dev.h
 *  @brief      RCoS+ device driver abstraction structures and definitions
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       5.11.2018
 */
#ifndef FILE_DEV_H
#define FILE_DEV_H

#include <stdarg.h> ///< included for va_list in devComPrint
#include "libs/libs.h"

/**
 *  @addtogroup RCoS
 *  @{
 *      @defgroup   DEV DEV
 *      @brief      Device abstraction which allows flexible programming
 *      @{
 *          @defgroup   DEV_IO DEV_IO
 *          @brief      Single data input and/or output devices
 *
 *          @defgroup   DEV_COM DEV_COM
 *          @brief      Communication devices
 *
 *          @defgroup   DEV_MEM DEV_MEM
 *          @brief      Memory devices
 *
 *          @defgroup   DEV_CPX DEV_CPX
 *          @brief      Complex devices with multiple functionalities that cannot be grouped with others
 *
 *          @defgroup   PLATFORM PLATFORM
 *          @brief      CPU specific functions and definitions
 *      @}
 *  @}
 */
#ifndef FILE_DEV_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @addtogroup DEV_IO
 *  @{
 */

/// @brief  I/O device abstraction object structure
typedef struct _tsDevIo tsDevIo;

/// @brief  DEV_IO init function prototype
#define DEV_IO_FUNC_INIT(_name) uint8_t _name(const tsDevIo *device, const void *config)
/// @brief  DEV_IO deinit function prototype
#define DEV_IO_FUNC_DEINIT(_name) uint8_t _name(const tsDevIo *device)
/// @brief  DEV_IO get function prototype
#define DEV_IO_FUNC_GET(_name) uint32_t _name(const tsDevIo *device)
/// @brief  DEV_IO put function prototype
#define DEV_IO_FUNC_PUT(_name) uint8_t _name(const tsDevIo *device, uint32_t data)

/// @brief  I/O device generic functions
typedef struct _tsDevIoFuncs
{
    DEV_IO_FUNC_INIT((*init));     ///< Initialize device with a configuration
    DEV_IO_FUNC_DEINIT((*deinit)); ///< Deinitialize device
    DEV_IO_FUNC_GET((*get));       ///< Get data from I/O device
    DEV_IO_FUNC_PUT((*put));       ///< Put a data to I/O device
} tsDevIoFuncs;                    ///< Generic functions required from I/O devices

/// @brief I/O device system control mechanisms
typedef struct _tsDevIoSystem
{
    uint8_t initialized; ///< Device initialized flag
} tsDevIoSystem;

struct _tsDevIo
{
    const tsDevIoFuncs *functions; ///< Device specific functions
    void *parameters;              ///< Device specific parameters
    const void *constants;         ///< Device specific constants
    tsDevIoSystem *sys;            ///< Device driver system values
};

/** @brief      Default devIo creation macro
 *  @details    Used by device creation macros as a main macro to create the device and connect to a specific driver
 *  @param      _name       Name of the device object
 *  @param      _devIoFuncs tsDevIoFuncs structure that will be used for this device
 *  @param      _paramsPtr  Pointer to parameters object that will be used for this device
 *  @param      _constsPtr  Pointer to constants object that will be used for this device
 */
#define DEV_IO_CREATE(_name, _devIoFuncs, _paramsPtr, _constsPtr) \
    tsDevIoSystem _name##Sys = {0};                               \
    const tsDevIo _name =                                         \
        {                                                         \
            .functions  = &_devIoFuncs,                           \
            .parameters = (void *)_paramsPtr,                     \
            .constants  = (const void *)_constsPtr,               \
            .sys        = &_name##Sys,                            \
    };

/** @brief  Initialize an I/O device with a pointer to device specific configuration structure
 *  @param  device I/O device pointer
 *  @param  config pointer to device specific configuration structure
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_IO_FUNC_INIT(devIoInit);

/** @brief  Deinitialize an I/O device
 *  @param  device I/O device pointer
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_IO_FUNC_DEINIT(devIoDeinit);

/** @brief  Get a single data from I/O device
 *  @param  device I/O device pointer
 *  @return data
 */
INTERFACE DEV_IO_FUNC_GET(devIoGet);

/** @brief  Put a single data to I/O device
 *  @param  device I/O device pointer
 *  @param  data value of data to place
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_IO_FUNC_PUT(devIoPut);

/** @} */

/**
 *  @addtogroup DEV_COM
 *  @{
 */

/// @brief  Communication device abstraction object structure
typedef struct _tsDevCom tsDevCom;

/// @brief  Common target object for devCom
typedef struct _tsTarget tsTarget;

/// @brief  DEV_COM init function prototype
#define DEV_COM_FUNC_INIT(_name) uint8_t _name(const tsDevCom *device)
/// @brief  DEV_COM deinit function prototype
#define DEV_COM_FUNC_DEINIT(_name) uint8_t _name(const tsDevCom *device)
/// @brief  DEV_COM open function prototype
#define DEV_COM_FUNC_OPEN(_name) uint8_t _name(const tsDevCom *device, tsTarget *target)
/// @brief  DEV_COM close function prototype
#define DEV_COM_FUNC_CLOSE(_name) uint8_t _name(const tsDevCom *device, tsTarget *target)
/// @brief  DEV_COM send function prototype
#define DEV_COM_FUNC_SEND(_name) uint16_t _name(const tsDevCom *device, const void *txb, uint16_t length)
/// @brief  DEV_COM receive function prototype
#define DEV_COM_FUNC_RECEIVE(_name) uint16_t _name(const tsDevCom *device, void *rxb, uint16_t length)
/// @brief  DEV_COM ticket get function prototype
#define DEV_COM_FUNC_TICKET_GET(_name) tTicket _name(const tsDevCom *device)
/// @brief  DEV_COM ticket check function prototype
#define DEV_COM_FUNC_TICKET_VALID(_name) teBool _name(const tsDevCom *device, tTicket ticket)

/// @brief  Generic functions required from communication devices
typedef struct _tsDevComFuncs
{
    DEV_COM_FUNC_INIT((*init));                ///< Initialize device
    DEV_COM_FUNC_DEINIT((*deinit));            ///< Deinitialize device
    DEV_COM_FUNC_OPEN((*open));                ///< Open a channel for the target on communication device
    DEV_COM_FUNC_CLOSE((*close));              ///< Close the channel for the target
    DEV_COM_FUNC_SEND((*send));                ///< Send data to last opened target
    DEV_COM_FUNC_RECEIVE((*receive));          ///< Receive data from last opened target
    DEV_COM_FUNC_TICKET_GET((*ticketGet));     ///< Get a ticket from com device
    DEV_COM_FUNC_TICKET_VALID((*ticketValid)); ///< Check if ticket is valid for an operation
} tsDevComFuncs;

/// @brief  Communication device system control mechanisms
typedef struct _tsDevComSystem
{
    tsTvm tvm;           ///< Ticket Vending Machine
    uint8_t initialized; ///< Device initialized flag
    tsTarget *opened;    ///< Currently open target
    tsTarget *closed;    ///< Target that is ready for closure
} tsDevComSystem;

/// @brief  Communication device abstraction object structure
struct _tsDevCom
{
    const tsDevComFuncs *functions; ///< Device specific functions
    void *parameters;               ///< Device specific parameters
    const void *constants;          ///< Device specific constants
    tsDevComSystem *sys;            ///< Device driver system values
};

/** @brief      Default devCom creation macro
 *  @details    Used by device creation macros as a main macro to create the device and connect to a specific driver
 *  @param      _name           Name of the device object
 *  @param      _devComFuncs    tsDevComFuncs structure that will be used for this device
 *  @param      _paramsPtr      Pointer to Parameters object that will be used for this device
 *  @param      _constsPtr      Pointer to constants object that will be used for this device
 */
#define DEV_COM_CREATE(_name, _devComFuncs, _paramsPtr, _constsPtr) \
    tsDevComSystem _name##Sys = {TVM_INIT(), 0, 0, 0};              \
    const tsDevCom _name =                                          \
        {                                                           \
            .functions  = &_devComFuncs,                            \
            .parameters = (void *)_paramsPtr,                       \
            .constants  = (const void *)_constsPtr,                 \
            .sys        = &_name##Sys,                              \
    };

/** @brief  Initialize a communication device
 *  @param  device communication device pointer
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_COM_FUNC_INIT(devComInit);

/** @brief  Deinitialize a communication device
 *  @param  device communication device pointer
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_COM_FUNC_DEINIT(devComDeinit);

/** @brief  Open a channel to target on communication device
 *  @param  device communication device pointer
 *  @param  target pointer to target configuration
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_COM_FUNC_OPEN(devComOpen);

/** @brief  Close the channel to target on communication device
 *  @param  device communication device pointer
 *  @param  target pointer to target configuration
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_COM_FUNC_CLOSE(devComClose);

/** @brief  Send data to last opened target
 *          If length is entered as 0 returns amount of space available in transmit buffers
 *  @param  device communication device pointer
 *  @param  txb pointer to buffer area where data is
 *  @param  length length of data
 *  @return length length of data send
 */
INTERFACE DEV_COM_FUNC_SEND(devComSend);

/** @brief  Receive data from last opened target
 *          If length is entered as 0 returns amount of data in receive buffers
 *  @param  device communication device pointer
 *  @param  rxb pointer to buffer area where received data will be placed
 *  @param  length length of data
 *  @return length length of data received
 */
INTERFACE DEV_COM_FUNC_RECEIVE(devComReceive);

/** @brief  Get a ticket from communication device
 *  @param  device communication device pointer
 *  @return Ticket value with type tTicket
 */
INTERFACE DEV_COM_FUNC_TICKET_GET(devComTicketGet);

/** @brief  Generic devComTicketGet function
 *  @param  _devicePtr Pointer to device
 */
#define DEV_COM_FUNC_TICKET_GET_GENERIC(_devicePtr)  \
    {                                                \
        /* if (_devicePtr->sys->initialized) */      \
        {                                            \
            return TICKET_GET(_devicePtr->sys->tvm); \
        }                                            \
    }

/** @brief  Check the validity of a ticket for target communication device
 *  @param  device communication device pointer
 *  @param  ticket compare current queue number with this ticket
 *  @retval 0 this ticket is not processed right now
 *  @retval 1 ticket is valid for a single operation
 */
INTERFACE DEV_COM_FUNC_TICKET_VALID(devComTicketValid);

/** @brief  Generic devComTicketValid function
 *  @param  _devicePtr Pointer to device
 */
#define DEV_COM_FUNC_TICKET_VALID_GENERIC(_devicePtr)          \
    {                                                          \
        if (_devicePtr->sys->initialized)                      \
        {                                                      \
            return TICKET_IS_UP(_devicePtr->sys->tvm, ticket); \
        }                                                      \
    }

/** @brief      Send a formatted text through com device
 *  @warning    Result text is limited to 256 bytes
 *  @param      device Communication device pointer
 *  @param      format Pointer to a format text
 *  @param      ... Arguments that will placed into formatted text
 *  @return     length Length of data send
 */
INTERFACE uint16_t devComPrint(const tsDevCom *device, const char *format, ...);

/** @brief      Send a formatted text through com device while arguments are supplied as va_list
 *  @warning    Result text is limited to 256 bytes
 *  @param      device Communication device pointer
 *  @param      format Pointer to a format text
 *  @param      args Argument list that will placed into formatted text
 *  @return     length Length of data send
 */
INTERFACE uint16_t _devComVPrint(const tsDevCom *device, const char *format, va_list args);

/// @brief  Common target object for devCom
struct _tsTarget
{
    const void *config; ///< Device specific configuration if any
    union {
        const tsDevIo *cs;  ///< SPI: Target device chip select
        const tsDevIo *txe; ///< UART: Transmit enable
        struct
        {
            uint8_t regAdrSize; ///< I2C: Registry address size(1 or 2 byte(s))
            uint8_t slave;      ///< I2C: 7-bit slave address
        };
    };
};

/** @} */

/**
 *  @addtogroup DEV_MEM
 *  @{
 */

/// @brief  Memory device abstraction object structure
typedef struct _tsDevMem tsDevMem; ///< Abstract device object for memory devices

/// @brief  DEV_MEM init function prototype
#define DEV_MEM_FUNC_INIT(_name) uint8_t _name(const tsDevMem *device)
/// @brief  DEV_MEM deinit function prototype
#define DEV_MEM_FUNC_DEINIT(_name) uint8_t _name(const tsDevMem *device)
/// @brief  DEV_MEM read function prototype
#define DEV_MEM_FUNC_READ(_name) uint16_t _name(const tsDevMem *device, uint32_t address, void *readData, uint16_t length)
/// @brief  DEV_MEM write function prototype
#define DEV_MEM_FUNC_WRITE(_name) uint16_t _name(const tsDevMem *device, uint32_t address, const void *writeData, uint16_t length)
/// @brief  DEV_MEM program function prototype
#define DEV_MEM_FUNC_PROGRAM(_name) uint16_t _name(const tsDevMem *device, uint32_t address, const void *progData, uint16_t length)
/// @brief  DEV_MEM erase function prototype
#define DEV_MEM_FUNC_ERASE(_name) uint32_t _name(const tsDevMem *device, uint32_t address, uint32_t size)
/// @brief  DEV_MEM ticket get function prototype
#define DEV_MEM_FUNC_TICKET_GET(_name) tTicket _name(const tsDevMem *device)
/// @brief  DEV_MEM ticket validation function prototype
#define DEV_MEM_FUNC_TICKET_VALID(_name) teBool _name(const tsDevMem *device, tTicket ticket)

/// @brief  Generic functions required from memory devices
typedef struct _tsDevMemFuncs
{
    DEV_MEM_FUNC_INIT((*init));                ///< Initialize device
    DEV_MEM_FUNC_DEINIT((*deinit));            ///< Deinitialize device
    DEV_MEM_FUNC_READ((*read));                ///< Read data from address
    DEV_MEM_FUNC_WRITE((*write));              ///< Write data to address
    DEV_MEM_FUNC_PROGRAM((*program));          ///< Program data to address(set bits to 0)
    DEV_MEM_FUNC_ERASE((*erase));              ///< Erase target area
    DEV_MEM_FUNC_TICKET_GET((*ticketGet));     ///< Get a new ticket
    DEV_MEM_FUNC_TICKET_VALID((*ticketValid)); ///< Check if ticket is valid for an operation
} tsDevMemFuncs;

/// @brief  Memory device system control mechanisms
typedef struct _tsDevMemSystem
{
    tsTvm tvm;           ///< Ticket Vending Machine
    uint8_t initialized; ///< Device initialized flag
} tsDevMemSystem;

struct _tsDevMem
{
    const tsDevMemFuncs *functions; ///< Device specific functions
    void *parameters;               ///< Device specific parameters
    const void *constants;          ///< Device specific constants
    tsDevMemSystem *sys;            ///< Device driver system values
};

/** @brief      Default devMem creation macro
 *  @details    Used by device creation macros as a main macro to create the device and connect to a specific driver
 *  @param      _name           Name of the device object
 *  @param      _devMemFuncs    tsDevMemFuncs structure that will be used for this device
 *  @param      _paramsPtr      Pointer to parameters object that will be used for this device
 *  @param      _constsPtr      Pointer to constants object that will be used for this device
 */
#define DEV_MEM_CREATE(_name, _devMemFuncs, _paramsPtr, _constsPtr) \
    tsDevMemSystem _name##Sys = {TVM_INIT(), 0};                    \
    const tsDevMem _name =                                          \
        {                                                           \
            .functions  = &_devMemFuncs,                            \
            .parameters = (void *)_paramsPtr,                       \
            .constants  = (const void *)_constsPtr,                 \
            .sys        = &_name##Sys,                              \
    };

/** @brief  Initialize a memory device
 *  @param  device memory device pointer
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_MEM_FUNC_INIT(devMemInit);

/** @brief  Deinitialize a memory device
 *  @param  device memory device pointer
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_MEM_FUNC_DEINIT(devMemDeinit);

/** @brief  Read data from address on memory device
 *  @param  device      memory device pointer
 *  @param  address     memory address
 *  @param  readData    pointer to buffer to read data into
 *  @param  length      length of data to read
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_MEM_FUNC_READ(devMemRead);

/** @brief  Write data to address on memory device
 *  @param  device      memory device pointer
 *  @param  address     memory address
 *  @param  writeData   pointer to buffer to read data into
 *  @param  length      length of data to read
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_MEM_FUNC_WRITE(devMemWrite);

/** @brief  Program data to address on memory device( set 1 to 0 )
 *  @param  device      memory device pointer
 *  @param  address     memory address
 *  @param  progData    pointer to buffer to read data into
 *  @param  length      length of data to read
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_MEM_FUNC_PROGRAM(devMemProgram);

/** @brief  Erase target area on memory device( set to 1 )
 *  @param  device  memory device pointer
 *  @param  address memory address
 *  @param  size    size of area
 *  @return erased area size
 */
INTERFACE DEV_MEM_FUNC_ERASE(devMemErase);

/** @brief  Get a ticket from memory device
 *  @param  device memory device pointer
 *  @return Ticket value with type tTicket
 */
INTERFACE DEV_MEM_FUNC_TICKET_GET(devMemTicketGet);

/** @brief  Generic deMemTicketGet function
 *  @param  _devicePtr Pointer to device
 */
#define DEV_MEM_FUNC_TICKET_GET_GENERIC(_devicePtr)  \
    {                                                \
        /* if (_devicePtr->sys->initialized) */      \
        {                                            \
            return TICKET_GET(_devicePtr->sys->tvm); \
        }                                            \
    }

/** @brief  Check the validity of a ticket for target memory device
 *  @param  device memory device pointer
 *  @param  ticket compare current queue number with this ticket
 *  @retval 0 this ticket is not processed right now
 *  @retval 1 ticket is valid for a single operation
 */
INTERFACE DEV_MEM_FUNC_TICKET_VALID(devMemTicketValid);

/** @brief  Generic devMemTicketValid function
 *  @param  _devicePtr Pointer to device
 */
#define DEV_MEM_FUNC_TICKET_VALID_GENERIC(_devicePtr)          \
    {                                                          \
        if (_devicePtr->sys->initialized)                      \
        {                                                      \
            return TICKET_IS_UP(_devicePtr->sys->tvm, ticket); \
        }                                                      \
    }

/** @} */

/**
 *  @addtogroup DEV_CPX
 *  @{
 */

/// @brief  Complex device abstraction object structure
typedef struct _tsDevCpx tsDevCpx;

/// @brief  DEV_CPX init function prototype
#define DEV_CPX_FUNC_INIT(_name) uint8_t _name(const tsDevCpx *device, const void *config)
/// @brief  DEV_CPX deinit function prototype
#define DEV_CPX_FUNC_DEINIT(_name) uint8_t _name(const tsDevCpx *device)

/// @brief  Complex device generic functions
typedef struct _tsDevCpxFuncs
{
    DEV_CPX_FUNC_INIT((*init));     ///< Initialize device
    DEV_CPX_FUNC_DEINIT((*deinit)); ///< Deinitialize device
} tsDevCpxFuncs;

/// @brief  Complex device system control mechanisms
typedef struct _tsDevCpxSystem
{
    tsTvm tvm;           ///< Ticket Vending Machine
    uint8_t initialized; ///< Device initialized flag
} tsDevCpxSystem;

struct _tsDevCpx
{
    const tsDevCpxFuncs *functions; ///< Device specific functions
    void *parameters;               ///< Device specific parameters
    const void *constants;          ///< Device specific constants
    tsDevCpxSystem *sys;            ///< Device driver system values
};

/** @brief      Default devCpx creation macro
 *  @details    Used by device creation macros as a main macro to create the device and connect to a specific driver
 *  @param      _name           Name of the device object
 *  @param      _devCpxFuncs    tsDevCpxFuncs structure that will be used for this device
 *  @param      _paramsPtr      Pointer to parameters object that will be used for this device
 *  @param      _constsPtr      Pointer to constants object that will be used for this device
 */
#define DEV_CPX_CREATE(_name, _devCpxFuncs, _paramsPtr, _constsPtr) \
    tsDevCpxSystem _name##Sys = {TVM_INIT(), 0};                    \
    const tsDevCpx _name =                                          \
        {                                                           \
            .functions  = &_devCpxFuncs,                            \
            .parameters = (void *)_paramsPtr,                       \
            .constants  = (const void *)_constsPtr,                 \
            .sys        = &_name##Sys,                              \
    };

/** @brief  Initialize a complex device
 *  @param  device complex device pointer
 *  @param  config pointer to device specific configuration structure
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_CPX_FUNC_INIT(devCpxInit);

/** @brief  Initialize a complex device
 *  @param  device complex device pointer
 *  @return EXIT_FAILURE or EXIT_SUCCESS
 */
INTERFACE DEV_CPX_FUNC_DEINIT(devCpxDeinit);

/** @brief Adding dev version to platform versioning
 */
INTERFACE void addDevVersion();
/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_DEV_H
