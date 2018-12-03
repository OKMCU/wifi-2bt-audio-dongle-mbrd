#ifndef __HAL_BT_H__
#define __HAL_BT_H__


#include <stdint.h>

#define HAL_BT_STATE_UNKNOWN        0x00
#define HAL_BT_STATE_OFF            0x01
#define HAL_BT_STATE_CONNECTABLE    0x02
#define HAL_BT_STATE_DISCOVERABLE   0x03
#define HAL_BT_STATE_PAUSED         0x04
#define HAL_BT_STATE_PLAYING        0x05

#define HAL_BT_CTL_PIN_MFB          0x01
#define HAL_BT_CTL_PIN_INC          0x02
#define HAL_BT_CTL_PIN_DEC          0x04
#define HAL_BT_CTL_PIN_PAIR         0x08
#define HAL_BT_CTL_PIN_RST          0x10
#define HAL_BT_CTL_PIN_ALL          (HAL_BT_CTL_PIN_MFB + HAL_BT_CTL_PIN_INC + HAL_BT_CTL_PIN_DEC + HAL_BT_CTL_PIN_PAIR + HAL_BT_CTL_PIN_RST)

#define HAL_BT_MOD_0                0x01
#define HAL_BT_MOD_1                0x02

extern void    hal_bt_init      ( void );
extern uint8_t hal_bt_get_state ( uint8_t mod );
extern void    hal_bt_ctrl_pin  ( uint8_t mods, uint8_t pins, uint8_t value );

#endif /* __HAL_BT_H__ */

