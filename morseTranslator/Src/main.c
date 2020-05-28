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
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint8_t data[50];// Tablica przechowujaca wysylana wiadomosc.
uint16_t size = 0; // Rozmiar wysylanej wiadomosci ++cnt; // Zwiekszenie licznika wyslanych wiadomosci.
uint32_t buttonOnTime; //czas pocz¹tku wciœniêcia guzika
uint32_t buttonOffTime; //czas pocz¹tku wciœniêcia guzika
uint8_t character = 0b00000001; //odczytany znak
uint8_t toSendFlag = 0;
uint8_t spaceFlag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM11_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){
		if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)){
			buttonOnTime = HAL_GetTick();
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);


		}
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)){
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			if(HAL_GetTick()-buttonOnTime>300)	{

				character = (character<<1) + 1;
			}
			else	{
				character = character<<1;

			}
			buttonOffTime = HAL_GetTick();
			toSendFlag = 1;
			spaceFlag = 1;
		}
	}
}
void sendChar(uint8_t *str){
	size = sprintf(data,str);
	HAL_UART_Transmit_IT(&huart2, data, size);
}
uint16_t signalToChar(uint8_t ch){
		uint16_t charToSend;
		switch(ch){
	  	  case 0b00000101:
		  	  charToSend = "a";
		  break;
	  	  case 0b00011000:
		  	  charToSend = "b";
		  break;
	  	  case 0b00011010:
		  	  charToSend = "c";
		  break;
	  	  case 0b00001100:
		  	  charToSend = "d";
		  break;
	  	  case 0b00000010:
		  	  charToSend = "e";
		  break;
	  	  case 0b00010010:
		  	  charToSend = "f";
		  break;
	  	  case 0b00001110:
		  	  charToSend = "g";
		  break;
	  	  case 0b00010000:
		  	  charToSend = "h";
		  break;
	  	  case 0b00000100:
		  	  charToSend = "i";
		  break;
	  	  case 0b00010111:
		  	  charToSend = "j";
		  break;
	  	  case 0b00001101:
		  	  charToSend = "k";
		  break;
	  	  case 0b00010100:
		  	  charToSend = "l";
		  break;
	  	  case 0b00000111:
		  	  charToSend = "m";
		  break;
	  	  case 0b00000110:
		  	  charToSend = "n";
		  break;
	  	  case 0b00001111:
		  	  charToSend = "o";
		  break;
	  	  case 0b00010110:
		  	  charToSend = "p";
		  break;
	  	  case 0b00011101:
		  	  charToSend = "q";
		  break;
	  	  case 0b00001010:
		  	  charToSend = "r";
		  break;
	  	  case 0b00001000:
		  	  charToSend = "s";
		  break;
	  	  case 0b00000011:
		  	  charToSend = "t";
		  break;
	  	  case 0b00001001:
		  	  charToSend = "u";
		  break;
	  	  case 0b0010001:
		  	  charToSend = "v";
		  break;
	  	  case 0b00001011:
		  	  charToSend = "w";
		  break;
	  	  case 0b00011001:
		  	  charToSend = "x";
		  break;
	  	  case 0b00011011:
		  	  charToSend = "y";
		  break;
	  	  case 0b00011100:
		  	  charToSend = "z";
		  break;
	  	  case 0b00101111:
		  	  charToSend = "1";
		  break;
	  	  case 0b00100111:
		  	  charToSend = "2";
		  break;
	  	  case 0b00100011:
		  	  charToSend = "3";
		  break;
	  	  case 0b00100001:
		  	  charToSend = "4";
		  break;
	  	  case 0b00100000:
		  	  charToSend = "5";
		  break;
	  	  case 0b00110000:
		  	  charToSend = "6";
		  break;
	  	  case 0b00111000:
		  	  charToSend = "7";
		  break;
	  	  case 0b00111100:
		  	  charToSend = "8";
		  break;
	  	  case 0b00111110:
		  	  charToSend = "9";
		  break;
	  	  case 0b00111111:
		  	  charToSend = "0";
		  break;
	  	  case 0b01010101:
		  	  charToSend = ".";
		  break;
	  	  case 0b01110011:
		  	  charToSend = ",";
		  break;
	  	  case 0b01011110:
		  	  charToSend = "\'";
		  break;
	  	  case 0b01010010:
		  	  charToSend = "\"";
		  break;
	  	  case 0b01001101:
		  	  charToSend = "_";
		  break;
	  	  case 0b01111000:
		  	  charToSend = ":";
		  break;
	  	  case 0b01101010:
		  	  charToSend = ";";
		  break;
	  	  case 0b01001100:
		  	  charToSend = "?";
		  break;
	  	  case 0b01101011:
		  	  charToSend = "!";
		  break;
	  	  case 0b01100001:
		  	  charToSend = "-";
		  break;
	  	  case 0b00101010:
		  	  charToSend = "+";
		  break;
	  	  case 0b00110010:
		  	  charToSend = "/";
		  break;
	  	  case 0b00110110:
		  	  charToSend = "(";
		  break;
	  	  case 0b01101101:
		  	  charToSend = ")";
		  break;
	  	  case 0b00110001:
		  	  charToSend = "=";
		  break;
	  	  case 0b01011010:
		  	  charToSend = "@";
		  break;
	  	  default:
	  		  charToSend = "no such char\n";

	  }
	  return charToSend;
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
  MX_TIM11_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim11);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)){

		  if(HAL_GetTick()-buttonOffTime > 2000  & spaceFlag == 1){
			  sendChar(" ");
			  character = 0b00000001;
			  spaceFlag = 0;
		  }
		  else if(HAL_GetTick()-buttonOffTime > 300  & toSendFlag == 1){
			  sendChar(signalToChar(character));
			  character = 0b00000001;
			  toSendFlag = 0;
		  }
	  }

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 0;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 0;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
