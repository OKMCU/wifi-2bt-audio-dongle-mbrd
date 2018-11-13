/******************************************************************************

 @file  app_config.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#define FIRMWARE_VER            "V1.1.0"
#define HARDWARE_VER            "V2.0.0"

//#define LED_WIFI_IND_BOOTING()  st( hal_led_set(HAL_LED_WIFIB, HAL_LED_MODE_OFF); \
//                                    hal_led_blink(HAL_LED_WIFIR, 0, 50, 1000); )
#define LED_WIFI_IND_MODE_HN_INIT()      hal_led_blink(HAL_LED_WIFIB+HAL_LED_WIFIR, 0, 50, 1000)
#define LED_WIFI_IND_MODE_HN()           hal_led_set(HAL_LED_WIFIB+HAL_LED_WIFIR, HAL_LED_MODE_ON)
#define LED_WIFI_IND_MODE_SA_INIT()  st( hal_led_blink(HAL_LED_WIFIB, 0, 50, 1000);    \
                                         hal_led_set(HAL_LED_WIFIR, HAL_LED_MODE_OFF); )
#define LED_WIFI_IND_MODE_SA()       st( hal_led_set(HAL_LED_WIFIR, HAL_LED_MODE_OFF); \
                                         hal_led_set(HAL_LED_WIFIB, HAL_LED_MODE_ON);  )
#define LED_WIFI_IND_MODE_CFG_INIT() st( hal_led_blink(HAL_LED_WIFIR, 0, 50, 1000);    \
                                         hal_led_set(HAL_LED_WIFIB, HAL_LED_MODE_OFF); )             
#define LED_WIFI_IND_MODE_CFG()      st( hal_led_set(HAL_LED_WIFIR, HAL_LED_MODE_ON);  \
                                         hal_led_set(HAL_LED_WIFIB, HAL_LED_MODE_OFF); )

#define DSP_SET_CHANNEL_AUXIN()
#define DSP_SET_CHANNEL_WIFI()
#define DSP_SET_CHANNEL_BT_SINGLE()
#define DSP_SET_CHANNEL_SET_

#define APP_ASSERT_EN           1

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif

