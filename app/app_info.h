/******************************************************************************

 @file  app_info.h

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __APP_INFO_H__
#define __APP_INFO_H__

#include "app_config.h"
#include <stdint.h>


typedef struct app_info_t {
    uint8_t wifi_state_prev;
    uint8_t wifi_state_curr;
    uint8_t vol;
    uint8_t src;
    uint8_t bt_mode;
    uint8_t bt_state[2];
} APP_INFO_t;
/* ------------------------------------------------------------------------------------------------
 *                                           CONSTANTS
 * ------------------------------------------------------------------------------------------------
 */
#define WIFI_STATE_OFF                  0
#define WIFI_STATE_INIT                 1
#define WIFI_STATE_HN                   2
#define WIFI_STATE_SA                   3
#define WIFI_STATE_CFG                  4
#define WIFI_STATE_HN_INIT              5
#define WIFI_STATE_SA_INIT              6
#define WIFI_STATE_CFG_INIT             7

#define AUDIO_SOURCE_NONE               0
#define AUDIO_SOURCE_AUXIN              1
#define AUDIO_SOURCE_SD                 2
#define AUDIO_SOURCE_WIFI               3
#define AUDIO_SOURCE_BT                 4

#define BT_MODE_OFF                     0
#define BT_MODE_SINGLE                  1
#define BT_MODE_PARTY                   2
#define BT_MODE_MIXER                   3

/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 */
extern APP_INFO_t app_info;

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif

