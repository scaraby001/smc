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

/* Includes ------------------------------------------------------------------*/
#include "tasks.h"

/**
  * @brief  Указатели на необходимые процессы
  * 		Здесь задаются адреса на процессы
  */
tasks_pool_t tasks = {

	.clock		= &task_clock,
	.keyboard	= &task_keyboard,
	.power		= &task_power,

};


/* Functions -----------------------------------------------------------------*/

/**
 * @brief Карусель событий определяет последовательность исполнения процессов
 */
void tasks_loop(void)
{
	tasks.clock->execute();
	tasks.keyboard->execute();
	tasks.power->execute();
}

/**
 * @brief Запуск необходимых процессов после получения подтверждения включения притания устройства
 */
void tasks_start(void)
{
	TASK_START(tasks.keyboard->state);
	TASK_START(tasks.clock->state);
}

/**
 * @brief Остановка необходимых процессов
 *
 */
void tasks_stop(void)
{
	TASK_STOP(tasks.keyboard->state);
	TASK_STOP(tasks.clock->state);
}


/**
 *  @brief	Раздел с функциями выше уровня процессов
 */


void rgb_color_restore(void);
void power_slip_mode_enable(void);

common_function_t high_task = {
	
	.rgb.restore = rgb_color_restore,
	
	.power.sleep = power_slip_mode_enable,

};

/**
 * @brief Восстановление состояния RGB светодиода (пример использования функции)
 */
void rgb_color_restore(void)
{

}

/**
 * @brief Перевод устройства в спящий режим
 */
void power_slip_mode_enable(void)
{
	arm.sound->play(SOUND_POWER_OFF);
	arm.indicator->send_text("slip");

	tasks_stop();

	tasks.power->state = TASK_POWER_SLEEP;
}
