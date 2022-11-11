/** @file       ticket.h
 *  @brief      Ticketing system to wait in a queue for a resource
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       18.10.2018
 */
#ifndef FILE_TICKET_H
#define FILE_TICKET_H

/** @defgroup   TICKET TICKET
 *  @ingroup    LIBS
 *  @brief      Create a queue that anyone can get a ticket and wait in line
 *              and only the ticket master can remove a ticket.
 *  @details    Usage:
 *              @code
 *                  tsTvm ticketMaster;
 *
 *                  ticketMasterIsr()
 *                  {
 *                      // If transaction is complete
 *                      TICKET_REMOVE( ticketMaster );      // Remove current ticket
 *                  }
 *
 *                  ...
 *
 *                  tTicket myTicket;
 *
 *                  ticketHolderProcess()
 *                  {
 *                      case EVENT_ENTER:
 *                          myTicket = TICKET_GET( ticketMaster );      // Get a ticket
 *                          // break; // Fall-through is intentional
 *                      case EVENT_TRY: // There's a fall-through
 *                          if( TICKET_IS_UP( ticketMaster, myTicket ) )// Wait in line until it is your turn
 *                          {
 *                              // You're up, conduct your business
 *                              ...
 *                          }
 *                          else
 *                          {
 *                              // resend EVENT_TRY after a timeout?
 *                              break;
 *                          }
 *                          // break; // Fall-through is intentional
 *                      case EVENT_EXIT: // There's a fall-through
 *                          if( TICKET_IS_DONE( ticketMaster, myTicket ) )   // It is done
 *                          {
 *                              ...
 *                          }
 *                          else
 *                          {
 *                              // send EVENT_EXIT after a timeout?
 *                          }
 *                          break;
 *                  }
 *              @endcode
 *  @warning    don't get more tickets than needed but get a ticket for every process
 *  @warning    max ticket count is limited by the size of tTicket and is not checked
 *  @todo       implement multiple ticket for a single process
 *  @{
 */

/// @brief  Ticket typedef
typedef uint16_t tTicket;

/// @brief  Ticket vending machine
typedef struct
{
    tTicket current; ///< Current ticket that is processed
    tTicket next;    ///< Next ticket to give out
} tsTvm;

/// @brief  Initial value for every ticket vending machine
#define TVM_INIT()  {0, 0}

/// @brief  Get a ticket from _tvm
#define TICKET_GET(_tvm) ((_tvm).next++)
/// @brief  Check if your _ticket is ready on _tvm
#define TICKET_IS_UP(_tvm, _ticket) ((_tvm).current == (_ticket))
/// @brief  Check if your _ticket is done on _tvm
#define TICKET_IS_DONE(_tvm, _ticket) ((_tvm).current != (_ticket))
/// @brief  Remove a ticket from _tvm
#define TICKET_REMOVE(_tvm)                \
    {                                      \
        if ((_tvm).current != (_tvm).next) \
        {                                  \
            (_tvm).current++;              \
        }                                  \
    }

/** @} */

#endif // FILE_TICKET_H
