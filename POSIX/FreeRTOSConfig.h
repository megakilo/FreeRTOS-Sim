#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				1
#define configUSE_TICK_HOOK				0
#define configTICK_RATE_HZ				( ( portTickType ) 1000 )
#define configMINIMAL_STACK_SIZE		( ( unsigned portSHORT ) 4 ) /* This can be made smaller if required. */
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 32 * 1024 ) )
#define configMAX_TASK_NAME_LEN			( 16 )
#define configUSE_TRACE_FACILITY    	0
#define configUSE_16_BIT_TICKS      	0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_MUTEXES				1
#define configCHECK_FOR_STACK_OVERFLOW	0 /* Do not use this option on the PC port. */
#define configUSE_RECURSIVE_MUTEXES		1
//#define configQUEUE_REGISTRY_SIZE		0
#define configQUEUE_REGISTRY_SIZE				20
#define configUSE_MALLOC_FAILED_HOOK			0

#define configUSE_APPLICATION_TASK_TAG	1
#define configUSE_COUNTING_SEMAPHORES	1
#define configUSE_ALTERNATIVE_API		0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY	1

#define configUSE_QUEUE_SETS					1
#define configUSE_TASK_NOTIFICATIONS			1

/* Software timer related configuration options. */
#define configUSE_TIMERS						1
#define configTIMER_TASK_PRIORITY				( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH				20
#define configTIMER_TASK_STACK_DEPTH			( configMINIMAL_STACK_SIZE * 2 )

#define configMAX_PRIORITIES		( 10 )

#define configGENERATE_RUN_TIME_STATS		1

/* Co-routine related configuration options. */
#define configUSE_CO_ROUTINES 			1
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )


/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function.  In most cases the linker will remove unused
functions anyway. */
#define INCLUDE_vTaskPrioritySet        	1
#define INCLUDE_uxTaskPriorityGet       	1
#define INCLUDE_vTaskDelete             	1
#define INCLUDE_vTaskCleanUpResources   	1
#define INCLUDE_vTaskSuspend            	1
#define INCLUDE_vTaskDelayUntil				1
#define INCLUDE_vTaskDelay					1
#define INCLUDE_uxTaskGetStackHighWaterMark 0 /* Do not use this option on the PC port. */
#define INCLUDE_xTaskGetSchedulerState		1

#define INCLUDE_xTimerGetTimerDaemonTaskHandle	1
#define INCLUDE_xTaskGetIdleTaskHandle			1
#define INCLUDE_pcTaskGetTaskName				1
#define INCLUDE_eTaskGetState					1
#define INCLUDE_xSemaphoreGetMutexHolder		1
#define INCLUDE_xTimerPendFunctionCall			1

/* An example "task switched in" hook macro definition. */
#define traceTASK_SWITCHED_IN() xTaskCallApplicationTaskHook( NULL, ( void * ) 0xabcd )

extern void vMainQueueSendPassed( void );
#define traceQUEUE_SEND( pxQueue ) vMainQueueSendPassed()

#endif /* FREERTOS_CONFIG_H */
