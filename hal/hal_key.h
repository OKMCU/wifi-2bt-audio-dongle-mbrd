#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__


#include <stdint.h>

#define     HAL_KEY_MAX             5

#if ( HAL_KEY_MAX > 0 )
#define HAL_KEY_0   0
#endif
#if ( HAL_KEY_MAX > 1 )
#define HAL_KEY_1   1
#endif
#if ( HAL_KEY_MAX > 2 )
#define HAL_KEY_2   2
#endif
#if ( HAL_KEY_MAX > 3 )
#define HAL_KEY_3   3
#endif
#if ( HAL_KEY_MAX > 4 )
#define HAL_KEY_4   4
#endif
#if ( HAL_KEY_MAX > 5 )
#define HAL_KEY_5   5
#endif
#if ( HAL_KEY_MAX > 6 )
#define HAL_KEY_6   6
#endif
#if ( HAL_KEY_MAX > 7 )
#define HAL_KEY_7   7
#endif

#define HAL_KEY_WIFI    (1<<HAL_KEY_0)
#define HAL_KEY_BT      (1<<HAL_KEY_1)
#define HAL_KEY_AUX     (1<<HAL_KEY_2)
#define HAL_KEY_VOLDN   (1<<HAL_KEY_3)
#define HAL_KEY_VOLUP   (1<<HAL_KEY_4)

extern void    hal_key_init (void);
extern uint8_t hal_key_read( void );

#endif /* __HAL_KEY_H__ */

