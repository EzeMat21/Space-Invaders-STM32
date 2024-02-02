/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include "invaders.h"
#include "task.h"
#include "memoria.h"

#include "tonos.h"
#include "menu.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct{

	uint8_t x_value;
	uint8_t y_value;
	uint8_t boton;

}botones_t;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//------------------------------------------VARIABLES - MUSICA PARA EL GAME OVER, DISPARO Y EXPLOSIONES  ------------------------------------

musica_t musica;

volatile uint8_t *puntero_musica;
uint8_t *puntero_musica_final_f622 = tono_622hz + TAMANO_BUFF_TONO_622;
uint8_t *puntero_musica_final_f587 = tono_587hz + TAMANO_BUFF_TONO_587;
uint8_t *puntero_musica_final_f554 = tono_554hz + TAMANO_BUFF_TONO_554;
uint8_t *puntero_musica_final_tonos = tonos_variables + TAMANO_BUFF_TONOS_VARIABLES;
uint8_t *puntero_final_disparo = audio_disparo + TAMANO_BUFF_AUDIO_DISPARO;
uint8_t *puntero_final_explosion = audio_explosion + TAMANO_BUFF_AUDIO_EXPLOSION;

volatile tonos_t tonos;
volatile uint8_t conteo_musica;


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* Definitions for JoystickTask */
osThreadId_t JoystickTaskHandle;
const osThreadAttr_t JoystickTask_attributes = {
  .name = "JoystickTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for PantallaTask */
osThreadId_t PantallaTaskHandle;
const osThreadAttr_t PantallaTask_attributes = {
  .name = "PantallaTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MemoriaTask */
osThreadId_t MemoriaTaskHandle;
const osThreadAttr_t MemoriaTask_attributes = {
  .name = "MemoriaTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for SonidoTask */
osThreadId_t SonidoTaskHandle;
const osThreadAttr_t SonidoTask_attributes = {
  .name = "SonidoTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for queueJoystPant */
osMessageQueueId_t queueJoystPantHandle;
const osMessageQueueAttr_t queueJoystPant_attributes = {
  .name = "queueJoystPant"
};
/* Definitions for queueSonidoMenu */
osMessageQueueId_t queueSonidoMenuHandle;
const osMessageQueueAttr_t queueSonidoMenu_attributes = {
  .name = "queueSonidoMenu"
};
/* Definitions for myMutexPuntaje */
osMutexId_t myMutexPuntajeHandle;
const osMutexAttr_t myMutexPuntaje_attributes = {
  .name = "myMutexPuntaje"
};
/* Definitions for mySem01 */
osSemaphoreId_t mySem01Handle;
const osSemaphoreAttr_t mySem01_attributes = {
  .name = "mySem01"
};
/* USER CODE BEGIN PV */

//Notificacion entre MemoriaTask y PantallaTask
osEventFlagsId_t notificationFlag;
osEventFlagsId_t notificationFlag2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
void entryJoystick(void *argument);
void entryPantalla(void *argument);
void entryMemoria(void *argument);
void entrySonido(void *argument);

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
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of myMutexPuntaje */
  myMutexPuntajeHandle = osMutexNew(&myMutexPuntaje_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of mySem01 */
  mySem01Handle = osSemaphoreNew(1, 1, &mySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of queueJoystPant */
  queueJoystPantHandle = osMessageQueueNew (10, sizeof(botones_t), &queueJoystPant_attributes);

  /* creation of queueSonidoMenu */
  queueSonidoMenuHandle = osMessageQueueNew (10, sizeof(musica_t), &queueSonidoMenu_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of JoystickTask */
  JoystickTaskHandle = osThreadNew(entryJoystick, NULL, &JoystickTask_attributes);

  /* creation of PantallaTask */
  PantallaTaskHandle = osThreadNew(entryPantalla, NULL, &PantallaTask_attributes);

  /* creation of MemoriaTask */
  MemoriaTaskHandle = osThreadNew(entryMemoria, NULL, &MemoriaTask_attributes);

  /* creation of SonidoTask */
  SonidoTaskHandle = osThreadNew(entrySonido, NULL, &SonidoTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  //Creacion de la cola notificacion.
  notificationFlag = osEventFlagsNew(NULL);
  notificationFlag2 = osEventFlagsNew(NULL);

  /* USER CODE END RTOS_EVENTS */

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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
  hadc1.Init.ContinuousConvMode = DISABLE;
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
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

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
  hi2c1.Init.ClockSpeed = 400000;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 255-1;
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
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

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

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 125-1;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/* USER CODE BEGIN Header_entryJoystick */
/**
  * @brief  Function implementing the JoystickTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_entryJoystick */
void entryJoystick(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */

	//vTaskSetApplicationTaskTag( NULL, ( void * ) 0 );

	botones_t joystick;
	uint16_t val_x, val_y;

	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

  for(;;)
  {

	  //Joystick eje X: derecha o izquierda.
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  val_x = HAL_ADC_GetValue(&hadc1);
	  HAL_ADC_Stop(&hadc1);

	  //Joystick eje Y: arriba o abajo.
	  HAL_ADC_Start(&hadc2);
	  HAL_ADC_PollForConversion(&hadc2, 10);
	  val_y = HAL_ADC_GetValue(&hadc2);
	  HAL_ADC_Stop(&hadc2);

	  if(val_x > 2200){
		  joystick.x_value = derecha;
	  }
	  else if(val_x < 1600){
		  joystick.x_value = izquierda;
	  }
	  else{
		  joystick.x_value = nulo;
	  }


	  if(val_y > 2200){
		  joystick.y_value = abajo;
	  }
	  else if(val_y < 1600){
		  joystick.y_value = arriba;
	  }
	  else{
		  joystick.y_value = nulo;
	  }

	  //Boton del Joystick.

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET){
		  joystick.boton = true;
	  }
	  else{
		  joystick.boton = false;
	  }


	osStatus_t res = osMessageQueuePut(queueJoystPantHandle, &joystick, 0, 0);
	if(res != osOK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_entryPantalla */
/**
* @brief Function implementing the PantallaTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_entryPantalla */
void entryPantalla(void *argument)
{
  /* USER CODE BEGIN entryPantalla */
  /* Infinite loop */


	//vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );

	//Se inicializan los botones (eje y, eje x del joystick y boton)
	botones_t joystick;
	menuInit();


  for(;;)
  {
	osStatus_t res = osMessageQueueGet(queueJoystPantHandle, &joystick, 0 , osWaitForever);	//Se espera a recibir los valores de los botones del joystick

	if(res != osOK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);	//Si no se recibio correctamente, prender led.

	else{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);	//Si se recibio correctamente de la cola.


		menuActualizar(joystick.x_value, joystick.y_value, joystick.boton);
		actualizarPantalla();

	}

  }
  /* USER CODE END entryPantalla */
}

/* USER CODE BEGIN Header_entryMemoria */
/**
* @brief Function implementing the MemoriaTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_entryMemoria */
void entryMemoria(void *argument)
{
  /* USER CODE BEGIN entryMemoria */

	//vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );

	uint8_t permutaciones;

	//Se leen los datos desde la memoria.
	memoriaInit();

  /* Infinite loop */
  for(;;)
  {
	  //Espero la notificacion 1 desde la tarea Pantalla/Menu (desde el menu de guardado de nombre) para sincronizar el ordenamiento y guardado del nuevo puntaje.
	  uint32_t flags = osEventFlagsWait(notificationFlag, NOTIFICATION_VALUE, osFlagsWaitAny, osWaitForever);

	      // Realiza acciones basadas en la notificación recibida
	      if (flags == NOTIFICATION_VALUE)
	      {

				permutaciones = Ordenamiento_Puntajes();
				//writeNuevosPuntajes(permutaciones);

	    	  //Envio la notificacion 2 para que la tarea PantallaTask pueda pasar del menu guardado_nombre al menu de puntajes una vez que los puntajes ya se encuentran ordenadas y guardadas, ya que sin esta segunda sincronizacion, puede pasarse al menu puntajes sin que estos se encuentren ordenados. El ordenado se realiza en esta tarea MemoriaTask ya que las escrituras de puntajes se realizan solo en esta tarea.
	    	  osEventFlagsSet(notificationFlag2, NOTIFICATION_VALUE2);
	      }

  }
  /* USER CODE END entryMemoria */
}

/* USER CODE BEGIN Header_entrySonido */
/**
* @brief Function implementing the SonidoTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_entrySonido */
void entrySonido(void *argument)
{
  /* USER CODE BEGIN entrySonido */

  /* Infinite loop */
  for(;;)
  {
	  osStatus_t res = osMessageQueueGet(queueSonidoMenuHandle, &musica, 0, osWaitForever);	//Se espera a recibir los valores de los botones del joystick

	  if(res != osOK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);	//Si no se recibio correctamente, prender led.

	  else{
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
			HAL_TIM_Base_Stop_IT(&htim3);

			//musica = musica_;

		  switch(musica){
		  case gameover_:

				tonos = f622;
				conteo_musica = VECES_622HZ;
				puntero_musica = tono_622hz;

			  break;
		  case disparo_:
			  puntero_musica = audio_disparo;

			  break;
		  case explosion_:
			  puntero_musica = audio_explosion;
			  break;
		  default:
			  break;

		  }

		  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		  HAL_TIM_Base_Start_IT(&htim3);

	  }

  }
  /* USER CODE END entrySonido */
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


  if (htim->Instance == TIM3) {

		TIM2->CCR1 = *puntero_musica >> 1;
		puntero_musica++;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);

		switch(musica){

					 case gameover_:
						 //Musica del Game Over
								switch(tonos){
								case f622:
									if(puntero_musica == puntero_musica_final_f622){
											puntero_musica = tono_622hz;
											conteo_musica--;
									}
									break;
								case f587:
									if(puntero_musica == puntero_musica_final_f587){
											puntero_musica = tono_587hz;
											conteo_musica--;
									}
									break;
								case f554:
									if(puntero_musica == puntero_musica_final_f554){
											puntero_musica = tono_554hz;
											conteo_musica--;
									}
									break;
								default:	//Tonos variables
									if(puntero_musica == puntero_musica_final_tonos){
											puntero_musica = tonos_variables;
											conteo_musica--;
									}
									break;
								}

									if(conteo_musica == 0){

										switch(tonos){
										case f622:
											puntero_musica = tono_587hz;
											tonos = f587;
											conteo_musica = VECES_587HZ;
											break;
										case f587:
											puntero_musica = tono_554hz;
											tonos = f554;
											conteo_musica = VECES_554HZ;
											break;
										case f554:
											puntero_musica = tonos_variables;
											tonos = variables;
											conteo_musica = VECES_TONOS_VARIABLES;
											break;
										case variables:	//Tonos variables
											HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
											HAL_TIM_Base_Stop_IT(&htim3);

											break;
										default:
											break;
									}
								}
					 break;
					 case disparo_:

						 if(puntero_musica == puntero_final_disparo){
								HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
								HAL_TIM_Base_Stop_IT(&htim3);
						 }

						 break;
					 case explosion_:

						 if(puntero_musica == puntero_final_explosion){
								HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
								HAL_TIM_Base_Stop_IT(&htim3);
						 }
						 break;
					 default:
						 break;
				 }
  }

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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
