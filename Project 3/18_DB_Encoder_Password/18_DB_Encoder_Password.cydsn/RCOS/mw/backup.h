/** @file       backup.h
 *  @brief      Backup or restore a list of variables using an NVM
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     
 *  @date       7.05.2019
 */
#ifndef FILE_BACKUP_H
#define FILE_BACKUP_H

/**
 *  @defgroup   BACKUP BACKUP
 *  @ingroup    MW
 *  @brief      Backup or restore a list of variables using an NVM
 *  @details    During initialization backup process tries to read the NVM to
 *              restore all data items. After initialization it runs an update
 *              operation each second if there're any data items that are changed.
 *  @warning    THERE CAN BE ONLY ONE.
 *  @warning    BLOCKING OPERATION.
 *  @warning    [8,4]Hamming coding is used, hence everything is doubled in size.
 *  @warning    Each item is limited to 4 bytes at maximum.
 *  @warning    If the backup data is corrupted, has a different size or not present they're replaced with current data.
 *  @code
 *      +-------------------+
 *      |   tsBackupHeader  +
 *      +-------------------+
 *      |   item0 cpy0      |
 *      |   item1 cpy0      |
 *      |   item2 cpy0      |
 *      +-------------------+
 *      |   item0 cpy1      |
 *      |   item1 cpy1      |
 *      |   item2 cpy1      |
 *      +-------------------+
 *  @endcode
 *  @{
 */

/** INCLUDES ******************************************************************/
#include "rcos.h"
#include "libs/hamming.h" // Dependant on hamming code
#include "libs/crc.h"     // Dependent on crc 16 ccitt

/// @brief  Backup area header
typedef struct PLATFORM_PACKED
{
#define BACKUP_SIGNATURE (0x2c05)
    uint16_t signature; ///< Check if there's already a backup here
    uint16_t itemArrId; ///< Check if the itemArr is changed
    uint16_t totalSize; ///< Total size of array items
} tsBackupHeader;

/// @brief  Backup item definition structure
typedef struct
{
    void *address;  ///< Address of item
    uint8_t length; ///< Length of item
} tsBackupItem;

/// @brief  Backup item initializer
#define BACKUP_ITEM(_variable)        \
    {                                 \
        &_variable, sizeof(_variable) \
    }

/// @brief  Parameters of a Backup process
typedef struct
{
    tsTimerEvent tev;            ///< Event timer to be used for periodic maintenance
    tsBackupHeader header;       ///< Header structure stored in NVM
    const tsDevMem *nvm;         ///< Target NVM device
    uint32_t start;              ///< Starting address of backup area on nvm
    const tsBackupItem *itemArr; ///< Data item's array
    uint16_t *crcBuffer;         ///< CRC buffer to check changes
    uint16_t period;             ///< Backup update period
    tTicket nvmTicket;           ///< NVM device ticket
    uint8_t itemArrSize;         ///< Data item's count
} tsBackupParams;

/** EXPORTED MACROS ***********************************************************/

/** @brief  Backup process object creation macro
 *  @param  _name           Name of process object
 *  @param  _mem            NVM device to be used
 *  @param  _start          Starting address of backup area on nvm
 *  @param  _itemArr        tsBackupItem array for selecting memory areas for backup
 *  @param  _updatePeriod   Period time in miliseconds for backup update operation
 */
#define PROCESS_BACKUP_CREATE(_name, _mem, _start, _itemArr, _updatePeriod) \
    uint16_t _name##Crc[ARRAY_SIZE(_itemArr)];                              \
    tsBackupParams _name##Params =                                          \
        {                                                                   \
            TIMER_EVENT_INIT(PROCESS_NONE, PROCESS_NONE, EVENT_NONE),       \
            {0, 0, 0},            /* header */                              \
            &_mem,                /* nvm */                                 \
            _start,               /* start */                               \
            _itemArr,             /* itemArr */                             \
            _name##Crc,           /* crcBuffer */                           \
            _updatePeriod,        /* period */                              \
            0,                    /* nvmTicket */                           \
            ARRAY_SIZE(_itemArr), /* itemArrSize */                         \
    };                                                                      \
    PROCESS_CREATE(_name, backupInit, backupDeinit, PROCESS_NONE, &_name##Params, NULL)

/** INTERFACES ****************************************************************/

#ifndef FILE_BACKUP_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** INTERFACES: FUNCTIONS *****************************************************/

/// @brief      Emergency write function
/// @details    Write everything marked as backup data into NVM
/// @warning    BLOCKING FUNCTION
INTERFACE void backupWriteAll(void);

/// @brief      Trigger an update operation inside backup process
/// @details    Checks each item and updates if item has changed
INTERFACE void backupUpdateTrigger(void);

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_BACKUP_H
