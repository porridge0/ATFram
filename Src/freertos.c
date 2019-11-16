/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */  
#include <cm_backtrace.h>
#include "main.h"
#include "AT.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LedTaskHandle;
osThreadId ATTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */


/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void LEDStartTask(void const * argument);
void ATStartTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

	
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LedTask */
  osThreadDef(LedTask, LEDStartTask, osPriorityLow, 0, 128);
  LedTaskHandle = osThreadCreate(osThread(LedTask), NULL);

  /* definition and creation of ATTask */
  osThreadDef(ATTask, ATStartTask, osPriorityNormal, 0, 150);
  ATTaskHandle = osThreadCreate(osThread(ATTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);
    osDelay(200);

  }
  /* USER CODE END StartDefaultTask */
}

typedef struct ring_buffer {
	  /**< Buffer items are stored into */
volatile uint16_t head;         /**< Index of the next item to remove */
volatile uint16_t tail;         /**< Index where the next item will get inserted */
volatile uint16_t size;         /**< Buffer capacity minus one */
volatile uint8_t * buf; 
} ring_buffer;

void rb_init(ring_buffer *rb, uint16_t size, uint8_t *pbuff) {
    rb->head = 0;
    rb->tail = 0;
    rb->size = size-1;
    rb->buf = pbuff;
	
}

/* USER CODE BEGIN Header_LEDStartTask */
/**
* @brief Function implementing the LedyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LEDStartTask */
void LEDStartTask(void const * argument)
{
  /* USER CODE BEGIN LEDStartTask */
	static int i=0;
	unsigned portBASE_TYPE uxHighWaterMark;
  /* Infinite loop */
  for(;;)
  {
	
	if(i==MAXCMDNUM)
	{i=0;}
	ATCommandRegister(ATCommandList[i++].ATCommandName);

	uxHighWaterMark=uxTaskGetStackHighWaterMark( ATTaskHandle );
	__LOG("ATTask剩余栈空间是多少：%ld\r\n",uxHighWaterMark);
	uxHighWaterMark=uxTaskGetStackHighWaterMark( defaultTaskHandle );
	__LOG("defaultTaskHandle剩余栈空间是多少：%ld\r\n",uxHighWaterMark);
	uxHighWaterMark=uxTaskGetStackHighWaterMark( LedTaskHandle );
	__LOG("LedTaskHandle剩余栈空间是多少：%ld\r\n",uxHighWaterMark);
	
	volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
	*SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */
//    x = 10;
//    y = 0;
//    z = x / y;
//    printf("z:%d\n", z);
//	void fault_test_by_unalign(void);
//	fault_test_by_unalign();
    osDelay(100);
  }
  /* USER CODE END LEDStartTask */
}


/* USER CODE BEGIN Header_ATStartTask */
/**
* @brief Function implementing the ATTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ATStartTask */
void ATStartTask(void const * argument)
{
  /* USER CODE BEGIN ATStartTask */
  /* Infinite loop */
  for(;;)
  {
	ATCommandSendScheduler();
//	uint8_t ch[2];
//	ring_buffer *rb;
//	rb_init(rb,128,ch);
  }
  /* USER CODE END ATStartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
