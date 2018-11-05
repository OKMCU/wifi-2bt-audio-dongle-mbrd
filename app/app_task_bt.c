/******************************************************************************

 @file  app_task_bt.c

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "osal.h"
#include "hal.h"

#include "app_config.h"
#include "app_assert.h"
#include "app_task_bt.h"
#include "app_event_bt.h"

#include "main.h"
/**************************************************************************************************
 * TYPES
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/


/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/**************************************************************************************************
 * LOCAL FUNCTION IMPLEMENTATIONS
 **************************************************************************************************/

/**************************************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS
 **************************************************************************************************/
 
/**************************************************************************************************
 * @fn      app_task_bt_init
 *
 * @brief   Bluetooth task init
 *
 * @param   None
 *          
 * @return  None
 **************************************************************************************************/
extern void app_task_bt_init ( void )
{
    
}


/**************************************************************************************************
 * @fn      app_task_bt
 *
 * @brief   Bluetooth process task
 *
 * @param   task_id - BT Task Id
 *          events - events
 *
 * @return  None
 **************************************************************************************************/
void app_task_bt( uint8_t task_id, uint8_t event_id )
{
    uint8_t *p_msg;
    uint8_t type;
    uint8_t state;

    APP_ASSERT(event_id == OSAL_TASK_EVENT_MSG);

    p_msg = (uint8_t *)osal_msg_recv(task_id);
    
    while(p_msg)
    {
        type = osal_msg_get_type(p_msg);
        state = *p_msg;
        if(type == TASK_MSG_APP_BT_BT0_STATE)
        {
            app_event_bt_state_update(HAL_BT_MOD_0, state);
        }
        else if(type == TASK_MSG_APP_BT_BT1_STATE)
        {
            app_event_bt_state_update(HAL_BT_MOD_1, state);
        }
        else
        {
            APP_ASSERT_FORCED();
        }

        osal_msg_delete( p_msg );
        p_msg = (uint8_t *)osal_msg_recv( task_id );
    }
}

/**************************************************************************************************
**************************************************************************************************/

