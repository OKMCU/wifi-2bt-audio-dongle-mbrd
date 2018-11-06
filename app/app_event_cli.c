/******************************************************************************

 @file  app_event_cli.c

 @brief This file contains the command line interface events handlers.

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
#include "app_config.h"
#include "app_assert.h"
#include "app_event_cli.h"

#include <string.h>
#include "stringx.h"
/**************************************************************************************************
 * TYPE DEFINES
 **************************************************************************************************/

typedef struct cli_cmd_list_t {
    char *cmd;
    void ( *pFxn )( char *pArg );
} CLI_CMD_LIST_t;

 /**************************************************************************************************
 * LOCAL API DECLARATION
 **************************************************************************************************/
static void app_cli_cmd_test         ( char *p_arg );
static void app_cli_cmd_dsp          ( char *p_arg );
static void app_cli_cmd_uibrd        ( char *p_arg );
static void app_cli_cmd_led          ( char *p_arg );
static void app_cli_cmd_bt           ( char *p_arg );
static void app_cli_cmd_lucicmd      ( char *p_arg );
static void app_cli_cmd_wifimod      ( char *p_arg );

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
static const CLI_CMD_LIST_t cmdList[] = {
    { "test", app_cli_cmd_test },
    { "dsp",  app_cli_cmd_dsp },
    { "uibrd", app_cli_cmd_uibrd },
    { "led", app_cli_cmd_led },
    { "bt", app_cli_cmd_bt },
    { "lucicmd", app_cli_cmd_lucicmd },
    { "wifimod", app_cli_cmd_wifimod },
};



extern void app_event_cli_process_cmd( char *s )
{
    char *pArg = NULL;
    char *cmd;
    uint16_t i;
    
    cmd = strtok_r( s, " ", &pArg);

    for ( i = 0; i < sizeof(cmdList)/sizeof(CLI_CMD_LIST_t); i++ )
    {
        if( strcmp( cmd, cmdList[i].cmd ) == 0 )
        {
            if( cmdList[i].pFxn != NULL )
            {
                cmdList[i].pFxn( pArg );
            }
            break;
        }
    }
}

static void app_cli_cmd_test( char *pArg )
{
    hal_cli_print_str( "TEST\r\n\r\n" );
}

static void app_cli_cmd_dsp( char *p_arg )
{
    uint32_t u32tmp;
    char *p_opt;
    char *p_str;
    uint8_t src;

    p_str = NULL;
    p_opt = strtok_r( p_arg, " ", &p_str );

    if( strcmp( p_opt, "-reset" ) == 0 )
    {
        hal_dsp_disable();
        hal_mcu_delayus(10000);
        hal_dsp_enable();
        hal_cli_print_str("OK!\r\n\r\n");
        return;
    }
    else if( strcmp( p_opt, "-init" ) == 0 )
    {
        hal_dsp_write_init_code();
        hal_cli_print_str("OK!\r\n\r\n");
        return;
    }
    else if( strcmp( p_opt, "-vol" ) == 0 )
    {
        p_arg = strtok_r( NULL, " ", &p_str );
        if( decstr2uint( p_arg, &u32tmp ) )
        {
            hal_dsp_set_vol( u32tmp );
            hal_cli_print_str( "OK!\r\n\r\n" );
            return;
        }
    }
    else if( strcmp( p_opt, "-src" ) == 0 )
    {
        src = HAL_DSP_SRC_NONE;
        p_arg = strtok_r( NULL, " ", &p_str );
        while(p_arg != NULL)
        {
            if( strcmp( p_arg, "SDI0" ) == 0 )
            {
                src |= HAL_DSP_SRC_SDI0;
            } 
            else if( strcmp( p_arg, "SDI1" ) == 0 )
            {
                src |= HAL_DSP_SRC_SDI1;
            }
            else if( strcmp( p_arg, "ADC" ) == 0 )
            {
                src |= HAL_DSP_SRC_ADC;
            }
            p_arg = strtok_r( NULL, " ", &p_str );
        }
            
        hal_dsp_set_src( src );
        hal_cli_print_str( "OK!\r\n\r\n" );
        return;
    }
    hal_cli_print_str( "Bad command!\r\n\r\n" );
}

