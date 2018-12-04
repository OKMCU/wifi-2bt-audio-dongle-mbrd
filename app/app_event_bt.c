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

#include "main.h"
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


extern void app_event_bt_state_update( void )
{
    const char *p_names[] = {
        "UNKNOWN",
        "OFF",
        "CONNECTABLE",
        "DISCOVERABLE",
        "CONNECTED(PAUSED)",
        "CONNECTED(PALYING)"
    };
    uint8_t bt_state[2];
    uint8_t i;
    
    bt_state[0] = hal_bt_get_state( HAL_BT_MOD_0 );
    bt_state[1] = hal_bt_get_state( HAL_BT_MOD_1 );

    for( i = 0; i < 2; i++ )
    {
        if( bt_state[i] != app_info.bt_state[i] )
        {
            switch ( bt_state[i] )
            {
                case HAL_BT_STATE_OFF:
                {
                    if( i == 0 )
                    {
                        LED_BT0_IND_STATE_OFF();
                    }
                    else
                    {
                        LED_BT1_IND_STATE_OFF();
                    }
                }
                break;

                case HAL_BT_STATE_CONNECTABLE:
                    //does not exist this state
                break;

                case HAL_BT_STATE_DISCOVERABLE:
                {
                    if( i == 0 )
                    {
                        LED_BT0_IND_STATE_DISCOVERABLE();
                    }
                    else
                    {
                        LED_BT1_IND_STATE_DISCOVERABLE();
                    }
                }
                break;

                case HAL_BT_STATE_PAUSED:
                {
                    if( i == 0 )
                    {
                        LED_BT0_IND_STATE_CONNECTED_PAUSED();
                    }
                    else
                    {
                        LED_BT1_IND_STATE_CONNECTED_PAUSED();
                    }
                }
                break;

                case HAL_BT_STATE_PLAYING:
                {
                    if( i == 0 )
                    {
                        LED_BT0_IND_STATE_CONNECTED_PLAYING();
                    }
                    else
                    {
                        LED_BT1_IND_STATE_CONNECTED_PLAYING();
                    }
                }
                break;

                case HAL_BT_STATE_UNKNOWN:
                    //wait, do not update the LED state
                break;
            }
            hal_cli_print_str( "BT" );
            hal_cli_print_uint( i );
            hal_cli_print_str( " state = " );
            hal_cli_print_str( p_names[bt_state[i]] );
            hal_cli_print_str( ".\r\n" );
        }
    
    }

    if( bt_state[0] == HAL_BT_STATE_PLAYING || bt_state[1] == HAL_BT_STATE_PLAYING )
    {
        app_info.src = AUDIO_SOURCE_BT;
        hal_dsp_set_vol( 0 );
        DSP_SET_CHANNEL_BT();
        hal_wifimod_set_src( HAL_WIFIMOD_SRC_EXT_SRC_CTL_BY_MCU );
        osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
    }
    else if( bt_state[0] == HAL_BT_STATE_OFF && bt_state[1] == HAL_BT_STATE_OFF )
    {
        app_info.src = AUDIO_SOURCE_NONE;
        hal_dsp_set_vol( 0 );
        DSP_SET_CHANNEL_NONE();
        hal_wifimod_set_src( HAL_WIFIMOD_SRC_NONE );
    }

    if( app_info.bt_mode == BT_MODE_PARTY )
    {
        if( bt_state[0] == HAL_BT_STATE_PAUSED && bt_state[1] == HAL_BT_STATE_PAUSED )
        {
            if( app_info.bt_state[0] != bt_state[0] )
            {
                osal_timer_event_create( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT1, BT_PARTY_MODE_TIMEOUT );
            }
            else
            {
                osal_timer_event_create( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT0, BT_PARTY_MODE_TIMEOUT );
            }
        }
        else
        {
            osal_timer_event_delete( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT0 );
            osal_timer_event_delete( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT1 );

            if( app_info.bt_state[0] != bt_state[0] )
            {
                if( bt_state[0] == HAL_BT_STATE_PLAYING )
                {
                    hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_PAIRING );
                }
            }

            if( app_info.bt_state[1] != bt_state[1] )
            {
                if( bt_state[1] == HAL_BT_STATE_PLAYING )
                {
                    hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_PAIRING );
                }
            }
        }
    }
    else if( app_info.bt_mode == BT_MODE_MIXER )
    {
        if( app_info.bt_state[0] != bt_state[0] )
        {
            if( bt_state[0] == HAL_BT_STATE_PAUSED )
            {
                osal_timer_event_create( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT0, BT_MIXER_MODE_TIMEOUT );
            }
            else
            {
                osal_timer_event_delete( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT0 );
            }
        }

        if( app_info.bt_state[1] != bt_state[1] )
        {
            if( bt_state[1] == HAL_BT_STATE_PAUSED )
            {
                osal_timer_event_create( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT1, BT_MIXER_MODE_TIMEOUT );
            }
            else
            {
                osal_timer_event_delete( TASK_ID_APP_BT, TASK_EVT_APP_BT_DISCONNECT_BT1 );
            }
        }
    }
    
    app_info.bt_state[0] = bt_state[0];
    app_info.bt_state[1] = bt_state[1];
}
 
/**************************************************************************************************
**************************************************************************************************/

