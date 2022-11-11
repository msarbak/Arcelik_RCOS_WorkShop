/**
 *  @file       deque.h
 *  @brief      Generic structures and functions to create double ended queues
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *  @author     Deniz Fer
 *  @date       12.10.2018
 */
#ifndef FILE_DEQUE_H
#define FILE_DEQUE_H

/** @addtogroup LIBS
 *  @{
 *      @defgroup   DEQUE DEQUE
 *      @brief      Creates and manages a structure to use a buffer area as a FIFO or LIFO queue.
 *      @details    Usage:
 *                  @code
 *                      DEQUE_CREATE( rxFifo, 256 );
 *                      DEQUE_CREATE( stack, 512 );
 *                      ...
 *                      main()
 *                      {
 *                          ...
 *                          // Use rxFifo to store incoming data
 *                          FIFO_PUSH( &rxFifo, incoming, sizeof(incoming) );
 *                          FIFO_PUSH( &rxFifo, incoming, sizeof(incoming) );
 *                          FIFO_PUSH( &rxFifo, incoming, sizeof(incoming) );
 *                          ...
 *                          // Parse incoming data as in order it has arrived
 *                          FIFO_POP( &rxFifo, data1, sizeof(data1) );
 *                          FIFO_POP( &rxFifo, data2, sizeof(data2) );
 *                          ...
 *                          // Place procedural information on a stack
 *                          LIFO_PUSH( &stack, info1, sizeof(info1) );
 *                          LIFO_PUSH( &stack, info2, sizeof(info2) );
 *                          ...
 *                          // Get procedure information from stak
 *                          LIFO_POP( &stack, data2, sizeof(data2) );
 *                          LIFO_POP( &stack, data1, sizeof(data1) );
 *                          ...
 *                      }
 *                  @endcode
 *      @{
 *          @defgroup   FIFO FIFO
 *          @brief      First-In First-Out queues
 *          @defgroup   LIFO LIFO
 *          @brief      Last-In First-Out queues
 */

#include <stdint.h>

#ifndef FILE_DEQUE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */
/// Structure definition of a DEQUE object
typedef struct
{
    uint8_t *buffer; ///< Location of buffer
    uint16_t size;   ///< Total size of buffer
    uint16_t count;  ///< Amount of data in buffer
    uint16_t front;  ///< Front of the queue
    uint16_t back;   ///< Back of the queue
} tsDeque;

/*********************************************************************
 *  EXPORTED MACROS
 */

/// @brief  Fill a tsDeque object with inital data
#define DEQUE_INIT(_array)        \
    {                             \
        .buffer = _array,         \
        .size   = sizeof(_array), \
        .count  = 0,              \
        .front  = 0,              \
        .back   = 0               \
    }

/** @brief  Create a deque object with name and a buffer area with size
 *  @param  _name   Name of deque object
 *  @param  _size   Size of buffer area used
 */
