/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "rng.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stdbool.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHOW_TIME					600
#define PAUSE_TIME					150

#define SHORT_ANIMATION_TIME		200
#define LONG_ANIMATION_TIME			500
#define EXTRA_LONG_ANIMATION_TIME	1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch);

bool is_button_pressed(char button_char);

bool are_buttons_released(void);

void set_led_state(char led_char, bool gpio_state);

void show_current_round_sequence(uint8_t round_iter, char* this_round_sequence);

void show_led_response(char led_button_char);

void game_start_animation(void);

void game_over_animation(void);

char get_random_led_character(void);

void wait_for_user_input(void);

void game_over_message(void);

void game_over_loop(void);
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
  MX_USART2_UART_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
  HAL_RNG_Init(&hrng);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("Let's play!\n");
  game_start_animation();

  uint8_t round_iteration = 0;
  char sequence[100];

  while (1)
  {
	  printf("Round: %d\n", round_iteration + 1);

	  sequence[round_iteration] = get_random_led_character();

	  show_current_round_sequence(round_iteration, sequence);

	  // Get and check user input
	  uint8_t pressed_button_iterator = 0;
	  while (pressed_button_iterator <= round_iteration) {
		  wait_for_user_input();

		  char current_rgby_char = sequence[pressed_button_iterator];

		  if (is_button_pressed(current_rgby_char)) {
			  show_led_response(current_rgby_char);
		  } else {
			  game_over_message();
			  game_over_loop();
		  }

		  pressed_button_iterator++;
		  HAL_Delay(300);
	  }

	  printf("Correct sequence!\n");

	  round_iteration++;
	  HAL_Delay(1000);
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch)
{
  if (ch == '\n') {
    __io_putchar('\r');
  }

  HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

  return 1;
}

bool is_button_pressed(char button_char)
{
	switch (button_char) {
	case 'R':
		if (HAL_GPIO_ReadPin(BUTTON_RED_GPIO_Port, BUTTON_RED_Pin) == GPIO_PIN_RESET)
			return true;
		else
			return false;
		break;
	case 'G':
		if (HAL_GPIO_ReadPin(BUTTON_GREEN_GPIO_Port, BUTTON_GREEN_Pin) == GPIO_PIN_RESET)
			return true;
		else
			return false;
		break;
	case 'B':
		if (HAL_GPIO_ReadPin(BUTTON_BLUE_GPIO_Port, BUTTON_BLUE_Pin) == GPIO_PIN_RESET)
			return true;
		else
			return false;
		break;
	case 'Y':
		if (HAL_GPIO_ReadPin(BUTTON_YELLOW_GPIO_Port, BUTTON_YELLOW_Pin) == GPIO_PIN_RESET)
			return true;
		else
			return false;
		break;
	default:
		printf("Error: given incorrect button char.\n");
		Error_Handler();
		break;
	}
}

bool are_buttons_released(void)
{
	if (is_button_pressed('R') == false && is_button_pressed('G') == false && is_button_pressed('B') == false && is_button_pressed('Y') == false)
		return true;
	else
		return false;
}

void set_led_state(char led_char, bool gpio_state)
{
	switch (led_char) {
	case 'R':
		if (gpio_state == 1)
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		break;
	case 'G':
		if (gpio_state == 1)
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		break;
	case 'B':
		if (gpio_state == 1)
			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
		break;
	case 'Y':
		if (gpio_state == 1)
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
		break;
	default:
		printf("Error: given incorrect led char.\n");
		Error_Handler();
		break;
	}
}

void show_current_round_sequence(uint8_t round_iter, char* this_round_sequence)
{
	for (int i = 0; i <= round_iter; i++) {
		set_led_state(*(this_round_sequence + i), 1);
		HAL_Delay(SHOW_TIME);
		set_led_state(*(this_round_sequence + i), 0);
		HAL_Delay(PAUSE_TIME);
	}
}

void show_led_response(char led_button_char)
{
	set_led_state(led_button_char, 1);

	while (is_button_pressed(led_button_char)) {
		;
	}
	set_led_state(led_button_char, 0);
}

void game_start_animation(void)
{
	set_led_state('R', 1);
	set_led_state('G', 1);
	set_led_state('B', 1);
	set_led_state('Y', 1);
	HAL_Delay(EXTRA_LONG_ANIMATION_TIME);
	set_led_state('R', 0);
	set_led_state('G', 0);
	set_led_state('B', 0);
	set_led_state('Y', 0);
	HAL_Delay(EXTRA_LONG_ANIMATION_TIME);
}

void game_over_animation(void)
{
	set_led_state('R', 0);
	set_led_state('Y', 1);
	HAL_Delay(LONG_ANIMATION_TIME);
	set_led_state('Y', 0);
	set_led_state('B', 1);
	HAL_Delay(LONG_ANIMATION_TIME);
	set_led_state('B', 0);
	set_led_state('G', 1);
	HAL_Delay(LONG_ANIMATION_TIME);
	set_led_state('G', 0);
	set_led_state('R', 1);
	HAL_Delay(LONG_ANIMATION_TIME);
}

char get_random_led_character(void)
{
	char output_character;
	uint8_t random_number = HAL_RNG_GetRandomNumber(&hrng) % 4;

	switch (random_number) {
	case 0:
		output_character = 'R';
		break;
	case 1:
		output_character = 'G';
		break;
	case 2:
		output_character = 'B';
		break;
	case 3:
		output_character = 'Y';
	break;
	default:
		printf("Error: given incorrect random number.\n");
		Error_Handler();
		break;
	}

	return output_character;
}

void wait_for_user_input(void)
{
	while (are_buttons_released()) {
		;
	}
}

void game_over_message(void)
{
	printf("Game Over\n");
	printf("Press RESET button to start again.\n");
}

void game_over_loop(void)
{
	while (1) {
		game_over_animation();
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