static void app_cli_cmd_uibrd( char *p_arg )
{
    uint8_t u8tmp;
    char *p_opt;
    char *p_str;

    p_str = NULL;
    p_opt = strtok_r( p_arg, " ", &p_str );

    if( strcmp( p_opt, "-irqpin" ) == 0 )
    {
        if(hal_uibrd_irq_pin())
        {
            hal_cli_print_str("HIGH!");
        }
        else
        {
            hal_cli_print_str("LOW!");
        }
        hal_cli_print_str("\r\n\r\n");
        return;
    }
    else if( strcmp( p_opt, "-irqevt" ) == 0 )
    {
        u8tmp = hal_uibrd_irq_evt();
        hal_cli_print_str( "IRQ event 0x" );
        hal_cli_print_hex8( u8tmp );
        hal_cli_print_str( ".\r\n" );
        return;
    }
    else if( strcmp( p_opt, "-swver" ) == 0 )
    {
        u8tmp = hal_uibrd_sw_ver();
        hal_cli_print_str( "SW version 0x" );
        hal_cli_print_hex8( u8tmp );
        hal_cli_print_str( ".\r\n" );
        return;
    }
    else if( strcmp( p_opt, "-hwver" ) == 0 )
    {
        u8tmp = hal_uibrd_hw_ver();
        hal_cli_print_str( "HW version 0x" );
        hal_cli_print_hex8( u8tmp );
        hal_cli_print_str( ".\r\n" );
        return;
    }
    hal_cli_print_str( "Bad command!\r\n\r\n" );
}

static void app_cli_cmd_led          ( char *p_arg )
{
    uint32_t u32tmp;
    uint8_t leds;
    uint8_t cnt;
    uint8_t duty;
    uint16_t time;
    char *p_opt;
    char *p_str;

    p_str = NULL;

    //get "-on" "-off" "-blink"
    p_opt = strtok_r( p_arg, " ", &p_str );

    //get leds bitmask
    p_arg = strtok_r( NULL, " ", &p_str );
    if( hexstr2uint( p_arg, &u32tmp ) == 0 )
    {
        hal_cli_print_str( "Bad command!\r\n\r\n" );
        return;
    }
    leds = (uint8_t)u32tmp;

    //check leds bitmask value
    leds &= HAL_LED_ALL;
    if(leds == 0x00)
    {
        hal_cli_print_str( "Bad command!\r\n\r\n" );
        return;
    }

    //check options
    if( strcmp( p_opt, "-on" ) == 0 )
    {
        hal_led_set(leds, HAL_LED_MODE_ON);
        hal_cli_print_str( "OK!\r\n\r\n" );
        return;
    }

    if( strcmp( p_opt, "-off" ) == 0 )
    {
        hal_led_set(leds, HAL_LED_MODE_OFF);
        hal_cli_print_str( "OK!\r\n\r\n" );
        return;
    }

    if( strcmp( p_opt, "-blink" ) == 0 )
    {
        cnt = HAL_LED_DEFAULT_FLASH_COUNT;
        duty = HAL_LED_DEFAULT_DUTY_CYCLE;
        time = HAL_LED_DEFAULT_FLASH_TIME;

        p_opt = strtok_r( NULL, " ", &p_str );
        while(p_opt)
        {
            p_arg = strtok_r( NULL, " ", &p_str );
            if( decstr2uint( p_arg, &u32tmp ) == 0 )
            {
                hal_cli_print_str( "Bad command!\r\n\r\n" );
                return;
            }
            
            if( strcmp( p_opt, "-c" ) == 0 )
            {
                //count
                cnt = (uint8_t)u32tmp;
            }
            else if( strcmp( p_opt, "-d" ) == 0 )
            {
                //duty
                duty = (uint8_t)u32tmp;
            }
            else if( strcmp( p_opt, "-t" ) == 0 )
            {
                //time
                time = (uint16_t)u32tmp;
            }
            else
            {
                hal_cli_print_str( "Bad command!\r\n\r\n" );
                return;
            }
            
            p_opt = strtok_r( NULL, " ", &p_str );
        }
        hal_led_blink(leds, cnt, duty, time);
        hal_cli_print_str( "OK!\r\n\r\n" );
        return;
    }
    
}

static void app_cli_cmd_bt           ( char *p_arg )
{
    char *p_opt;
    char *p_str;
    uint8_t mod;
    uint8_t ctrl;

    p_str = NULL;

    //get "-off" "-on" "-pairing"
    p_opt = strtok_r( p_arg, " ", &p_str );
    //get "BT0" "BT1"
    p_arg = strtok_r( NULL, " ", &p_str );

    //check option
    if(strcmp(p_opt, "-on") == 0)
    {
        ctrl = HAL_BT_CTRL_ON;
    }
    else if(strcmp(p_opt, "-off") == 0)
    {
        ctrl = HAL_BT_CTRL_OFF;
    }
    else if(strcmp(p_opt, "-pairing") == 0)
    {
        ctrl = HAL_BT_CTRL_PAIRING;
    }
    else
    {
        hal_cli_print_str( "Bad command!\r\n\r\n" );
        return;
    }

    //check module id
    if(strcmp(p_arg, "BT0") == 0)
    {
        mod = HAL_BT_MOD_0;
    }
    else if(strcmp(p_arg, "BT1") == 0)
    {
        mod = HAL_BT_MOD_1;
    }
    else
    {
        hal_cli_print_str( "Bad command!\r\n\r\n" );
        return;
    }

    hal_bt_ctrl(mod, ctrl);
    hal_cli_print_str( "OK!\r\n\r\n" );
    
}

