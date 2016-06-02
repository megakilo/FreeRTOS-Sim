/*
    FreeRTOS V8.2.2 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/**
 * Creates all the demo application tasks and co-routines, then starts the
 * scheduler.
 *
 * Main. c also creates a task called "Print".  This only executes every
 * five seconds but has the highest priority so is guaranteed to get
 * processor time.  Its main function is to check that all the other tasks
 * are still operational.  Nearly all the tasks in the demo application
 * maintain a unique count that is incremented each time the task successfully
 * completes its function.  Should any error occur within the task the count is
 * permanently halted.  The print task checks the count of each task to ensure
 * it has changed since the last time the print task executed.  If any count is
 * found not to have changed the print task displays an appropriate message.
 * If all the tasks are still incrementing their unique counts the print task
 * displays an "OK" message.
 *
 * The LED flash tasks do not maintain a count as they already provide visual
 * feedback of their status.
 *
 * The print task blocks on the queue into which messages that require
 * displaying are posted.  It will therefore only block for the full 5 seconds
 * if no messages are posted onto the queue.
 *
 * \page MainC main.c
 * \ingroup DemoFiles
 * <HR>
 */

#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

/* Demo file headers. */
#include "BlockQ.h"
#include "blocktim.h"
#include "countsem.h"
#include "death.h"
#include "dynamic.h"
#include "fileIO.h"
#include "flop.h"
#include "GenQTest.h"
#include "integer.h"
#include "PollQ.h"
#include "print.h"
#include "QPeek.h"
#include "recmutex.h"
#include "semtest.h"

/* Priority definitions for the tasks in the demo application. */
#define mainLED_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainCREATOR_TASK_PRIORITY	( tskIDLE_PRIORITY + 3 )
#define mainPRINT_TASK_PRIORITY		( tskIDLE_PRIORITY + 4 )
#define mainQUEUE_POLL_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_BLOCK_PRIORITY	( tskIDLE_PRIORITY + 3 )
#define mainSEMAPHORE_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define mainGENERIC_QUEUE_PRIORITY	( tskIDLE_PRIORITY )

#define mainPRINT_STACK_SIZE		( ( unsigned short ) 512 )
#define mainDEBUG_LOG_BUFFER_SIZE	( ( unsigned short ) 20480 )

/* The number of flash co-routines to create. */
#define mainNUM_FLASH_CO_ROUTINES	( 8 )

void vApplicationIdleHook( void );

/* Task function for the "Print" task as described at the top of the file. */
static void vErrorChecks( void *pvParameters );

/* Function that checks the unique count of all the other tasks as described at
the top of the file. */
static void prvCheckOtherTasksAreStillRunning( void );

/* Constant definition used to turn on/off the pre-emptive scheduler. */
static const short sUsingPreemption = configUSE_PREEMPTION;

/* Used to demonstrate the "task switched in" callback function. */
static portBASE_TYPE prvExampleTaskHook( void * pvParameter );

/* Just used to count the number of times the example task callback function is
called, and the number of times a queue send passes. */
static unsigned long long uxCheckTaskHookCallCount = 0;
static unsigned long long uxQueueSendPassedCount = 0;

/*-----------------------------------------------------------*/

int main( void )
{
	/* Initialise hardware and utilities. */
	vPrintInitialise();

	/* CREATE ALL THE DEMO APPLICATION TASKS. */
	vStartMathTasks( tskIDLE_PRIORITY );
	vStartBlockingQueueTasks( mainQUEUE_BLOCK_PRIORITY );
	vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
	vStartSemaphoreTasks( mainSEMAPHORE_TASK_PRIORITY );
	vStartDynamicPriorityTasks();

	vCreateBlockTimeTasks();
	vStartGenericQueueTasks( mainGENERIC_QUEUE_PRIORITY );
	vStartQueuePeekTasks();
	vStartCountingSemaphoreTasks();
	vStartRecursiveMutexTasks();

	/* Create the "Print" task as described at the top of the file. */
	xTaskCreate( vErrorChecks, "Print", mainPRINT_STACK_SIZE, NULL, mainPRINT_TASK_PRIORITY, NULL );

	// This task has to be created last as it keeps account of the number of tasks
	// it expects to see running.
	//vCreateSuicidalTasks( mainCREATOR_TASK_PRIORITY );

	/* Set the scheduler running.  This function will not return unless a task
	calls vTaskEndScheduler(). */
	vTaskStartScheduler();

	return 1;
}
/*-----------------------------------------------------------*/

static portBASE_TYPE prvExampleTaskHook( void * pvParameter )
{
	if( pvParameter != ( void * ) 0xabcd )
	{
		/* The parameter did not contain the value we expected, so cause an
		error to be detected by setting the call count back to zero. */
		uxCheckTaskHookCallCount = 0;
	}
	else
	{
		/* Simply increment a number so we know the callback has been executed. */
		uxCheckTaskHookCallCount++;
	}

	return 0;
}
/*-----------------------------------------------------------*/

