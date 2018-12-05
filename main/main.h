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

#define TASK_ID_HAL_DRIVER_BASIC            0
#define TASK_ID_HAL_DRIVER_WIFIMOD          1
//#define TASK_ID_APP_LUCICMD                 2
#define TASK_ID_APP_WIFIMOD                 3
#define TASK_ID_APP_CLI                     4
#define TASK_ID_APP_UIBRD                   5
#define TASK_ID_APP_BT                      6
#define TASK_ID_APP_KEY                     7
#define TASK_ID_APP_MAIN                    9

#define TASK_EVT_APP_MAIN_POR               1
#define TASK_EVT_APP_MAIN_INIT              2
#define TASK_EVT_APP_MAIN_LED_WIFIR_CRS_BLK 3
#define TASK_EVT_APP_MAIN_LED_WIFIB_CRS_BLK 4
#define TASK_EVT_APP_MAIN_SET_DSP_VOL       5
#define TASK_EVT_APP_MAIN_DEC_VOL           6
#define TASK_EVT_APP_MAIN_INC_VOL           7
#define TASK_EVT_APP_MAIN_APP_EXCEPTION     8
#define TASK_EVT_APP_MAIN_HAL_EXCEPTION     9
#define TASK_EVT_APP_MAIN_OSAL_EXCEPTION    10

#define TASK_EVT_APP_WIFIMOD_BOOT_UP        1
#define TASK_EVT_APP_WIFIMOD_UPD_VOL        2
#define TASK_EVT_APP_WIFIMOD_UPD_SRC        3
#define TASK_EVT_APP_WIFIMOD_UPD_MODE       4
#define TASK_EVT_APP_WIFIMOD_UPD_STATE      5
#define TASK_EVT_APP_WIFIMOD_RESET          6
#define TASK_EVT_APP_WIFIMOD_HN_TIMEOUT     7
#define TASK_EVT_APP_WIFIMOD_HN_DISCONNECT  8
#define TASK_EVT_APP_WIFIMOD_MISS_LUCICMD   9

#define TASK_EVT_APP_UIBRD_UPD_KEY          1
#define TASK_EVT_APP_UIBRD_UPD_BT           2

#define TASK_EVT_APP_BT_UPDATE_STATE             1
#define TASK_EVT_APP_BT_TRIG_MOD0_PAIRING_STOP   2
#define TASK_EVT_APP_BT_TRIG_MOD1_PAIRING_STOP   3
#define TASK_EVT_APP_BT_DISCONNECT_BT0           4
#define TASK_EVT_APP_BT_DISCONNECT_BT1           5

#define TASK_EVT_HAL_DRIVER_BASIC_LUCICMD_RXNE   2
#define TASK_EVT_HAL_DRIVER_BASIC_CLI_RXNE       3
#define TASK_EVT_HAL_DRIVER_BASIC_UIBRD_TRIG_IRQ 4
#define TASK_EVT_HAL_DRIVER_BASIC_LUCICMD_TXE    5
#define TASK_EVT_HAL_DRIVER_BASIC_CLI_TXE        6
#define TASK_EVT_HAL_DRIVER_BASIC_UIBRD_POLL_IRQ 7

#define TASK_EVT_HAL_DRIVER_WIFIMOD_RX_OVF      1
#define TASK_EVT_HAL_DRIVER_WIFIMOD_RESET       2
#define TASK_EVT_HAL_DRIVER_WIFIMOD_MODE_CTRL   3
#define TASK_EVT_HAL_DRIVER_WIFIMOD_HN_TIMEOUT  4
#define TASK_EVT_HAL_DRIVER_WIFIMOD_SRC_CTRL    5



//#define TASK_MSG_APP_BT_BT0_STATE           0        
//#define TASK_MSG_APP_BT_BT1_STATE           1

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif

