#ifndef __HAL_MULTIPLEXER_H__
#define __HAL_MULTIPLEXER_H__


#include <stdint.h>

#define HAL_MULTIPLEXER_DEV0        0
#define HAL_MULTIPLEXER_DEV1        1

#define HAL_MULTIPLEXER_CH0         0
#define HAL_MULTIPLEXER_CH1         1
     

extern void     hal_multiplexer_init( void );
extern void     hal_multiplexer_set( uint8_t dev, uint8_t ch );
extern uint8_t  hal_multiplexer_get( uint8_t dev );

#endif
