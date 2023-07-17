/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip/apps/httpd.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "api.h"
#include "string.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#include "lwip/netbuf.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "err.h"
#include "lwip/ip.h"
#include "lwip/ip4.h"
#include "lwip/ip4_addr.h"
#include "lwip/ip_addr.h"
#include "lwip/sockets.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mbap_conf.h"
#include "mbap_user.h"
#include "tcp1.h"

//#include "udpclient.h"

#include <float.h>
#include <math.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printf.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PORT     2020 
#define MAXLINE  1024 

u32_t number_of_bytes=0;
char buffer[MAXLINE]; 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
err_t err, recv_err;
ip_addr_t dest_addr;
int indx = 0;
char smsg[200];
int sendReq = 1;
void udpsend(char *data);

#define TIMCLOCK_1   80000000
#define TIMCLOCK_2   160000000
#define PRESCALAR_1  80
#define PRESCALAR_2  80


#define ARRAY_LEN(x)  (sizeof(x)/sizeof((x)[0]))
uint8_t usart_rx[2500];
size_t pos;
#define NUMBERS_COUNT 17
#define NUMBER_LENGTH 4
#define BUFFER_LENGTH 84
uint8_t Rx_data[84];  
int sl_inputs[NUMBERS_COUNT];

uint32_t IC_Val1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t IC_Val2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t Difference[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Is_First_Captured[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t frequency[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t frequency_f[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

char update_status[] = "mode0";
uint16_t capture_freq[100][16];
uint8_t readCount = 0;
uint8_t counter = 0;

uint16_t selected_data[25][16];
uint16_t out_data[800];

int sensorStatus = 0;
int sensorNumber = 0;
int readStatus = 0;

int slave_recive = 0;
uint32_t slave_data[400];

char outstr[4802];
int sendData = 0;

extern int sendReq;
uint8_t sendCounter = 0;

uint8_t recBlock = 0;
uint8_t sendOutUdp = 0;
uint8_t socketStatus = 0;

char header[1200];
char footer[1200];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* Definitions for modbusTaskTcp */
osThreadId_t modbusTaskTcpHandle;
const osThreadAttr_t modbusTaskTcp_attributes = {
  .name = "modbusTaskTcp",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 1024 * 4
};
/* Definitions for Freq_Cal */
osThreadId_t Freq_CalHandle;
const osThreadAttr_t Freq_Cal_attributes = {
  .name = "Freq_Cal",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
void modebusTaskTcpFunc(void *argument);
void Freq_Cal_entry(void *argument);

/* USER CODE BEGIN PFP */
void usart_transmit_data(const void* data, uint16_t len) {
  const uint8_t* b = data;
  while (len--) {
    LL_USART_TransmitData8(USART3, *b++);
    while (!LL_USART_IsActiveFlag_TXE(USART3));
  }
}

void usart_rx_check(void) {
  static size_t old_pos;
  
  /* Calculate current position in buffer */
  pos = ARRAY_LEN(usart_rx) - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_5);
  old_pos = pos;                              /* Save current position as old */
  /* Check and manually update if we reached end of buffer */
  if (old_pos == ARRAY_LEN(usart_rx)) {
    old_pos = 0;
  }
}

/**
* \brief           Process received data over UART
* \note            Either process them directly or copy to other bigger buffer
* \param[in]       data: Data to process
* \param[in]       len: Length in units of bytes
*/
void usart_process_data(const void* data, size_t len) {
  const uint8_t* d = data;
  while (len--) {
    LL_USART_TransmitData8(USART3, *d++);
    while (!LL_USART_IsActiveFlag_TXE(USART3));
  }
  while (!LL_USART_IsActiveFlag_TC(USART3));
}


/**
* @brief USART3 Initialization Function
* @param None
* @retval None
*/
static void MX_USART3_UART_Init(void)
{
  
  /* USER CODE BEGIN USART3_Init 0 */
  
  /* USER CODE END USART3_Init 0 */
  
  LL_USART_InitTypeDef USART_InitStruct = {0};
  
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
  
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  /**USART3 GPIO Configuration  
  PD8   ------> USART3_TX
  PD9   ------> USART3_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /* USART3 DMA Init */
  
  /* USART3_RX Init */
  /*
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_1, LL_DMA_CHANNEL_4);
  
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  
  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_1, LL_DMA_PRIORITY_LOW);
  
  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MODE_CIRCULAR);
  
  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_PERIPH_NOINCREMENT);
  
  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MEMORY_INCREMENT);
  
  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_1, LL_DMA_PDATAALIGN_BYTE);
  
  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_1, LL_DMA_MDATAALIGN_BYTE);
  
  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_1);
  */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_1, LL_DMA_CHANNEL_4);
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_1, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MODE_CIRCULAR);
  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_1, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_1, LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_1);
  
  LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_1, (uint32_t)&USART3->DR);
  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_1, (uint32_t)usart_rx);
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, ARRAY_LEN(usart_rx));
  
  
  /* USART3 interrupt Init */
  NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(USART3_IRQn);
  
  /* USER CODE BEGIN USART3_Init 1 */
  
  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_EnableDMAReq_RX(USART3);
  LL_USART_EnableIT_IDLE(USART3);
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
  LL_USART_Enable(USART3);
  /* USER CODE BEGIN USART3_Init 2 */
  
  /* USER CODE END USART3_Init 2 */
  
}



