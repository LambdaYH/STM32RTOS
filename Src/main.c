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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "GUI.h"
#include "MPU6050.h"
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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osThreadId KeyTaskHandle;
osThreadId DispTaskHandle;
osThreadId LedTaskHandle;
/* USER CODE BEGIN PV */
#define MAX_RECV_LEN 128
extern GUI_FLASH const GUI_FONT GUI_FontHZ_SimSun_12;
extern GUI_CONST_STORAGE GUI_BITMAP bmAvatar_Invert;
extern GUI_CONST_STORAGE GUI_BITMAP bmAvatar;
uint8_t K1_sta =0;
uint8_t K2_sta =0;
uint8_t K3_sta =0;
uint8_t K4_sta =0;
uint8_t func_switch=1;
int mpuok = 0;
uint8_t style = 0;
uint16_t val[20];
uint16_t adval=0;
int page_sta=0;
uint8_t Ledsta =0;
uint8_t LED_flowflag=0;

int start_sig=0;
int func_in=0;

char  recv[100];
int recvtype=0;

int playmode=0;
int ballx=64;
int bally=32;//陀螺仪控制球

int sendTime =100; //1ms
int sendTime_switch=0;
int bSend=0;
int dataStyle=0;
int sendCount=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
void StartDefaultTask(void const * argument);
void StartTKeyTask(void const * argument);
void StartDispTask(void const * argument);
void StartLedTask(void const * argument);

/* USER CODE BEGIN PFP */
uint8_t rx1_buff[MAX_RECV_LEN] = {0};  // 串口接收数据缓冲
uint8_t * pBuf = rx1_buff;  // 当前接收字节存放位置指针
uint8_t line_flag = 0;      // 一行数据接收标志
HAL_StatusTypeDef rxit_ok; 	// 接收中断是否开启
uint8_t rx2_buff[MAX_RECV_LEN] = {0};  // 串口接收数据缓冲
uint8_t * pBuf2 = rx2_buff; // 当前接收字节存放位置指针
uint8_t line_flag2 = 0;     // 一行数据接收标志
HAL_StatusTypeDef rxit_ok2; // 接收中断是否开启
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff); // 向串口1发送一个字符
    return 0;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  int i;
  uint16_t sum = 0;
  for (i = 0; i < 20; ++i)
    sum += val[i];
  adval = sum / 20;
}
//LED SET BEGIN 
void setLEDS(uint8_t sta){
	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,(sta & 0x01)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,(sta & 0x02)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,(sta & 0x04)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,(sta & 0x08)?GPIO_PIN_RESET:GPIO_PIN_SET);
}
// LED SET END
//流水灯 start
void RunLsd(void)
{
  setLEDS(Ledsta);
  
  if (LED_flowflag)
  {
    Ledsta >>= 1;
    if (0 == Ledsta)
      Ledsta = 0x08;
		HAL_Delay(100);
  }
}
//流水灯 end
//接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
  uint8_t cnt = 0; // 临时变量，用于重复计数
  if (UartHandle->Instance == USART1)
  {
    ++ pBuf;  // 已接收一个字节数据，当前存储位置指针后移
    if(pBuf == rx1_buff + MAX_RECV_LEN)  // 如果指针已移出数组边界
        pBuf = rx1_buff;    // 重新指向数组开头
    else if(*(pBuf - 1) == '\n')  // 如果之前接收到‘\n’换行符，则表示接收完成
        line_flag  = 1;
    // 重新开启接收中断
    do {
        rxit_ok = HAL_UART_Receive_IT(UartHandle, pBuf, 1);
        if (++cnt >= 5)
            break;
    } while(rxit_ok != HAL_OK);
  }
  else if(UartHandle->Instance == USART2)
  {
    ++ pBuf2;  // 已接收一个字节数据，当前存储位置指针后移
    if(pBuf2 == rx2_buff + MAX_RECV_LEN)  // 如果指针已移出数组边界
        pBuf2 = rx2_buff;    // 重新指向数组开头
    else if(*(pBuf2 - 1) == '\n')  // 如果之前接收到‘\n’换行符，则表示接收完成
        line_flag2  = 1;
    // 重新开启接收中断
    do {
        rxit_ok2 = HAL_UART_Receive_IT(UartHandle, pBuf2, 1);
        if (++cnt >= 5)
            break;
    } while(rxit_ok2 != HAL_OK);
  }
}