static void app_cli_cmd_lucicmd ( char *p_arg )
{
    char *p_opt;
    char *p_str;
    uint32_t u32tmp;
    HAL_LUCI_CMD_t lucicmd;

    p_str = NULL;

    p_opt = strtok_r( p_arg, " ", &p_str );
    //"-m MSGBOX"
    if(strcmp(p_opt, "-m") == 0)
    {
        //get value of "msgbox" 
        p_arg = strtok_r( NULL, " ", &p_str );
        if( p_arg )
        {
            if( decstr2uint(p_arg, &u32tmp) )
            {
                lucicmd.msgbox = (uint16_t)u32tmp;
                lucicmd.remote_id = 0xAAAA;
                lucicmd.type = HAL_LUCICMD_TYPE_SET;
                lucicmd.status = HAL_LUCICMD_STATUS_NA;
                lucicmd.len = 0;
                lucicmd.p_data = NULL;

                do
                {
                    p_opt = strtok_r( NULL, " ", &p_str );
                    if( p_opt == NULL )
                        break;
                    if( strcmp(p_opt, "-t") == 0 )
                    {
                        //"-t SET" or "-t GET"
                        p_arg = strtok_r( NULL, " ", &p_str );
                        if( strcmp(p_arg, "SET") == 0 )
                        {
                            lucicmd.type = HAL_LUCICMD_TYPE_SET;
                        }
                        else if( strcmp(p_arg, "GET") == 0 )
                        {
                            lucicmd.type = HAL_LUCICMD_TYPE_GET;
                        }
                        else
                        {
                            hal_cli_print_str( "Bad command!\r\n\r\n" );
                            return;
                        }
                    }
                    else if( strcmp(p_opt, "-d") == 0 )
                    {
                        //"-d data_string"
                        p_arg = strtok_r( NULL, " ", &p_str );
                        if( p_arg )
                        {
                            lucicmd.len = strlen( p_arg );
                            lucicmd.p_data = (uint8_t *)p_arg;
                        }
                        else
                        {
                            hal_cli_print_str( "Bad command!\r\n\r\n" );
                            return;
                        }
                    }
                    else
                    {
                        hal_cli_print_str( "Bad command!\r\n\r\n" );
                        return;
                    }
                }
                while(p_opt != NULL);
                lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );
                hal_lucicmd_send( &lucicmd );
                hal_cli_print_str( "OK!\r\n\r\n" );
                return;
            }
        }
    }
    hal_cli_print_str( "Bad command!\r\n\r\n" );
}

static void app_cli_cmd_wifimod      ( char *p_arg )
{
    char *p_opt;
    char *p_str;
    uint8_t mode;

    p_str = NULL;

    p_opt = strtok_r( p_arg, " ", &p_str );
    if( strcmp(p_opt, "-enable") == 0 )
    {
        hal_wifimod_enable();
        hal_cli_print_str( "OK!\r\n\r\n" );
        return;
    }
    else if( strcmp(p_opt, "-disable") == 0 )
    {
        hal_wifimod_disable();
        hal_cli_print_str( "OK!\r\n\r\n" );
        return;
    }
    else if( strcmp(p_opt, "-mode") == 0 )
    {
        p_arg = strtok_r( NULL, " ", &p_str );
        mode = HAL_WIFIMOD_MODE_NONE;
        if( strcmp(p_arg, "HN") == 0 )
            mode = HAL_WIFIMOD_MODE_HN;
        else if( strcmp(p_arg, "SA") == 0 )
            mode = HAL_WIFIMOD_MODE_SA;
        else if( strcmp(p_arg, "CFG") == 0 )
            mode = HAL_WIFIMOD_MODE_CFG;
        
        if( mode != HAL_WIFIMOD_MODE_NONE )
        {
            hal_wifimod_set_mode( mode );
            hal_cli_print_str( "OK!\r\n\r\n" );
            return;
        }
    }
    
    hal_cli_print_str( "Bad command!\r\n\r\n" );
}

/**************************************************************************************************
**************************************************************************************************/