/**
* \brief           Send string to USART
* \param[in]       str: String to send
*/
void usart_send_string(const char* str) {
  usart_process_data(str, strlen(str));
}
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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);
  
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
  
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
  
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);
  
  
  //HAL_UART_Receive_IT (&huart3, Rx_data, 4);
  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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
  /* creation of modbusTaskTcp */
  modbusTaskTcpHandle = osThreadNew(modebusTaskTcpFunc, NULL, &modbusTaskTcp_attributes);

  /* creation of Freq_Cal */
  Freq_CalHandle = osThreadNew(Freq_Cal_entry, NULL, &Freq_Cal_attributes);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */
  
  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */
  
  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 80-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */
  
  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
  
  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 80-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */
  
  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */
  
  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 80-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  
  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */
  
  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */
  
  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 80-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
  
  /* USER CODE END TIM4_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Stream1_IRQn);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : Input_Sensor1_Pin Input_Sensor2_Pin Input_Sensor3_Pin Input_Sensor4_Pin */
  GPIO_InitStruct.Pin = Input_Sensor1_Pin|Input_Sensor2_Pin|Input_Sensor3_Pin|Input_Sensor4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : ETH_RST_Pin */
  GPIO_InitStruct.Pin = ETH_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ETH_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SensorInput_Pin */
  GPIO_InitStruct.Pin = SensorInput_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SensorInput_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* Measure Frequency */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim1)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      if (Is_First_Captured[0]==0) // if the first rising edge is not captured
      {
        IC_Val1[0] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
        Is_First_Captured[0] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[0] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
        
        if (IC_Val2[0] > IC_Val1[0])
        {
          Difference[0] = IC_Val2[0]-IC_Val1[0];
        }
        
        else if (IC_Val1[0] > IC_Val2[0])
        {
          Difference[0] = (0xffffffff - IC_Val1[0]) + IC_Val2[0];
        }
        
        float refClock = TIMCLOCK_2/(PRESCALAR_2);
        
        frequency[7] = (int)refClock/Difference[0];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[0] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      if (Is_First_Captured[1]==0) // if the first rising edge is not captured
      {
        IC_Val1[1] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
        Is_First_Captured[1] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[1] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
        
        if (IC_Val2[1] > IC_Val1[1])
        {
          Difference[1] = IC_Val2[1]-IC_Val1[1];
        }
        
        else if (IC_Val1[1] > IC_Val2[1])
        {
          Difference[1] = (0xffffffff - IC_Val1[1]) + IC_Val2[1];
        }
        
        float refClock = TIMCLOCK_2/(PRESCALAR_2);
        
        frequency[6] = (int)refClock/Difference[1];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[1] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      if (Is_First_Captured[2]==0) // if the first rising edge is not captured
      {
        IC_Val1[2] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
        Is_First_Captured[2] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[2] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value
        
        if (IC_Val2[2] > IC_Val1[2])
        {
          Difference[2] = IC_Val2[2]-IC_Val1[2];
        }
        
        else if (IC_Val1[2] > IC_Val2[2])
        {
          Difference[2] = (0xffffffff - IC_Val1[2]) + IC_Val2[2];
        }
        
        float refClock = TIMCLOCK_2/(PRESCALAR_2);
        
        frequency[5] = (int)refClock/Difference[2];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[2] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      if (Is_First_Captured[3]==0) // if the first rising edge is not captured
      {
        IC_Val1[3] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
        Is_First_Captured[3] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[3] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
        
        if (IC_Val2[3] > IC_Val1[3])
        {
          Difference[3] = IC_Val2[3]-IC_Val1[3];
        }
        
        else if (IC_Val1[3] > IC_Val2[3])
        {
          Difference[3] = (0xffffffff - IC_Val1[3]) + IC_Val2[3];
        }
        
        float refClock = TIMCLOCK_2/(PRESCALAR_2);
        
        frequency[4] = (int)refClock/Difference[3];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[3] = 0; // set it back to false
      }
    }
  }
  ///////////////////////////Timer 2
  else if (htim == &htim2)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      if (Is_First_Captured[4]==0) // if the first rising edge is not captured
      {
        IC_Val1[4] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
        Is_First_Captured[4] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[4] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
        
        if (IC_Val2[4] > IC_Val1[4])
        {
          Difference[4] = IC_Val2[4]-IC_Val1[4];
        }
        
        else if (IC_Val1[4] > IC_Val2[4])
        {
          Difference[4] = (0xffffffff - IC_Val1[4]) + IC_Val2[4];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[3] = (int)refClock/Difference[4];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[4] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      if (Is_First_Captured[5]==0) // if the first rising edge is not captured
      {
        IC_Val1[5] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
        Is_First_Captured[5] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[5] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
        
        if (IC_Val2[5] > IC_Val1[5])
        {
          Difference[5] = IC_Val2[5]-IC_Val1[5];
        }
        
        else if (IC_Val1[5] > IC_Val2[5])
        {
          Difference[5] = (0xffffffff - IC_Val1[5]) + IC_Val2[5];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[2] = (int)refClock/Difference[5];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[5] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      if (Is_First_Captured[6]==0) // if the first rising edge is not captured
      {
        IC_Val1[6] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
        Is_First_Captured[6] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[6] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value
        
        if (IC_Val2[6] > IC_Val1[6])
        {
          Difference[6] = IC_Val2[6]-IC_Val1[6];
        }
        
        else if (IC_Val1[6] > IC_Val2[6])
        {
          Difference[6] = (0xffffffff - IC_Val1[6]) + IC_Val2[6];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[1] = (int)refClock/Difference[6];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[6] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      if (Is_First_Captured[7]==0) // if the first rising edge is not captured
      {
        IC_Val1[7] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
        Is_First_Captured[7] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[7] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
        
        if (IC_Val2[7] > IC_Val1[7])
        {
          Difference[7] = IC_Val2[7]-IC_Val1[7];
        }
        
        else if (IC_Val1[7] > IC_Val2[7])
        {
          Difference[7] = (0xffffffff - IC_Val1[7]) + IC_Val2[7];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[0] = (int)refClock/Difference[7];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[7] = 0; // set it back to false
      }
    }
  }
  ////////////////////////Timer 3
  else if (htim == &htim3)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      if (Is_First_Captured[8]==0) // if the first rising edge is not captured
      {
        IC_Val1[8] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
        Is_First_Captured[8] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[8] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
        
        if (IC_Val2[8] > IC_Val1[8])
        {
          Difference[8] = IC_Val2[8]-IC_Val1[8];
        }
        
        else if (IC_Val1[8] > IC_Val2[8])
        {
          Difference[8] = (0xffffffff - IC_Val1[8]) + IC_Val2[8];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[8] = (int)refClock/Difference[8];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[8] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      if (Is_First_Captured[9]==0) // if the first rising edge is not captured
      {
        IC_Val1[9] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
        Is_First_Captured[9] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[9] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
        
        if (IC_Val2[9] > IC_Val1[9])
        {
          Difference[9] = IC_Val2[9]-IC_Val1[9];
        }
        
        else if (IC_Val1[9] > IC_Val2[9])
        {
          Difference[9] = (0xffffffff - IC_Val1[9]) + IC_Val2[9];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[9] = (int)refClock/Difference[9];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[9] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      if (Is_First_Captured[10]==0) // if the first rising edge is not captured
      {
        IC_Val1[10] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
        Is_First_Captured[10] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[10] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value
        
        if (IC_Val2[10] > IC_Val1[10])
        {
          Difference[10] = IC_Val2[10]-IC_Val1[10];
        }
        
        else if (IC_Val1[10] > IC_Val2[10])
        {
          Difference[10] = (0xffffffff - IC_Val1[10]) + IC_Val2[10];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[10] = (int)refClock/Difference[10];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[10] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      if (Is_First_Captured[11]==0) // if the first rising edge is not captured
      {
        IC_Val1[11] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
        Is_First_Captured[11] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[11] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
        
        if (IC_Val2[11] > IC_Val1[11])
        {
          Difference[11] = IC_Val2[11]-IC_Val1[11];
        }
        
        else if (IC_Val1[11] > IC_Val2[11])
        {
          Difference[11] = (0xffffffff - IC_Val1[11]) + IC_Val2[11];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[11] = (int)refClock/Difference[11];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[11] = 0; // set it back to false
      }
    }
  }
  /////////////////////////Timer 4
  else if (htim == &htim4)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      if (Is_First_Captured[12]==0) // if the first rising edge is not captured
      {
        IC_Val1[12] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
        Is_First_Captured[12] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[12] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
        
        if (IC_Val2[12] > IC_Val1[12])
        {
          Difference[12] = IC_Val2[12]-IC_Val1[12];
        }
        
        else if (IC_Val1[12] > IC_Val2[12])
        {
          Difference[12] = (0xffffffff - IC_Val1[12]) + IC_Val2[12];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[12] = (int)refClock/Difference[12];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[12] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      if (Is_First_Captured[13]==0) // if the first rising edge is not captured
      {
        IC_Val1[13] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
        Is_First_Captured[13] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[13] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
        
        if (IC_Val2[13] > IC_Val1[13])
        {
          Difference[13] = IC_Val2[13]-IC_Val1[13];
        }
        
        else if (IC_Val1[13] > IC_Val2[13])
        {
          Difference[13] = (0xffffffff - IC_Val1[13]) + IC_Val2[13];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[13] = (int)refClock/Difference[13];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[13] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      if (Is_First_Captured[14]==0) // if the first rising edge is not captured
      {
        IC_Val1[14] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
        Is_First_Captured[14] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[14] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value
        
        if (IC_Val2[14] > IC_Val1[14])
        {
          Difference[14] = IC_Val2[14]-IC_Val1[14];
        }
        
        else if (IC_Val1[14] > IC_Val2[14])
        {
          Difference[14] = (0xffffffff - IC_Val1[14]) + IC_Val2[14];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[14] = (int)refClock/Difference[14];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[14] = 0; // set it back to false
      }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      if (Is_First_Captured[15]==0) // if the first rising edge is not captured
      {
        IC_Val1[15] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
        Is_First_Captured[15] = 1;  // set the first captured as true
      }
      
      else   // If the first rising edge is captured, now we will capture the second edge
      {
        IC_Val2[15] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
        
        if (IC_Val2[15] > IC_Val1[15])
        {
          Difference[15] = IC_Val2[15]-IC_Val1[15];
        }
        
        else if (IC_Val1[11] > IC_Val2[11])
        {
          Difference[15] = (0xffffffff - IC_Val1[15]) + IC_Val2[15];
        }
        
        float refClock = TIMCLOCK_1/(PRESCALAR_1);
        
        frequency[15] = (int)refClock/Difference[15];
        
        //        __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
        Is_First_Captured[15] = 0; // set it back to false
      }
    }
  }
}

char *convert_to_string(uint16_t data[], size_t len) {
  char *str = malloc(len * 6); // 5 digits + 1 separator
  if (str == NULL) {
    return NULL;
  }
  
  for (size_t i = 0; i < len; i++) {
    sprintf(str + i * 6, "%u$", data[i]);
  }
  str[len * 6 - 1] = '\0';
  
  return str;
}

void arrayToString(uint16_t* dataArray, int size, char* result) {
  for (int i = 0; i < size; i++) {
    char temp[6];
    tfp_sprintf(temp,"%05u$",dataArray[i]);
    //      sprintf(temp,"%u$",dataArray[i]);
    strcat(result,temp);
  }
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_modebusTaskTcpFunc */
/**
* @brief  Function implementing the modbusTaskTcp thread.
* @param  argument: Not used
* @retval None
*/
/* USER CODE END Header_modebusTaskTcpFunc */
void modebusTaskTcpFunc(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */
  
  
  int sockfd; 
  struct sockaddr_in servaddr, cliaddr; 
  u32_t len; 
  
  
  
  /*
  
  char payload[] = "Message from STM32";
  
  
  struct sockaddr_in  dest_addr_udp;
  dest_addr_udp.sin_addr.s_addr = inet_addr("192.168.0.30");
  dest_addr_udp.sin_family = AF_INET;
  dest_addr_udp.sin_port = htons(5555);
  
  int sock_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  */
  
  // Bind the socket with the server address 
//  bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));  //****
  
  len = sizeof(servaddr);  //len is value/resuslt
  
  for(int i = 0; i < 1200; i++){
    header[i] = '1';
  }
  for(int i = 0; i < 1200; i++){
    footer[i] = '*';
  }
  
  /* Infinite loop */
  for(;;)
  {
    if(socketStatus == 0){
        memset(&servaddr, 0, sizeof(servaddr)); 
        memset(&cliaddr, 0, sizeof(cliaddr)); 
        
        // Filling server information 
        cliaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
        cliaddr.sin_family    = AF_INET; // IPv4 
        cliaddr.sin_port = htons(2030); 
        
        
        servaddr.sin_addr.s_addr = inet_addr("192.168.0.30");
        servaddr.sin_family    = AF_INET; // IPv4 
        servaddr.sin_port = htons(2020); 
        
        // Creating socket file descriptor 
        sockfd = socket(AF_INET, SOCK_DGRAM, 0); //****

      bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));
      socketStatus = 1;
    }
    number_of_bytes = recvfrom(sockfd, (char *)smsg, MAXLINE,  MSG_DONTWAIT, ( struct sockaddr *) &cliaddr,&len); 
    
    char temp[5]= "Send\0";
    char inp[5];
    memcpy(inp,smsg,4);
    inp[4] = '\0';
    sendReq = strcmp(temp,inp);
    if(sendReq == 0 && recBlock == 0){
      strcpy(update_status,"mode1");
      usart_process_data("1",1);  
      counter = 0;
      recBlock = 1;
      osDelay(10);
      sendReq = 1;
      smsg[0] = '\0';
      smsg[1] = '\0';
      smsg[2] = '\0';
      smsg[3] = '\0';
      smsg[0] = '\0';
    }
    else{
      sendReq = 0;
    }
    if(sendOutUdp == 1){
      char tempO[1200];
      //--------- Header -----------//
      sendto(sockfd, (const char *)header, 1200, 1, (const struct sockaddr *) &servaddr, len); 
      osDelay(10);
//      memset(tempO,'\0',1200);
//      osDelay(10);
      //----------- Data -----------//
      for(int i = 0; i < 4; i++){
        memcpy(tempO,outstr+(i*1200),1200);
        osDelay(50);
        sendto(sockfd, (const char *)tempO, 1200, 1, (const struct sockaddr *) &servaddr, len); 
        osDelay(10);
//        memset(tempO,'\0',1200);
//        osDelay(10);
      }
      //--------- Footer -----------//
//      memset(tempO,'*',1200);
//      osDelay(10);
      
      sendto(sockfd, (const char *)footer, 1200, 1, (const struct sockaddr *) &servaddr, len); 
      osDelay(10);
//      HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_RESET);
//      osDelay(300);
//      HAL_NVIC_SystemReset(); // Temperary
      
      usart_rx[0] = '\0';
      osDelay(10);
      recBlock = 0;
      sendReq = 1;
      readStatus = 0;
      close(sockfd);
      socketStatus = 0;
      sendOutUdp = 0;
      
      memset(usart_rx, '\0', sizeof(usart_rx));
      osDelay(100);
      memset(outstr, '\0', sizeof(outstr));
      osDelay(100);

    }
    //    buffer[number_of_bytes] = '\0'; 
    
    
    //number_of_bytes = 0;
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Freq_Cal_entry */
/**
* @brief Function implementing the Freq_Cal thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Freq_Cal_entry */
void Freq_Cal_entry(void *argument)
{
  /* USER CODE BEGIN Freq_Cal_entry */
  usart_process_data("3",1);
  memset(usart_rx, '\0', sizeof(usart_rx));
  osDelay(50);
  /* Infinite loop */
  for(;;)
  {
    if(sendOutUdp == 0){
      frequency_f[0] = frequency[1];
      frequency_f[1] = frequency[1];
      frequency_f[2] = frequency[2];
      frequency_f[3] = frequency[3];
      frequency_f[4] = frequency[4];
      frequency_f[5] = frequency[5];
      frequency_f[6] = frequency[6];
      frequency_f[7] = frequency[7];
      frequency_f[8] = frequency[8];
      frequency_f[9] = frequency[8];
      frequency_f[10] = frequency[10];
      frequency_f[11] = frequency[11];
      frequency_f[12] = frequency[12];
      frequency_f[13] = frequency[12];
      frequency_f[14] = frequency[14];
      frequency_f[15] = frequency[15];
      
      if(usart_rx[0] != '\0'){
        slave_recive = 1;
      }
      if(!HAL_GPIO_ReadPin(Input_Sensor1_GPIO_Port,Input_Sensor1_Pin)){
        sensorNumber = 1;
      }
      else if(!HAL_GPIO_ReadPin(Input_Sensor2_GPIO_Port,Input_Sensor2_Pin)){
        sensorNumber = 2;
      }
      else if(!HAL_GPIO_ReadPin(Input_Sensor3_GPIO_Port,Input_Sensor3_Pin)){
        sensorNumber = 3;
      }
      else if(!HAL_GPIO_ReadPin(Input_Sensor4_GPIO_Port,Input_Sensor4_Pin)){
        sensorNumber = 4;
      }
      
      if(!HAL_GPIO_ReadPin(SensorInput_GPIO_Port,SensorInput_Pin)){
        sensorStatus = 1;
      }
      else{
        sensorStatus = 0;
      }
      
      if(sensorStatus == 0){
        if(readStatus == 1){
          readStatus = 2;
          sensorStatus = 0;
        }
      }
      
      if(sensorStatus != 0 && !strcmp(update_status,"mode0")){
        readStatus = 1;
        strcpy(update_status,"mode1");
        usart_process_data("1",1);     
        osDelay(10);
        counter = 0;
      }
      
      if(!strcmp(update_status,"mode0")){
        for(int j = 0; j < 800; j++){
          out_data[j] = 0;
        }
      }
      
      if(readStatus == 1 || recBlock == 1){
        if(counter >= 100){
          counter = 0;
          
          for(int i = 0; i < 16; i++){
            //          uint16_t test[16] = {43000,10000,535,3486,10000,25000,40020,356,2546,15000,153,25,35000,15000,6500,700}; // Test Data
            //          capture_freq[readCount][i] = test[i];
            capture_freq[readCount][i] = frequency_f[i];
          }
          readCount++;
        }
        if(readCount == 100){
          uint8_t sampleRate;
          if(readCount > 25){
            sampleRate = readCount/25;
          }
          else{
            sampleRate = 1;
          }
          for(int i = 0; i < 25; i++){
            for(int j = 0; j < 16; j++){
              selected_data[i][j] = capture_freq[i*sampleRate][j];
            }
          }
          uint16_t count = 0;
          for(int i = 0; i < 25; i++){
            for(int j = 0; j < 16; j++){
              out_data[count++] = selected_data[i][j];
            }
          }
          strcpy(update_status, "mode2");
        }
      }
      if(readStatus == 2){
        usart_process_data("2",1);
        osDelay(10);
        readStatus = 0;
        strcpy(update_status, "mode2");
      }
      if(slave_recive == 1 && !strcmp(update_status,"mode2")){
        uint8_t sampleRate;
        if(readCount > 25){
          sampleRate = readCount/25;
        }
        else{
          sampleRate = 1;
        }
        for(int i = 0; i < 25; i++){
          for(int j = 0; j < 16; j++){
            selected_data[i][j] = capture_freq[i*sampleRate][j];
          }
        }
        uint16_t count = 0;
        for(int i = 0; i < 25; i++){
          for(int j = 0; j < 16; j++){
            out_data[count++] = selected_data[i][j];
          }
        }
        // initialize data here
        arrayToString(out_data,400,outstr);
        outstr[2399] = '$';
        usart_rx[299] = '$';
        for (int i = 0; i < 24; i++){
          memcpy(outstr+(2400+(i*100)),usart_rx+(i*100),100);
          osDelay(20);
        }
        outstr[4799] = '$';
        outstr[4800] = sensorNumber + '0';
        outstr[4801] = '\0';
        //      sensorNumber = 0;
        osDelay(20);
              
        strcpy(update_status,"mode0");
        readCount = 0;
        slave_recive = 0;
        readStatus = 0;
        recBlock = 0;
        
        sendOutUdp = 1;
        
        for(int i = 0; i < 25; i++){
          for(int j = 0; j < 16; j++){
            selected_data[i][j] = 0;
          }
        }
        uint16_t cnt = 0;
        for(int i = 0; i < 25; i++){
          for(int j = 0; j < 16; j++){
            out_data[cnt++] = 0;
          }
        }
        for(int i = 0; i < 100; i++){
          for(int j = 0; j < 16; j++){
            capture_freq[i][j] = 0;
          }
        }
        
        
        //      if(sendReq == 0){
        //        counter = 0;
        //        sendCounter++;
        //        usart_process_data("1",1);     
        //        osDelay(10);
        //        if(sendCounter == 6){
        //          sendCounter = 0;
        //          sendReq = 1;
        //          memset(smsg, '\0', sizeof(smsg));
        //        }
        //      }
      }
    }
    osDelay(1);
  }
  /* USER CODE END Freq_Cal_entry */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM5 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5) {
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
