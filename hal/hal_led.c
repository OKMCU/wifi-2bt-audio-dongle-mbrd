/******************************************************************************

 @file  hal_led.c

 @brief This file contains the implementations to the HAL LED Service.

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

/***************************************************************************************************
 *                                             INCLUDES
 ***************************************************************************************************/
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_led.h"
#include "hal_uibrd.h"

/***************************************************************************************************
 *                                              TYPEDEFS
 ***************************************************************************************************/


/***************************************************************************************************
 *                                           GLOBAL VARIABLES
 ***************************************************************************************************/



/***************************************************************************************************
 *                                            LOCAL FUNCTION
 ***************************************************************************************************/



/***************************************************************************************************
 *                                            FUNCTIONS - API
 ***************************************************************************************************/

/***************************************************************************************************
 * @fn      hal_led_init
 *
 * @brief   Initialize LED Service
 *
 * @param   init - pointer to void that contains the initialized value
 *
 * @return  None
 ***************************************************************************************************/
void hal_led_init (void)
{
    
}

/***************************************************************************************************
 * @fn      hal_led_set
 *
 * @brief   Tun ON/OFF/TOGGLE given LEDs
 *
 * @param   led - bit mask value of leds to be turned ON/OFF/TOGGLE
 *          mode - ON, OFF
 * @return  None
 ***************************************************************************************************/
void hal_led_set ( uint8_t leds, uint8_t mode )
{
    if( mode == HAL_LED_MODE_OFF )
    {
        hal_led_blink( leds, 0, 0, 0 );
    }
    else
    {
        hal_led_blink( leds, 0, 100, 1 );
    }
}

/***************************************************************************************************
 * @fn      hal_led_blink
 *
 * @brief   Blink the leds
 *
 * @param   leds       - bit mask value of leds to be blinked
 *          numBlinks  - number of blinks
 *          percent    - the percentage in each period where the led
 *                       will be on
 *          period     - length of each cycle in milliseconds
 *
 * @return  None
 ***************************************************************************************************/
void hal_led_blink ( uint8_t leds, uint8_t cnt, uint8_t duty, uint16_t time )
{
    uint8_t pb[5];

    pb[0] = leds;
    pb[1] = cnt;
    pb[2] = duty;
    pb[3] = HI_UINT16( time );
    pb[4] = LO_UINT16( time );

    hal_uibrd_write( HAL_UIBRD_REG_LED_CTRL, pb, sizeof(pb) );
}


/***************************************************************************************************
 * @fn      hal_led_enter_sleep
 *
 * @brief   Store current LEDs state before sleep
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void hal_led_enter_sleep( void )
{

}

/***************************************************************************************************
 * @fn      hal_led_exit_sleep
 *
 * @brief   Restore current LEDs state after sleep
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void hal_led_exit_sleep( void )
{

}

/***************************************************************************************************
***************************************************************************************************/




