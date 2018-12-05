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

extern void app_event_wifimod_bootup( void )
{
    hal_cli_print_str( "Wi-Fi module boot up.\r\n" );
    if( app_info.src != AUDIO_SOURCE_NONE )
    {
        if( app_info.src == AUDIO_SOURCE_AUXIN )
        {
            hal_wifimod_set_src( HAL_WIFIMOD_SRC_AUXIN );
        }
    }
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

    osal_timer_event_delete( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_LED_WIFIB_CRS_BLK );
    osal_timer_event_delete( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_LED_WIFIR_CRS_BLK );

    app_info.wifi_state_prev = app_info.wifi_state_curr;
    
    switch (mode)
    {
        case HAL_WIFIMOD_MODE_CFG:
            LED_WIFI_IND_MODE_CFG();
            app_info.wifi_state_curr = WIFI_STATE_CFG;
        break;

        case HAL_WIFIMOD_MODE_HN:
            LED_WIFI_IND_MODE_HN();
            app_info.wifi_state_curr = WIFI_STATE_HN;
        break;

        case HAL_WIFIMOD_MODE_SA:
            LED_WIFI_IND_MODE_SA();
            app_info.wifi_state_curr = WIFI_STATE_SA;
        break;

        default:
            APP_ASSERT_FORCED();
        break;
    }
}

extern void app_event_wifimod_update_source( uint8_t src )
{
    const char *p_names[] = {
        "NONE",//                 0
        "AIRPLAY",//              1
        "DMR",//                  2
        "DMP",//                  3
        "SPOTIFY",//              4
        "USB",//                  5
        "SD",//                   6
        "MELON",//                7
        "VTUNER",//               8
        "TUNEIN",//               9
        "MIRACAST",//             10
        "RESERVED11",//           11
        "DDMS_SLAVE",//           12
        "RESERVED13",//           13
        "AUXIN",//                14
        "RESERVED15",//           15
        "APPLE_DEVICE",//         16
        "DIRECT_URL",//           17
        "RESERVED18",//           18
        "BLUETOOTH",//            19
        "RESERVED20",//           20
        "DEEZER",//               21
        "TIDAL",//                22
        "FAVORITES",//            23
        "GOOGLE_CAST",//          24
        "EXT_SRC_CTL_BY_MCU",//   25
    };
    
    hal_cli_print_str( "Wi-Fi module src is " );
    hal_cli_print_str( (char *)p_names[src] );
    hal_cli_print_str( ".\r\n" );

    if( src == HAL_WIFIMOD_SRC_NONE )
        src = AUDIO_SOURCE_NONE;
    else if( src == HAL_WIFIMOD_SRC_AUXIN )
        src = AUDIO_SOURCE_AUXIN;
    else if( src == HAL_WIFIMOD_SRC_SD )
        src = AUDIO_SOURCE_SD;
    else if( src == HAL_WIFIMOD_SRC_EXT_SRC_CTL_BY_MCU )
        src = AUDIO_SOURCE_BT;
    else
        src = AUDIO_SOURCE_WIFI;

    if( src != app_info.src )
    {
        if( app_info.src == AUDIO_SOURCE_AUXIN )
        {
            hal_led_set( HAL_LED_AUX, HAL_LED_MODE_OFF );
        }
        else if( app_info.src == AUDIO_SOURCE_BT )
        {
            //hal_led_set( HAL_LED_BT, HAL_LED_MODE_OFF );
        }
        
        app_info.src = src;
        switch ( src )
        {
            case AUDIO_SOURCE_NONE:
            {
                hal_led_set( HAL_LED_AUX, HAL_LED_MODE_OFF );
                hal_dsp_set_vol( 0 );
                DSP_SET_CHANNEL_NONE();
            }
            break;

            case AUDIO_SOURCE_AUXIN:
            {
                hal_led_set( HAL_LED_AUX, HAL_LED_MODE_ON );
                hal_dsp_set_vol( 0 );
                DSP_SET_CHANNEL_AUXIN();
                osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
            }
            break;

            case AUDIO_SOURCE_SD:
            case AUDIO_SOURCE_WIFI:
            {
                hal_led_set( HAL_LED_AUX, HAL_LED_MODE_OFF );
                hal_dsp_set_vol( 0 );
                DSP_SET_CHANNEL_WIFIMOD();
                osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
            }
            break;

            case AUDIO_SOURCE_BT:
            {
                hal_dsp_set_vol( 0 );
                DSP_SET_CHANNEL_BT();
                osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
            }
            break;
        }
    }
}

extern void app_event_wifimod_update_volume( uint8_t vol )
{
    hal_cli_print_str( "Wi-Fi module vol is " );
    hal_cli_print_uint( vol );
    hal_cli_print_str( ".\r\n" );

    app_info.vol = vol;
    osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_SET_DSP_VOL );
}

extern void app_event_wifimod_update_state( uint8_t state )
{
    const char *p_names[] = {
        "PLAYING",
        "STOPPED",
        "PAUSED",
        "CONNECTING",
        "RECEIVING",
        "BUFFERING",
    };

    hal_cli_print_str( "Wi-Fi module play state is " );
    hal_cli_print_str( p_names[state] );
    hal_cli_print_str( ".\r\n" );
}

extern void app_event_wifimod_hn_timeout ( void )
{
    osal_timer_event_delete( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_LED_WIFIB_CRS_BLK );
    osal_timer_event_delete( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_LED_WIFIR_CRS_BLK );
    
    LED_WIFI_IND_MODE_CFG_INIT();
    app_info.wifi_state_curr = WIFI_STATE_CFG_INIT;
    hal_cli_print_str( "Wi-Fi module failed to connect to router. Entering config mode.\r\n" );
}

extern void app_event_wifimod_hn_disconnect( void )
{
    LED_WIFI_IND_MODE_CFG_INIT();
    app_info.wifi_state_curr = WIFI_STATE_CFG_INIT;
    hal_cli_print_str( "Wi-Fi disconnected from router.\r\n" );
}

extern void app_event_wifimod_miss_lucicmd( void )
{
    hal_cli_print_str( "Miss LS5B Luci command!\r\n" );
}

/**************************************************************************************************
**************************************************************************************************/

