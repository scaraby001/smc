/**
  * @file   task_keyboard.h
  * @brief  Процесс обработки нажатий на кнопки
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASK_KEYBOARD_H_
#define __TASK_KEYBOARD_H_


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**
 * Состояния процесса
 */
enum{
	TASK_KEYBOARD_IDLE,
	TASK_KEYBOARD_START,
	TASK_KEYBOARD_PROCEED,
	
	TASK_KEYBOARD_STOP = 255
};

/**
 * @brief	Структура управления процессом
 * 			Содержит состояние процесса, указатель на функцию выполнения процесса
 * 			и локальные переменные процесса
 */
typedef struct{

	uint8_t state;								/*!< Состояние процесса											*/
	void(*execute)(void);						/*!< Функция однократного выполнения процесса					*/
	
	uint32_t user_inactivity_timing;			/*!< Временная метка начала бейздействия пользователя			*/

}task_keyboard_t;

extern task_keyboard_t task_keyboard;

#endif /* __TASK_KEYBOARD_H_ */