void vMainQueueSendPassed( void )
{
	/* This is just an example implementation of the "queue send" trace hook. */
	uxQueueSendPassedCount++;
}
/*-----------------------------------------------------------*/

static void vErrorChecks( void *pvParameters )
{
TickType_t xExpectedWakeTime;
const TickType_t xPrintRate = ( TickType_t ) 5000 / portTICK_PERIOD_MS;
const long lMaxAllowableTimeDifference = ( long ) 0;
TickType_t xWakeTime;
long lTimeDifference;
const char *pcReceivedMessage;
const char * const pcTaskBlockedTooLongMsg = "Print task blocked too long!\r\n";
const char * const pcUnexpectedHookValueMsg = "Task hook has unexpected value!\r\n";

	( void ) pvParameters;

	/* Register our callback function. */
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );

	/* Just for test purposes. */
	if( xTaskGetApplicationTaskTag( NULL ) != prvExampleTaskHook )
	{
		vPrintDisplayMessage( &pcUnexpectedHookValueMsg );
	}

	/* Loop continuously, blocking, then checking all the other tasks are still
	running, before blocking once again.  This task blocks on the queue of
	messages that require displaying so will wake either by its time out expiring,
	or a message becoming available. */
	for( ;; )
	{
		/* Calculate the time we will unblock if no messages are received
		on the queue.  This is used to check that we have not blocked for too long. */
		xExpectedWakeTime = xTaskGetTickCount();
		xExpectedWakeTime += xPrintRate;

		/* Block waiting for either a time out or a message to be posted that
		required displaying. */
		pcReceivedMessage = pcPrintGetNextMessage( xPrintRate );

		/* Was a message received? */
		if( pcReceivedMessage == NULL )
		{
			/* A message was not received so we timed out, did we unblock at the
			expected time? */
			xWakeTime = xTaskGetTickCount();

			/* Calculate the difference between the time we unblocked and the
			time we should have unblocked. */
			if( xWakeTime > xExpectedWakeTime )
			{
				lTimeDifference = ( long ) ( xWakeTime - xExpectedWakeTime );
			}
			else
			{
				lTimeDifference = ( long ) ( xExpectedWakeTime - xWakeTime );
			}

			if( lTimeDifference > lMaxAllowableTimeDifference )
			{
				/* We blocked too long - create a message that will get
				printed out the next time around.  If we are not using
				preemption then we won't expect the timing to be so
				accurate. */
				if( sUsingPreemption == pdTRUE )
				{
					vPrintDisplayMessage( &pcTaskBlockedTooLongMsg );
				}
			}

			/* Check the other tasks are still running, just in case. */
			prvCheckOtherTasksAreStillRunning();
		}
		else
		{
			/* We unblocked due to a message becoming available.  Send the message
			for printing. */
			vDisplayMessage( pcReceivedMessage );
		}
	}
}
/*-----------------------------------------------------------*/

static void prvCheckOtherTasksAreStillRunning( void )
{
static short sErrorHasOccurred = pdFALSE;
static unsigned long long uxLastHookCallCount = 0;
static unsigned long long uxLastQueueSendCount = 0;

	if( xAreMathsTaskStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Maths task count unchanged!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xAreBlockingQueuesStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Blocking queues count unchanged!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xArePollingQueuesStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Polling queue count unchanged!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}
    
    /*
	if( xIsCreateTaskStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Incorrect number of tasks running!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}
    */

	if( xAreSemaphoreTasksStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Semaphore take count unchanged!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xAreDynamicPriorityTasksStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Dynamic priority count unchanged!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Error in block time test tasks!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xAreGenericQueueTasksStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Error in generic queue test task!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xAreQueuePeekTasksStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Error in queue peek test task!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xAreCountingSemaphoreTasksStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Error in counting semaphore demo task!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	if( xAreRecursiveMutexTasksStillRunning() != pdTRUE )
	{
		vDisplayMessage( "Error in recursive mutex tasks!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}

	/* The hook function associated with this task is called each time the task
	is switched in.  We therefore expect the number of times the callback
	function has been executed to have increrment since the last time this
	function executed. */
	if( uxCheckTaskHookCallCount <= uxLastHookCallCount )
	{
		vDisplayMessage( "Error in task hook call count!\r\n" );
        printf("uxCheckTaskHookCallCount: %llu\n", uxCheckTaskHookCallCount);
        printf("uxLastHookCallCount: %llu\n", uxLastHookCallCount);
		sErrorHasOccurred = pdTRUE;
	}
	else
	{
		uxLastHookCallCount = uxCheckTaskHookCallCount;
	}

	/* We would expect some queue sending to occur between calls of this
	function. */
	if( uxQueueSendPassedCount <= uxLastQueueSendCount )
	{
		vDisplayMessage( "Error in queue send hook call count!\r\n" );
		sErrorHasOccurred = pdTRUE;
	}
	else
	{
		uxLastQueueSendCount = uxQueueSendPassedCount;
	}

	if( sErrorHasOccurred == pdFALSE )
	{
		vDisplayMessage( "OK " );
	}
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
}
/*-----------------------------------------------------------*/
