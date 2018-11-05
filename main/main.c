/******************************************************************************

 @file  main.c

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

#include "osal.h"
#include "hal.h"
#include "app.h"
#include "main.h"

int main( void )
{
    /* Disable Interrupts */
    OSAL_ENTER_CRITICAL();
    
    /* Initialize the OS's vars */
    osal_system_init();
    
    /* Initialize mcu and devices */
    hal_driver_init();
    
    /* Create HAL Tasks */
    osal_task_create( hal_task_driver_basic, TASK_ID_HAL_DRIVERS );
    osal_task_create( hal_task_driver_wifimod, TASK_ID_HAL_DRIVER_WIFIMOD );
    
    /* Create APP Tasks */
    osal_task_create( app_task_wifimod, TASK_ID_APP_WIFIMOD );
    app_task_wifimod_init();
    
    osal_task_create( app_task_cli, TASK_ID_APP_CLI );
    app_task_cli_init();

    osal_task_create( app_task_uibrd, TASK_ID_APP_UIBRD );
    app_task_uibrd_init();

    osal_task_create( app_task_bt, TASK_ID_APP_BT );
    app_task_bt_init();
    
    osal_task_create( app_task_key, TASK_ID_APP_KEY );
    app_task_key_init();
    
    osal_task_create( app_task_main, TASK_ID_APP_MAIN );
    app_task_main_init();

    /* Enable Interrupts */
    OSAL_EXIT_CRITICAL();
    /* Start OSAL */
    osal_system_start(); // No Return from here

    return 0;

}

