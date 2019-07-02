/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "GUI.h"
#include "MPU6050.h"
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osThreadId KeyTaskHandle;
osThreadId DispTaskHandle;
osThreadId LedTaskHandle;
/* USER CODE BEGIN PV */
extern GUI_FLASH const GUI_FONT GUI_FontHZ_SimSun_12;
extern GUI_CONST_STORAGE GUI_BITMAP bmAvatar_Invert;
uint8_t K1_sta =0;
uint8_t K2_sta =0;
uint8_t K3_sta =0;
uint8_t K4_sta =0;
uint8_t func_switch=1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void const * argument);
void StartTKeyTask(void const * argument);
void StartDispTask(void const * argument);
void StartLedTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

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

  /* definition and creation of KeyTask */
  osThreadDef(KeyTask, StartTKeyTask, osPriorityIdle, 0, 128);
  KeyTaskHandle = osThreadCreate(osThread(KeyTask), NULL);

  /* definition and creation of DispTask */
  osThreadDef(DispTask, StartDispTask, osPriorityIdle, 0, 128);
  DispTaskHandle = osThreadCreate(osThread(DispTask), NULL);

  /* definition and creation of LedTask */
  osThreadDef(LedTask, StartLedTask, osPriorityIdle, 0, 128);
  LedTaskHandle = osThreadCreate(osThread(LedTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED3_Pin|LED4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OLED_SCL_Pin|OLED_SDA_Pin|MPU6050_SCL_Pin|MPU6050_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : K1_Pin K2_Pin K3_Pin K4_Pin */
  GPIO_InitStruct.Pin = K1_Pin|K2_Pin|K3_Pin|K4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3_Pin LED4_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_SCL_Pin OLED_SDA_Pin MPU6050_SCL_Pin MPU6050_SDA_Pin */
  GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin|MPU6050_SCL_Pin|MPU6050_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
/* LED SET BEGIN */
void setLEDS(uint8_t sta){
	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,(sta & 0x01)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,(sta & 0x02)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,(sta & 0x04)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,(sta & 0x08)?GPIO_PIN_RESET:GPIO_PIN_SET);
}
/* LED SET END */
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTKeyTask */
/**
* @brief Function implementing the KeyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTKeyTask */
void StartTKeyTask(void const * argument)
{
  /* USER CODE BEGIN StartTKeyTask */
  /* Infinite loop */
  for(;;)
  {
		if(HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin) == GPIO_PIN_RESET) {
			K1_sta=1;
			while(HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin) == GPIO_PIN_RESET){
				osDelay(10);
			}
			osDelay(10);
			K1_sta=0;
			} else if (HAL_GPIO_ReadPin(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET){
					K2_sta=1;
					while(HAL_GPIO_ReadPin(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET){
					osDelay(10);
					}
					osDelay(10);
					K2_sta=0;
				} else if (HAL_GPIO_ReadPin(K3_GPIO_Port,K3_Pin) == GPIO_PIN_RESET){
						K3_sta=1;
						while(HAL_GPIO_ReadPin(K3_GPIO_Port,K3_Pin) == GPIO_PIN_RESET){
						osDelay(10);
						}
						osDelay(10);
						K3_sta=0;
					}else if (HAL_GPIO_ReadPin(K4_GPIO_Port,K4_Pin) == GPIO_PIN_RESET){
							K4_sta=1;
							while(HAL_GPIO_ReadPin(K4_GPIO_Port,K4_Pin) == GPIO_PIN_RESET){
							osDelay(10);
							}
							osDelay(10);
							K4_sta=0;
						}
						
				
    osDelay(1);
  }
  /* USER CODE END StartTKeyTask */
}

/* USER CODE BEGIN Header_StartDispTask */
/**
* @brief Function implementing the DispTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDispTask */
void StartDispTask(void const * argument)
{
  /* USER CODE BEGIN StartDispTask */
	char buf[200];
	GUI_Init();
  GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_Clear();
  GUI_DispStringAt("电子系统综合课程设计",5,0);
	GUI_DispStringAt("16041412",40,20);
	GUI_DispStringAt("陈逸航",45,40);
	GUI_Update();
	HAL_Delay(1000);
	int start_sig=0;
	int func_in=0;
  /* Infinite loop */
  for(;;)
  {
		GUI_Clear();
		if((K1_sta==1||K2_sta==1||K3_sta==1||K4_sta==1)&!start_sig){
			sprintf(buf,"基于STM32的无线节点\n\n->系统测试\n  陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");
			start_sig=1;
		}
		if(K1_sta&start_sig&func_in==0){
			switch(func_switch){
				case 0: sprintf(buf,"基于STM32的无线节点\n\n  陀螺仪姿态解算\n  串口通信测试\n->WIFI通信测试\n");func_switch=3;break;
				case 1: sprintf(buf,"基于STM32的无线节点\n\n->系统测试\n  陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");func_switch=0;break;
				case 2: sprintf(buf,"基于STM32的无线节点\n\n  系统测试\n->陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");func_switch=1;break;
				case 3: sprintf(buf,"基于STM32的无线节点\n\n  系统测试\n  陀螺仪姿态解算\n->串口通信测试\n  WIFI通信测试\n");func_switch=2;break;
				default:sprintf(buf,"基于STM32的无线节点\n\n->系统测试\n  陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");func_switch=0;break;
			}
			osDelay(300);
		}
		if(K2_sta&start_sig&func_in==0){
			switch(func_switch){
				case 0: sprintf(buf,"基于STM32的无线节点\n\n  系统测试\n->陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");func_switch=1;break;
				case 1: sprintf(buf,"基于STM32的无线节点\n\n  系统测试\n  陀螺仪姿态解算\n->串口通信测试\n  WIFI通信测试\n");func_switch=2;break;
				case 2: sprintf(buf,"基于STM32的无线节点\n\n  陀螺仪姿态解算\n  串口通信测试\n->WIFI通信测试\n");func_switch=3;break;
				case 3: sprintf(buf,"基于STM32的无线节点\n\n->系统测试\n  陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");func_switch=0;break;
				default:sprintf(buf,"基于STM32的无线节点\n\n  系统测试\n  陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");func_switch=0;break;
			}
			osDelay(300);
		}
		if(K3_sta&start_sig&func_in==0){
			switch(func_switch){
				case 0:sprintf(buf,"系统测试\nTODO");break;
				case 1:sprintf(buf,"陀螺仪姿态解算\nTODO");break;
				case 2:sprintf(buf,"串口通信测试\nTODO");break;
				case 3:sprintf(buf,"WIFI通信测试\nTODO");break;
				default:sprintf(buf,"error");break;
				}
			func_in=1;
			osDelay(300);
		}
		if(K4_sta&start_sig){
			sprintf(buf,"基于STM32的无线节点\n\n->系统测试\n  陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");
			func_in=0;
		}
		if(func_in&start_sig){
			char 
			sprintf(buf,"K1:%d K2:%d K3:%d K4:%d",K1_sta,K2_sta,K3_sta,K4_sta);
		}
		if(start_sig==0){
			GUI_DrawBitmap(&bmAvatar_Invert,0,0);
		}else{
			GUI_DispStringAt(buf,0,0);
		}
		GUI_Update();
    osDelay(1);
  }
  /* USER CODE END StartDispTask */
}

/* USER CODE BEGIN Header_StartLedTask */
/**
* @brief Function implementing the LedTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedTask */
void StartLedTask(void const * argument)
{
  /* USER CODE BEGIN StartLedTask */
	uint8_t ledstatus = 0;
  ledstatus=0xFF;
  for(int i=0;i<10;i++){
	setLEDS(ledstatus);
	HAL_Delay(100);
	ledstatus=~ledstatus;
  }
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartLedTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
