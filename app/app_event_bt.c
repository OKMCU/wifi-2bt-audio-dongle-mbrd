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
    hal_cli_print_uint( mod );
    hal_cli_print_str( " state = " );
    hal_cli_print_str( p_names[state] );
    hal_cli_print_str( ".\r\n" );
    
}
 
/**************************************************************************************************
**************************************************************************************************/
