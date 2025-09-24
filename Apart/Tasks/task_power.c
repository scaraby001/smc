/**
  * @file   task_power.с
  * @brief  Процесс управления питанием устройства
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "tasks.h"


/**
 * 	@brief	Функция выполонения процесса
 * 			Для процессов функции унифицированы.
 * 			Каждый процесс содержит переменную state, которая рекурсивно обновляется при каждом выполнении
 * 			в зависимости от изменения состояния процесса
 * 			При входе в процесс, определяется адрес, по которому расположены параметры процесса
 * 			Благодаря тому, что каждый процесс имеет свой тип, обращение к локальным переменным или функциям
 * 			доступно через одинаковое имя указателя task->
 */
static void loop(void);		/* Функция выполнения процесса	*/

/**
 * Персональные параметры процесса. Начальные значения указателей и
 * локальных переменных, доступных внутри процесса
 */
task_power_t task_power = {

	.state = CASE_IDLE,
	.execute = loop,

};

static void loop(void)
{
	task_power_t * task = &task_power;

	switch(task->state)
	{
		case CASE_IDLE:
			break;

			
		case CASE_START:
		
			arm.ms_time->start();			// Включение пользовательских прерываний 1 мс
			
			printf("\n\r\n\rSoftware and methodological complex started\n\r");
			
			arm.indicator->timer.start();
			arm.indicator->brightness_set(100);

			arm.indicator->send_text("slip");

//			arm.indicator->sign.put_sign(1, '2');
//			arm.indicator->sign.point_enable(1, true);

//			arm.leds->led.green1.mode = LED_OFF;
//			arm.leds->led.green2.mode = LED_ON;
//			arm.leds->led.yellow1.mode = LED_2X_BLINK;
//			arm.leds->led.yellow2.mode = LED_BLINK;
//			arm.leds->led.red1.mode = LED_2X_BLINK;
//			arm.leds->led.red2.mode = LED_BLINK;

//			printf("IIC RTC register = 0x%x\n\r", ds3231_read_register(0x0E));
//			arm.rtc->isr_set(true);
//			arm.rtc->time_set.seconds(15);
//			arm.rtc->time_set.minutes(22);
//			arm.rtc->time_set.hours(14);
//			arm.rtc->time_set.day(1);
//			arm.rtc->time_set.date(22);
//			arm.rtc->time_set.month(9);
//			arm.rtc->time_set.year(2025);

//			arm.sound->volume.set(SOUND_LEVEL_HIGH);
//			arm.sound->play(SOUND_CASA_INTRO_1);
//			arm.sound->play(SOUND_CASA_INTRO_1);
//			arm.sound->play(SOUND_CASA_INTRO_2);

//			task->temp = arm.adc->median(ADC_CHANNEL_TEMP_1, 1);
			
//			arm.indicator->sign.put_sign(0, task->temp % 10 + 0x30);
//			task->temp /= 10;
//			arm.indicator->sign.put_sign(1, task->temp % 10 + 0x30);
//			task->temp /= 10;
//			arm.indicator->sign.put_sign(2, task->temp % 10 + 0x30);
//			task->temp /= 10;
//			arm.indicator->sign.put_sign(3 ,task->temp % 10 + 0x30);			
			
//			task->timer = HAL_GetTick() + 500;
//			task->point_state = 0;

			task->state = TASK_POWER_SLEEP;
			break;


		case TASK_POWER_SLEEP:

			if(arm.keyboard->key.is_pressed || arm.keyboard->key.is_holded)
			{
				if(arm.keyboard->key.code == KB_KEY_CODE_STAR)
				{
					arm.sound->play(SOUND_POWER_ON);
					arm.indicator->send_text("  on");

					tasks_start();

					arm.keyboard->key.is_pressed = false;
					arm.keyboard->key.is_holded = false;

					task->state = TASK_POWER_PROCEED;
				}
			}
			break;

		case TASK_POWER_PROCEED:
		
//			if(HAL_GetTick() > task->timer)
//			{
//				task->timer = HAL_GetTick() + 500;
//
//				task->point_state ^= 0x01;
//
//				arm.indicator->sign.point_enable(2, task->point_state);
//
//				task->temp = DS3231_GetRegByte(1);
////				printf("REG0=0x%x\n\r", task->temp);
//				arm.indicator->sign.put_sign(0, (task->temp & 0x0F) + 0x30);
//				arm.indicator->sign.put_sign(1, ((task->temp & 0xF0)>>4) + 0x30);
//
//				task->temp = DS3231_GetRegByte(2);
//				printf("REG2=0x%x\n\r", task->temp);
//				arm.indicator->sign.put_sign(2, (task->temp & 0x0F) + 0x30);
//				arm.indicator->sign.put_sign(3, ((task->temp & 0xF0)>>4) + 0x30);
//			}
		
			
			break;

		case CASE_STOP:
			task->state = CASE_IDLE;		
			break;
	
		default:
			task->state = CASE_IDLE;
			break;
	}
}
