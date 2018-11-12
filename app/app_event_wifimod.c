/******************************************************************************

 @file  app_event_wifimod.c

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
#include "app_event_wifimod.h"

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

extern void app_event_wifimod_bootup( void )
{
    hal_cli_print_str( "Wi-Fi module boot up.\r\n" );
}

extern void app_event_wifimod_reboot( void )
{
    hal_cli_print_str( "Wi-Fi module rebooting.\r\n" );
}

extern void app_event_wifimod_update_mode( uint8_t mode )
{
    hal_cli_print_str( "Wi-Fi module mode is " );
    hal_cli_print_uint( mode );
    hal_cli_print_str( ".\r\n" );

    switch (mode)
    {
        case HAL_WIFIMOD_MODE_CFG:
            LED_WIFI_IND_MODE_CFG();
        break;

        case HAL_WIFIMOD_MODE_HN:
            LED_WIFI_IND_MODE_HN();
        break;

        case HAL_WIFIMOD_MODE_SA:
            LED_WIFI_IND_MODE_SA();
        break;

        default:
            APP_ASSERT_FORCED();
        break;
    }
}

extern void app_event_wifimod_update_source( uint8_t src )
{
    hal_cli_print_str( "Wi-Fi module src is " );
    hal_cli_print_uint( src );
    hal_cli_print_str( ".\r\n" );
}

extern void app_event_wifimod_update_volume( uint8_t vol )
{
    hal_cli_print_str( "Wi-Fi module vol is " );
    hal_cli_print_uint( vol );
    hal_cli_print_str( ".\r\n" );
}

extern void app_event_wifimod_update_state( uint8_t state )
{
    hal_cli_print_str( "Wi-Fi module play state is " );
    hal_cli_print_uint( state );
    hal_cli_print_str( ".\r\n" );
}
 
/**************************************************************************************************
**************************************************************************************************/

