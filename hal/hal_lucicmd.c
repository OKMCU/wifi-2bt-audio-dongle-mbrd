#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_lucicmd.h"

#include "main.h"

#include <string.h>
#include "stringx.h"
#include "bufmgr.h"

#define HAL_LUCICMD_RX_DATA_BLK_SIZE     8

static void *tx_fifo;

static void hal_lucicmd_send_byte( uint8_t b )
{
    uint8_t *pc;
    if(tx_fifo == NULL)
        tx_fifo = osal_fifo_create();
    HAL_ASSERT( tx_fifo != NULL );
    pc = osal_fifo_put( tx_fifo, b );
    HAL_ASSERT( pc != NULL );
}

extern void hal_lucicmd_init( void )
{
    tx_fifo = NULL;
}

extern uint16_t hal_lucicmd_crc16_ccitt( const HAL_LUCI_CMD_t *p_lucicmd )
{
	uint16_t i;
	uint16_t crc = 0;
    
    const uint16_t crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
    };
    
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ ((p_lucicmd->remote_id >> 8) & 0xFF))         & 0xFF];
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ ((p_lucicmd->remote_id >> 0) & 0xFF))         & 0xFF];
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ ((uint8_t)p_lucicmd->type   & 0xFF))         & 0xFF];
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ (((uint16_t)p_lucicmd->msgbox >> 8) & 0xFF)) & 0xFF];
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ (((uint16_t)p_lucicmd->msgbox >> 0) & 0xFF)) & 0xFF];
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ ((uint8_t)p_lucicmd->status & 0xFF))         & 0xFF];
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ ((p_lucicmd->len  >> 8)     & 0xFF))         & 0xFF];
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ ((p_lucicmd->len  >> 0)     & 0xFF))         & 0xFF];
    
    for( i = 0; i < p_lucicmd->len; i++ )
    {
        crc = (crc<<8) ^ crc16tab[((crc>>8) ^ p_lucicmd->p_data[i])&0x00FF];
    }
	return crc;
}


extern void hal_lucicmd_send( const HAL_LUCI_CMD_t *p_lucicmd )
{
    uint16_t i;
    
    hal_lucicmd_send_byte((p_lucicmd->remote_id         >> 8) & 0xFF );
    hal_lucicmd_send_byte((p_lucicmd->remote_id         >> 0) & 0xFF );
    hal_lucicmd_send_byte((uint8_t)p_lucicmd->type           & 0xFF );
    hal_lucicmd_send_byte(((uint16_t)p_lucicmd->msgbox >> 8) & 0xFF );
    hal_lucicmd_send_byte(((uint16_t)p_lucicmd->msgbox >> 0) & 0xFF );
    hal_lucicmd_send_byte((uint8_t)p_lucicmd->status         & 0xFF );
    hal_lucicmd_send_byte((p_lucicmd->crc      >> 8)         & 0xFF );
    hal_lucicmd_send_byte((p_lucicmd->crc      >> 0)         & 0xFF );
    hal_lucicmd_send_byte((p_lucicmd->len      >> 8)         & 0xFF );
    hal_lucicmd_send_byte((p_lucicmd->len      >> 0)         & 0xFF );

    for( i = 0; i < p_lucicmd->len; i++ )
    {
        hal_lucicmd_send_byte( p_lucicmd->p_data[i] );
    }

    osal_event_set( TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_LUCICMD_TXE );
}

extern void hal_lucicmd_send_IsHostPresent ( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_IsHostPresent;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = "1";
    lucicmd.len = 1;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );
    hal_lucicmd_send( &lucicmd );
}

extern void hal_lucicmd_send_SetVolume( uint8_t vol )
{
    HAL_LUCI_CMD_t lucicmd;
    uint16_t len;
    char buf[UINT_STR_LEN_MAX];
    
    if( vol > 100 )
        vol = 100;
    
    len = uintstr( vol, buf );
    
    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_VolumeCtrl;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = (uint8_t *)buf;
    lucicmd.len = len;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
    
}

extern void hal_lucicmd_send_QryVolume( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_VolumeCtrl;
    lucicmd.type = HAL_LUCICMD_TYPE_GET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = NULL;
    lucicmd.len = 0;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}

extern void hal_lucicmd_send_SavVolume( uint8_t vol )
{
    HAL_LUCI_CMD_t lucicmd;
    uint16_t len;
    char *p_data;

    if( vol > 100 )
        vol = 100;
    
    len = (uint16_t)strlen( "WRITE_current_volume," );
    p_data = osal_mem_alloc( len + UINT_STR_LEN_MAX );
    HAL_ASSERT( p_data != NULL );
    memcpy( p_data, "WRITE_current_volume,", len );
    len += uintstr( vol, p_data + len );
    
    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_NvReadWrite;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = (uint8_t *)p_data;
    lucicmd.len = len;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );

    osal_mem_free( p_data );
    
}

