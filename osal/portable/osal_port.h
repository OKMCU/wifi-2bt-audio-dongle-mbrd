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
#ifndef __OSAL_PORT_H__
#define __OSAL_PORT_H__

#include <stdint.h>
#include "osal_comdef.h"
#include "osal_config.h"
#include "stdmacro.h"

#if (OSAL_MEM_EN > 0)
#include "umm_malloc.h"
#endif /* (OSAL_MEM_EN > 0) */

/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/*
 *  ASSERT( expression ) - The given expression must evaluate as "true" or else the assert
 *  handler is called.  From here, the call stack feature of the debugger can pinpoint where
 * the problem occurred.
 *
 *  ASSERT_FORCED( ) - If asserts are in use, immediately calls the assert handler.
 *
 *  ASSERT_STATEMENT( statement ) - Inserts the given C statement but only when asserts
 *  are in use.  This macros allows debug code that is not part of an expression.
 *
 *  ASSERT_DECLARATION( declaration ) - Inserts the given C declaration but only when asserts
 *  are in use.  This macros allows debug code that is not part of an expression.
 *
 *  Asserts can be disabled for optimum performance and minimum code size (ideal for
 *  finalized, debugged production code).  To disable, define the preprocessor
 *  symbol HALNODEBUG at the project level.
 */






__attribute__( ( always_inline ) ) static __inline void __osal_exit_critical(void)
{
  __asm volatile ("cpsie i");
}

__attribute__( ( always_inline ) ) static __inline void __osal_enter_critical(void)
{
  __asm volatile ("cpsid i");
}

#define     OSAL_ENTER_CRITICAL()           __osal_enter_critical()
#define     OSAL_EXIT_CRITICAL()            __osal_exit_critical()

#if (OSAL_MEM_EN > 0)
#define     osal_mem_init()                 umm_init()
#define     osal_mem_alloc(size)            umm_malloc(size)
#define     osal_mem_calloc(num, size)      umm_calloc(num, size)
#define     osal_mem_realloc(ptr, size)     umm_realloc(ptr, size)
#define     osal_mem_free(ptr)              umm_free(ptr)
#endif /* (OSAL_MEM_EN > 0) */

#if (OSAL_ASSERT_EN > 0)
extern void    osal_assert_handler( void );
#endif


















#endif //__OSAL_PORT_H__
