/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
 MORSE CODE TRANSLATOR
 Basically communication is based on dot (short signal) and dash (long signal).
 Every character has its own code, composed of these two elements e.g:
 'a' = .-
 'b' = -...
 In my program I used binary system to represent it. Dot is 0 and dash is 1.
 The longest char is composed of 6 dots or dashes, so I decided to use uint8_t
 to describe it. Begin of dots and dashes train is marked by 1 e.g
 'a' = 0b00000101  ->first 1 is begin mark and 01 is right code
 'b' = 0b00011000  ->same situation
 This is in case of reading message from button.

 In case of transmitting message received from UART, the order of the bits
 has been reversed e.g
 'a' = 0b00000110 -> also 1 is begin mark, but this time, code is reversed
 'b' = 0b00010001 -> same
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_RECEIVE_BUFFER_SIZE (uint8_t) 50
#define DOT_DURATION 300
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */


uint32_t buttonOnTime; //time when the button was pressed
uint32_t buttonOffTime; //time when the button was released
uint8_t character = 0b00000001; //char to send
uint8_t toSendFlag = 0; //if every bits in char are set, flag 1
uint8_t spaceFlag = 0; //when end of word

uint8_t receiveByte; //char receive from UART
uint8_t receiveMessage[MAX_RECEIVE_BUFFER_SIZE]; //message receive from UART
uint8_t messageLength;
uint8_t FLAG_messageToTransmit = 0; //messege ready to transmit flag
uint8_t LCDcounter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM11_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){
		if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)){
			buttonOnTime = HAL_GetTick(); // get time of start pushing button
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET); //set LED in high state  to facilitate broadcasting


		}
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)){
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET); //same as in set LED case

			//if the button is pressed for more than 300 ms then its DASH
			if(HAL_GetTick()-buttonOnTime>300)	{

				character = (character<<1) + 1; //
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	static uint8_t receive_buffer[MAX_RECEIVE_BUFFER_SIZE] = "";
	static uint16_t counter = 0;
	static uint8_t FLAG_needsCleaning = 0;


	if (1 == FLAG_needsCleaning)
		{
			// buffer reset, set all on '\0'
			memset(receive_buffer,'\0',(MAX_RECEIVE_BUFFER_SIZE*sizeof(receive_buffer[0])));
			FLAG_needsCleaning = 0;
		}


	if (('\r' != receiveByte))
		{
			if (MAX_RECEIVE_BUFFER_SIZE >= counter)
			{
				receive_buffer[counter] = receiveByte;
			}
			else
			{
				// if buffer is overflow
				uint16_t size = 0;
				uint8_t data[MAX_RECEIVE_BUFFER_SIZE];

				size = sprintf(data,"\nBuffer overflow. Resteting buffor\n");
				HAL_UART_Transmit_IT(&huart2, data, size);

				memset(receive_buffer,'\0',(MAX_RECEIVE_BUFFER_SIZE*sizeof(receive_buffer[0])));
				counter = 0;
			}

			++counter;
		}
		else
		{

			messageLength = counter;
			counter = 0;
			FLAG_needsCleaning = 1;
			FLAG_messageToTransmit = 1;
			memcpy(receiveMessage, receive_buffer,MAX_RECEIVE_BUFFER_SIZE*sizeof(receive_buffer[0]));


		}

		HAL_UART_Receive_IT(&huart2, &receiveByte, 1u);
}

