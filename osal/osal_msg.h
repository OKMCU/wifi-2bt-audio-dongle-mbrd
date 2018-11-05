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
#ifndef __OSAL_MSG_H__
#define __OSAL_MSG_H__

#include <stdint.h>

#if ( OSAL_MSG_EN > 0 )
extern void        osal_msg_init         ( void );
extern void       *osal_msg_create       ( uint16_t len );
extern void        osal_msg_delete       ( void *p_msg );
extern void        osal_msg_send         ( void *p_msg, uint8_t task_id );
extern void        osal_msg_fwrd         ( void *p_msg, uint8_t task_id );
extern void       *osal_msg_recv         ( uint8_t task_id );
extern uint16_t    osal_msg_len          ( void *p_msg );
extern uint8_t     osal_msg_get_type     ( void *p_msg );
extern void        osal_msg_set_type     ( void *p_msg, uint8_t type );
#endif /* ( OSAL_MSG_EN > 0 ) */

#endif //__OSAL_MSG_H__

