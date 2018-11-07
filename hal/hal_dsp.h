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
 
#ifndef __HAL_DSP_H__
#define __HAL_DSP_H__


#include <stdint.h>
#include "hal_config.h"

#if (HAL_DSP_EN > 0)

#define HAL_DSP_SRC_NONE    0x00
#define HAL_DSP_SRC_SDI0    0x01
#define HAL_DSP_SRC_SDI1    0x02
#define HAL_DSP_SRC_ADC     0x04

#define HAL_DSP_SRC_ALL     (HAL_DSP_SRC_SDI0 + HAL_DSP_SRC_SDI1 + HAL_DSP_SRC_ADC)

#define HAL_DSP_VOL_MAX     0x7FFFFF

extern void     hal_dsp_disable(void);
extern void     hal_dsp_enable(void);
extern void     hal_dsp_init(void);
extern void     hal_dsp_read(const uint8_t cmd[3], uint8_t val[3]);
extern void     hal_dsp_write(const uint8_t *buf, uint16_t len);
extern void     hal_dsp_write_init_code(void);
extern void     hal_dsp_set_src(uint8_t src);
extern void     hal_dsp_clr_src(uint8_t src);
extern uint8_t  hal_dsp_get_src(void);
extern void     hal_dsp_set_vol(uint32_t vol);
extern uint32_t hal_dsp_get_vol(void);
#endif /* (HAL_DSP_EN > 0) */

#endif /* __HAL_DSP_H__ */

