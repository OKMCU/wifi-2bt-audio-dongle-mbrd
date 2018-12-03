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
#define LED_BT_IND_MODE_OFF()            hal_led_set(HAL_LED_MIXER+HAL_LED_PARTY, HAL_LED_MODE_OFF)
#define LED_BT_IND_MODE_SINGLE()         hal_led_set(HAL_LED_MIXER+HAL_LED_PARTY, HAL_LED_MODE_OFF)
#define LED_BT_IND_MODE_PARTY()      st( hal_led_set(HAL_LED_PARTY, HAL_LED_MODE_ON); \
                                         hal_led_set(HAL_LED_MIXER, HAL_LED_MODE_OFF);)
#define LED_BT_IND_MODE_MIXER()      st( hal_led_set(HAL_LED_PARTY, HAL_LED_MODE_OFF);\
                                         hal_led_set(HAL_LED_MIXER, HAL_LED_MODE_ON);)
#define LED_BT0_IND_STATE_OFF()                 hal_led_set(HAL_LED_BT0, HAL_LED_MODE_OFF)
#define LED_BT0_IND_STATE_DISCOVERABLE()        hal_led_blink(HAL_LED_BT0, 0, 50, 1000)
#define LED_BT0_IND_STATE_CONNECTED_PAUSED()    hal_led_set(HAL_LED_BT0, HAL_LED_MODE_ON)
#define LED_BT0_IND_STATE_CONNECTED_PLAYING()   hal_led_blink(HAL_LED_BT0, 0, 10, 2000)
#define LED_BT1_IND_STATE_OFF()                 hal_led_set(HAL_LED_BT1, HAL_LED_MODE_OFF)
#define LED_BT1_IND_STATE_DISCOVERABLE()        hal_led_blink(HAL_LED_BT1, 0, 50, 1000)
#define LED_BT1_IND_STATE_CONNECTED_PAUSED()    hal_led_set(HAL_LED_BT1, HAL_LED_MODE_ON)
#define LED_BT1_IND_STATE_CONNECTED_PLAYING()   hal_led_blink(HAL_LED_BT1, 0, 10, 2000)
                                         
#define DSP_SET_CHANNEL_NONE()           hal_dsp_clr_src(HAL_DSP_SRC_ALL)
#define DSP_SET_CHANNEL_AUXIN()      st( hal_dsp_clr_src(HAL_DSP_SRC_ALL); \
                                         hal_dsp_set_src(HAL_DSP_SRC_ADC); )
#define DSP_SET_CHANNEL_WIFIMOD()    st( hal_dsp_clr_src(HAL_DSP_SRC_ALL); \
                                         hal_multiplexer_set(HAL_MULTIPLEXER_DEV0, HAL_MULTIPLEXER_CH0); \
                                         hal_dsp_set_src(HAL_DSP_SRC_SDI0); )
#define DSP_SET_CHANNEL_BT()         st( hal_dsp_clr_src(HAL_DSP_SRC_ALL); \
                                         hal_multiplexer_set(HAL_MULTIPLEXER_DEV0, HAL_MULTIPLEXER_CH1); \
                                         hal_multiplexer_set(HAL_MULTIPLEXER_DEV1, HAL_MULTIPLEXER_CH1); \
                                         hal_dsp_set_src(HAL_DSP_SRC_SDI0); \
                                         hal_dsp_set_src(HAL_DSP_SRC_SDI1); )

#define APP_ASSERT_EN           1

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif

