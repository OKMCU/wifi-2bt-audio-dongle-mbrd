/******************************************************************************

 @file  main.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

#define TASK_ID_HAL_DRIVERS                 0
#define TASK_ID_HAL_DRIVER_WIFIMOD          1
//#define TASK_ID_APP_LUCICMD                 2
#define TASK_ID_APP_WIFIMOD                 3
#define TASK_ID_APP_CLI                     4
#define TASK_ID_APP_UIBRD                   5
#define TASK_ID_APP_BT                      6
#define TASK_ID_APP_KEY                     7
#define TASK_ID_APP_MAIN                    9

#define TASK_EVT_APP_MAIN_POR               1
#define TASK_EVT_APP_MAIN_APP_EXCEPTION     2
#define TASK_EVT_APP_MAIN_HAL_EXCEPTION     3
#define TASK_EVT_APP_MAIN_OSAL_EXCEPTION    4

#define TASK_EVT_APP_WIFIMOD_BOOT_UP        1
#define TASK_EVT_APP_WIFIMOD_UPD_VOL        2
#define TASK_EVT_APP_WIFIMOD_UPD_SRC        3
#define TASK_EVT_APP_WIFIMOD_UPD_MODE       4
#define TASK_EVT_APP_WIFIMOD_UPD_STATE      5
#define TASK_EVT_APP_WIFIMOD_RESET          6

#define TASK_EVT_APP_UIBRD_UPD_KEY          1
#define TASK_EVT_APP_UIBRD_UPD_BT           2

#define TASK_EVT_HAL_DRIVERS_LUCICMD_RXNE   1
#define TASK_EVT_HAL_DRIVERS_CLI_RXNE       2
#define TASK_EVT_HAL_DRIVERS_UIBRD_TRIG_IRQ 3
#define TASK_EVT_HAL_DRIVERS_LUCICMD_TXE    4
#define TASK_EVT_HAL_DRIVERS_CLI_TXE        5
#define TASK_EVT_HAL_DRIVERS_UIBRD_POLL_IRQ 6

#define TASK_EVT_HAL_DRIVER_WIFIMOD_RESET       1
#define TASK_EVT_HAL_DRIVER_WIFIMOD_MODE_CTRL   2



#define TASK_MSG_APP_BT_BT0_STATE           0        
#define TASK_MSG_APP_BT_BT1_STATE           1

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif

