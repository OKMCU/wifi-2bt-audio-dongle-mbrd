/******************************************************************************

 @file  app_event_bt.c

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "osal.h"
#include "hal.h"
#include "app_config.h"
#include "app_assert.h"
#include "app_event_bt.h"
#include "app_info.h"

#include <string.h>
#include "stringx.h"
/**************************************************************************************************
 * TYPE DEFINES
 **************************************************************************************************/

 /**************************************************************************************************
 * LOCAL API DECLARATION
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/


extern void app_event_bt_state_update( uint8_t mod, uint8_t state )
{
    const char *p_names[] = {
        "UNKNOWN",
        "OFF",
        "CONNECTABLE",
        "DISCOVERABLE",
        "CONNECTED(PAUSED)",
        "CONNECTED(PALYING)"
    };

    APP_ASSERT(mod <= HAL_BT_MOD_1);
    APP_ASSERT(state <= HAL_BT_STATE_PLAYING);

    hal_cli_print_str( "BT" );
    hal_cli_print_uint( mod == HAL_BT_MOD_0 ? 0 : 1 );
    hal_cli_print_str( " state = " );
    hal_cli_print_str( p_names[state] );
    hal_cli_print_str( ".\r\n" );

    switch ( state )
    {
        case HAL_BT_STATE_OFF:
            if( mod == HAL_BT_MOD_0 )
            {
                LED_BT0_IND_STATE_OFF();
            }
            else
            {
                LED_BT1_IND_STATE_OFF();
            }
        break;

        case HAL_BT_STATE_CONNECTABLE:
            //should not exist this state
        break;

        case HAL_BT_STATE_DISCOVERABLE:
            if( mod == HAL_BT_MOD_0 )
            {
                LED_BT0_IND_STATE_DISCOVERABLE();
            }
            else
            {
                LED_BT1_IND_STATE_DISCOVERABLE();
            }
        break;

        case HAL_BT_STATE_PAUSED:
            if( mod == HAL_BT_MOD_0 )
            {
                LED_BT0_IND_STATE_CONNECTED_PAUSED();
            }
            else
            {
                LED_BT1_IND_STATE_CONNECTED_PAUSED();
            }
        break;

        case HAL_BT_STATE_PLAYING:
            if( mod == HAL_BT_MOD_0 )
            {
                LED_BT0_IND_STATE_CONNECTED_PLAYING();
            }
            else
            {
                LED_BT1_IND_STATE_CONNECTED_PLAYING();
            }
        break;

        case HAL_BT_STATE_UNKNOWN:
            //wait, do not the LED state
        break;
    }
    

    if( app_info.bt_mode == BT_MODE_SINGLE )
    {
        
        return;
    }

    if( app_info.bt_mode == BT_MODE_PARTY )
    {
        
        return;
    }

    if( app_info.bt_mode == BT_MODE_MIXER )
    {
        
        return;
    }
    
    if( app_info.bt_mode == BT_MODE_OFF )
    {
        
        return;
    }

    
    
}
 
/**************************************************************************************************
**************************************************************************************************/

