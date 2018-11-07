#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_wifimod.h"
#include "hal_lucicmd.h"

#include "main.h"

#include <string.h>
#include "stringx.h"
#include "bufmgr.h"


/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/
#define LUCICMD_HEAD_LEN        10
#define LUCICMD_DATA_LEN_MAX    128

#define HAL_WIFIMOD_HN_TIMEOUT  15000

#define HAL_WIFIMOD_FLAG_RDY            0x01

/**************************************************************************************************
 * TYPES
 **************************************************************************************************/
typedef struct lucicmd_rx_ctrl_t {
    uint8_t *p_data;
    uint8_t head[LUCICMD_HEAD_LEN];
    uint16_t len;
} LUCICMD_RX_CTRL_t;

typedef struct wifi_mod_info_t {
    uint8_t flags;
    uint8_t mode_current;
    uint8_t mode_target;
    uint8_t src_current;
    uint8_t state;
    uint8_t vol;
} WIFI_MOD_INFO_t;


#define HAL_WIFIMOD_RESET_LOW()     spl_gpio_write_pin(SPL_GPIO_PORT_1, SPL_GPIO_PIN_1, 0)
#define HAL_WIFIMOD_RESET_HIGH()    spl_gpio_write_pin(SPL_GPIO_PORT_1, SPL_GPIO_PIN_1, 1)

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
static LUCICMD_RX_CTRL_t *p_rx_ctrl;
static WIFI_MOD_INFO_t wifi_mod_info;
/**************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/
static uint16_t hal_wifimod_process_luci_cmd( const HAL_LUCI_CMD_t *p_luci_cmd );
static void hal_wifimod_info_init( void );
static void hal_wifimod_reboot( void );


extern void hal_wifimod_init( void )
{
    p_rx_ctrl = NULL;
    HAL_WIFIMOD_RESET_LOW();
    hal_wifimod_info_init();
}

extern void hal_wifimod_enable ( void )
{
    HAL_WIFIMOD_RESET_HIGH();
}

extern void hal_wifimod_disable    ( void )
{
    HAL_WIFIMOD_RESET_LOW();
    hal_wifimod_info_init();
    osal_event_clr( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_RESET );
    osal_event_clr( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_MODE_CTRL );
    osal_event_clr( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT );
    osal_timer_event_delete( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT );
}

extern void     hal_wifimod_set_mode( uint8_t mode )
{
    if( wifi_mod_info.mode_current != mode && mode != HAL_WIFIMOD_MODE_NONE )
    {
        wifi_mod_info.mode_target = mode;
        if( wifi_mod_info.flags & HAL_WIFIMOD_FLAG_RDY )
        {
            osal_event_set( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_MODE_CTRL );
        }
    }
}

extern uint8_t  hal_wifimod_get_mode( void )
{
    return wifi_mod_info.mode_current;
}

extern void     hal_wifimod_set_src( uint8_t src )
{
    if( wifi_mod_info.flags & HAL_WIFIMOD_FLAG_RDY )
    {
        if( src == HAL_WIFIMOD_SRC_AUXIN )
        {
            if( wifi_mod_info.src_current != HAL_WIFIMOD_SRC_AUXIN )
            {
                hal_lucicmd_send_AuxInStart();
            }
        }
        else
        {
            if( wifi_mod_info.src_current == HAL_WIFIMOD_SRC_AUXIN )
            {
                hal_lucicmd_send_AuxInStop();
            }
        }
    }
}

extern uint8_t  hal_wifimod_get_src( void )
{
    return wifi_mod_info.src_current;
}

extern void     hal_wifimod_set_vol( uint8_t vol )
{
    if( wifi_mod_info.flags & HAL_WIFIMOD_FLAG_RDY )
    {
        if( vol != wifi_mod_info.vol )
        {
            wifi_mod_info.vol = vol;
            hal_lucicmd_send_SetVolume( vol );
        }
    }
}

extern uint8_t  hal_wifimod_get_vol( void )
{
    return wifi_mod_info.vol;
}

extern uint8_t  hal_wifimod_play_state( void )
{
    return wifi_mod_info.state;
}

extern void     hal_wifimod_play_ctrl( uint8_t plctl )
{

}

extern void hal_wifimod_driver_handle_rx_bytes( uint8_t *p_data, uint16_t len )
{
    HAL_LUCI_CMD_t *p_luci_cmd;
    uint16_t data_len;
    uint8_t b;

    while( len )
    {
        len--;
        b = *p_data++;

        if( p_rx_ctrl == NULL )
        {
            if( b == 0xAA )
            {
                //YES! Received a new luci command, allocate the rx ctrl space
                p_rx_ctrl = (LUCICMD_RX_CTRL_t *)osal_mem_alloc( sizeof(LUCICMD_RX_CTRL_t) );
                HAL_ASSERT( p_rx_ctrl != NULL );
                p_rx_ctrl->len = 0;
                p_rx_ctrl->p_data = NULL;
                p_rx_ctrl->head[p_rx_ctrl->len++] = b;
            }
        }
        else
        {
            if( p_rx_ctrl->len == 1 )
            {
                if( b == 0xAA )
                {
                    p_rx_ctrl->head[p_rx_ctrl->len++] = b;
                }
                else
                {
                    osal_mem_free( p_rx_ctrl );
                    p_rx_ctrl = NULL;
                }
            }
            else
            {
                p_luci_cmd = NULL;
                if( p_rx_ctrl->len < LUCICMD_HEAD_LEN )
                {
                    p_rx_ctrl->head[p_rx_ctrl->len++] = b;
                    if( p_rx_ctrl->len == LUCICMD_HEAD_LEN )
                    {
                        data_len = BUILD_UINT16( p_rx_ctrl->head[9], p_rx_ctrl->head[8] );
                        if( data_len == 0 )
                        {
                            p_luci_cmd = (HAL_LUCI_CMD_t *)osal_msg_create( sizeof(HAL_LUCI_CMD_t) );
                            HAL_ASSERT( p_luci_cmd != NULL );
                        }
                        else
                        {
                            if( data_len < LUCICMD_DATA_LEN_MAX )
                                p_rx_ctrl->p_data = (uint8_t *)osal_mem_alloc( data_len + 1 );
                            HAL_ASSERT( p_rx_ctrl->p_data != NULL );
                        }
                    }
                }
                else
                {
                    data_len = BUILD_UINT16( p_rx_ctrl->head[9], p_rx_ctrl->head[8] );
                    HAL_ASSERT( data_len != 0 );
                    HAL_ASSERT( p_rx_ctrl->p_data != NULL );
                    p_rx_ctrl->p_data[p_rx_ctrl->len - LUCICMD_HEAD_LEN] = b;
                    p_rx_ctrl->len++;
                    
                    if( p_rx_ctrl->len - LUCICMD_HEAD_LEN == data_len )
                    {
                        p_luci_cmd = (HAL_LUCI_CMD_t *)osal_msg_create( sizeof(HAL_LUCI_CMD_t) );
                        HAL_ASSERT( p_luci_cmd != NULL );
                    }
                }

                if( p_luci_cmd != NULL )
                {
                    p_luci_cmd->remote_id = BUILD_UINT16( p_rx_ctrl->head[1], p_rx_ctrl->head[0] );
                    p_luci_cmd->type = p_rx_ctrl->head[2];
                    p_luci_cmd->msgbox = BUILD_UINT16( p_rx_ctrl->head[4], p_rx_ctrl->head[3] );
                    p_luci_cmd->status = p_rx_ctrl->head[5];
                    p_luci_cmd->crc = BUILD_UINT16( p_rx_ctrl->head[7], p_rx_ctrl->head[6] );
                    p_luci_cmd->len = BUILD_UINT16( p_rx_ctrl->head[9], p_rx_ctrl->head[8] );
                    p_luci_cmd->p_data = p_rx_ctrl->p_data;
                    if( p_luci_cmd->p_data )
                        p_luci_cmd->p_data[p_luci_cmd->len] = '\0';
                    
                    osal_mem_free( p_rx_ctrl );
                    p_rx_ctrl = NULL;

                    HAL_ASSERT( hal_lucicmd_crc16_ccitt( p_luci_cmd ) == p_luci_cmd->crc );
                    if( hal_wifimod_process_luci_cmd( p_luci_cmd ) == HAL_LUCICMD_MSGBOX_NULL )
                    {
                        //unprocessed command, send to APP layer task
                        osal_msg_send( p_luci_cmd, TASK_ID_APP_WIFIMOD );
                    }
                    else
                    {
                        //processed command, APP layer do not care, free the RAM allocated
                        if( p_luci_cmd->p_data )
                            osal_mem_free( p_luci_cmd->p_data );
                        osal_msg_delete( p_luci_cmd );
                    }
                }
            }
        }
    }
}

//This is a internal event handler in HAL Wi-Fi module driver. 
//It will be triggered when user want to change mode.
//It will be triggered when the mode is updated on module side.
extern void hal_wifimod_driver_handle_mode_ctrl( void )
{
    uint8_t cases;

    cases = BUILD_UINT8( wifi_mod_info.mode_target, wifi_mod_info.mode_current );

    switch (cases)
    {
        case BUILD_UINT8( HAL_WIFIMOD_MODE_NONE, HAL_WIFIMOD_MODE_NONE ):
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_NONE, HAL_WIFIMOD_MODE_SA ):
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_MODE );
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_NONE, HAL_WIFIMOD_MODE_HN ):
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_MODE );
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_NONE, HAL_WIFIMOD_MODE_CFG ):
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_MODE );
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_SA, HAL_WIFIMOD_MODE_NONE ):
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_SA, HAL_WIFIMOD_MODE_SA ):
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_MODE );
            wifi_mod_info.mode_target = HAL_WIFIMOD_MODE_NONE;
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_SA, HAL_WIFIMOD_MODE_HN ):
            hal_lucicmd_send_SetWiFiModeSA();
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_SA, HAL_WIFIMOD_MODE_CFG ):
            hal_lucicmd_send_SetWiFiModeSA();
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_HN, HAL_WIFIMOD_MODE_NONE ):
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_HN, HAL_WIFIMOD_MODE_SA ):
            hal_lucicmd_send_SetWiFiModeHN();
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_HN, HAL_WIFIMOD_MODE_HN ):
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_MODE );
            wifi_mod_info.mode_target = HAL_WIFIMOD_MODE_NONE;
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_HN, HAL_WIFIMOD_MODE_CFG ):
            hal_lucicmd_send_SetWiFiModeSA();
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_CFG, HAL_WIFIMOD_MODE_NONE ):
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_CFG, HAL_WIFIMOD_MODE_SA ):
            hal_lucicmd_send_SetWiFiModeHN();
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_CFG, HAL_WIFIMOD_MODE_HN ):
            hal_lucicmd_send_SetWiFiModeCFG();
        break;

        case BUILD_UINT8( HAL_WIFIMOD_MODE_CFG, HAL_WIFIMOD_MODE_CFG ):
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_MODE );
            wifi_mod_info.mode_target = HAL_WIFIMOD_MODE_NONE;
        break;
    }
}

extern void hal_wifimod_driver_handle_src_ctrl( void )
{
    
}

extern void hal_wifimod_driver_handle_hn_timeout( void )
{
    if( wifi_mod_info.mode_target == HAL_WIFIMOD_MODE_SA )
    {
        hal_lucicmd_send_SetWiFiModeSA();
    }
    else
    {
        wifi_mod_info.mode_target = HAL_WIFIMOD_MODE_CFG;
        hal_lucicmd_send_SetWiFiModeCFG();
    }
}

static void hal_wifimod_info_init( void )
{
    wifi_mod_info.mode_current = HAL_WIFIMOD_MODE_NONE;
    wifi_mod_info.mode_target = HAL_WIFIMOD_MODE_NONE;
    wifi_mod_info.src_current = HAL_WIFIMOD_SRC_NONE;
    wifi_mod_info.state = HAL_WIFIMOD_PLSTA_STOPPED;
    wifi_mod_info.vol = 0;
    wifi_mod_info.flags = 0;

    if( p_rx_ctrl != NULL )
    {
        osal_mem_free( p_rx_ctrl );
        p_rx_ctrl = NULL;
    }
}

static void hal_wifimod_reboot( void )
{
    HAL_WIFIMOD_RESET_LOW();
    
    wifi_mod_info.mode_current = HAL_WIFIMOD_MODE_NONE;
    //wifi_mod_info.mode_target = HAL_WIFIMOD_MODE_NONE;
    wifi_mod_info.src_current = HAL_WIFIMOD_SRC_NONE;
    wifi_mod_info.state = HAL_WIFIMOD_PLSTA_STOPPED;
    wifi_mod_info.vol = 0;
    wifi_mod_info.flags = 0;
    
    if( p_rx_ctrl != NULL )
    {
        osal_mem_free( p_rx_ctrl );
        p_rx_ctrl = NULL;
    }

    osal_event_clr( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_MODE_CTRL );
    osal_timer_event_create( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_RESET, 10 );
    
    osal_event_clr( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT );
    osal_timer_event_delete( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT );

    osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_RESET);
    
}

static uint16_t hal_wifimod_process_luci_cmd( const HAL_LUCI_CMD_t *p_luci_cmd )
{
    uint32_t u32tmp;
    char cActiveNetwork;
    char wlanState;
    uint8_t mode;

    switch ( p_luci_cmd->msgbox )
    {
        case HAL_LUCICMD_MSGBOX_CurrentPlayState:
            HAL_ASSERT( p_luci_cmd->p_data != NULL );
            decstr2uint( (char *)(p_luci_cmd->p_data), &u32tmp );
            wifi_mod_info.state = (uint8_t)u32tmp;
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_STATE );
        return HAL_LUCICMD_MSGBOX_CurrentPlayState;

        case HAL_LUCICMD_MSGBOX_UartInterfaceReady:
            hal_lucicmd_send_IsHostPresent();
        return HAL_LUCICMD_MSGBOX_UartInterfaceReady;

        case HAL_LUCICMD_MSGBOX_IsHostPresent:
            wifi_mod_info.flags |= HAL_WIFIMOD_FLAG_RDY;
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_BOOT_UP );
        return HAL_LUCICMD_MSGBOX_IsHostPresent;

        case HAL_LUCICMD_MSGBOX_VolumeCtrl:
            if( p_luci_cmd->status == HAL_LUCICMD_STATUS_NA )
            {
                //Volume from APP side
                u32tmp = UINT32_MAX;
                HAL_ASSERT( p_luci_cmd->p_data != NULL );
                decstr2uint( (char *)(p_luci_cmd->p_data), &u32tmp );
                HAL_ASSERT( u32tmp <= 100 );
                if( (uint8_t)u32tmp != wifi_mod_info.vol )
                {
                    wifi_mod_info.vol = (uint8_t)u32tmp;
                    osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_VOL );
                }
            }
            else if( p_luci_cmd->status == HAL_LUCICMD_STATUS_OK )
            {
                //Volume from HW, APP confirmed this volume
            }
            else
            {
                HAL_ASSERT_FORCED();
            }
        return HAL_LUCICMD_MSGBOX_VolumeCtrl;

        case HAL_LUCICMD_MSGBOX_ZoneVolumeCtrl:
            if( p_luci_cmd->status == HAL_LUCICMD_STATUS_NA &&
                p_luci_cmd->type   == HAL_LUCICMD_TYPE_SET )
            {
                //Volume from APP
                u32tmp = UINT32_MAX;
                HAL_ASSERT( p_luci_cmd->p_data != NULL );
                decstr2uint( (char *)(p_luci_cmd->p_data), &u32tmp );
                HAL_ASSERT( u32tmp <= 100 );
                if( (uint8_t)u32tmp != wifi_mod_info.vol )
                {
                    wifi_mod_info.vol = (uint8_t)u32tmp;
                    osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_VOL );
                }
            }
        return HAL_LUCICMD_MSGBOX_ZoneVolumeCtrl;

        case HAL_LUCICMD_MSGBOX_CurrentSource:
            if( decstr2uint( (char *)(p_luci_cmd->p_data), &u32tmp ) == 0 )
                HAL_ASSERT_FORCED();
            if( u32tmp == HAL_LUCICMD_AUDIO_SRC_NONE )
                wifi_mod_info.src_current = HAL_WIFIMOD_SRC_NONE;
            else if( u32tmp == HAL_LUCICMD_AUDIO_SRC_LineIn )
                wifi_mod_info.src_current = HAL_WIFIMOD_SRC_AUXIN;
            else if( u32tmp == HAL_LUCICMD_AUDIO_SRC_SDCard )
                wifi_mod_info.src_current = HAL_WIFIMOD_SRC_SD;
            else
                wifi_mod_info.src_current = HAL_WIFIMOD_SRC_WIFI;
            osal_event_set( TASK_ID_APP_WIFIMOD, TASK_EVT_APP_WIFIMOD_UPD_SRC );
        return HAL_LUCICMD_MSGBOX_CurrentSource;

        case HAL_LUCICMD_MSGBOX_UpdateSource:
            hal_lucicmd_send_IsAllowed();
        return HAL_LUCICMD_MSGBOX_UpdateSource;

        case HAL_LUCICMD_MSGBOX_RebootRequest:
            hal_wifimod_reboot();
        return HAL_LUCICMD_MSGBOX_RebootRequest;

        case HAL_LUCICMD_MSGBOX_NetworkConnectionStatus:
            cActiveNetwork = p_luci_cmd->p_data[0];
            wlanState = p_luci_cmd->p_data[4];

            if(cActiveNetwork == '3' || cActiveNetwork == '1')
            {
                if(wlanState == '0')        mode = HAL_WIFIMOD_MODE_SA;
                else                        mode = HAL_WIFIMOD_MODE_HN;
            }
            else if(cActiveNetwork == '0')  mode = HAL_WIFIMOD_MODE_NONE;
            else if(cActiveNetwork == '4')  mode = HAL_WIFIMOD_MODE_CFG;
            else                            mode = HAL_WIFIMOD_MODE_NONE;

            if( mode != wifi_mod_info.mode_current )
            {
                wifi_mod_info.mode_current = mode;
                osal_event_set( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_MODE_CTRL );
            }

            if( mode == HAL_WIFIMOD_MODE_NONE )
            {
                osal_timer_event_create( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT, HAL_WIFIMOD_HN_TIMEOUT );
            }
            else
            {
                osal_timer_event_delete( TASK_ID_HAL_DRIVER_WIFIMOD, TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT );
            }
        return HAL_LUCICMD_MSGBOX_NetworkConnectionStatus;


        //case HAL_LUCICMD_MSGBOX_NetworkConfigurationStatus:
        //    if( decstr2uint( (char *)(p_luci_cmd->p_data), &u32tmp ) == 0 )
        //    {
        //        HAL_ASSERT_FORCED();
        //    }
        //
        //    AppEventLuciUpdateNwConfigStatus((uint8_t)u32tmp);
        //break;
        
        //case HAL_LUCICMD_MSGBOX_AuxInputStart:
        //break;

        //case HAL_LUCICMD_MSGBOX_AuxInputStop:
        //break;

        //case HAL_LUCICMD_MSGBOX_NvReadWrite:
        //break;

        //case HAL_LUCICMD_MSGBOX_NetworkConfigurationStatus:
        //break;

        //case HAL_LUCICMD_MSGBOX_MACID:
        //break;
    }


    return HAL_LUCICMD_MSGBOX_NULL;
}



