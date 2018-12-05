/******************************************************************************

 @file  app_event_key.c

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


extern void app_event_key_update( uint8_t keyValue, uint8_t keyEvent )
{
    uint16_t keyMerge;
    uint8_t i;
    uint8_t cnt;
    
    
    const char *pKeyEvent[] = {
        "LEAVE",
        "ENTER",
        "SHORT",
        "LONG",
        "VLONG"
    };

    const char *pKeyName[] = {
        "WIFI",
        "BT",
        "AUX",
        "VOL-",
        "VOL+"
    };
    
    hal_cli_print_str( "KEY=" );
    for( i = 0, cnt = 0; i < sizeof(pKeyName)/sizeof(const char *); i++ )
    {
        if( keyValue & BV(i) )
        {
            if( cnt != 0 )
            {
                hal_cli_print_char( '+' );
            }
            hal_cli_print_str( pKeyName[i] );
            cnt++;
        }
    }
    hal_cli_print_char( ' ' );
    hal_cli_print_str( pKeyEvent[keyEvent] );
    hal_cli_print_str( "\r\n" );

    keyMerge = BUILD_UINT16( keyValue, keyEvent );

    switch( keyMerge )
    {
        case BUILD_UINT16( HAL_KEY_WIFI, APP_EVENT_KEY_SHORT ):
        {
            if( app_info.wifi_state_curr == WIFI_STATE_SA )
            {
                LED_WIFI_IND_MODE_HN_INIT();
                hal_wifimod_set_mode( HAL_WIFIMOD_MODE_HN );
                app_info.wifi_state_prev = app_info.wifi_state_curr;
                app_info.wifi_state_curr = WIFI_STATE_HN_INIT;
            }
            else if( app_info.wifi_state_curr == WIFI_STATE_HN )
            {
                LED_WIFI_IND_MODE_SA_INIT();
                hal_wifimod_set_mode( HAL_WIFIMOD_MODE_SA );
                app_info.wifi_state_prev = app_info.wifi_state_curr;
                app_info.wifi_state_curr = WIFI_STATE_SA_INIT;
            }
            else if( app_info.wifi_state_curr == WIFI_STATE_CFG )
            {
                if( app_info.wifi_state_prev == WIFI_STATE_HN )
                {
                    LED_WIFI_IND_MODE_HN_INIT();
                    hal_wifimod_set_mode( HAL_WIFIMOD_MODE_HN );
                    app_info.wifi_state_prev = app_info.wifi_state_curr;
                    app_info.wifi_state_curr = WIFI_STATE_HN_INIT;
                }
                else
                {
                    LED_WIFI_IND_MODE_SA_INIT();
                    hal_wifimod_set_mode( HAL_WIFIMOD_MODE_SA );
                    app_info.wifi_state_prev = app_info.wifi_state_curr;
                    app_info.wifi_state_curr = WIFI_STATE_SA_INIT;
                }
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_WIFI, APP_EVENT_KEY_LONG ):
        {
            if( app_info.wifi_state_curr == WIFI_STATE_HN || app_info.wifi_state_curr == WIFI_STATE_SA )
            {
                LED_WIFI_IND_MODE_CFG_INIT();
                hal_wifimod_set_mode( HAL_WIFIMOD_MODE_CFG );
                app_info.wifi_state_prev = app_info.wifi_state_curr;
                app_info.wifi_state_curr = WIFI_STATE_CFG_INIT;
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_AUX, APP_EVENT_KEY_SHORT ):
        {
            if( app_info.src == AUDIO_SOURCE_AUXIN )
            {
                app_info.src = AUDIO_SOURCE_NONE;
                hal_led_set( HAL_LED_AUX, HAL_LED_MODE_OFF );
                hal_dsp_set_vol( 0 );
                hal_wifimod_set_src( HAL_WIFIMOD_SRC_NONE );
                DSP_SET_CHANNEL_NONE();
            }
            else
            {
                app_info.src = AUDIO_SOURCE_AUXIN;
                hal_led_set( HAL_LED_AUX, HAL_LED_MODE_ON );
                hal_dsp_set_vol( 0 );
                hal_wifimod_set_src( HAL_WIFIMOD_SRC_AUXIN );
                DSP_SET_CHANNEL_AUXIN();
                osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_BT, APP_EVENT_KEY_LONG ):
        {
            if( app_info.bt_mode == BT_MODE_OFF )
            {
                app_info.bt_mode = BT_MODE_SINGLE;
                hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_ON );
                LED_BT_IND_MODE_SINGLE();
                LED_BT0_IND_STATE_DISCOVERABLE();
                LED_BT1_IND_STATE_OFF();

                app_info.src = AUDIO_SOURCE_BT;
                hal_dsp_set_vol( 0 );
                DSP_SET_CHANNEL_BT();
                hal_wifimod_set_src( HAL_WIFIMOD_SRC_EXT_SRC_CTL_BY_MCU );
                osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
                
                hal_cli_print_str( "Bluetooth mode = " );
                hal_cli_print_str( "SINGLE.\r\n" );
            }
            else
            {
                app_info.bt_mode = BT_MODE_OFF;
                hal_bt_ctrl( HAL_BT_MOD_0 + HAL_BT_MOD_1, HAL_BT_CTRL_OFF );
                LED_BT_IND_MODE_OFF();
                LED_BT0_IND_STATE_OFF();
                LED_BT1_IND_STATE_OFF();

                if( app_info.src == AUDIO_SOURCE_BT )
                {
                    app_info.src = AUDIO_SOURCE_NONE;
                    hal_dsp_set_vol( 0 );
                    DSP_SET_CHANNEL_NONE();
                    hal_wifimod_set_src( HAL_WIFIMOD_SRC_NONE );
                }
                
                hal_cli_print_str( "Bluetooth mode = " );
                hal_cli_print_str( "OFF.\r\n" );
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_BT, APP_EVENT_KEY_SHORT ):
        {
            if( app_info.bt_mode != BT_MODE_OFF )
            {
                if( app_info.src == AUDIO_SOURCE_BT )
                {
                    app_info.bt_mode++;
                    if( app_info.bt_mode == BT_MODE_PARTY )
                    {
                        hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_ON );
                        LED_BT_IND_MODE_PARTY();
                        LED_BT0_IND_STATE_DISCOVERABLE();
                        LED_BT1_IND_STATE_DISCOVERABLE();
                        
                        hal_cli_print_str( "Bluetooth mode = " );
                        hal_cli_print_str( "PARTY.\r\n" );
                        
                    }
                    else if( app_info.bt_mode == BT_MODE_MIXER )
                    {
                        LED_BT_IND_MODE_MIXER();
                        LED_BT0_IND_STATE_DISCOVERABLE();
                        LED_BT1_IND_STATE_DISCOVERABLE();
                        
                        hal_cli_print_str( "Bluetooth mode = " );
                        hal_cli_print_str( "MIXER.\r\n" );
                    }
                    else
                    {
                        app_info.bt_mode = BT_MODE_SINGLE;
                        hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_OFF );
                        LED_BT_IND_MODE_SINGLE();
                        LED_BT0_IND_STATE_DISCOVERABLE();
                        LED_BT1_IND_STATE_OFF();
                        
                        hal_cli_print_str( "Bluetooth mode = " );
                        hal_cli_print_str( "SINGLE.\r\n" );
                    }
                }
                else
                {
                    if( app_info.src == AUDIO_SOURCE_AUXIN )
                    {
                        hal_led_set( HAL_LED_AUX, HAL_LED_MODE_OFF );
                    }
                    
                    app_info.src = AUDIO_SOURCE_BT;
                    hal_cli_print_str( "Set audio source as bluetooth.\r\n" );
                    hal_dsp_set_vol( 0 );
                    DSP_SET_CHANNEL_BT();
                    hal_wifimod_set_src( HAL_WIFIMOD_SRC_EXT_SRC_CTL_BY_MCU );
                    osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
                }
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_VOLDN, APP_EVENT_KEY_ENTER ):
        {
            if( app_info.vol > 0 )
            {
                if( app_info.vol >= MASTER_VOL_SINGLE_TUNE_STEP )
                    app_info.vol -= MASTER_VOL_SINGLE_TUNE_STEP;
                else
                    app_info.vol = 0;
                
                hal_wifimod_set_vol( app_info.vol );
                osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_VOLUP, APP_EVENT_KEY_ENTER ):
        {
            if( app_info.vol < 100 )
            {
                app_info.vol += MASTER_VOL_SINGLE_TUNE_STEP;
                if( app_info.vol > 100 )
                    app_info.vol = 100;
                
                hal_wifimod_set_vol( app_info.vol );
                osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_VOLUP, APP_EVENT_KEY_LONG ):
        {
            osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_INC_VOL );
        }
        break;

        case BUILD_UINT16( HAL_KEY_VOLDN, APP_EVENT_KEY_LONG ):
        {
            osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_DEC_VOL );
        }
        break;
        
        case BUILD_UINT16( HAL_KEY_VOLUP, APP_EVENT_KEY_LEAVE ):
        {
            osal_timer_event_delete( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_INC_VOL );
        }
        break;

        case BUILD_UINT16( HAL_KEY_VOLDN, APP_EVENT_KEY_LEAVE ):
        {
            osal_timer_event_delete( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_DEC_VOL );
        }
        break;

        default:
        break;
    }
}




/**************************************************************************************************
**************************************************************************************************/

