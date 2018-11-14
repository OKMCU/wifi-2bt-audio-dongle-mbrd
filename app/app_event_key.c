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
                hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_PAIRING );
                hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_OFF );
                LED_BT_IND_MODE_SINGLE();
                LED_BT0_IND_STATE_DISCOVERABLE();
                LED_BT1_IND_STATE_OFF();

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
                hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_OFF );
                hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_OFF );
                LED_BT_IND_MODE_OFF();
                LED_BT0_IND_STATE_OFF();
                LED_BT1_IND_STATE_OFF();

                hal_dsp_set_vol( 0 );
                DSP_SET_CHANNEL_NONE();
                hal_wifimod_set_src( HAL_WIFIMOD_SRC_NONE );
                
                hal_cli_print_str( "Bluetooth mode = " );
                hal_cli_print_str( "OFF.\r\n" );
            }
        }
        break;

        case BUILD_UINT16( HAL_KEY_BT, APP_EVENT_KEY_SHORT ):
        {
            if( app_info.bt_mode != BT_MODE_OFF )
            {
                app_info.bt_mode++;
                if( app_info.bt_mode == BT_MODE_PARTY )
                {
                    hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_PAIRING );
                    hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_PAIRING );
                    LED_BT_IND_MODE_PARTY();
                    LED_BT0_IND_STATE_DISCOVERABLE();
                    LED_BT1_IND_STATE_DISCOVERABLE();
                    
                    hal_cli_print_str( "Bluetooth mode = " );
                    hal_cli_print_str( "PARTY.\r\n" );
                    
                }
                else if( app_info.bt_mode == BT_MODE_MIXER )
                {
                    hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_PAIRING );
                    hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_PAIRING );
                    LED_BT_IND_MODE_MIXER();
                    LED_BT0_IND_STATE_DISCOVERABLE();
                    LED_BT1_IND_STATE_DISCOVERABLE();
                    
                    hal_cli_print_str( "Bluetooth mode = " );
                    hal_cli_print_str( "MIXER.\r\n" );
                }
                else
                {
                    app_info.bt_mode = BT_MODE_SINGLE;
                    hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_PAIRING );
                    hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_OFF );
                    LED_BT_IND_MODE_SINGLE();
                    LED_BT0_IND_STATE_DISCOVERABLE();
                    LED_BT1_IND_STATE_OFF();
                    
                    hal_cli_print_str( "Bluetooth mode = " );
                    hal_cli_print_str( "SINGLE.\r\n" );
                }
            }
        }
        break;

        default:
        break;
    }
