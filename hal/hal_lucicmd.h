#ifndef __HAL_LUCICMD_H__
#define __HAL_LUCICMD_H__


#include <stdint.h>

#define HAL_LUCICMD_MSGBOX_NULL                                 0
#define HAL_LUCICMD_MSGBOX_VersionInfo                          5
#define HAL_LUCICMD_MSGBOX_HostVersionInfo                      6
#define HAL_LUCICMD_MSGBOX_IsHostPresent                        9
#define HAL_LUCICMD_MSGBOX_UpdateSource                         10
#define HAL_LUCICMD_MSGBOX_IsAllowed                            11
#define HAL_LUCICMD_MSGBOX_Airplay                              12
#define HAL_LUCICMD_MSGBOX_ACPShareCmd                          14
#define HAL_LUCICMD_MSGBOX_ACPShareResponse                     15
#define HAL_LUCICMD_MSGBOX_PowerSaveDeepSleepStart              20
#define HAL_LUCICMD_MSGBOX_PowerSaveDeepSleepEnd                21
#define HAL_LUCICMD_MSGBOX_PowerSaveStandByStart                22
#define HAL_LUCICMD_MSGBOX_PowerSaveStandByEnd                  23
#define HAL_LUCICMD_MSGBOX_PowerSaveStatus                      24
#define HAL_LUCICMD_MSGBOX_DeviceDeattachmentStatus             36
#define HAL_LUCICMD_MSGBOX_GracefulShutdown                     37
#define HAL_LUCICMD_MSGBOX_DeviceAttachmentStatus               38
#define HAL_LUCICMD_MSGBOX_PlayCtrlCmd                          40
#define HAL_LUCICMD_MSGBOX_BrowseCtrlCmd                        41
#define HAL_LUCICMD_MSGBOX_GetUI                                42
#define HAL_LUCICMD_MSGBOX_CurrentTime                          49
#define HAL_LUCICMD_MSGBOX_CurrentSource                        50
#define HAL_LUCICMD_MSGBOX_CurrentPlayState                     51
#define HAL_LUCICMD_MSGBOX_IsPlayStatus                         54
#define HAL_LUCICMD_MSGBOX_VolumeCtrl                           64
#define HAL_LUCICMD_MSGBOX_FirmwareUpgradeRequest               65
#define HAL_LUCICMD_MSGBOX_FirmwareUpgradeProcess               66
#define HAL_LUCICMD_MSGBOX_HostImagePresent                     68
#define HAL_LUCICMD_MSGBOX_RequestFirmwareUpgrade               69
#define HAL_LUCICMD_MSGBOX_AppCtrl                              70
#define HAL_LUCICMD_MSGBOX_SDCardStatus                         71
#define HAL_LUCICMD_MSGBOX_SpotifyPresetActions                 75
#define HAL_LUCICMD_MSGBOX_PlayAudioIndex                       80
#define HAL_LUCICMD_MSGBOX_DeviceName                           90
#define HAL_LUCICMD_MSGBOX_MACID                                91
#define HAL_LUCICMD_MSGBOX_AuxInputStart                        95
#define HAL_LUCICMD_MSGBOX_AuxInputStop                         96
#define HAL_LUCICMD_MSGBOX_ExternalPlayback                     97
#define HAL_LUCICMD_MSGBOX_DDMS_Trigger                         100
#define HAL_LUCICMD_MSGBOX_StandAloneMode                       101
#define HAL_LUCICMD_MSGBOX_SearchLsModule                       102
#define HAL_LUCICMD_MSGBOX_QueryDDMS                            103
#define HAL_LUCICMD_MSGBOX_SetZoneID                            104
#define HAL_LUCICMD_MSGBOX_DDMS_SSID                            105
#define HAL_LUCICMD_MSGBOX_SpeakerType                          106
#define HAL_LUCICMD_MSGBOX_SceneName                            107
#define HAL_LUCICMD_MSGBOX_TcpipTunnelingStart                  111
#define HAL_LUCICMD_MSGBOX_TunelData                            112
#define HAL_LUCICMD_MSGBOX_MiracastCtrl                         113
#define HAL_LUCICMD_MSGBOX_RebootRequest                        114
#define HAL_LUCICMD_MSGBOX_RebootCmd                            115
#define HAL_LUCICMD_MSGBOX_NetworkConnectionStatus              124
#define HAL_LUCICMD_MSGBOX_ConfigureNetwork                     125
#define HAL_LUCICMD_MSGBOX_iDeviceWifiSettingsSharing           126
#define HAL_LUCICMD_MSGBOX_WPS_ConfigurationStatus              140
#define HAL_LUCICMD_MSGBOX_WPS_Configuration                    141
#define HAL_LUCICMD_MSGBOX_NetworkConfiguration                 142
#define HAL_LUCICMD_MSGBOX_NetworkConfigurationStatus           143
#define HAL_LUCICMD_MSGBOX_StopWAC                              144
#define HAL_LUCICMD_MSGBOX_FactoryReset                         150
#define HAL_LUCICMD_MSGBOX_NvReadWrite                          208
#define HAL_LUCICMD_MSGBOX_BluetoothCtrlCmd                     209
#define HAL_LUCICMD_MSGBOX_DMR_Restart                          210
#define HAL_LUCICMD_MSGBOX_StartFirmwareUpgrade                 211
#define HAL_LUCICMD_MSGBOX_NamePasswordNotifier                 213
#define HAL_LUCICMD_MSGBOX_EnShareMode                          214
#define HAL_LUCICMD_MSGBOX_EnPairMode                           215
#define HAL_LUCICMD_MSGBOX_ZoneVolumeCtrl                       219
#define HAL_LUCICMD_MSGBOX_PairStatus                           221
#define HAL_LUCICMD_MSGBOX_CastOTAUpgrade                       222
#define HAL_LUCICMD_MSGBOX_FirmwareUpgradeInternet              223
#define HAL_LUCICMD_MSGBOX_CastIsEnabled                        224
#define HAL_LUCICMD_MSGBOX_GoogleCastSettingInfo                226
#define HAL_LUCICMD_MSGBOX_GetNTPTime                           229
#define HAL_LUCICMD_MSGBOX_AC_Powered                           232
#define HAL_LUCICMD_MSGBOX_UartInterfaceReady                   0xAAAA
#define HAL_LUCICMD_MSGBOX_CASTSetupStart                       0x01EE
    
