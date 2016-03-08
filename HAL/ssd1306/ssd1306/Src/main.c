/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
	uint8_t res = SSD1306_Init();
	printf("OLED init: %d\n", res);
	SSD1306_GotoXY(10,27);
	SSD1306_Puts("OLED inited", &Font_7x10, 1);
	SSD1306_GotoXY(10,52);
	SSD1306_Puts("Lutsai Alexander", &Font_7x10, 1);
	SSD1306_UpdateScreen(); //display
	HAL_Delay(1000);

	SSD1306_Fill(0); //clear oled
	int y1 = 64, y2 = 0;
	while(y1 > 0)
	{
		SSD1306_DrawLine(0, y1, 128, y2, 1);
		SSD1306_UpdateScreen();
		y1 -= 2;
		y2 += 2;
	}
	HAL_Delay(1000);
	SSD1306_Fill(1); //clear oled
	SSD1306_UpdateScreen();
	SSD1306_DrawCircle(64, 32, 25, 0);
	SSD1306_UpdateScreen();
	SSD1306_DrawCircle(128, 32, 25, 0);
	SSD1306_UpdateScreen();
	SSD1306_DrawCircle(0, 32, 25, 0);
	SSD1306_UpdateScreen();
	SSD1306_DrawCircle(32, 32, 25, 0);
	SSD1306_UpdateScreen();
	SSD1306_DrawCircle(96, 32, 25, 0);
	SSD1306_UpdateScreen();
	HAL_Delay(1000);
	
	SSD1306_Fill(0); //clear oled
	SSD1306_UpdateScreen();
	int32_t i = -100;
	char buf[10];
	while(i<=100)
	{
		memset(&buf[0], 0, sizeof(buf));
		sprintf(buf, "%d", i);
		SSD1306_GotoXY(50,27);
		SSD1306_Puts(buf, &Font_7x10, 1);
		SSD1306_DrawLine(64, 10, (i+100)*128/200, (i+100)*64/200, 1);
		SSD1306_UpdateScreen();
		SSD1306_Fill(0); //clear oled
		i++;
	}
	SSD1306_GotoXY(50,27);
	sprintf(buf, "END");
	SSD1306_Puts(buf, &Font_7x10, 1);
	SSD1306_GotoXY(10,52);
	SSD1306_Puts("Lutsai Alexander", &Font_7x10, 1);
	SSD1306_UpdateScreen();
	SSD1306_Fill(0); //clear oled
	HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint32_t lst = 0, cu;
  while (1)
  {
  /* USER CODE END WHILE */
		cu = HAL_GetTick();
		SSD1306_GotoXY(30,27);
		sprintf(buf, "fps: %f", 1000.0 / (double)(cu - lst));
		SSD1306_Puts(buf, &Font_7x10, 1);
		SSD1306_GotoXY(7,52);
		SSD1306_Puts("Lutsai Alexander", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		SSD1306_Fill(0); //clear oled
		lst = cu;
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
