/**
  * @file   interrupt.с
  * @brief  Модуль настройки обработчиков прерываний
  * 		Не нужно вносить изменения в файл stm32f4xx_it.c!
  * 		Все необходимые __weak функции находятся в этом файле
  */

/* Includes ------------------------------------------------------------------*/
#include "modules.h"


/* Functions -----------------------------------------------------------------*/

/**
  * @brief  PWM Pulse finished callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == arm.indicator->timer.htim)
	{
		arm.indicator->timer.isr_pulse();
	}
}


/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == arm.ms_time->htim)
	{
		arm.keyboard->isr();
		arm.leds->isr();
		arm.sound->isr();
	}

	if(htim == arm.indicator->timer.htim)
	{
		arm.indicator->timer.isr_period();
	}
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == SQW_Pin)
	{
		arm.rtc->isr();
	}
}
