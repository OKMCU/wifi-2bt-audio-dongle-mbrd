/******************************************************************************

 @file  hal_bt.c

 @brief This file contains the implementations to the HAL BT Service.

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
#include "hal_assert.h"
#include "hal_bt.h"
#include "hal_uibrd.h"

#define HAL_UIBRD_BT_STATE_OFF            0x07
#define HAL_UIBRD_BT_STATE_CONNECTABLE    0x03
#define HAL_UIBRD_BT_STATE_DISCOVERABLE   0x05
#define HAL_UIBRD_BT_STATE_PAUSED         0x01
#define HAL_UIBRD_BT_STATE_PLAYING        0x06

#define HAL_UIBRD_BT_MOD0_STATE(x)        (x&0xF)
#define HAL_UIBRD_BT_MOD1_STATE(x)        (x>>4)

/***************************************************************************************************
 *                                              TYPEDEFS
 ***************************************************************************************************/


/***************************************************************************************************
 *                                           GLOBAL VARIABLES
 ***************************************************************************************************/
extern uint8_t hal_uibrd_bt_state;


/***************************************************************************************************
 *                                            LOCAL FUNCTION
 ***************************************************************************************************/



/***************************************************************************************************
 *                                            FUNCTIONS - API
 ***************************************************************************************************/

/***************************************************************************************************
 * @fn      hal_bt_init
 *
 * @brief   Initialize BT Service
 *
 * @param   init - pointer to void that contains the initialized value
 *
 * @return  None
 ***************************************************************************************************/
extern void hal_bt_init (void)
{

}

/***************************************************************************************************
 * @fn      hal_bt_get
 *
 * @brief   
 *
 * @param   
 *          
 * @return  
 ***************************************************************************************************/
extern uint8_t hal_bt_get_state ( uint8_t mod )
{
    const uint8_t mapping[] = {
        HAL_BT_STATE_UNKNOWN,
        HAL_BT_STATE_PAUSED,
        HAL_BT_STATE_UNKNOWN,
        HAL_BT_STATE_CONNECTABLE,
        HAL_BT_STATE_UNKNOWN,
        HAL_BT_STATE_DISCOVERABLE,
        HAL_BT_STATE_PLAYING,
        HAL_BT_STATE_OFF
    };
    uint8_t state;
    
    if(mod == HAL_BT_MOD_0)
    {
        state = HAL_UIBRD_BT_MOD0_STATE(hal_uibrd_bt_state);
    }
    else if(mod == HAL_BT_MOD_1)
    {
        state = HAL_UIBRD_BT_MOD1_STATE(hal_uibrd_bt_state);
    }
    else
    {
        return HAL_BT_STATE_UNKNOWN;
    }

    HAL_ASSERT(state < sizeof(mapping));
    
    return mapping[state];
}

/***************************************************************************************************
 * @fn      hal_bt_ctrl_pin
 *
 * @brief   
 *
 * @param   
 *          
 *          
 *          
 *          
 *
 * @return  
 ***************************************************************************************************/
extern void    hal_bt_ctrl_pin ( uint8_t mods, uint8_t pins, uint8_t value )
{
    uint8_t pb[3];
    
    pb[0] = mods;
    pb[1] = pins;
    pb[2] = value;
    
    hal_uibrd_write( HAL_UIBRD_REG_BT_CTRL, pb, sizeof(pb) );
}


/***************************************************************************************************
***************************************************************************************************/




