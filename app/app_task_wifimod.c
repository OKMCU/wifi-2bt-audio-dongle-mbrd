/******************************************************************************

 @file  app_task_wifimod.c

 @brief This file contains the Wi-Fi module service.

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
#include "app_task_wifimod.h"

#include "main.h"

#include <string.h>
#include "stringx.h"

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/


/**************************************************************************************************
 * TYPES
 **************************************************************************************************/

/**************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

static void luci_cmd_print( const HAL_LUCI_CMD_t *p_lucicmd );
/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

extern void app_task_wifimod_init ( void )
{
     
}

/**************************************************************************************************
 * @fn      app_task_wifimod
 *
 * @brief   app_task_wifimod
 *
 * @param   task_id - LUCICOP Task Id
 *          events - events
 *
 * @return  None
 **************************************************************************************************/
void app_task_wifimod( uint8_t task_id, uint8_t event_id )
{
    HAL_LUCI_CMD_t *p_luci_cmd;

    switch ( event_id )
    {
        case OSAL_TASK_EVENT_MSG:
        {
            p_luci_cmd = (HAL_LUCI_CMD_t *)osal_msg_recv( task_id );
            while ( p_luci_cmd )
            {
                luci_cmd_print( p_luci_cmd );
                hal_cli_print_str( "\r\n" );
                if(p_luci_cmd->p_data)
                    osal_mem_free( p_luci_cmd->p_data );
                osal_msg_delete( p_luci_cmd );
                p_luci_cmd = osal_msg_recv( task_id );
            }
        }
        break;

        case TASK_EVT_APP_WIFIMOD_BOOT_UP:
        {
            hal_cli_print_str( "Wi-Fi module boot up.\r\n" );
        }
        break;

        case TASK_EVT_APP_WIFIMOD_RESET:
        {
            hal_cli_print_str( "Wi-Fi module rebooting.\r\n" );
        }
        break;

        case TASK_EVT_APP_WIFIMOD_UPD_MODE:
        {
            hal_cli_print_str( "Wi-Fi module mode is " );
            hal_cli_print_uint( hal_wifimod_get_mode() );
            hal_cli_print_str( ".\r\n" );
        }
        break;

        case TASK_EVT_APP_WIFIMOD_UPD_SRC:
        {
            hal_cli_print_str( "Wi-Fi module src is " );
            hal_cli_print_uint( hal_wifimod_get_src() );
            hal_cli_print_str( ".\r\n" );
        }
        break;

        case TASK_EVT_APP_WIFIMOD_UPD_VOL:
        {
            hal_cli_print_str( "Wi-Fi module vol is " );
            hal_cli_print_uint( hal_wifimod_get_vol() );
            hal_cli_print_str( ".\r\n" );
        }
        break;

        case TASK_EVT_APP_WIFIMOD_UPD_STATE:
        {
            hal_cli_print_str( "Wi-Fi module play state is " );
            hal_cli_print_uint( hal_wifimod_play_state() );
            hal_cli_print_str( ".\r\n" );
        }
        break;
        
        default:
            APP_ASSERT_FORCED();
        break;
    }
}

static void luci_cmd_print( const HAL_LUCI_CMD_t *p_lucicmd )
{
    uint16_t i;

    const char *cmdStatusStr[] = {
        "NA",
        "OK",
        "GENERIC_ERR",
        "DEV_NOT_RDY",
        "CRC_ERR"
    };

    //print msgbox ID
    hal_cli_print_str("MSGBOX=");
    if(p_lucicmd->msgbox > 255)
    {
        hal_cli_print_str("0x");
        hal_cli_print_hex16( p_lucicmd->msgbox );
    }
    else
    {
        hal_cli_print_uint( p_lucicmd->msgbox );
    }
    hal_cli_print_str("; ");

    //print TYPE
    hal_cli_print_str("TYPE=");
    if(p_lucicmd->type == HAL_LUCICMD_TYPE_GET)
    {
        hal_cli_print_str("GET");
    }
    else if(p_lucicmd->type == HAL_LUCICMD_TYPE_SET)
    {
        hal_cli_print_str("SET");
    }
    else
    {
        hal_cli_print_str("0x");
        hal_cli_print_hex8(p_lucicmd->type);
    }
    hal_cli_print_str("; ");

    //print status
    hal_cli_print_str("STATUS=");
    if(p_lucicmd->status < sizeof(cmdStatusStr)/sizeof(char *))
        hal_cli_print_str( cmdStatusStr[(uint8_t)p_lucicmd->status] );
    else
        hal_cli_print_str( "UNKNOWN" );
    hal_cli_print_str("; ");
    
    //print data
    if( p_lucicmd->len )
    {
        hal_cli_print_str("DATA=");
        for(i = 0; i < p_lucicmd->len; i++)
        {
            //if(cmd->msgbox > 255)
            if( p_lucicmd->p_data[i] > 127 || p_lucicmd->p_data[i] < 32 )
            {
                hal_cli_print_str( "0x" );
                hal_cli_print_hex8( p_lucicmd->p_data[i] );
                hal_cli_print_char(' ');
            }
            else
            {
                hal_cli_print_char(p_lucicmd->p_data[i]);
            }
        }
    }
}


/**************************************************************************************************
**************************************************************************************************/

