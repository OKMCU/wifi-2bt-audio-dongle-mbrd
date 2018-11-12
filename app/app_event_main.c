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
#include "app_config.h"
#include "app_assert.h"
#include "app_event_main.h"

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

    hal_dsp_enable();
    hal_wifimod_enable();
    
}

extern void app_event_main_init( void )
{

    //clear IRQs
    hal_cli_print_str( "Init UI board..." );
    hal_uibrd_irq_evt();
    hal_led_set( HAL_LED_ALL, HAL_LED_MODE_OFF );
    hal_bt_ctrl( HAL_BT_MOD_0, HAL_BT_CTRL_OFF );
    hal_bt_ctrl( HAL_BT_MOD_1, HAL_BT_CTRL_OFF );
    hal_cli_print_str( "OK!\r\n" );

    hal_cli_print_str( "Init DSP..." );
    hal_dsp_write_init_code();
    hal_cli_print_str( "OK!\r\n" );

    hal_cli_print_str( "Waiting for Wi-Fi boot up...\r\n" );
    LED_WIFI_IND_BOOTING();
}
/**************************************************************************************************
**************************************************************************************************/

