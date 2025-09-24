/**
  * @file   task_power.h
  * @brief  Процесс управления питанием устройства
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASK_POWER_H_
#define __TASK_POWER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/**
 * Состояния процесса
 */
enum{
	TASK_POWER_IDLE,
	TASK_POWER_START,
	
	TASK_POWER_SLEEP,

	TASK_POWER_PROCEED,
	
	TASK_POWER_STOP = 255
};

/**
 * @brief	Структура управления процессом
 * 			Содержит состояние процесса, указатель на функцию выполнения процесса
 * 			и локальные переменные процесса
 */
typedef struct{

	uint8_t state;									// Состояние
	void(*execute)(void);							// Указатель на функцию выполнения процесса

	uint16_t temp;
	uint32_t timer;
	uint8_t point_state;
	
}task_power_t;

extern task_power_t task_power;		/*!< Экземпляр процесса				*/

#endif /* __TASK_POWER_H_ */