#if 0
    

    switch( keyValue )
    {
        case HAL_KEY_AUX:
        {
            if( keyEvent == APP_EVENT_KEY_SHORT )
            {
                if( AppInfo.dspAudioSrc != HAL_DSP_AUDIO_SRC_ADC )
                {
                    HalLuciCmdSendAuxInStart();
                    
                }
                else
                {
                    HalLuciCmdSendAuxInStop();
                }
            }
        }
        break;

        case HAL_KEY_WIFI:
        {
            if( keyEvent == APP_EVENT_KEY_SHORT )
            {
                if( AppInfo.lsWiFiMode == HAL_LUCICMD_WIFIMODE_SA )
                {
                    HalLuciCmdSendSetWiFiModeHN();
                }
                else if( AppInfo.lsWiFiMode == HAL_LUCICMD_WIFIMODE_HN )
                {
                    HalLuciCmdSendSetWiFiModeSA();
                }
                else if( AppInfo.lsWiFiMode == HAL_LUCICMD_WIFIMODE_CFG )
                {
                    HalLuciCmdSendSetWiFiModeSA();
                }
                else
                {
                    //HAL_LED_WIFI_INDICATE_MODE_NONE();
                    //AppInfo.lsWiFiMode = HAL_LUCICMD_WIFIMODE_NONE;
                    //HalLuciCmdSendSetWiFiModeSA();
                }
            }
            else if( keyEvent == APP_EVENT_KEY_LONG )
            {
                if( AppInfo.lsWiFiMode == HAL_LUCICMD_WIFIMODE_HN )
                {
                    HalLuciCmdSendSetWiFiModeCFG();
                }
                else if( AppInfo.lsWiFiMode == HAL_LUCICMD_WIFIMODE_SA )
                {
                    HalLuciCmdSendSetWiFiModeCFG();
                }
                else if( AppInfo.lsWiFiMode == HAL_LUCICMD_WIFIMODE_CFG )
                {
                    HalLuciCmdSendSetWiFiModeSA();
                }
                else
                {
                    //HAL_LED_WIFI_INDICATE_MODE_NONE();
                    //AppInfo.lsWiFiMode = HAL_LUCICMD_WIFIMODE_NONE;
                    //HalLuciCmdSendSetWiFiModeHN();
                }
            }
        }
        break;

        case HAL_KEY_BT:
        {
            if( keyEvent == APP_EVENT_KEY_SHORT )
            {
                if( AppInfo.btMode != APP_BT_MODE_OFF )
                {
                    AppInfo.btMode++;
                    if( AppInfo.btMode == APP_BT_MODE_PARTY )
                    {
                        HalBtCtrl( HAL_BT_DEVICE_0 + HAL_BT_DEVICE_1, HAL_BT_CTRL_PAIRING );
                        HAL_LED_BTMODE_IND_PARTY();
                        HAL_LED_BT0_IND_DISCOVERABLE();
                        HAL_LED_BT1_IND_DISCOVERABLE();
                        
                        HalTerminalPrintStr( "Bluetooth mode = " );
                        HalTerminalPrintStr( "PARTY.\r\n" );
                        
                    }
                    else if( AppInfo.btMode == APP_BT_MODE_DJ )
                    {
                        HalBtCtrl( HAL_BT_DEVICE_0 + HAL_BT_DEVICE_1, HAL_BT_CTRL_PAIRING );
                        HAL_LED_BTMODE_IND_DJ();
                        HAL_LED_BT0_IND_DISCOVERABLE();
                        HAL_LED_BT1_IND_DISCOVERABLE();
                        
                        HalTerminalPrintStr( "Bluetooth mode = " );
                        HalTerminalPrintStr( "DJ.\r\n" );
                    }
                    else
                    {
                        AppInfo.btMode = APP_BT_MODE_SINGLE;
                        HalBtCtrl( HAL_BT_DEVICE_0, HAL_BT_CTRL_PAIRING );
                        HalBtCtrl( HAL_BT_DEVICE_1, HAL_BT_CTRL_OFF );
                        HAL_LED_BTMODE_IND_SINGLE();
                        HAL_LED_BT0_IND_DISCOVERABLE();
                        HAL_LED_BT1_IND_OFF();
                        
                        HalTerminalPrintStr( "Bluetooth mode = " );
                        HalTerminalPrintStr( "SINGLE.\r\n" );
                    }
                    
                    
                }     
            }
            else if( keyEvent == APP_EVENT_KEY_LONG )
            {
                if( AppInfo.btMode == APP_BT_MODE_OFF )
                {
                    AppInfo.btMode = APP_BT_MODE_SINGLE;
                    HalBtCtrl( HAL_BT_DEVICE_0, HAL_BT_CTRL_PAIRING );
                    HalBtCtrl( HAL_BT_DEVICE_1, HAL_BT_CTRL_OFF );
                    HAL_LED_BTMODE_IND_SINGLE();
                    HAL_LED_BT0_IND_DISCOVERABLE();
                    HAL_LED_BT1_IND_OFF();
                    
                    HalTerminalPrintStr( "Bluetooth mode = " );
                    HalTerminalPrintStr( "SINGLE.\r\n" );
                }
                else
                {
                    AppInfo.btMode = APP_BT_MODE_OFF;
                    HalBtCtrl( HAL_BT_DEVICE_0 + HAL_BT_DEVICE_1, HAL_BT_CTRL_OFF );
                    HAL_LED_BTMODE_IND_OFF();
                    HAL_LED_BT0_IND_OFF();
                    HAL_LED_BT1_IND_OFF();
                    
                    HalTerminalPrintStr( "Bluetooth mode = " );
                    HalTerminalPrintStr( "OFF.\r\n" );
                }
            }

            if( keyEvent == APP_EVENT_KEY_SHORT || keyEvent == APP_EVENT_KEY_LONG )
            {
                if( AppInfo.dspAudioSrc != HAL_DSP_AUDIO_SRC_NONE )
                {
                    HAL_DSP_SET_AUDIOSRC_NONE();

                    //Stop AUX-IN
                    if( AppInfo.dspAudioSrc == HAL_DSP_AUDIO_SRC_ADC )
                        HalLuciCmdSendAuxInStop();

                    //Stop Wi-Fi
                    AppInfo.dspAudioSrc = HAL_DSP_AUDIO_SRC_NONE;
                }
            }
        }
        break;

        case HAL_KEY_VOLDN:
            if( keyEvent == APP_EVENT_KEY_SHORT )
            {
                if( AppInfo.lsAudioSrc == HAL_LUCICMD_AUDIOSRC_LineIn )
                {
                    if( AppInfo.volAuxIn > 100 )
                        AppInfo.volAuxIn = AppInfo.volAuxInNv;
                    if( AppInfo.volAuxIn > 0 )
                        AppInfo.volAuxIn--;
                    
                    if( AppInfo.volAuxIn == 100 )
                        AppInfo.dspVol = HAL_DSP_VOL_MAX;
                    else
                        AppInfo.dspVol = (HAL_DSP_VOL_MAX * AppInfo.volAuxIn)/100;
                    HalDspSetVol( AppInfo.dspVol );
                    
                    AppInfo.volDisp = AppInfo.volAuxIn;
                    HalLuciCmdSendSetVolume( AppInfo.volDisp );
                }
                else
                {
                    if( AppInfo.lsVolDevice > 100 )
                        AppInfo.lsVolDevice = AppInfo.lsVolDeviceNv;
                    if( AppInfo.lsVolDevice > 0 )
                        AppInfo.lsVolDevice--;

                    AppInfo.volDisp = AppInfo.lsVolDevice;
                    HalLuciCmdSendSetVolume( AppInfo.volDisp );
                }

                HalTerminalPrintStr( "Update display volume = " );
                HalTerminalPrintUint( AppInfo.volDisp );
                HalTerminalPrintStr( ".\r\n" );
            }
        break;

        case HAL_KEY_VOLUP:
            if( keyEvent == APP_EVENT_KEY_SHORT )
            {
                if( AppInfo.lsAudioSrc == HAL_LUCICMD_AUDIOSRC_LineIn )
                {
                    if( AppInfo.volAuxIn > 100 )
                        AppInfo.volAuxIn = AppInfo.volAuxInNv;
                    if( AppInfo.volAuxIn < 100 )
                        AppInfo.volAuxIn++;
                    
                    if( AppInfo.volAuxIn == 100 )
                        AppInfo.dspVol = HAL_DSP_VOL_MAX;
                    else
                        AppInfo.dspVol = (HAL_DSP_VOL_MAX * AppInfo.volAuxIn)/100;
                    HalDspSetVol( AppInfo.dspVol );
                    
                    AppInfo.volDisp = AppInfo.volAuxIn;
                    HalLuciCmdSendSetVolume( AppInfo.volDisp );
                }
                else
                {
                    if( AppInfo.lsVolDevice > 100 )
                        AppInfo.lsVolDevice = AppInfo.lsVolDeviceNv;
                    if( AppInfo.lsVolDevice < 100 )
                        AppInfo.lsVolDevice++;
                    

                    AppInfo.volDisp = AppInfo.lsVolDevice;
                    HalLuciCmdSendSetVolume( AppInfo.volDisp );
                }
                HalTerminalPrintStr( "Update display volume = " );
                HalTerminalPrintUint( AppInfo.volDisp );
                HalTerminalPrintStr( ".\r\n" );
            }
        break;
        
    }
#endif
}




/**************************************************************************************************
**************************************************************************************************/

