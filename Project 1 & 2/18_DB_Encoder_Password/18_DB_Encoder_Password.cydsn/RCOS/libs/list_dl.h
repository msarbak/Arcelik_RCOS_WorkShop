/**
 *  @file       list_dl.h
 *  @brief      Doubly Linked List structures
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       19.10.2018
 */
#ifndef FILE_LIST_DL_H
#define FILE_LIST_DL_H

#include <stddef.h>
#include <stdint.h>

#ifndef FILE_LIST_DL_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   LIST_DL LIST_DL
 *  @ingroup    LIBS
 *  @brief      Double Linked List structures and functions.
 *  @todo       Allow tsLdlItem to be placed on random location in target structure
 *  @details    Ex:
 *              @code
 *                  typedef struct
 *                  {
 *                      tsLdlItem  _li;
 *                      uint32_t   data;
 *                      uint16_t   *anotherData;
 *                  }tsMyStruct;
 *                  tsMyStruct exampleItem1;
 *                  tsMyStruct exampleItem2;
 *                  tsMyStruct exampleItem3;
 *                  tsMyStruct exampleItem4;
 *                  LIST_DL_CREATE( myList );
 *                  ...
 *                  main()
 *                  {
 *                      ...
 *                      ldlInsertTail( &mylist, &exampleItem1 );
 *                      ldlInsertTail( &mylist, &exampleItem3 );
 *                      ...
 *                      tsMyStruct *iteration;
 *                      LIST_DL_FOREACH( iteration, &mylist )
 *                      {
 *                          ...
 *                      }
 *                  }
 *              @endcode
 *  @{
 */

/// @brief      List item structure
/// @details    Place as object heading
typedef struct _tsLdlItem
{
    struct _tsLdlItem *head; ///< Previous item in list
    struct _tsLdlItem *tail; ///< Next item in list
} tsLdlItem;

/// @brief      List structure
typedef struct
{
    tsLdlItem *head; ///< First item in list
    tsLdlItem *tail; ///< Last item in list
    uint32_t count;  ///< Item count for checking
} tsLdl;

/// @brief  Initialize data of a list item
#define LIST_DL_ITEM_INIT() \
    {                       \
        NULL, NULL          \
    }

/// @brief  Initialize data of a list
#define LIST_DL_INIT() \
    {                  \
        NULL, NULL, 0  \
    }

/** @brief  Create and initialise a list
 *  @param  _name of list object
 */
#define LIST_DL_CREATE(_name) \
    tsLdl _name = LIST_DL_INIT();

/** @brief      For loop header to process a list
 *  @warning    Does not support ldlDelete inside the loop
 *  @param      _itemPtr pointer to be used to target list item during loop
 *  @param      _listPtr pointer to the list
 */
#define LIST_DL_FOREACH(_itemPtr, _listPtr)     \
    for ((_itemPtr) = (void *)(_listPtr)->head; \
         (_itemPtr) != NULL;                    \
         (_itemPtr) = (void *)((tsLdlItem *)_itemPtr)->tail)

/** @brief  Insert a newItem after target item in the list
 *  @param  list to add newItem into
 *  @param  item used as reference
 *  @param  newItem to be added into list
 */
INTERFACE void ldlInsertAfter(tsLdl *list, void *item, void *newItem);

/** @brief  Insert a newItem before target item in the list
 *  @param  list to add newItem into
 *  @param  item used as reference
 *  @param  newItem to be added into list
 */
INTERFACE void ldlInsertBefore(tsLdl *list, void *item, void *newItem);

/** @brief  Insert a newItem to the head of a list
 *  @param  list to add newItem into
 *  @param  newItem to be added into list
 */
INTERFACE void ldlInsertHead(tsLdl *list, void *newItem);

/** @brief  Insert a newItem to the tail of a list
 *  @param  list to add newItem into
 *  @param  newItem to be added into list
 */
INTERFACE void ldlInsertTail(tsLdl *list, void *newItem);

/** @brief  Delete an item from list
 *  @param  list to be modified
 *  @param  item to be removed
 */
INTERFACE void ldlDelete(tsLdl *list, void *item);

/** @brief  Remove head item from list and return its pointer
 *  @param  list Pointer to the list that will be modified
 *  @return Pointer to head object
 */
INTERFACE void *ldlPopHead(tsLdl *list);

/** @brief  Remove tail item from list and return its pointer
 *  @param  list Pointer to the list that will be modified
 *  @return Pointer to tail object
 */
INTERFACE void *ldlPopTail(tsLdl *list);

/** @brief  Find the list head from target item
 *  @param  item to be used as reference
 *  @return Head item of the list
 */
INTERFACE void *ldlHeadFrom(void *item);

/** @brief  Find the list tail from target item
 *  @param  item to be used as reference
 *  @return Tail item of the list
 */
INTERFACE void *ldlTailFrom(void *item);

/** @brief  Recount number of items in a list and return parsed count
 *  @param  list to be parsed
 *  @return number of items
 */
INTERFACE uint32_t ldlRecount(tsLdl *list);

/** @brief  Pull the item pointer at index
 *  @param  list to be parsed
 *  @param  idx index number of target item
 *  @return item pointer
 */
INTERFACE void *ldlItemAtIdx(tsLdl *list, uint32_t idx);

/** @brief  Pull the item pointer at index
 *  @param  list to be parsed
 *  @param  item target item pointer
 *  @return item pointer
 */
INTERFACE uint32_t ldlIdxOfItem(tsLdl *list, void *item);

/** @brief Adding list_dl version to platform versioning
 */
INTERFACE void addListdlVersion();

/** @} */

#undef INTERFACE

#endif // FILE_LIST_DL_H
