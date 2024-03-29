/******************************************************************************

 @file  hal_mcu.h

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __HAL_MCU_H__
#define __HAL_MCU_H__

#include <stdint.h>
/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 */

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

extern void hal_mcu_init( void );
extern void hal_mcu_reset( void );
extern void hal_mcu_delayus( uint32_t us);

#endif

