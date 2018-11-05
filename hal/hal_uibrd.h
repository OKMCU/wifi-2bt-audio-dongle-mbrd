/******************************************************************************

 @file  hal_uibrd.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __HAL_UIBRD_H__
#define __HAL_UIBRD_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "hal_config.h"

#if (HAL_UIBRD_EN > 0)
/*********************************************************************
 * MACROS
 */   


/*********************************************************************
 * CONSTANTS
 */
#define HAL_UIBRD_REG_HW_VER        0x00 //READ ONLY,  len = 1, check hardware version by this REG
#define HAL_UIBRD_REG_SW_VER        0x01 //READ ONLY,  len = 1, check software version by this REG
#define HAL_UIBRD_REG_IRQ_EVENT     0x02 //READ ONLY,  len = 1, check the interrupt request events by this REG
#define HAL_UIBRD_REG_KEY_VALUE     0x03 //READ ONLY,  len = 1, check the key press/release status by this REG
#define HAL_UIBRD_REG_BT_STATE      0x04 //READ ONLY,  len = 1, check the bluetooth module working status by this REG
#define HAL_UIBRD_REG_BT_CTRL       0x05 //WRITE ONLY, len = 2, set the bluetooth working state
#define HAL_UIBRD_REG_LED_CTRL      0x06 //WRITE ONLY, len = 5, set the LED ON/OFF/BLINKING state

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */

 /*********************************************************************
 * FUNCTION APIs
 */
extern void     hal_uibrd_init ( void );
extern void     hal_uibrd_read ( uint8_t reg, uint8_t *pb, uint8_t len );
extern void     hal_uibrd_write( uint8_t reg, const uint8_t *pb, uint8_t len );

extern uint8_t  hal_uibrd_hw_ver( void );
extern uint8_t  hal_uibrd_sw_ver( void );
extern uint8_t  hal_uibrd_irq_pin( void );
extern uint8_t  hal_uibrd_irq_evt( void );

/*********************************************************************
*********************************************************************/
#endif /* HAL_UIBRD_EN > 0 */

#ifdef __cplusplus
}
#endif

#endif
