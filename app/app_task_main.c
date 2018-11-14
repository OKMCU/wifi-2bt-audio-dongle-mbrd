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
    switch (event_id)
    {
        case TASK_EVT_APP_MAIN_POR:
        {
            app_event_main_por();
        }
        break;

        case TASK_EVT_APP_MAIN_INIT:
        {
            app_event_main_init();
        }
        break;

        case TASK_EVT_APP_MAIN_LED_WIFIR_CRS_BLK:
        {
            app_event_main_led_wifir_cross_blink();
        }
        break;

        case TASK_EVT_APP_MAIN_LED_WIFIB_CRS_BLK:
        {
            app_event_main_led_wifib_cross_blink();
        }
        break;

        case TASK_EVT_APP_MAIN_SET_DSP_VOL:
        {
            app_event_main_set_dsp_vol();
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

