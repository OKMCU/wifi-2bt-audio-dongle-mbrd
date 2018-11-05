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
#ifndef __OSAL_FIFO_H__
#define __OSAL_FIFO_H__

#include <stdint.h>
#include "osal_config.h"

#if (OSAL_FIFO_EN > 0)
extern void *osal_fifo_create(void);
extern void osal_fifo_delete(void *fifo);
extern uint8_t *osal_fifo_put(void *fifo, uint8_t byte);
extern uint32_t osal_fifo_len(void *fifo);
extern uint8_t osal_fifo_get(void *fifo);
#endif /* (OSAL_FIFO_EN > 0) */


#endif /* __OSAL_FIFO_H__ */

