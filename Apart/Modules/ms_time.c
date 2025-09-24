/**
  * @file   ms_time.c
  * @brief  Модуль генерации пользовательских прерываний 1 мс.
  * 		Таймер настраивается на генерацию прерываний по окончанию периода
  *			Обработка прерываний настраивается в отдельном модуле interrupt.c
  */


/* Includes ------------------------------------------------------------------*/
#include "ms_time.h"


/* Variables -----------------------------------------------------------------*/
extern TIM_HandleTypeDef htim11;
#define MILLISECONDS_TIMER			&htim11


/* Functions -----------------------------------------------------------------*/
void isr_ms(void);
void timer_interrupt_start(void)
{
	HAL_TIM_Base_Start_IT(MILLISECONDS_TIMER);
}


/**
  * @brief  Модуль лиллисекундных прерываний
  * 		Элементам структуры назначаются адреса переменных и функций
  */
module_ms_time_t module_ms_time = {

	.htim				= MILLISECONDS_TIMER,
	.isr = isr_ms,
	.start = timer_interrupt_start,
};


/**
  * @brief  Пользовательский обработчик прерываний миллисекундного таймера
  * 		Зарезервировано для потенциального применения
  */
void isr_ms(void)
{

}
