#ifndef __HAL_WIFIMOD_H__
#define __HAL_WIFIMOD_H__

#include <stdint.h>

#define HAL_WIFIMOD_MODE_NONE                0
#define HAL_WIFIMOD_MODE_SA                  1
#define HAL_WIFIMOD_MODE_HN                  2
#define HAL_WIFIMOD_MODE_CFG                 3

#define HAL_WIFIMOD_SRC_NONE                 0
#define HAL_WIFIMOD_SRC_AIRPLAY              1
#define HAL_WIFIMOD_SRC_DMR                  2
#define HAL_WIFIMOD_SRC_DMP                  3
#define HAL_WIFIMOD_SRC_SPOTIFY              4
#define HAL_WIFIMOD_SRC_USB                  5
#define HAL_WIFIMOD_SRC_SD                   6
#define HAL_WIFIMOD_SRC_MELON                7
#define HAL_WIFIMOD_SRC_VTUNER               8
#define HAL_WIFIMOD_SRC_TUNEIN               9
#define HAL_WIFIMOD_SRC_MIRACAST             10
#define HAL_WIFIMOD_SRC_RESERVED11           11
#define HAL_WIFIMOD_SRC_DDMS_SLAVE           12
#define HAL_WIFIMOD_SRC_RESERVED13           13
#define HAL_WIFIMOD_SRC_AUXIN                14
#define HAL_WIFIMOD_SRC_RESERVED15           15
#define HAL_WIFIMOD_SRC_APPLE_DEVICE         16
#define HAL_WIFIMOD_SRC_DIRECT_URL           17
#define HAL_WIFIMOD_SRC_RESERVED18           18
#define HAL_WIFIMOD_SRC_BLUETOOTH            19
#define HAL_WIFIMOD_SRC_RESERVED20           20
#define HAL_WIFIMOD_SRC_DEEZER               21
#define HAL_WIFIMOD_SRC_TIDAL                22
#define HAL_WIFIMOD_SRC_FAVORITES            23
#define HAL_WIFIMOD_SRC_GOOGLE_CAST          24
#define HAL_WIFIMOD_SRC_EXT_SRC_CTL_BY_MCU   25
//#define HAL_WIFIMOD_SRC_NONE                 0x00
//#define HAL_WIFIMOD_SRC_AUXIN                0x01
//#define HAL_WIFIMOD_SRC_SD                   0x02
//#define HAL_WIFIMOD_SRC_WIFI                 0x04
//#define HAL_WIFIMOD_SRC_BT_SINGLE            26
//#define HAL_WIFIMOD_SRC_BT_PARTY             27
//#define HAL_WIFIMOD_SRC_BT_MIXER             28

#define HAL_WIFIMOD_PLSTA_PLAYING            0
#define HAL_WIFIMOD_PLSTA_STOPPED            1
#define HAL_WIFIMOD_PLSTA_PAUSED             2
#define HAL_WIFIMOD_PLSTA_CONNECTING         3
#define HAL_WIFIMOD_PLSTA_RECEIVING          4
#define HAL_WIFIMOD_PLSTA_BUFFERING          5

#define HAL_WIFIMOD_PLCTL_PLAY     .         0
#define HAL_WIFIMOD_PLCTL_PAUSE              1
#define HAL_WIFIMOD_PLCTL_STOP               2
#define HAL_WIFIMOD_PLCTL_RESUME             3
#define HAL_WIFIMOD_PLCTL_NEXT               4
#define HAL_WIFIMOD_PLCTL_PREV               5
#define HAL_WIFIMOD_PLCTL_MUTE               6
#define HAL_WIFIMOD_PLCTL_UNMUTE             7

extern void     hal_wifimod_init   ( void );
extern void     hal_wifimod_enable ( void );
extern void     hal_wifimod_disable( void );

extern void     hal_wifimod_set_mode( uint8_t mode );
extern uint8_t  hal_wifimod_get_mode( void );
extern void     hal_wifimod_set_src( uint8_t src );
extern uint8_t  hal_wifimod_get_src( void );
extern void     hal_wifimod_set_vol( uint8_t vol );
extern uint8_t  hal_wifimod_get_vol( void );
extern uint8_t  hal_wifimod_play_state( void );
extern void     hal_wifimod_play_ctrl( uint8_t plctl );

#endif /* __HAL_WIFIMOD_H__ */

