/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

#include <float.h>
#include <math.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "EEPROM.h"
#include "http_ssi.h"
#include "ee24.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
GPIO_InitTypeDef gpio;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart3;

/* Definitions for modbusTaskTcp */
osThreadId_t modbusTaskTcpHandle;
const osThreadAttr_t modbusTaskTcp_attributes = {
  .name = "modbusTaskTcp",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 256 * 4
};
/* Definitions for sensorTask */
osThreadId_t sensorTaskHandle;
const osThreadAttr_t sensorTask_attributes = {
  .name = "sensorTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* USER CODE BEGIN PV */
float ADC_data[4];

float O2cvt = 0.56153;
float O2 = 0;
float CO2 = 0;
float CO = 0;
float Temperature = 0;
float readADC(uint8_t select);

uint8_t dataRead;
uint8_t dataWrite;

uint8_t ipAdr0,ipAdr1,ipAdr2,ipAdr3;
uint8_t subentM0,subentM1,subentM2,subentM3;
uint8_t getew0,getew1,getew2,getew3;

float resetCounter = 0;

uint8_t timer_flag = 0;
uint32_t timerValue = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI2_Init(void);
static void MX_UART5_Init(void);
void modebusTaskTcpFunc(void *argument);
void sensorTaskFunc(void *argument);

/* USER CODE BEGIN PFP */
void device_init();
void device_reset();
double convert_adc2temp(double voltage);
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
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_I2C3_Init();
  MX_SPI2_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
  //ee24_eraseChip();
  device_init();
  
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

  /* creation of sensorTask */
  sensorTaskHandle = osThreadNew(sensorTaskFunc, NULL, &sensorTask_attributes);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
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
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
  
  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
  
  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
  
  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 400000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

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

  /* USER CODE BEGIN USART3_Init 1 */
  
  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  
  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
float readADC(uint8_t select){
  #define ADS1115_ADDRESS 0x48
  unsigned char ADSwrite[6];
  int16_t reading;
  float voltage[4];
  const float voltageConv = 6.114 / 32768.0;
  
  for(int i=0; i< 4; i++){
      ADSwrite[0] = 0x01;
      
      switch(i){
              case(0):
                      ADSwrite[1] = 0xC1; //11000001
              break;
              case(1):
                      ADSwrite[1] = 0xD1; //11010001
              break;
              case(2):
                      ADSwrite[1] = 0xE1;
              break;
              case(3):
                      ADSwrite[1] = 0xF1;
              break;
      }
      
      ADSwrite[2] = 0x83; //10000011 LSB
      
      HAL_I2C_Master_Transmit(&hi2c1, ADS1115_ADDRESS << 1, ADSwrite, 3, 100);
      ADSwrite[0] = 0x00;
      HAL_I2C_Master_Transmit(&hi2c1, ADS1115_ADDRESS << 1 , ADSwrite, 1 ,100);
      //HAL_Delay(20);
      osDelay(20);
      
      HAL_I2C_Master_Receive(&hi2c1, ADS1115_ADDRESS <<1, ADSwrite, 2, 100);
      reading = (ADSwrite[0] << 8 | ADSwrite[1] );
      if(reading < 0) {
              reading = 0;
      }
      voltage[i] = reading * voltageConv; 
      //voltage[i] = (reading*3.3)/65.536;       
  }
  return voltage[select];
}


void device_init(){
  ee24_read(0, &dataRead, 1, 100);
    HAL_Delay(10);
  if(dataRead != 110){
    dataWrite = 110;
    ee24_write(0, &dataWrite, 1, 100);
    HAL_Delay(10);
    
    // IP address
    dataWrite = 192;
    ee24_write(3, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 168;
    ee24_write(4, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 1;
    ee24_write(5, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 92;
    ee24_write(6, &dataWrite, 1, 100);
    HAL_Delay(10);
    
    // Subnet mask
    dataWrite = 255;
    ee24_write(7, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 255;
    ee24_write(8, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 255;
    ee24_write(9, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 0;
    ee24_write(10, &dataWrite, 1, 100);
    HAL_Delay(10);
    
    // Geteway
    dataWrite = 192;
    ee24_write(11, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 168;
    ee24_write(12, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 1;
    ee24_write(13, &dataWrite, 1, 100);
    HAL_Delay(10);
    dataWrite = 1;
    ee24_write(14, &dataWrite, 1, 100);
    HAL_Delay(10);
  }
    ee24_read(0, &dataRead, 1, 100);
    HAL_Delay(10);
    ee24_read(3, &ipAdr0, 1, 100);
    HAL_Delay(10);
    ee24_read(4, &ipAdr1, 1, 100);
    HAL_Delay(10);
    ee24_read(5, &ipAdr2, 1, 100);
    HAL_Delay(10);
    ee24_read(6, &ipAdr3, 1, 100);
    HAL_Delay(10);
    
    ee24_read(7, &subentM0, 1, 100);
    HAL_Delay(10);
    ee24_read(8, &subentM1, 1, 100);
    HAL_Delay(10);
    ee24_read(9, &subentM2, 1, 100);
    HAL_Delay(10);
    ee24_read(10, &subentM3, 1, 100);
    HAL_Delay(10);
    
    ee24_read(11, &getew0, 1, 100);
    HAL_Delay(10);
    ee24_read(12, &getew1, 1, 100);
    HAL_Delay(10);
    ee24_read(13, &getew2, 1, 100);
    HAL_Delay(10);
    ee24_read(14, &getew3, 1, 100); 
    HAL_Delay(10);
}


void device_reset(){
    dataWrite = 110;
    ee24_write(0, &dataWrite, 1, 100);
    osDelay(10);
    
    // IP address
    dataWrite = 192;
    ee24_write(3, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 168;
    ee24_write(4, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 1;
    ee24_write(5, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 92;
    ee24_write(6, &dataWrite, 1, 100);
    osDelay(10);
    
    // Subnet mask
    dataWrite = 255;
    ee24_write(7, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 255;
    ee24_write(8, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 255;
    ee24_write(9, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 0;
    ee24_write(10, &dataWrite, 1, 100);
    osDelay(10);
    
    // Geteway
    dataWrite = 192;
    ee24_write(11, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 168;
    ee24_write(12, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 1;
    ee24_write(13, &dataWrite, 1, 100);
    osDelay(10);
    dataWrite = 1;
    ee24_write(14, &dataWrite, 1, 100);
    osDelay(10);
    
    HAL_NVIC_SystemReset();
}

double convert_adc2temp(double voltage){
  
  double resistance = 0 ;
  double temp = 0;
  double Aa = 0.00076021447 ;
  double Bb = 0.00027381013 ;
  double Cc = 0.00000009274426173 ;
  
  resistance = 10000 * (1/((3.3/voltage)-1)) ;
  //resistance = 10000 * (1/((3.3/channel_voltage)-1)) ;       //for 2.2 kilo ohm resistor
  //temp = 1/(Aa+(Bb*(log(resistance)))+(Cc*(log((pow(resistance,3)))))) ;
  temp = 1/(Aa+(Bb*(log(resistance)))+(Cc*((pow(log(resistance),3))))) ;
  temp = temp - 273.15 ;
  //return round(temp);
  return temp;
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
     // mu_Init();
    //tcp_Init();
  extern struct netif gnetif;
  http_server_init();
  /* Infinite loop */
  for(;;)
  {
    //uncomment for modbus tcp slave
    //ethernetif_input(&gnetif);
    sys_check_timeouts();
          
    mu_Init();
    tcp_Init();
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_sensorTaskFunc */
/**
* @brief Function implementing the sensorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sensorTaskFunc */
void sensorTaskFunc(void *argument)
{
  /* USER CODE BEGIN sensorTaskFunc */
  /* Infinite loop */
  
  for(;;)
  {
    for(int i = 0; i < 4; i++){
      ADC_data[i] = readADC(i);
    }
    O2 = (ADC_data[0] * 100) / O2cvt;
    CO2 = ADC_data[3] * 482;
    CO = ADC_data[1]/(1001000);
//    CO = CO * 500;
    CO = CO * 500000000;
    Temperature = convert_adc2temp((double)ADC_data[2]);
    
    g_sHoldingRegsBuf[1] = (int)O2;
    g_sHoldingRegsBuf[2] = (int)CO2;
    g_sHoldingRegsBuf[3] = (int)CO;
    g_sHoldingRegsBuf[4] = (int)Temperature;
    
    char temp[17];
    int c= snprintf(temp, sizeof(temp),"%03d,%03d,%03d,%03d\n", (int)O2, (int)CO2, (int)CO, (int)Temperature);
    
    HAL_UART_Transmit(&huart3,(uint8_t *)temp,sizeof(temp),HAL_MAX_DELAY);
    //HAL_Delay(100);
    osDelay(100);
    
    osDelay(1);
    
  }
  /* USER CODE END sensorTaskFunc */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
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
