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
#include "hal_key.h"
#include "hal_uibrd.h"

/***************************************************************************************************
 *                                              TYPEDEFS
 ***************************************************************************************************/


/***************************************************************************************************
 *                                           GLOBAL VARIABLES
 ***************************************************************************************************/
extern uint8_t hal_uibrd_key_value;


/***************************************************************************************************
 *                                            LOCAL FUNCTION
 ***************************************************************************************************/



/***************************************************************************************************
 *                                            FUNCTIONS - API
 ***************************************************************************************************/

/***************************************************************************************************
 * @fn      hal_key_init
 *
 * @brief   Initialize KEY Service
 *
 * @param   
 *
 * @return  None
 ***************************************************************************************************/
extern void hal_key_init (void)
{
    
}

/***************************************************************************************************
 * @fn      hal_key_read
 *
 * @brief   
 *
 * @param   
 *          
 * @return  
 ***************************************************************************************************/
extern uint8_t hal_key_read( void )
{
    return hal_uibrd_key_value;
}


/***************************************************************************************************
***************************************************************************************************/



