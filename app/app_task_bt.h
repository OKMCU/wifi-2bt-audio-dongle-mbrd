/******************************************************************************

 @file  app_task_bt.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#ifndef __APP_TASK_BT_H__
#define __APP_TASK_BT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdint.h>

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/


/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/


/**************************************************************************************************
 * FUNCTIONS - API
 **************************************************************************************************/

extern void app_task_bt_init ( void );
extern void app_task_bt ( uint8_t task_id, uint8_t event_id );



#ifdef __cplusplus
}
#endif

#endif

/**************************************************************************************************
**************************************************************************************************/
