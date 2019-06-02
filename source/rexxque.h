#if !defined(__RXU_REXXQUE__)

#define __RXU_REXXQUE__

#define RXQUEUE_LIFO   ((BOOL)1)     /* Type for RexxAddQueue */
#define RXQUEUE_FIFO   ((BOOL)0)     /* Type for RexxAddQueue */

#define RXQUEUE_WAIT   ((BOOL)1)     /* Type for RexxPullQueue */
#define RXQUEUE_NOWAIT ((BOOL)0)     /* Type for RexxPullQueue */

/* To add an element to a Rexx Queue */
INT APIENTRY RexxAddQueue(PSZ Name,
                          PRXSTRING Data,
                          BOOL type_lifo_or_fifo);

/*******************************************************************
To create a Rexx Queue:
   If "RequestedName" pointer is null, generated name is returned
   in "CreatedName".
   bDupName is boolean:  TRUE  = Queue "RequestedName" already exists
   bDupName is boolean:  FALSE = Queue "RequestedName" is new
*******************************************************************/
INT APIENTRY RexxCreateQueue(PSZ CreatedName,
                             ULONG CreatedNamelen,
                             PSZ RequestedName,
                             PULONG bDupName);

/* To delete a Rexx Queue */
INT APIENTRY RexxDeleteQueue(PSZ Name);

/* To query number of elements in a Rexx Queue */
INT APIENTRY RexxQueryQueue(PSZ Name,
                            PULONG Count);

/* To pull an element off a Rexx Queue */
/* Make "Data" a null RXSTRING to have Rexx supply the storage */
INT APIENTRY RexxPullQueue(PSZ Name,
                           PRXSTRING Data,
                           PDATETIME enqueuetime,
                           BOOL type_wait_or_nowait);

#endif
