/**
 *  @file       list_sl.h
 *  @brief      Singly Linked List structures
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       8.5.2020
 */
#ifndef FILE_LIST_SL_H
#define FILE_LIST_SL_H

#include <stddef.h>
#include <stdint.h>

#ifndef FILE_LIST_SL_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   LIST_SL LIST_SL
 *  @ingroup    LIBS
 *  @brief      Singly Linked List structures and functions.
 *  @todo       Allow tsLslItem to be placed on random location in target structure
 *  @details    Ex:
 *              @code
 *                  typedef struct
 *                  {
 *                      tsLslItem  _li;
 *                      uint32_t   data;
 *                      uint16_t   *anotherData;
 *                  }tsMyStruct;
 *                  tsMyStruct exampleItem1;
 *                  tsMyStruct exampleItem2;
 *                  tsMyStruct exampleItem3;
 *                  tsMyStruct exampleItem4;
 *                  LIST_SL_CREATE( myList );
 *                  ...
 *                  main()
 *                  {
 *                      ...
 *                      lslInsertTail( &mylist, &exampleItem1 );
 *                      lslInsertTail( &mylist, &exampleItem3 );
 *                      ...
 *                      tsMyStruct *iteration;
 *                      LIST_SL_FOREACH( iteration, &mylist )
 *                      {
 *                          ...
 *                      }
 *                  }
 *              @endcode
 *  @{
 */

/// @brief      List item structure
/// @details    Place as object heading
typedef struct _tsLslItem
{
    struct _tsLslItem *next; ///< Next item in list
} tsLslItem;

/// @brief      List structure
typedef struct
{
    tsLslItem *next; ///< First item in list
    uint32_t count;  ///< Item count for checking
} tsLsl;

/// @brief  Initialize data of a list item
#define LIST_SL_ITEM_INIT() \
    {                       \
        .next = NULL        \
    }

/// @brief  Initialize data of a list
#define LIST_SL_INIT() \
    {                  \
        .next  = NULL, \
        .count = 0     \
    }

/** @brief  Create and initialise a list
 *  @param  _name of list object
 */
#define LIST_SL_CREATE(_name) \
    tsLsl _name = LIST_SL_INIT();

/** @brief      For loop header to process a list
 *  @warning    Does not support lslDelete inside the loop
 *  @param      _itemPtr pointer to be used to target list item during loop
 *  @param      _listPtr pointer to the list
 */
#define LIST_SL_FOREACH(_itemPtr, _listPtr)     \
    for ((_itemPtr) = (void *)(_listPtr)->next; \
         (_itemPtr) != NULL;                    \
         (_itemPtr) = (void *)((tsLslItem *)_itemPtr)->next)

/** @brief  Insert a newItem after target item in the list
 *  @param  list to add newItem into
 *  @param  item used as reference
 *  @param  newItem to be added into list
 */
INTERFACE void lslInsertAfter(tsLsl *list, void *item, void *newItem);

/** @brief  Insert a newItem before target item in the list
 *  @param  list to add newItem into
 *  @param  item used as reference
 *  @param  newItem to be added into list
 */
INTERFACE void lslInsertBefore(tsLsl *list, void *item, void *newItem);

/** @brief  Insert a newItem to the head of a list
 *  @param  list to add newItem into
 *  @param  newItem to be added into list
 */
INTERFACE void lslInsertHead(tsLsl *list, void *newItem);

/** @brief  Insert a newItem to the tail of a list
 *  @param  list to add newItem into
 *  @param  newItem to be added into list
 */
INTERFACE void lslInsertTail(tsLsl *list, void *newItem);

/** @brief  Delete an item from list
 *  @param  list to be modified
 *  @param  item to be removed
 */
INTERFACE void lslDelete(tsLsl *list, void *item);

/** @brief  Remove head item from list and return its pointer
 *  @param  list Pointer to the list that will be modified
 *  @return Pointer to head object
 */
INTERFACE void *lslPopHead(tsLsl *list);

/** @brief  Remove tail item from list and return its pointer
 *  @param  list Pointer to the list that will be modified
 *  @return Pointer to tail object
 */
INTERFACE void *lslPopTail(tsLsl *list);

/** @brief  Find the list tail from target item
 *  @param  item to be used as reference
 *  @return Tail item of the list
 */
INTERFACE void *lslTailFrom(void *item);

/** @brief  Recount number of items in a list and return parsed count
 *  @param  list Pointer to list that will be parsed
 *  @return Total number of items in the list
 */
INTERFACE uint32_t lslRecount(tsLsl *list);

/** @brief  Pull the item pointer at index
 *  @param  list Pointer to list that will be parsed
 *  @param  idx index number of target item
 *  @return Pointer to the indexed item
 */
INTERFACE void *lslItemAtIdx(tsLsl *list, uint32_t idx);

/** @brief  Pull the item pointer at index
 *  @param  list Pointer to list that will be parsed
 *  @param  item Pointer to the target item
 *  @retval (uint32_t)-1ul if item did not found
 *  @retval Index number of the item
 */
INTERFACE uint32_t lslIdxOfItem(tsLsl *list, void *item);

/** @brief Adding list_sl version to platform versioning
 */
INTERFACE void addListslVersion();

/** @} */

#undef INTERFACE

#endif // FILE_LIST_SL_H
