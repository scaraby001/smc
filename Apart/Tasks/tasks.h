/**
  * @file   tasks.h
  * @brief  Супервизор выполняемых процессов
  *
  * 		Этот файл управляет процессами, выполняемыми в проекте.
  *
  * 		Для добавления процесса необходимо включить #include "<имя_файла_процесса>.h",
  * 		в структуре tasks_pool_t добавить указатель требуемого типа и назначить ему имя,
  * 		в экземпляре структуры tasks присвоить этому указателю адрес на экземпляр процесса.
  *
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASKS_H_
#define __TASKS_H_


/* Includes ------------------------------------------------------------------*/
#include "modules.h"
#include "task_clock.h"							// Часы и календарь
#include "task_keyboard.h"							// Обработка нажатий на кнопки
#include "task_power.h"								// Управление питанием устройства


/* Defines -------------------------------------------------------------------*/

/**
  * @brief  Общие состояния и макросы управления процессами
  */
#define CASE_IDLE 				0
#define CASE_START				1
#define CASE_STOP				255


#define TASK_START(x)			(x=1)
#define TASK_BREAK(x)			(x=CASE_IDLE)
#define TASK_STOP(x)			(x=CASE_STOP)
#define TASK_STOPPED(x)			(x==CASE_IDLE)
#define TASK_RUNNING(x)			(x!=CASE_IDLE)


/**
  * @brief  Указатели на необходимые процессы
  * 		Имена указателей выбираются по их функциональному назначению
  * 		Тип указателей объявляется в соответствующих файлах процессов
  */
typedef struct{

	task_clock_t	* clock;
	task_keyboard_t * keyboard;
	task_power_t 	* power;

}tasks_pool_t;

extern tasks_pool_t tasks;				/*	Экземпляр супервизора доступа к модулям			*/


/* Functions -----------------------------------------------------------------*/

void tasks_start(void);				/*	Включение необходимых процессов после инициализации POWER_ON	*/
void tasks_stop(void);				/*	Выключение необходимых процессов								*/

void tasks_loop(void);				/*	Карусель событий. Последовательный вызов параллельных процессов	*/

/**
 * 	Структура содержит общие функции выше уровня процессов
 */
typedef struct{
	struct
	{
		void(*restore)(void);
	}rgb;

	struct
	{
		void(*sleep)(void);
	}power;

}common_function_t;

extern common_function_t high_task;

#endif /* __TASKS_H_ */
