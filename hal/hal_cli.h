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
 
#ifndef __HAL_CLI_H__
#define __HAL_CLI_H__


#include <stdint.h>
#include "hal_config.h"

#if (HAL_CLI_EN > 0)
extern void hal_cli_init         ( void );
extern void hal_cli_print_char   ( char c );
extern void hal_cli_print_str    ( const char *s );
extern void hal_cli_print_sint   ( int32_t num );
extern void hal_cli_print_uint   ( uint32_t num );
extern void hal_cli_print_hex8   ( uint8_t num );
extern void hal_cli_print_hex16  ( uint16_t num );
extern void hal_cli_print_hex32  ( uint32_t num );
#endif /* (HAL_CLI_EN > 0) */

#endif /* __HAL_CLI_H__ */