void sendChar(uint8_t*);
uint16_t signalToChar(uint8_t);
uint8_t toBinaryForm(uint8_t);
uint8_t charEnd(uint8_t);
void charTransmission(uint8_t);
void transmissionByLED(uint8_t *,uint8_t);

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
  MX_I2C1_Init();
  lcd_init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim11);
  HAL_UART_Receive_IT(&huart2, &receiveByte, 1u);
  /* USER CODE END 2 */
  lcd_send_string("Morse Translator");
  HAL_Delay(1000);
  lcd_clear();
  lcd_put_cur(0,0);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)){

	 		  if(HAL_GetTick()-buttonOffTime > DOT_DURATION*9  & spaceFlag == 1){
	 			  sendChar(" ");
	 			  character = 0b00000001;
	 			  spaceFlag = 0;
	 		  }
	 		  else if(HAL_GetTick()-buttonOffTime > DOT_DURATION*2  & toSendFlag == 1){
	 			  sendChar(signalToChar(character));
	 			  character = 0b00000001;
	 			  toSendFlag = 0;
	 		  }
	 	  }

	 	  if(FLAG_messageToTransmit==1){
	 	 		  transmissionByLED(receiveMessage,messageLength);

	 	 		  FLAG_messageToTransmit = 0;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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

void sendChar(uint8_t *str){
	uint8_t data[50];//
	uint16_t size = 0; //

	size = sprintf(data,str);
	HAL_UART_Transmit_IT(&huart2, data, size);
	LCDcounter ++;

	if(LCDcounter==17)	lcd_put_cur(1,0);
	else if(LCDcounter>32){
		lcd_clear();
		lcd_put_cur(0,0);
		LCDcounter = 0;
	}
	lcd_send_string(str);
}

uint8_t toBinaryForm(uint8_t ch){
	switch(ch){
			case 'a':
				return 0b00000110;
			break;
			case 'b':
				return 0b00010001;
			break;
			case 'c':
				return 0b00010101;
			break;
			case 'd':
				return 0b00001001;
			break;
			case 'e':
				return 0b00000010;
			break;
			case 'f':
				return 0b00010100;
			break;
			case 'g':
				return 0b00001011;
			break;
			case 'h':
				return 0b00010000;
			break;
			case 'i':
				return 0b00000100;
			break;
			case 'j':
				return 0b00011110;
			break;
			case 'k':
				return 0b00001101;
			break;
			case 'l':
				return 0b00010010;
			break;
			case 'm':
				return 0b00000111;
			break;
			case 'n':
				return 0b00000101;
			break;
			case 'o':
				return 0b00001111;
			break;
			case 'p':
				return 0b00010110;
			break;
			case 'q':
				return 0b00011011;
			break;
			case 'r':
				return 0b00001010;
			break;
			case 's':
				return 0b00001000;
			break;
			case 't':
				return 0b00000011;
			break;
			case 'u':
				return 0b00001100;
			break;
			case 'v':
				return 0b00010001;
			break;
			case 'w':
				return 0b00001110;
			break;
			case 'x':
				return 0b00011001;
			break;
			case 'y':
				return 0b00011011;
			break;
			case 'z':
				return 0b00010011;
			break;
			case '9':
				return 0b00111110;
			break;
			case '8':
				return 0b00111100;
			break;
			case '7':
				return 0b00111000;
			break;
			case '6':
				return 0b00110000;
			break;
			case '5':
				return 0b00100000;
			break;
			case '4':
				return 0b00110000;
			break;
			case '3':
				return 0b00111000;
			break;
			case '2':
				return 0b00111100;
			break;
			case '1':
				return 0b00111110;
			break;
			case '0':
				return 0b00111111;
			break;
			case '.':
				return 0b01101010;
			break;
			case ',':
				return 0b01110011;
			break;
			case '\'':
				return 0b01010010;
			break;
			case '\"':
				return 0b01010010;
			break;
			case '_':
				return 0b01101100;
			break;
			case ':':
				return 0b01000111;
			break;
			case ';':
				return 0b01010101;
			break;
			case '?':
				return 0b01001100;
			break;
			case '!':
				return 0b01110101;
			break;
			case '-':
				return 0b01100001;
			break;
			case '+':
				return 0b00101010;
			break;
			case '/':
				return 0b00101001;
			break;

			case '(':
				return 0b00101101;
			break;
			case ')':
				return 0b01101101;
			break;
			case '=':
				return 0b00110001;
			break;
			case '@':
				return 0b01010110;
			break;
			default:
				return 0b00000001;
			break;
	}
}

uint8_t charEnd(uint8_t cha){
    uint8_t isFinished = 0;
    if(cha&1){
        for(uint8_t i=1;i<8;i++)
            isFinished += cha&(1<<i);

        if(isFinished == 0) return 1;
        else return 0;
    }
    return 0;
}

void charTransmission(uint8_t ch){

	for(uint8_t i=0;i<8;i++){
	        if(!charEnd(ch>>i)){

	        	if(ch&(1<<i)){ // DASH transmission, duration 4 times longer than DOT
	        		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	        		HAL_Delay(DOT_DURATION*4);
	        		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	        	}
	        	else{
	        		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	        		HAL_Delay(DOT_DURATION);
	        		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	        	}

	        }
	        else break;
	        HAL_Delay(DOT_DURATION); //delay between impluses is one DOT
	    }

}

void transmissionByLED(uint8_t *toSend,uint8_t len){

	HAL_UART_Transmit_IT(&huart2, toSend, len*sizeof(toSend[0]));
	for(uint8_t j=0;j<len;j++){
		if(toSend[j]==' ')	HAL_Delay(DOT_DURATION*7); //space it is 7 DOT
		else{
			charTransmission(toBinaryForm(toSend[j]));
			HAL_Delay(DOT_DURATION*4); //delay between chars is one DASH, so 4 DOT
		}

	}
}
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
