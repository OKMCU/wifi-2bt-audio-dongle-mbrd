/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#include "osal_task.h"
#include "osal_config.h"
#include "osal_port.h"
#include "osal_msg.h"
#include <string.h>


OSAL_TASK_t osal_task_list[ OSAL_TASK_MAX ];

#if ( OSAL_EVENT_MAX == 32 )
uint32_t osal_event_list[ OSAL_TASK_MAX ];
#elif ( OSAL_EVENT_MAX == 16 )
uint16_t osal_event_list[ OSAL_TASK_MAX ];
#elif ( OSAL_EVENT_MAX == 8 )
uint8_t  osal_event_list[ OSAL_TASK_MAX ];
#else
#error "OSAL_EVENT_MAX must be 8, 16 or 32"
#endif


extern void    osal_task_init   ( void )
{
    memset( osal_task_list, 0, sizeof(osal_task_list) );
    memset( osal_event_list, 0, sizeof(osal_event_list) );
}


extern void osal_task_create ( OSAL_TASK_t pfn_task, uint8_t task_id )
{
    OSAL_ASSERT( task_id < OSAL_TASK_MAX );
    OSAL_ASSERT( pfn_task != NULL );
    OSAL_ASSERT( osal_task_list[task_id] == NULL );
    osal_task_list[task_id] = pfn_task;
}

extern void osal_task_delete ( uint8_t task_id )
{
    void *p_msg;
    OSAL_ASSERT( task_id < OSAL_TASK_MAX );

    p_msg = osal_msg_recv( task_id );
    while( p_msg != NULL )
    {
        osal_msg_delete( p_msg );
        p_msg = osal_msg_recv( task_id );
    }
    
    osal_task_list[task_id] = NULL;
    osal_event_list[task_id] = 0;
}

extern void osal_event_set   ( uint8_t task_id, uint8_t event_id )
{
#if (OSAL_EVENT_MAX == 32)
    uint32_t event;
#elif (OSAL_EVENT_MAX == 16)
    uint16_t event;
#elif (OSAL_EVENT_MAX == 8)
    uint8_t  event;
#else
#error "OSAL_EVENT_MAX must be 8, 16 or 32"
#endif
    
    OSAL_ASSERT( task_id < OSAL_TASK_MAX);
    OSAL_ASSERT( event_id < OSAL_EVENT_MAX );
    OSAL_ASSERT( osal_task_list[task_id] != NULL );
    
    event = ( 1 << event_id );
    
    OSAL_ENTER_CRITICAL();
    osal_event_list[task_id] |= event;
    OSAL_EXIT_CRITICAL();
}

extern void osal_event_clr   ( uint8_t task_id, uint8_t event_id )
{
#if (OSAL_EVENT_MAX == 32)
    uint32_t event;
#elif (OSAL_EVENT_MAX == 16)
    uint16_t event;
#elif (OSAL_EVENT_MAX == 8)
    uint8_t  event;
#else
#error "OSAL_EVENT_MAX must be 8, 16 or 32"
#endif
    
    OSAL_ASSERT( task_id < OSAL_TASK_MAX);
    OSAL_ASSERT( event_id < OSAL_EVENT_MAX );
    OSAL_ASSERT( osal_task_list[task_id] != NULL );
    
    event = ( 1 << event_id );
    event = ~event;
    
    OSAL_ENTER_CRITICAL();
    osal_event_list[task_id] &= event;
    OSAL_EXIT_CRITICAL();
    
}




















