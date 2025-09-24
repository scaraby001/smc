/**
  * @file   ms_time.h
  * @brief  Модуль генерации пользовательских прерываний 1 мс.
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MS_TIME_H_
#define __MS_TIME_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/**
  * @brief Структура модуля миллисекундных прерываний
  */
typedef struct{

	TIM_HandleTypeDef *	htim;	/*!<  TIM Time Base Handle Structure pointer		*/
	void(*isr)(void);			/*!<  Вектор обработки прерывания таймера			*/
	void(*start)(void);			/*!<  Запуск таймера								*/

}module_ms_time_t;

extern module_ms_time_t module_ms_time;		/*	Экземпляр модуля пользовательских прерываний 1 мс	*/

#endif /* __MS_TIME_H_ */
