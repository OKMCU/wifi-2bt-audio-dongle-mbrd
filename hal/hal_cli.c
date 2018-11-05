/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
 
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_cli.h"

#include "main.h"

#include <string.h>
#include "stringx.h"
#include "bufmgr.h"

#if (HAL_CLI_EN > 0)

#define HAL_CLI_RX_DATA_BLK_SIZE    8

static void *tx_fifo;
//#define IS_CHAR(x)  ( x > 0 && x <= 127 )


extern void hal_cli_init( void )
{   
    tx_fifo = NULL;
}

extern void hal_cli_print_char(char c)
{
    uint8_t *pc;
    if(tx_fifo == NULL)
        tx_fifo = osal_fifo_create();
    HAL_ASSERT( tx_fifo != NULL );
    pc = osal_fifo_put( tx_fifo, (uint8_t)c );
    HAL_ASSERT( pc != NULL );
    osal_event_set( TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_CLI_TXE );
}

extern void hal_cli_print_str(const char *s)
{
    while(*s)
    {
        hal_cli_print_char(*s++);
    }
}

extern void hal_cli_print_sint(int32_t num)
{
    char str[SINT_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = sintstr(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_print_char(str[i]);
    }
    
}

extern void hal_cli_print_uint(uint32_t num)
{
    char str[UINT_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = uintstr(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_print_char(str[i]);
    }
}

extern void hal_cli_print_hex8(uint8_t num)
{
    char str[HEX8_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = hex8str(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_print_char(str[i]);
    }
}


extern void hal_cli_print_hex16(uint16_t num)
{
    char str[HEX16_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = hex16str(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_print_char(str[i]);
    }
}

extern void hal_cli_print_hex32(uint32_t num)
{
    char str[HEX32_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = hex32str(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_print_char(str[i]);
    }
}

extern void hal_cli_driver_handle_rxne( void )
{
    char c;
    uint16_t cnt;
    char *p_buf;
    void *p_msg;
    
    p_buf = (char *)osal_mem_alloc( HAL_CLI_RX_DATA_BLK_SIZE );
    HAL_ASSERT( p_buf != NULL );
    cnt = 0;
    
    while( !spl_uart_rxd_empty(SPL_UART_PORT_0) )
    {
        c = spl_uart_rxd( SPL_UART_PORT_0 );
        p_buf[cnt++] = c;
        if( (cnt % HAL_CLI_RX_DATA_BLK_SIZE) == 0 )
        {
            p_buf = (char *)osal_mem_realloc( p_buf, cnt + HAL_CLI_RX_DATA_BLK_SIZE );
            HAL_ASSERT( p_buf != NULL );
        }
    }

    if( cnt )
    {
        p_msg = osal_msg_create( cnt );
        memcpy( p_msg, p_buf, cnt );
        osal_msg_send( p_msg, TASK_ID_APP_CLI );
    }
    
    osal_mem_free( p_buf );
}

extern void hal_cli_driver_handle_txe( void )
{
    uint8_t b;
    
    if( tx_fifo != NULL )
    {
        while( !spl_uart_txd_full(SPL_UART_PORT_0) )
        {
            if( osal_fifo_len( tx_fifo ) )
            {
                b = osal_fifo_get(tx_fifo);
                spl_uart_txd( SPL_UART_PORT_0, b );
            }
            else
            {
                osal_fifo_delete( tx_fifo );
                tx_fifo = NULL;
                break;
            }
        }
    }
}

extern void spl_uart0_callback( uint8_t event )
{
    switch (event)
    {
        case SPL_UART_ISR_EVT_RXD:
            osal_event_set( TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_CLI_RXNE );
        break;

        case SPL_UART_ISR_EVT_TXD_EMPTY:
            osal_event_set( TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_CLI_TXE );
        break;
        
        case SPL_UART_ISR_EVT_RXD_FULL:
            //osal_event_set( OSAL_TASK_ID_HAL_DRIVERS, OSAL_TASK_EVT_HAL_CLI_RXF );
        break;

        default:
        break;
    }
    
}

#endif /* (HAL_CLI_EN > 0) */

