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
#ifndef __BUFMGR_H__
#define __BUFMGR_H__

#include <stdint.h>

typedef struct fifo_buf_t {
    uint8_t *buf;
    uint8_t size;
	uint8_t head;
	uint8_t tail;
} FIFO_BUF_t;

extern void    fifo_buf_flush( FIFO_BUF_t *p_fifo );
extern void    fifo_buf_put  ( FIFO_BUF_t *p_fifo, uint8_t byte );
extern uint8_t fifo_buf_get  ( FIFO_BUF_t *p_fifo );
extern uint8_t fifo_buf_full ( const FIFO_BUF_t *p_fifo );
extern uint8_t fifo_buf_empty( const FIFO_BUF_t *p_fifo );

#endif

