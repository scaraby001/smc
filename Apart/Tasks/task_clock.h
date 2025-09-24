/**
  * @file   task_clock.h
  * @brief  Процесс отображения часов и календаря
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASK_CLOCK_H_
#define __TASK_CLOCK_H_


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**
 * Состояния процесса
 */
enum{
	TASK_CLOCK_IDLE,
	TASK_CLOCK_START,
	
	TASK_CLOCK_HOURS,
	TASK_CLOCK_DATE,
	TASK_CLOCK_DAY,
	TASK_CLOCK_YEAR,



	TASK_CLOCK_STOP = 255
};

/**
 * @brief	Структура управления процессом
 * 			Содержит состояние процесса, указатель на функцию выполнения процесса
 * 			и локальные переменные процесса
 */
typedef struct{

	uint8_t state;								/*!< Состояние процесса												*/
	void(*execute)(void);						/*!< Функция однократного выполнения процесса						*/
	
	uint32_t timer;								/*!< Универсальный общий таймер пребывания процесса в состоянии		*/
	uint32_t timer_point;						/*!< Таймер для отображения мигающих секунд на часах				*/
	uint8_t point_state;						/*!< Состояние точки отображения секунд (светится/не светится)		*/
	uint8_t visible_seconds;					/*!< Секунды на индикаторе. Признак необходимости обновления		*/
	char buffer[8];								/*!< Буфер изображения на индикаторе (ASCII)						*/

}task_clock_t;

extern task_clock_t task_clock;

#endif /* __TASK_CLOCK_H_ */