//校验和
void addChkSum(char *buf,int len){
	const char TAB[16] = {
		'0', '1', '2','3','4','5','6','7',
		'8', '9', 'A','B','C','D','E','F'
	};
	int i=0;
	uint16_t sum =0;
	for (i=0;i<len;++i){
		if(buf[i] !='\n'&&buf[i]!=0)
			sum+=buf[i];
		else 
			break;
	}
	buf[i]=TAB[(sum&0xFF)>>4];//保留高位右移4个字节
	buf[i+1]=TAB[sum&0x0F];
	buf[i+2]='\n';
	buf[i+3]='\0';
}
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	printf("hello PC\n");         
	rxit_ok = HAL_UART_Receive_IT(&huart1, pBuf, 1); 
	rxit_ok2 = HAL_UART_Receive_IT(&huart2, pBuf2, 1);  
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of KeyTask */
  osThreadDef(KeyTask, StartTKeyTask, osPriorityIdle, 0, 128);
  KeyTaskHandle = osThreadCreate(osThread(KeyTask), NULL);

  /* definition and creation of DispTask */
  osThreadDef(DispTask, StartDispTask, osPriorityIdle, 0, 256);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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

/* USER CODE BEGIN 4 */
// Function 1 START
void Func1(char *buf){
				if(K1_sta) page_sta=0;
				if(K2_sta) page_sta=1;
				if(page_sta==0){
					GUI_DispStringAt("按键|LED|ADC状态",0,0);
					sprintf(buf,"\n\nK1%s K2%s K3%s K4%s\nL1%s L2%s L3%s L4:%s\nADC:%d",K1_sta ? "▁" : "▅",K2_sta ? "▁" : "▅",K3_sta ? "▁" : "▅",K4_sta ? "▁" : "▅",
					(HAL_GPIO_ReadPin(LED1_GPIO_Port,LED1_Pin) == GPIO_PIN_RESET)?"●" : "○",
					(HAL_GPIO_ReadPin(LED2_GPIO_Port,LED2_Pin) == GPIO_PIN_RESET)?"●" : "○",
					(HAL_GPIO_ReadPin(LED3_GPIO_Port,LED3_Pin) == GPIO_PIN_RESET)?"●" : "○",
					(HAL_GPIO_ReadPin(LED4_GPIO_Port,LED4_Pin) == GPIO_PIN_RESET)?"●" : "○",
					adval);
				}else{
					GUI_DispStringAt("陀螺仪原始数据",0,0);
					sprintf(buf, "\n\nax:%6d gx:%6d\nay:%6d gy:%6d\naz:%6d gz:%6d", ax, gx, ay, gy, az, gz);
				}
				if(K3_sta==1) {
					LED_flowflag=!LED_flowflag;
					osDelay(300);
			
				}
				char t[20];
				sprintf(t,"%d/2",page_sta+1);
				GUI_DispStringAt(t,110,0);			//页码显示
			}
// Function 1 END
			
// Function 2 START
void Func2(char *buf){
				if(K1_sta) page_sta=0;
				if(K2_sta) page_sta=1;
				if(K3_sta==1) playmode = !playmode;
				if(playmode ==1){
					sprintf(buf,"%s","");
				  ballx=(90-fAY)*0.667;
				  if(ballx<=0)
				     ballx=0;
					else if(ballx>=120)
				     ballx=120;
					else 
				     ballx=(90-fAY)*0.667;
					
					if(fAX>0){
						 bally=64-2*fAX/3;
					}
					else {
						 bally=64+2*fAX/3;
					}
		      GUI_DispStringAt("●",ballx, bally);
				  HAL_Delay(10);
				}
				if(playmode==0){
					switch(page_sta){
					case 0: GUI_DispStringAt("MPU6050姿态角", 0, 0);sprintf(buf, "\n\n俯仰角:%6.1f°\n横滚角:%6.1f°\n航向角:%6.1f°", fAX, fAY, fAZ);break;
					case 1: GUI_DispStringAt("陀螺仪原始数据",0,0);sprintf(buf, "\n\nax:%6d gx:%6d\nay:%6d gy:%6d\naz:%6d gz:%6d", ax, gx, ay, gy, az, gz);break;
					default:GUI_DispStringAt("ERROR", 0, 0);
					}
					char t[20];
					sprintf(t,"%d/2",page_sta+1);
					GUI_DispStringAt(t,110,0);			//页码显示
				}
			}	
// Function 2 END
			
// Function 3 START
void Func3(char *buf){
			GUI_DispStringAt("串口测试", 0, 0);
			sprintf(buf, "\n%s", recv);
			if(K1_sta==1) {
				sendTime_switch++;
				switch(sendTime_switch){
					case 0:sendTime=100;break;
					case 1:sendTime=200;break;
					case 2:sendTime=500;break;
					case 3:sendTime=1000;break;
					default:sendTime_switch=0;sendTime=100;break;
					}
				osDelay(300);
			}
			if(K3_sta==1) {
				bSend=!bSend;
				osDelay(300);
			}
			if(K2_sta==1) {
				dataStyle=!dataStyle;
			}
			if(bSend) GUI_DispStringAt("正在发送",70,0);
			else GUI_DispStringAt("停止发送",70,0);
			
		}
