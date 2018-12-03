/******************************************************************************

 @file  spl_config.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

#ifndef __SPL_CONFIG_H__
#define __SPL_CONFIG_H__

#include <stdint.h>

#define SPL_MCU_XTAL_EN                     1
#define SPL_UART_EN                         1
#define SPL_UART0_EN                        1
#define SPL_UART1_EN                        1
#define SPL_GPIO_EN                         1
#define SPL_I2C_EN                          1
#define SPL_I2C0_MASTER_EN                  1
#define SPL_I2C1_MASTER_EN                  0
#define SPL_I2C0_SLAVE_EN                   0
#define SPL_I2C1_SLAVE_EN                   0
#define SPL_EXTINT_EN                       1
#define SPL_EXTINT0_EN                      1
#define SPL_EXTINT1_EN                      0

#if (SPL_UART_EN > 0)
#if (SPL_UART0_EN > 0)
#define SPL_UART0_BAUDRATE                  115200
#define SPL_UART0_RX_CACHE_SIZE             16
#define SPL_UART0_TX_CACHE_SIZE             16
#define SPL_UART0_CALLBACK                  spl_uart0_callback
#endif /* (SPL_UART0_EN > 0) */
#if (SPL_UART1_EN > 0)
#define SPL_UART1_BAUDRATE                  57600
#define SPL_UART1_RX_CACHE_SIZE             64
#define SPL_UART1_TX_CACHE_SIZE             16
#define SPL_UART1_CALLBACK                  spl_uart1_callback
#endif /* (SPL_UART1_EN > 0) */
#endif

#if (SPL_I2C_EN > 0)
#if (SPL_I2C0_MASTER_EN > 0)
#define SPL_I2C0_CLOCK                      100000
#endif
#if (SPL_I2C1_MASTER_EN > 0)
#define SPL_I2C1_CLOCK                      100000
#endif
#if (SPL_I2C0_SLAVE_EN > 0)
#define SPL_I2C0_SLAVE_ADDR0                0x4A
#define SPL_I2C0_SLAVE_ADDR1                0x4A
#define SPL_I2C0_SLAVE_ADDR2                0x4A
#define SPL_I2C0_SLAVE_ADDR3                0x4A
#define SPL_I2C0_CALLBACK_ADDRW             spl_i2c0_callback_addrw
#define SPL_I2C0_CALLBACK_ADDRR             spl_i2c0_callback_addrr
#define SPL_I2C0_CALLBACK_RXD               spl_i2c0_callback_rxd
#define SPL_I2C0_CALLBACK_TXD               spl_i2c0_callback_txd
#define SPL_I2C0_CALLBACK_FRMEND            spl_i2c0_callback_frmend
#endif
#if (SPL_I2C1_SLAVE_EN > 0)
#define SPL_I2C1_SLAVE_ADDR0
#define SPL_I2C1_SLAVE_ADDR1
#define SPL_I2C1_SLAVE_ADDR2
#define SPL_I2C1_SLAVE_ADDR3
#define SPL_I2C1_CALLBACK_ADDRW             spl_i2c1_callback_addrw
#define SPL_I2C1_CALLBACK_ADDRR             spl_i2c1_callback_addrr
#define SPL_I2C1_CALLBACK_RXD               spl_i2c1_callback_rxd
#define SPL_I2C1_CALLBACK_TXD               spl_i2c1_callback_txd
#define SPL_I2C1_CALLBACK_FRMEND            spl_i2c1_callback_frmend
#endif
#endif

#if (SPL_EXTINT_EN > 0)
#if (SPL_EXTINT0_EN > 0)
#define SPL_EXTINT_INT0_CALLBACK            spl_extint_int0_callback
#endif
#if (SPL_EXTINT1_EN > 0)
#define SPL_EXTINT_INT1_CALLBACK            spl_extint_int1_callback
#endif
#endif

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif

