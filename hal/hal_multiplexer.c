#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_multiplexer.h"

#define MULTIPLEXER0_PIN_INIT()   spl_gpio_set_mode (SPL_GPIO_PORT_2, SPL_GPIO_PIN_2, SPL_GPIO_MODE_QUASI)
#define MULTIPLEXER0_PIN_CH0()    spl_gpio_write_pin(SPL_GPIO_PORT_2, SPL_GPIO_PIN_2, 0)
#define MULTIPLEXER0_PIN_CH1()    spl_gpio_write_pin(SPL_GPIO_PORT_2, SPL_GPIO_PIN_2, 1)
#define MULTIPLEXER0_PIN()        spl_gpio_read_pin (SPL_GPIO_PORT_2, SPL_GPIO_PIN_2)

#define MULTIPLEXER1_PIN_INIT()   spl_gpio_set_mode (SPL_GPIO_PORT_2, SPL_GPIO_PIN_3, SPL_GPIO_MODE_QUASI)
#define MULTIPLEXER1_PIN_CH0()    spl_gpio_write_pin(SPL_GPIO_PORT_2, SPL_GPIO_PIN_3, 0)
#define MULTIPLEXER1_PIN_CH1()    spl_gpio_write_pin(SPL_GPIO_PORT_2, SPL_GPIO_PIN_3, 1)
#define MULTIPLEXER1_PIN()        spl_gpio_read_pin (SPL_GPIO_PORT_2, SPL_GPIO_PIN_3)

extern void hal_multiplexer_init( void )
{
    MULTIPLEXER0_PIN_INIT();
    MULTIPLEXER1_PIN_INIT();
}


extern void hal_multiplexer_set( uint8_t dev, uint8_t ch )
{
    if( dev == HAL_MULTIPLEXER_DEV0 )
    {
        if( ch == HAL_MULTIPLEXER_CH0 )
        {
            MULTIPLEXER0_PIN_CH0();
        }
        else
        {
            MULTIPLEXER0_PIN_CH1();
        }
    }
    else
    {
        if( ch == HAL_MULTIPLEXER_CH0 )
        {
            MULTIPLEXER1_PIN_CH0();
        }
        else
        {
            MULTIPLEXER1_PIN_CH1();
        }
    }
    
}

extern uint8_t  hal_multiplexer_get( uint8_t dev )
{
    if( dev == HAL_MULTIPLEXER_DEV0 )
    {
        return MULTIPLEXER0_PIN();
    }
    else
    {
        return MULTIPLEXER1_PIN();
    }
    
}