// Function 3 END
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
	HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)val, 20);
  char buf[100];
	uint32_t oldtick = 0;
	mpuok = MPU_init();
	if (!mpuok)
		printf("MPU6050 init error!\n");
  /* Infinite loop */
  for(;;) {
		if (line_flag) {    // 如果串口1接收到一行数据
			printf("rx1_rec_data = %s", rx1_buff);        // 打印输出接收内容
			/////////////////////////////
			sprintf(recv,"%s",rx1_buff);
			int j=0;
			char t[100];
			for(int i=0;i<strlen(recv)-2;i++){
				t[j]=recv[i];
				j++;
				if(recv[i]=='\n'){
					memset(t, 0, sizeof(t));
					j=0;
				}
			}
			recvtype+=strlen(rx1_buff)-2;
			sprintf(recv, "\nReceive:\n%s\nSize:%d", t, recvtype);
			memset(t, 0, sizeof(t));
			// 接收数据处理
			/////////////////////////////
			memset(rx1_buff, 0, sizeof(rx1_buff));   // 清空串口1缓存区
			pBuf = rx1_buff;// 重新将串口1接收数据的存放指针指向接收缓存的头部
			(&huart1)->pRxBuffPtr = pBuf;    // 重新将串口1结构体中的接收缓冲指针指向缓冲数组头部
			line_flag = 0;  // 串口1接收标志清零
		}
		if (line_flag2) {		// 如果串口2接收到一行数据
			printf("rx2_buff = %s", rx2_buff);       // 打印接收内容到串口1
			//////////////////////////////
			// 接收数据处理
			//////////////////////////////
			memset(rx2_buff, 0, sizeof(rx2_buff));   // 清空串口2缓存区
			pBuf2 = rx2_buff;// 重新将串口2接收数据的存放指针指向接收缓存的头部
			(&huart2)->pRxBuffPtr = pBuf2;    // 重新将串口2结构体中的接收缓冲指针指向缓冲数组头部
			line_flag2 = 0;  // 串口2接收标志清零
		}
	if (rxit_ok != HAL_OK)		// 如果串口1接收中断还没有启动，尝试再次启动
		rxit_ok = HAL_UART_Receive_IT(&huart1, pBuf, 1);
	if (mpuok) MPU_getdata();
	
	if ((HAL_GetTick() >= oldtick + sendTime) && bSend)
		{
			oldtick = HAL_GetTick();
			if (mpuok)
			{
				if (dataStyle)	// 格式化计算后数据帧
					sprintf(buf, "7MR,%.1f,%.1f,%.1f,%.1f\n", adval * 3.3 / 4096, fAX, fAY, fAZ);
				else		// 格式化原始数据帧
					sprintf(buf, "7MY,%d,%d,%d,%d,%d,%d,%d\n", adval, ax, ay, az, gx, gy, gz);
				// 同时将字符串发送到串口1和串口2
				addChkSum(buf,strlen(buf));
				printf("%s", buf);	
				++sendCount;
			}
			osDelay(1);
			
			// 格式化状态帧
			sprintf(buf, "7MBD%c%c%c%c%c%c%c%c\n", 
									(Ledsta & 0x01) ? '1' : '0',
									(Ledsta & 0x02) ? '1' : '0',
									(Ledsta & 0x04) ? '1' : '0',
									(Ledsta & 0x08) ? '1' : '0',
									(K1_sta) ? 'D' : 'U',
									(K2_sta) ? 'D' : 'U',
									(K3_sta) ? 'D' : 'U',
									(K4_sta) ? 'D' : 'U');
			addChkSum(buf,strlen(buf));
			printf("%s", buf);		
			++sendCount;
		}
	
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
	char buf[100];
	GUI_Init();
  GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_Clear();
  GUI_DispStringAt("电子系统综合课程设计",5,0);
	GUI_DispStringAt("xxxxxxx",40,20);
	GUI_DispStringAt("xxx",45,40);
	GUI_Update();
	HAL_Delay(1000);
	
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
			func_in=1;
			osDelay(300);
		}
		if(K4_sta&start_sig){
			sprintf(buf,"基于STM32的无线节点\n\n->系统测试\n  陀螺仪姿态解算\n  串口通信测试\n  WIFI通信测试\n");
			func_in=0;
			func_switch=0;
			page_sta=0;
			recvtype=0;
			memset(recv, 0, sizeof(recv));
			ballx=0;
			bally=0;
			playmode=0;
		}
		
		if(func_in&start_sig){
			switch(func_switch){
				case 0:Func1(buf);break;
				case 1:Func2(buf);break;
				case 2:Func3(buf);break;
				default:Func1(buf);break;
		}
	}

		if(start_sig==0){
			GUI_DrawBitmap(&bmAvatar,0,0);
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
		RunLsd();
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
