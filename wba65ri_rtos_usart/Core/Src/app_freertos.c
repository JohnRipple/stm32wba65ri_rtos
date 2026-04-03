/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for UartTask01 */
osThreadId_t UartTask01Handle;
const osThreadAttr_t UartTask01_attributes = {
  .name = "UartTask01",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for UartTask02 */
osThreadId_t UartTask02Handle;
const osThreadAttr_t UartTask02_attributes = {
  .name = "UartTask02",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 128 * 4
};
/* Definitions for UartTask03 */
osThreadId_t UartTask03Handle;
const osThreadAttr_t UartTask03_attributes = {
  .name = "UartTask03",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

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

	UART_HandleTypeDef* huart3 = getHuart3();

  /* USER CODE BEGIN RTOS_QUEUES */
  uart_txq = xQueueCreate(256, sizeof(char));
  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of UartTask01 */
  UartTask01Handle = osThreadNew(uart_task, huart3, &UartTask01_attributes);

  /* Example task to show transmitting data on uart */
  //UartTask02Handle = osThreadNew(SimpleUartTransmitTask, NULL, &UartTask02_attributes);

  /* creation of UartTask03 */
  UartTask03Handle = osThreadNew(StartTask04, NULL, &UartTask03_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

static void uart_puts(const char *s) {
	for ( ; *s; ++s) {
		//blocks when queue is full
		xQueueSend(uart_txq, s, portMAX_DELAY);
	}
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  /* Infinite loop */
  for(;;)
  {
    uart_puts("Now this is a message.. \n\r");
    uart_puts("   send via FreeRTOS queues.\n\n\r");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  /* USER CODE END defaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the UartTask01 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void uart_task(void *argument) {
	uint8_t ch;
	UART_HandleTypeDef* huart3 = (UART_HandleTypeDef*)argument;

	for(;;) {
		// Receive char to be TX
		if (xQueueReceive(uart_txq, &ch, 500) == pdPASS) {
			while (HAL_UART_Transmit(huart3, &ch, sizeof(ch), 500) != HAL_OK)
				taskYIELD();
		}
	}
}


void SimpleUartTransmitTask(void *argument)
{
  /* USER CODE BEGIN UartTask01 */
  /* Infinite loop */
  UART_HandleTypeDef* huart3 = (UART_HandleTypeDef*)argument;
  uint8_t c = '0' - 1;
  uint8_t newline[] = "\r\n";
  for(;;)
  {
	  HAL_UART_Transmit(huart3, &c, sizeof(c), 500);
	  //send character
	  if ( ++c >= 'Z' ) {
		  // Send return and newline
		  HAL_UART_Transmit(huart3, newline, sizeof(newline), 500);
		  c = '0' - 1;
	  }
    osDelay(1);
  }
  /* USER CODE END UartTask01 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the UartTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  for(;;)
  {
	uart_puts("Task04\r\n");
	osDelay(500);
  }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

