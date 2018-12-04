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

    switch ( event_id )
    {
        case OSAL_TASK_EVENT_MSG:
        {
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
        break;

        case TASK_EVT_APP_BT_TRIG_MOD0_PAIRING_STOP:
        {
            hal_bt_set_pin( HAL_BT_MOD_0, HAL_BT_PIN_PAIR, 0 );
        }
        break;

        case TASK_EVT_APP_BT_TRIG_MOD1_PAIRING_STOP:
        {
            hal_bt_set_pin( HAL_BT_MOD_1, HAL_BT_PIN_PAIR, 0 );
        }
        break;

        default:
            APP_ASSERT_FORCED();
        break;
    }
    

}

/**************************************************************************************************
**************************************************************************************************/

