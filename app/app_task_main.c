/******************************************************************************

 @file  app_task_main.c

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
#include "app.h"

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
extern void app_task_main_init( void )
{
    osal_timer_event_create( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_POR, 3000 );
}


extern void app_task_main ( uint8_t task_id, uint8_t event_id )
{
    uint8_t u8tmp;
    switch (event_id)
    {
        case TASK_EVT_APP_MAIN_POR:
        {
            hal_cli_print_str( "\r\nThis is Xounts Audio Dongle project.\r\n" );
            hal_cli_print_str( "Power on reset.\r\n" );

            u8tmp = hal_uibrd_hw_ver();
            hal_cli_print_str( "UI board HW version 0x" );
            hal_cli_print_hex8( u8tmp );
            hal_cli_print_str( ".\r\n" );

            u8tmp = hal_uibrd_sw_ver();
            hal_cli_print_str( "UI board SW version 0x" );
            hal_cli_print_hex8( u8tmp );
            hal_cli_print_str( ".\r\n" );

            osal_event_set(TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_UIBRD_POLL_IRQ);
        }
        break;

        case TASK_EVT_APP_MAIN_OSAL_EXCEPTION:
        {
            hal_cli_print_str( "ERROR!\r\n" );
            hal_cli_print_str( "OSAL_EXCEPTION!\r\n" );
        }
        break;

        case TASK_EVT_APP_MAIN_HAL_EXCEPTION:
        {
            hal_cli_print_str( "ERROR!\r\n" );
            hal_cli_print_str( "HAL EXCEPTION!\r\n" );
        }
        break;

        case TASK_EVT_APP_MAIN_APP_EXCEPTION:
        {
            hal_cli_print_str( "ERROR!\r\n" );
            hal_cli_print_str( "APP EXCEPTION!\r\n" );
        }
        break;
        

        default:
            APP_ASSERT_FORCED();
        break;
    }
}



/**************************************************************************************************
**************************************************************************************************/