extern void hal_lucicmd_send_AuxInStart( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_AuxInputStart;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = NULL;
    lucicmd.len = 0;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );

}

extern void hal_lucicmd_send_AuxInStop( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_AuxInputStop;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = NULL;
    lucicmd.len = 0;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}

extern void hal_lucicmd_send_PlayUSB( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_AppCtrl;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = "PLAYUSB";
    lucicmd.len = 7;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}

extern void hal_lucicmd_send_SetWiFiModeHN( void )
{
    hal_lucicmd_send_SetWiFiModeCFG();
}

extern void hal_lucicmd_send_SetWiFiModeCFG( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_NetworkConfiguration;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = NULL;
    lucicmd.len = 0;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}

extern void hal_lucicmd_send_SetWiFiModeSA( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_StandAloneMode;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = NULL;
    lucicmd.len = 0;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}

extern void hal_lucicmd_send_PlayCtrl( uint8_t play_ctrl )
{
    HAL_LUCI_CMD_t lucicmd;
    
    const char *p_data[] = {
        "PLAY",
        "PAUSE",
        "STOP",
        "RESUME",
        "NEXT",
        "PREV",
        "MUTE",
        "UNMUTE",
    };

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_PlayCtrlCmd;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = (uint8_t *)p_data[play_ctrl];
    lucicmd.len = strlen( p_data[play_ctrl] );
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );

    
}

extern void hal_lucicmd_send_QryMacID      ( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_MACID;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = NULL;
    lucicmd.len = 0;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}


extern void hal_lucicmd_send_IsAllowed     ( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_IsAllowed;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = "1";
    lucicmd.len = 1;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}

extern void hal_lucicmd_send_IsNotAllowed     ( void )
{
    HAL_LUCI_CMD_t lucicmd;

    lucicmd.remote_id = 0xAAAA;
    lucicmd.msgbox = HAL_LUCICMD_MSGBOX_IsAllowed;
    lucicmd.type = HAL_LUCICMD_TYPE_SET;
    lucicmd.status = HAL_LUCICMD_STATUS_NA;
    lucicmd.p_data = "0";
    lucicmd.len = 1;
    lucicmd.crc = hal_lucicmd_crc16_ccitt( &lucicmd );

    hal_lucicmd_send( &lucicmd );
}


extern void hal_lucicmd_driver_handle_rxne( void )
{
    char c;
    uint16_t cnt;
    char *p_buf;
    void *p_msg;
    
    p_buf = (char *)osal_mem_alloc( HAL_LUCICMD_RX_DATA_BLK_SIZE );
    HAL_ASSERT( p_buf != NULL );
    cnt = 0;
    
    while( !spl_uart_rxd_empty( SPL_UART_PORT_1 ) )
    {
        c = spl_uart_rxd( SPL_UART_PORT_1 );
        p_buf[cnt++] = c;
        if( (cnt % HAL_LUCICMD_RX_DATA_BLK_SIZE) == 0 )
        {
            p_buf = (char *)osal_mem_realloc( p_buf, cnt + HAL_LUCICMD_RX_DATA_BLK_SIZE );
            HAL_ASSERT( p_buf != NULL );
        }
    }

    if( cnt )
    {
        p_msg = osal_msg_create( cnt );
        memcpy( p_msg, p_buf, cnt );
        osal_msg_send( p_msg, TASK_ID_HAL_DRIVER_WIFIMOD );
    }
    
    osal_mem_free( p_buf );
}

extern void hal_lucicmd_driver_handle_txe( void )
{
    uint8_t b;
    
    if( tx_fifo != NULL )
    {
        while( !spl_uart_txd_full(SPL_UART_PORT_1) )
        {
            if( osal_fifo_len(tx_fifo) )
            {
                b = osal_fifo_get( tx_fifo );
                spl_uart_txd( SPL_UART_PORT_1, b );
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

extern void spl_uart1_callback( uint8_t event )
{
    switch (event)
    {
        case SPL_UART_ISR_EVT_RXD:
            osal_event_set( TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_LUCICMD_RXNE );
        break;

        case SPL_UART_ISR_EVT_TXD_EMPTY:
            osal_event_set( TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_LUCICMD_TXE );
        break;
        
        case SPL_UART_ISR_EVT_RXD_FULL:
            //osal_event_set( OSAL_TASK_ID_HAL_DRIVERS, OSAL_TASK_EVT_HAL_CLI_RXF );
        break;

        default:
        break;
    }
    
}

