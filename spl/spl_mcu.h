/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

#ifndef __SPL_MCU_H__
#define __SPL_MCU_H__

#include "M051Series.h"

#define spl_mcu_disable_irq()    __disable_irq()
#define spl_mcu_enable_irq()     __enable_irq()
#define spl_mcu_reset()          spl_mcu_disable_irq(); SYS_UnlockReg(); SYS_ResetChip()
#define spl_mcu_delayus(us)      CLK_SysTickDelay(us)

extern void spl_mcu_init( void );

#endif