#define HAL_LUCICMD_TYPE_GET                       0x01
#define HAL_LUCICMD_TYPE_SET                       0x02

#define HAL_LUCICMD_STATUS_NA                      0x00
#define HAL_LUCICMD_STATUS_OK                      0x01
#define HAL_LUCICMD_STATUS_GENERIC_ERR             0x02
#define HAL_LUCICMD_STATUS_DEV_NOT_RDY             0x03
#define HAL_LUCICMD_STATUS_CRC_ERR                 0x04

#define HAL_LUCICMD_PLAY_CTRL_PLAY     .           0
#define HAL_LUCICMD_PLAY_CTRL_PAUSE                1
#define HAL_LUCICMD_PLAY_CTRL_STOP                 2
#define HAL_LUCICMD_PLAY_CTRL_RESUME               3
#define HAL_LUCICMD_PLAY_CTRL_NEXT                 4
#define HAL_LUCICMD_PLAY_CTRL_PREV                 5
#define HAL_LUCICMD_PLAY_CTRL_MUTE                 6
#define HAL_LUCICMD_PLAY_CTRL_UNMUTE               7

#define HAL_LUCICMD_PLAY_STATE_PLAYING              0
#define HAL_LUCICMD_PLAY_STATE_STOPPED              1
#define HAL_LUCICMD_PLAY_STATE_PAUSED               2
#define HAL_LUCICMD_PLAY_STATE_CONNECTING           3
#define HAL_LUCICMD_PLAY_STATE_RECEIVING            4
#define HAL_LUCICMD_PLAY_STATE_BUFFERING            5

#define HAL_LUCICMD_NW_CFG_STATE_Initializing       0
#define HAL_LUCICMD_NW_CFG_STATE_Ready              1
#define HAL_LUCICMD_NW_CFG_STATE_ConfigStart        2
#define HAL_LUCICMD_NW_CFG_STATE_ConfigReceived     3
#define HAL_LUCICMD_NW_CFG_STATE_ConfigCompleted    4
#define HAL_LUCICMD_NW_CFG_STATE_Stop               5
#define HAL_LUCICMD_NW_CFG_STATE_Error              6
#define HAL_LUCICMD_NW_CFG_STATE_WACMsgTimeout      7
#define HAL_LUCICMD_NW_CFG_STATE_WACMsgConfigFailed 8
#define HAL_LUCICMD_NW_CFG_STATE_SACFailed          9

typedef struct hal_luci_cmd_t {
    uint8_t *p_data;
    uint16_t remote_id;
    uint16_t msgbox;
    uint16_t len;
    uint16_t crc;
    uint8_t type;
    uint8_t status;
} HAL_LUCI_CMD_t;

extern void     hal_lucicmd_init       ( void );
extern void     hal_lucicmd_send       ( const HAL_LUCI_CMD_t *p_lucicmd );
extern uint16_t hal_lucicmd_crc16_ccitt( const HAL_LUCI_CMD_t *p_lucicmd );

extern void     hal_lucicmd_send_IsHostPresent ( void );
extern void     hal_lucicmd_send_SetVolume     ( uint8_t vol );
extern void     hal_lucicmd_send_QryVolume     ( void );
extern void     hal_lucicmd_send_SavVolume     ( uint8_t vol );
extern void     hal_lucicmd_send_AuxInStart    ( void );
extern void     hal_lucicmd_send_AuxInStop     ( void );
extern void     hal_lucicmd_send_PlayUSB       ( void );
extern void     hal_lucicmd_send_SetWiFiModeHN ( void );
extern void     hal_lucicmd_send_SetWiFiModeCFG( void );
extern void     hal_lucicmd_send_SetWiFiModeSA ( void );
extern void     hal_lucicmd_send_PlayCtrl      ( uint8_t play_ctrl );
extern void     hal_lucicmd_send_QryMacID      ( void );
extern void     hal_lucicmd_send_IsAllowed     ( void );
extern void     hal_lucicmd_send_IsNotAllowed  ( void );
extern void     hal_lucicmd_send_ExtSrcStart   ( void );
extern void     hal_lucicmd_send_ExtSrcStop    ( void );

#endif /* __HAL_LUCICMD_H__ */
