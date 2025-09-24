/**
  * @file   task_keyboard.h
  * @brief  Процесс обработки нажатий на кнопки
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "tasks.h"


/* Defines -------------------------------------------------------------------*/

#define USER_INACTIVITY_TIME	60000				/*!< Время бездействия пользователя, мс			*/


/**
 * 	@brief	Функция выполонения процесса
 * 			Для процессов функции унифицированы.
 * 			Каждый процесс содержит переменную state, которая рекурсивно обновляется при каждом выполнении
 * 			в зависимости от изменения состояния процесса
 * 			При входе в процесс, определяется адрес, по которому расположены параметры процесса
 * 			Благодаря тому, что каждый процесс имеет свой тип, обращение к локальным переменным или функциям
 * 			доступно через одинаковое имя указателя task->
 */
static void loop(void);

task_keyboard_t task_keyboard = {

	.state = CASE_IDLE,
	.execute = loop,

	.user_inactivity_timing = USER_INACTIVITY_TIME,

};

static void loop(void)
{
	task_keyboard_t * task = &task_keyboard;

	switch(task->state)
	{
		case CASE_IDLE:
			break;

			
		case CASE_START:

			task->user_inactivity_timing = HAL_GetTick() + USER_INACTIVITY_TIME;
		
			task->state = TASK_KEYBOARD_PROCEED;
			break;


		case TASK_KEYBOARD_PROCEED:
		
			if(arm.keyboard->key.is_pressed || arm.keyboard->key.is_holded)
			{
				arm.sound->play(SOUND_BEEP);
				task->user_inactivity_timing = HAL_GetTick() + USER_INACTIVITY_TIME;

				switch(arm.keyboard->key.code)
				{
					case KB_KEY_CODE_0:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'0');
						break;

					case KB_KEY_CODE_1:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'1');
						break;

					case KB_KEY_CODE_2:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'2');
						break;

					case KB_KEY_CODE_3:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'3');
						break;

					case KB_KEY_CODE_4:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'4');
						break;
						
					case KB_KEY_CODE_5:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'5');
						break;

					case KB_KEY_CODE_6:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'6');
						break;

					case KB_KEY_CODE_7:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'7');
						break;

					case KB_KEY_CODE_8:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'8');
						break;

					case KB_KEY_CODE_9:
						arm.indicator->sign.shift();
						arm.indicator->sign.put_sign(0,'9');
						break;
		
					case KB_KEY_CODE_HASH:
					
						arm.indicator->sign.put_sign(0,' ');
						arm.indicator->sign.put_sign(1,' ');
						arm.indicator->sign.put_sign(2,' ');
						arm.indicator->sign.put_sign(3,' ');
						break;

					case KB_KEY_CODE_STAR:

						high_task.power.sleep();
						break;

					default:
						break;
				}

				arm.keyboard->key.is_pressed = false;
				arm.keyboard->key.is_holded = false;
			}

			if(HAL_GetTick() > task->user_inactivity_timing)
			{
				high_task.power.sleep();
			}
			break;
			

		case CASE_STOP:
			task->state = CASE_IDLE;		
			break;
	
		default:
			task->state = CASE_IDLE;
			break;
	}
}