#define DEQUE_CREATE(_name, _size)  \
    uint8_t _name##Buffer[(_size)]; \
    tsDeque _name = DEQUE_INIT(_name##Buffer);

/// @brief  Return amount of data in DEQUE
#define DEQUE_LOAD(_deqPtr) ((_deqPtr)->count)
/// @brief  Return amount of free space in DEQUE
#define DEQUE_FREE(_deqPtr) ((_deqPtr)->size - (_deqPtr)->count)
/// @brief  Expression to check if DEQUE is empty
#define DEQUE_IS_EMPTY(_deqPtr) (!((_deqPtr)->count))
/// @brief  Expression to check if DEQUE is full
#define DEQUE_IS_FULL(_deqPtr) ((_deqPtr)->size == (_deqPtr)->count)

/** @addtogroup FIFO
 *  @{
 */
/// @brief  Fill a FIFO object with inital data
#define FIFO_INIT(_array) DEQUE_INIT(_array)
/** @brief  Create a FIFO object with name and a buffer area with size
 *  @param  _name   Name of FIFO object
 *  @param  _size   Size of buffer area used
 */
#define FIFO_CREATE(_name, _size) DEQUE_CREATE(_name, _size)
/// @brief  Return amount of data in FIFO
#define FIFO_LOAD(_fifoPtr) DEQUE_LOAD(_fifoPtr)
/// @brief  Return amount of free space in FIFO
#define FIFO_FREE(_fifoPtr) DEQUE_FREE(_fifoPtr)
/// @brief  Expression to check if FIFO is empty
#define FIFO_IS_EMPTY(_fifoPtr) DEQUE_IS_EMPTY(_fifoPtr)
/// @brief  Expression to check if FIFO is full
#define FIFO_IS_FULL(_fifoPtr) DEQUE_IS_FULL(_fifoPtr)
/// @brief      Flush the data in FIFO object
/// @warning    Does not clear the data
#define FIFO_FLUSH(_fifoPtr) dequeFlush(_fifoPtr)
/// @brief  Clear the data buffer in FIFO
#define FIFO_CLEAR(_fifoPtr) dequeClear(_fifoPtr)
/// @brief  Push new data into FIFO
#define FIFO_PUSH(_fifoPtr, _src, _size) dequePushBack(_fifoPtr, _src, _size)
/// @brief  Pop data from FIFO
#define FIFO_POP(_fifoPtr, _dest, _size) dequePopFront(_fifoPtr, _dest, _size)
/** @} */

/** @addtogroup LIFO
 *  @{
 */
/// @brief  Fill a LIFO object with inital data
#define LIFO_INIT(_array) DEQUE_INIT(_array)
/** @brief  Create a LIFO object with name and a buffer area with size
 *  @param  _name   Name of LIFO object
 *  @param  _size   Size of buffer area used
 */
#define LIFO_CREATE(_name, _size) DEQUE_CREATE(_name, _size)
/// @brief  Return amount of data in LIFO
#define LIFO_LOAD(_lifoPtr) DEQUE_LOAD(_lifoPtr)
/// @brief  Return amount of free space in LIFO
#define LIFO_FREE(_lifoPtr) DEQUE_FREE(_lifoPtr)
/// @brief  Expression to check if LIFO is empty
#define LIFO_IS_EMPTY(_lifoPtr) DEQUE_IS_EMPTY(_lifoPtr)
/// @brief  Expression to check if LIFO is full
#define LIFO_IS_FULL(_lifoPtr) DEQUE_IS_FULL(_lifoPtr)
/// @brief      Flush the data in LIFO object
/// @warning    Does not clear the data
#define LIFO_FLUSH(_lifoPtr) dequeFlush(_lifoPtr)
/// @brief  Clear the data buffer in LIFO
#define LIFO_CLEAR(_lifoPtr) dequeClear(_lifoPtr)
/// @brief  Push new data into LIFO
#define LIFO_PUSH(_lifoPtr, _src, _size) dequePushFront(_lifoPtr, _src, _size)
/// @brief  Pop data from LIFO
#define LIFO_POP(_lifoPtr, _dest, _size) dequePopFront(_lifoPtr, _dest, _size)
/** @} */

/*********************************************************************
 *  EXPORTED VARIABLES
 */

/*********************************************************************
 *  EXPORTED FUNCTION PROTOTYPES
 */

/** @brief      Works as if a data is pushed to the back of deque
 *  @warning    Does not modify data, only updates the indices
 *  @param      deq     Deque pointer
 *  @param      size    Amount that will be changed on indices
 */
INTERFACE void dequeAddBack(tsDeque *deq, uint16_t size);

/** @brief      Works as if a data is pushed to the front of deque
 *  @warning    Does not modify data, only updates the indices
 *  @param      deq     Deque pointer
 *  @param      size    Amount that will be changed on indices
 */
INTERFACE void dequeAddFront(tsDeque *deq, uint16_t size);

/** @brief      Works as if a data is poped from the back of deque
 *  @warning    Does not modify data, only updates the indices
 *  @param      deq     Deque pointer
 *  @param      size    Amount that will be changed on indices
 */
INTERFACE void dequeRemBack(tsDeque *deq, uint16_t size);

/** @brief      Works as if a data is poped from the front of deque
 *  @warning    Does not modify data, only updates the indices
 *  @param      deq     Deque pointer
 *  @param      size    Amount that will be changed on indices
 */
INTERFACE void dequeRemFront(tsDeque *deq, uint16_t size);

/** @brief  Unvalidate all data in buffer
 *  @param  deq pointer
 */
INTERFACE void dequeFlush(tsDeque *deq);

/** @brief  Set all data in buffer to '0' and flush
 *  @param  deq pointer
 */
INTERFACE void dequeClear(tsDeque *deq);

/** @brief  Reads as much as possible and returns the amount read
 *  @param  deq pointer
 *  @param  destination buffer pointer
 *  @param  size of data to peek
 *  @return uint16_t
 */
INTERFACE uint16_t dequePopBack(tsDeque *deq, void *destination, uint16_t size);

/** @brief  Reads as much as possible and returns the amount read
 *  @param  deq pointer
 *  @param  destination buffer pointer
 *  @param  size of data to peek
 *  @return uint16_t
 */
INTERFACE uint16_t dequePopFront(tsDeque *deq, void *destination, uint16_t size);

/** @brief  Place a series of bytes into buffer and overwrite if necessary
 *  @param  deq pointer
 *  @param  source buffer pointer
 *  @param  size of data to write into deq
 *  @return uint16_t
 */
INTERFACE uint16_t dequePushBack(tsDeque *deq, const void *source, uint16_t size);

/** @brief  Place a series of bytes into buffer and overwrite if necessary
 *  @param  deq pointer
 *  @param  source buffer pointer
 *  @param  size of data to write into deq
 *  @return uint16_t
 */
INTERFACE uint16_t dequePushFront(tsDeque *deq, const void *source, uint16_t size);

/** @brief  Peek value at target index of buffer from the front
 *  @param  deq         Dequeue pointer
 *  @param  index       Index to read from
 *  @param  destination Target buffer pointer
 *  @param  size        Size of data to read from deq
 */
INTERFACE void dequePeek(tsDeque *deq, uint16_t index, void *destination, uint16_t size);

/** @brief Adding deque version to platform versioning
 */
INTERFACE void addDequeVersion();

#undef INTERFACE

/**     @} */
/** @} */

#endif // FILE_DEQUE_H
