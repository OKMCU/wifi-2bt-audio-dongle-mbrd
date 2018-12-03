/******************************************************************************

 @file  app_event_main.c

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

extern void app_event_main_por( void )
{
    uint8_t u8tmp;

    hal_cli_print_str( "\r\nThis is Xounts Audio Dongle project.\r\n" );
    hal_cli_print_str( "Power on reset.\r\n" );

    hal_cli_print_str( "Firmware Version " );
    hal_cli_print_str( FIRMWARE_VER );
    hal_cli_print_str( "\r\n" );

    hal_cli_print_str( "Hardware Version " );
    hal_cli_print_str( HARDWARE_VER );
    hal_cli_print_str( "\r\n" );
    
    u8tmp = hal_uibrd_hw_ver();
    hal_cli_print_str( "UI board HW version 0x" );
    hal_cli_print_hex8( u8tmp );
    hal_cli_print_str( ".\r\n" );

    u8tmp = hal_uibrd_sw_ver();
    hal_cli_print_str( "UI board SW version 0x" );
    hal_cli_print_hex8( u8tmp );
    hal_cli_print_str( ".\r\n" );

    app_info.src = AUDIO_SOURCE_NONE;
    app_info.vol = 50;//need to load from EEPROM
    app_info.wifi_state_curr = WIFI_STATE_OFF;
    app_info.wifi_state_prev = WIFI_STATE_OFF;
    app_info.bt_mode = BT_MODE_OFF;
    
    hal_dsp_enable();
    hal_wifimod_enable();
    app_info.wifi_state_curr = WIFI_STATE_INIT;
    
    osal_timer_event_create( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_INIT, 100 );
}

extern void app_event_main_init( void )
{
    //clear IRQs
    hal_cli_print_str( "Init UI board..." );
    hal_uibrd_irq_evt();
    hal_led_set( HAL_LED_ALL, HAL_LED_MODE_OFF );
    BT_DISABLE( HAL_BT_MOD_0 + HAL_BT_MOD_1 );
    BT_ENABLE( HAL_BT_MOD_0 + HAL_BT_MOD_1 );
    hal_cli_print_str( "OK!\r\n" );

    hal_cli_print_str( "Init DSP..." );
    hal_dsp_write_init_code();
    hal_cli_print_str( "OK!\r\n" );

    hal_cli_print_str( "Waiting for Wi-Fi boot up...\r\n" );
    osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_LED_WIFIB_CRS_BLK );
}

extern void app_event_main_led_wifir_cross_blink( void )
{
    osal_timer_event_create( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_LED_WIFIB_CRS_BLK, 500 );
    hal_led_set( HAL_LED_WIFIB, HAL_LED_MODE_OFF );
    hal_led_set( HAL_LED_WIFIR, HAL_LED_MODE_ON );
}

extern void app_event_main_led_wifib_cross_blink( void )
{
    osal_timer_event_create( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_LED_WIFIR_CRS_BLK, 500 );
    hal_led_set( HAL_LED_WIFIR, HAL_LED_MODE_OFF );
    hal_led_set( HAL_LED_WIFIB, HAL_LED_MODE_ON );
}

extern void app_event_main_set_dsp_vol( void )
{
    uint32_t dsp_vol;
    
    switch ( app_info.src )
    {
        case AUDIO_SOURCE_AUXIN:
        case AUDIO_SOURCE_BT:
            dsp_vol = (uint32_t)app_info.vol*HAL_DSP_VOL_MAX/100;
            if( hal_dsp_get_vol() != dsp_vol )
                hal_dsp_set_vol( dsp_vol );
        break;

        case AUDIO_SOURCE_SD:
        case AUDIO_SOURCE_WIFI:
            if( hal_dsp_get_vol() != HAL_DSP_VOL_MAX )
            {
                hal_dsp_set_vol( HAL_DSP_VOL_MAX );
            }
        break;
    }
}
/**************************************************************************************************
**************************************************************************************************/

