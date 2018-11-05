/******************************************************************************

 @file  app_task_key.c

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "osal.h"
#include "hal.h"

#include "app_config.h"
#include "app_assert.h"
#include "app_task_key.h"
#include "app_event_key.h"

#include "main.h"
#include "stdconst.h"
/**************************************************************************************************
 * TYPES
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/
#define KEY_THRESHOLD_LP        2000
#define KEY_THRESHOLD_VLP       4000
#define KEY_MULTI_MAX           sizeof(multiKeyValue)

#define KEY_EVENT_ENTER         1
#define KEY_EVENT_SHORT         2
#define KEY_EVENT_LONG          3
#define KEY_EVENT_VLONG         4
#define KEY_EVENT_LEAVE         0


#define TASK_KEY_EVENT_KEY0_LP                       1
#define TASK_KEY_EVENT_KEY0_VLP                      2
#define TASK_KEY_EVENT_KEY1_LP                       3
#define TASK_KEY_EVENT_KEY1_VLP                      4
#define TASK_KEY_EVENT_KEY2_LP                       5
#define TASK_KEY_EVENT_KEY2_VLP                      6
#define TASK_KEY_EVENT_KEY3_LP                       7
#define TASK_KEY_EVENT_KEY3_VLP                      8
#define TASK_KEY_EVENT_KEY4_LP                       9
#define TASK_KEY_EVENT_KEY4_VLP                      10
#define TASK_KEY_EVENT_KEY01_LP                      11
#define TASK_KEY_EVENT_KEY01_VLP                     12
#define TASK_KEY_EVENT_KEY02_LP                      13
#define TASK_KEY_EVENT_KEY02_VLP                     14

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
static uint8_t oldKeyValue;

static const uint8_t singleKeyEventLP[] = { 
    TASK_KEY_EVENT_KEY0_LP, 
    TASK_KEY_EVENT_KEY1_LP,
    TASK_KEY_EVENT_KEY2_LP,
    TASK_KEY_EVENT_KEY3_LP,
    TASK_KEY_EVENT_KEY4_LP 
};

static const uint8_t singleKeyEventVLP[] = { 
    TASK_KEY_EVENT_KEY0_VLP, 
    TASK_KEY_EVENT_KEY1_VLP,
    TASK_KEY_EVENT_KEY2_VLP,
    TASK_KEY_EVENT_KEY3_VLP,
    TASK_KEY_EVENT_KEY4_VLP 
};

static const uint8_t multiKeyValue[] = {
    BV( HAL_KEY_0 ) + BV( HAL_KEY_1 ),
    BV( HAL_KEY_0 ) + BV( HAL_KEY_2 ),
};

static const uint8_t multiKeyEventLP[] = {
    TASK_KEY_EVENT_KEY01_LP,
    TASK_KEY_EVENT_KEY02_LP,
};

static const uint8_t multiKeyEventVLP[] = {
    TASK_KEY_EVENT_KEY01_VLP,
    TASK_KEY_EVENT_KEY02_VLP,
};

static const uint8_t eventId_to_keyEvent[][2] = {
    { 0                                , 0               },                   //NULL
    { BV( HAL_KEY_0 )                  , KEY_EVENT_LONG  },                   //TASK_KEY_EVENT_KEY0_LP
    { BV( HAL_KEY_0 )                  , KEY_EVENT_VLONG },                   //TASK_KEY_EVENT_KEY0_VLP
    { BV( HAL_KEY_1 )                  , KEY_EVENT_LONG  },                   //TASK_KEY_EVENT_KEY1_LP
    { BV( HAL_KEY_1 )                  , KEY_EVENT_VLONG },                   //TASK_KEY_EVENT_KEY1_VLP
    { BV( HAL_KEY_2 )                  , KEY_EVENT_LONG  },                   //TASK_KEY_EVENT_KEY2_LP
    { BV( HAL_KEY_2 )                  , KEY_EVENT_VLONG },                   //TASK_KEY_EVENT_KEY2_VLP
    { BV( HAL_KEY_3 )                  , KEY_EVENT_LONG  },                   //TASK_KEY_EVENT_KEY3_LP
    { BV( HAL_KEY_3 )                  , KEY_EVENT_VLONG },                   //TASK_KEY_EVENT_KEY3_VLP
    { BV( HAL_KEY_4 )                  , KEY_EVENT_LONG  },                   //TASK_KEY_EVENT_KEY4_LP
    { BV( HAL_KEY_4 )                  , KEY_EVENT_VLONG },                   //TASK_KEY_EVENT_KEY4_VLP
    { BV( HAL_KEY_0 ) + BV( HAL_KEY_1 ), KEY_EVENT_LONG  },                   //TASK_KEY_EVENT_KEY01_LP
    { BV( HAL_KEY_0 ) + BV( HAL_KEY_1 ), KEY_EVENT_VLONG },                   //TASK_KEY_EVENT_KEY01_VLP
    { BV( HAL_KEY_0 ) + BV( HAL_KEY_2 ), KEY_EVENT_LONG  },                   //TASK_KEY_EVENT_KEY02_LP
    { BV( HAL_KEY_0 ) + BV( HAL_KEY_2 ), KEY_EVENT_VLONG },                   //TASK_KEY_EVENT_KEY02_VLP
};


static void key_event( uint8_t keyValue, uint8_t keyEvent )
{
    const uint8_t lutKeyEvent[] = {
        APP_EVENT_KEY_LEAVE,
        APP_EVENT_KEY_ENTER,
        APP_EVENT_KEY_SHORT,
        APP_EVENT_KEY_LONG,
        APP_EVENT_KEY_VLONG
    };
    app_event_key_update( keyValue, lutKeyEvent[keyEvent] );
}

static void key_processor( uint8_t newKeyValue )
{
    uint8_t updKeyValue;
    uint8_t newKeyId;
    uint8_t isShortPress;
    uint8_t i;

    //HalTerminalPrintStr("[EVENT]: newKeyValue = 0x");
    //HalTerminalPrintHex8( newKeyValue );
    //HalTerminalPrintStr("\r\n");
    updKeyValue = newKeyValue ^ oldKeyValue;

    if( updKeyValue )
    {
        for( newKeyId = 0; newKeyId < HAL_KEY_MAX; newKeyId++ )
        {
            if( updKeyValue & BV( newKeyId ) )
            {
                //this key is changed
                if( newKeyValue & BV( newKeyId ) )
                {
                    //key update, key enter
                    if( newKeyValue == BV( newKeyId ) )
                    {
                        //this is a single key enter event
                        osal_timer_event_create( TASK_ID_APP_KEY, singleKeyEventLP[newKeyId], KEY_THRESHOLD_LP);
                        osal_timer_event_create( TASK_ID_APP_KEY, singleKeyEventVLP[newKeyId], KEY_THRESHOLD_VLP );
                    }
                    else
                    {
                        //this key enters, while some other keys are pressed previously
                        //now cancle the LP VLP timer events of the keys pressed previouly
                        for( i = 0; i < HAL_KEY_MAX; i++ )
                        {
                            if( oldKeyValue & BV( i ) )
                            {
                                //found a key which was pressed previously, delete its LP VLP timer event
                                osal_timer_event_delete( TASK_ID_APP_KEY, singleKeyEventLP[i] );
                                osal_timer_event_delete( TASK_ID_APP_KEY, singleKeyEventVLP[i] );
                            }
                        }

                        //now cancle all the LP VLP timer events of the previous multi-key-touch
                        for( i = 0; i < KEY_MULTI_MAX; i++ )
                        {
                            osal_timer_event_delete( TASK_ID_APP_KEY, multiKeyEventLP[i] );
                            osal_timer_event_delete( TASK_ID_APP_KEY, multiKeyEventVLP[i] );
                        }

                        //now setup the LP VLP timer events of the multi-key-touch
                        for( i = 0; i < KEY_MULTI_MAX; i++ )
                        {
                            if( newKeyValue == multiKeyValue[i] )
                            {
                                osal_timer_event_create( TASK_ID_APP_KEY, multiKeyEventLP[i], KEY_THRESHOLD_LP );
                                osal_timer_event_create( TASK_ID_APP_KEY, multiKeyEventVLP[i], KEY_THRESHOLD_VLP );
                            }
                        }
                    }
                    key_event( newKeyValue, KEY_EVENT_ENTER );
                }
                else
                {
                    //key update, key leave

                    //now cancle all the LP VLP timer events of the previous single-key-touch
                    if( osal_timer_event_delete( TASK_ID_APP_KEY, singleKeyEventLP[newKeyId] ) != NULL )
                    {
                        //LP event has not happened yet, thus, this is a short press event
                        isShortPress = TRUE;
                    }
                    else
                    {
                        //LP event has happened, thus, this is a touch leave event
                        isShortPress = FALSE;
                    }
                    
                    osal_timer_event_delete( TASK_ID_APP_KEY, singleKeyEventVLP[newKeyId] );
                    //now cancle all the LP VLP timer events of the previous multi-key-touch
                    for( i = 0; i < KEY_MULTI_MAX; i++ )
                    {
                        osal_timer_event_delete( TASK_ID_APP_KEY, multiKeyEventLP[i] );
                        osal_timer_event_delete( TASK_ID_APP_KEY, multiKeyEventVLP[i] );
                    }

                    if( isShortPress == TRUE )
                    {
                        key_event( updKeyValue, KEY_EVENT_SHORT );
                    }
                    else
                    {
                        key_event( updKeyValue, KEY_EVENT_LEAVE );
                    }
                }
                newKeyId++;
            }
        }
    }


    oldKeyValue = newKeyValue;
    
}


extern void app_task_key_init ( void )
{
    oldKeyValue = 0;
}




/**************************************************************************************************
 * @fn      app_task_key
 *
 * @brief   Task Key
 *
 * @param   task_id - Key Task Id
 *          events - events
 *
 * @return  None
 **************************************************************************************************/
void app_task_key( uint8_t task_id, uint8_t event_id )
{
    uint8_t *p_msg;

    if( event_id == OSAL_TASK_EVENT_MSG )
    {
        p_msg = (uint8_t *)osal_msg_recv( task_id );
        while ( p_msg )
        {
            key_processor( *p_msg );
            osal_msg_delete( p_msg );
            p_msg = (uint8_t *)osal_msg_recv( task_id );
        }
    }
    else
    {
        key_event( eventId_to_keyEvent[event_id][0], eventId_to_keyEvent[event_id][1] );
    }
    
}

/**************************************************************************************************
**************************************************************************************************/

