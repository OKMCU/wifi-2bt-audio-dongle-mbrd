/******************************************************************************

 @file  hal_uibrd.c

 @brief This file contains the interface to the UI Board control.

 Group: 
 Target Device: 

 ******************************************************************************


 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

/***************************************************************************************************
 * INCLUDES
 ***************************************************************************************************/
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_uibrd.h"
#include "hal_drivers.h"

#include "main.h"

/***************************************************************************************************
 * TYPEDEFS
 ***************************************************************************************************/


/***************************************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************************************/

/***************************************************************************************************
 * CONSTANTS
 ***************************************************************************************************/
#define WR_ADDR                     (0x94 | 0x00)
#define RD_ADDR                     (0x94 | 0x01)

#define HAL_UIBRD_IRQ_UPD_KEY       0x01
#define HAL_UIBRD_IRQ_UPD_BT        0x02

/***************************************************************************************************
 * MACROS
 ***************************************************************************************************/

uint8_t hal_uibrd_key_value;
uint8_t hal_uibrd_bt_state;

/***************************************************************************************************
 * LOCAL FUNCTION
 ***************************************************************************************************/
static void hal_uibrd_update ( void )
{
    uint8_t irqs;

    hal_uibrd_read(HAL_UIBRD_REG_IRQ_EVENT, &irqs, sizeof(uint8_t));
    if( irqs & HAL_UIBRD_IRQ_UPD_KEY )  //key update event
    {
        hal_uibrd_read(HAL_UIBRD_REG_KEY_VALUE, &hal_uibrd_key_value, sizeof(uint8_t));
        osal_event_set(TASK_ID_APP_UIBRD, TASK_EVT_APP_UIBRD_UPD_KEY);
    }

    if( irqs & HAL_UIBRD_IRQ_UPD_BT )   //bt update event
    {
        hal_uibrd_read( HAL_UIBRD_REG_BT_STATE, &hal_uibrd_bt_state, sizeof(uint8_t) );
        osal_event_set(TASK_ID_APP_UIBRD, TASK_EVT_APP_UIBRD_UPD_BT);
    }
}


/***************************************************************************************************
 *                                            FUNCTIONS - API
 ***************************************************************************************************/

/***************************************************************************************************
 * @fn      HalUibrdInit
 *
 * @brief   Initialize UI board including initial LED status etc.
 *
 * @param   
 *
 * @return  None
 ***************************************************************************************************/
extern void hal_uibrd_init( void )
{
    spl_gpio_set_mode(SPL_GPIO_PORT_3, SPL_GPIO_PIN_2, SPL_GPIO_MODE_QUASI);
    spl_extint_config(SPL_EXTINT_INT_0, SPL_EXTINT_TRIG_FALL);

    hal_uibrd_key_value = 0;
    hal_uibrd_bt_state = 0;
}



/***************************************************************************************************
 * @fn      HalUibrdRegRead
 *
 * @brief   
 *
 * @param   
 *          
 * @return  None
 ***************************************************************************************************/
extern void hal_uibrd_read ( uint8_t reg, uint8_t *pb, uint8_t len )
{
    uint8_t err;
    uint8_t i;
    
    err = spl_i2c_start(SPL_I2C_PORT_0);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);
    
    err = spl_i2c_wr_addr(SPL_I2C_PORT_0, WR_ADDR);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);

    err = spl_i2c_wr_data(SPL_I2C_PORT_0, reg);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);

    err = spl_i2c_restart(SPL_I2C_PORT_0);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);

    err = spl_i2c_wr_addr(SPL_I2C_PORT_0, RD_ADDR);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);
    
    if( len )
    {
        for( i = 0; i < len - 1; i++ )
        {
            err = spl_i2c_rd_data(SPL_I2C_PORT_0, TRUE, &pb[i]);
            HAL_ASSERT(err == SPL_I2C_ERR_NONE);
        }

        err = spl_i2c_rd_data(SPL_I2C_PORT_0, FALSE, &pb[i]);
        HAL_ASSERT(err == SPL_I2C_ERR_NONE);
    }
    
    err = spl_i2c_stop(SPL_I2C_PORT_0);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);
}

/***************************************************************************************************
 * @fn      HalUiBrdWrite
 *
 * @brief   
 *
 * @param   
 *          
 *          
 *                       
 *          
 *
 * @return  None
 ***************************************************************************************************/
extern void hal_uibrd_write ( uint8_t reg, const uint8_t *pb, uint8_t len )
{
    uint8_t err;
    uint8_t i;
    
    err = spl_i2c_start(SPL_I2C_PORT_0);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);

    err = spl_i2c_wr_addr(SPL_I2C_PORT_0, WR_ADDR);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);

    err = spl_i2c_wr_data(SPL_I2C_PORT_0, reg);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);
    
    for ( i = 0; i < len; i++ )
    {
        /* Send data */
        err = spl_i2c_wr_data(SPL_I2C_PORT_0, pb[i]);
        HAL_ASSERT(err == SPL_I2C_ERR_NONE);
    }
    
    err = spl_i2c_stop(SPL_I2C_PORT_0);
    HAL_ASSERT(err == SPL_I2C_ERR_NONE);
    
}

extern uint8_t  hal_uibrd_hw_ver( void )
{
    uint8_t u8tmp;
    hal_uibrd_read(HAL_UIBRD_REG_HW_VER, &u8tmp, sizeof(uint8_t));
    return u8tmp;
}

extern uint8_t  hal_uibrd_sw_ver( void )
{
    uint8_t u8tmp;
    hal_uibrd_read(HAL_UIBRD_REG_SW_VER, &u8tmp, sizeof(uint8_t));
    return u8tmp;
}

extern uint8_t  hal_uibrd_irq_pin( void )
{
    return spl_gpio_read_pin(SPL_GPIO_PORT_3, SPL_GPIO_PIN_2);
}

extern uint8_t  hal_uibrd_irq_evt( void )
{
    uint8_t u8tmp;
    hal_uibrd_read(HAL_UIBRD_REG_IRQ_EVENT, &u8tmp, sizeof(uint8_t));
    return u8tmp;
}

extern void hal_uibrd_driver_handle_trig_irq ( void )
{
    hal_uibrd_update();
    osal_timer_event_create(TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_UIBRD_POLL_IRQ, 1);
}

extern void hal_uibrd_driver_handle_poll_irq ( void )
{
    if(spl_gpio_read_pin(SPL_GPIO_PORT_3, SPL_GPIO_PIN_2) == 0)
    {
        hal_uibrd_update();
        osal_timer_event_create(TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_UIBRD_POLL_IRQ, 1);
    }
}

extern void spl_extint_int0_callback( void )
{
    osal_event_set( TASK_ID_HAL_DRIVERS, TASK_EVT_HAL_DRIVERS_UIBRD_TRIG_IRQ );
}


/***************************************************************************************************
***************************************************************************************************/




