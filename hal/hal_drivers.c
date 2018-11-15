/******************************************************************************

 @file  hal_drivers.c

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "osal.h"
#include "hal.h"

#include "main.h"

#include <stdint.h>

/* ------------------------------------------------------------------------------------------------
 *                                       Local Prototypes
 * ------------------------------------------------------------------------------------------------
 */
extern void hal_cli_driver_handle_rxne( void );
extern void hal_cli_driver_handle_txe( void );
extern void hal_uibrd_driver_handle_trig_irq( void );
extern void hal_uibrd_driver_handle_poll_irq( void );
extern void hal_lucicmd_driver_handle_rxne( void );
extern void hal_lucicmd_driver_handle_txe( void );
extern void hal_wifimod_driver_handle_rx_bytes( uint8_t *p_msg, uint16_t len );
extern void hal_wifimod_driver_handle_mode_ctrl( void );
extern void hal_wifimod_driver_handle_hn_timeout( void );
extern void hal_wifimod_driver_handle_rx_ovf( void );
/**************************************************************************************************
 * @fn          hal_driver_init
 *
 * @brief       
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
extern void hal_driver_init( void )
{
    hal_mcu_init();
    hal_cli_init();
    hal_dsp_init();
    hal_uibrd_init();
    hal_key_init();
    hal_led_init();
    hal_bt_init();
    hal_lucicmd_init();
    hal_wifimod_init();
    hal_multiplexer_init();
}

/**************************************************************************************************
 * @fn          hal_task_driver_basic
 *
 * @brief       
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
extern void hal_task_driver_basic ( uint8_t task_id, uint8_t event_id )
{
    switch ( event_id )
    {
        case TASK_EVT_HAL_DRIVER_BASIC_LUCICMD_RXNE:
            hal_lucicmd_driver_handle_rxne();
        break;
        
        case TASK_EVT_HAL_DRIVER_BASIC_LUCICMD_TXE:
            hal_lucicmd_driver_handle_txe();
        break;
        
        case TASK_EVT_HAL_DRIVER_BASIC_CLI_RXNE:
            hal_cli_driver_handle_rxne();
        break;

        case TASK_EVT_HAL_DRIVER_BASIC_CLI_TXE:
            hal_cli_driver_handle_txe();
        break;

        case TASK_EVT_HAL_DRIVER_BASIC_UIBRD_TRIG_IRQ:
            hal_uibrd_driver_handle_trig_irq();
        break;

        case TASK_EVT_HAL_DRIVER_BASIC_UIBRD_POLL_IRQ:
            hal_uibrd_driver_handle_poll_irq();
        break;
        
        default:
            HAL_ASSERT_FORCED();
        break;
    }
}

extern void hal_task_driver_wifimod( uint8_t task_id, uint8_t event_id )
{
    void *p_msg;
    uint16_t len;

    switch ( event_id )
    {
        case OSAL_TASK_EVENT_MSG:
        {
            p_msg = osal_msg_recv( task_id );
            while ( p_msg )
            {
                len = osal_msg_len( p_msg );
                hal_wifimod_driver_handle_rx_bytes( (uint8_t *)p_msg, len );
                osal_msg_delete( p_msg );
                p_msg = osal_msg_recv( task_id );
            }
        }
        break;

        case TASK_EVT_HAL_DRIVER_WIFIMOD_RX_OVF:
            hal_wifimod_driver_handle_rx_ovf();
        break;
        
        case TASK_EVT_HAL_DRIVER_WIFIMOD_RESET:
            hal_wifimod_enable();
        break;

        case TASK_EVT_HAL_DRIVER_WIFIMOD_MODE_CTRL:
            hal_wifimod_driver_handle_mode_ctrl();
        break;

        case TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT:
            hal_wifimod_driver_handle_hn_timeout();
        break;
        
        default:
            HAL_ASSERT_FORCED();
        break;
    }
    
}

/**************************************************************************************************
*/
