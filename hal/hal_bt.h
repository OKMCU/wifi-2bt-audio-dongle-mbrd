#ifndef __HAL_BT_H__
#define __HAL_BT_H__


#include <stdint.h>

#define HAL_BT_STATE_UNKNOWN        0x00
#define HAL_BT_STATE_OFF            0x01
#define HAL_BT_STATE_CONNECTABLE    0x02
#define HAL_BT_STATE_DISCOVERABLE   0x03
#define HAL_BT_STATE_PAUSED         0x04
#define HAL_BT_STATE_PLAYING        0x05

#define HAL_BT_CTRL_OFF             0x00
#define HAL_BT_CTRL_ON              0x01
#define HAL_BT_CTRL_PAIRING         0x02

#define HAL_BT_MOD_0                0
#define HAL_BT_MOD_1                1

extern void    hal_bt_init (void);
extern uint8_t hal_bt_get_state ( uint8_t mod );
extern void    hal_bt_ctrl ( uint8_t mod, uint8_t ctrl );

#endif /* __HAL_BT_H__ */

