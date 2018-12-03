/******************************************************************************

 @file  app_task_uibrd.c

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
#include "app_task_uibrd.h"

#include "main.h"

#include <string.h>
#include "stringx.h"
/**************************************************************************************************
 * TYPES
 **************************************************************************************************/

/**************************************************************************************************
 * MACROS
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/


/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
static uint8_t bt_state[2];

extern void app_task_uibrd_init ( void )
{
     bt_state[0] = HAL_BT_STATE_OFF;
     bt_state[1] = HAL_BT_STATE_OFF;
}


/**************************************************************************************************
 * @fn      app_task_uibrd
 *
 * @brief   app_task_uibrd
 *
 * @param   task_id - Task Id
 *          events - events
 *
 * @return  None
 **************************************************************************************************/
extern void app_task_uibrd( uint8_t task_id, uint8_t event_id )
{
    uint8_t *p_msg;
    uint8_t state;

    switch ( event_id )
    {
        case TASK_EVT_APP_UIBRD_UPD_KEY:
        {
            p_msg = (uint8_t *)osal_msg_create(sizeof(uint8_t));
            APP_ASSERT(p_msg != NULL);
            *p_msg = hal_key_read();
            osal_msg_send(p_msg, TASK_ID_APP_KEY);
        }
        break;

        case TASK_EVT_APP_UIBRD_UPD_BT:
        {
            state = hal_bt_get_state(HAL_BT_MOD_0);
            if(state != bt_state[0])
            {
                bt_state[0] = state;
                p_msg = (uint8_t *)osal_msg_create(sizeof(uint8_t));
                APP_ASSERT(p_msg != NULL);
                *p_msg = state;
                osal_msg_set_type(p_msg, TASK_MSG_APP_BT_BT0_STATE);
                osal_msg_send(p_msg, TASK_ID_APP_BT);
            }

            state = hal_bt_get_state(HAL_BT_MOD_1);
            if(state != bt_state[1])
            {
                bt_state[1] = state;
                p_msg = (uint8_t *)osal_msg_create(sizeof(uint8_t));
                APP_ASSERT(p_msg != NULL);
                *p_msg = state;
                osal_msg_set_type(p_msg, TASK_MSG_APP_BT_BT1_STATE);
                osal_msg_send(p_msg, TASK_ID_APP_BT);
            }
        }
        break;
        
        default:
            APP_ASSERT_FORCED();
        break;
    }
}

/**************************************************************************************************
**************************************************************************************************/

