/**
  * @file   task_clock.h
  * @brief  Процесс отображения часов и календаря
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "tasks.h"


/* Defines -------------------------------------------------------------------*/
#define HOURS_AND_MINUTES_TIME		5000
#define DATE_TIME					5000
#define DAY_OF_WEEK_TIME			5000
#define YEAR_TIME					5000

#define SECONDS_ERROR_STATE			0xFF



/* Variables -----------------------------------------------------------------*/

/**
 * 	@brief	Параметры процесса
 */
static void loop(void);

task_clock_t task_clock = {

	.state				= CASE_IDLE,
	.execute			= loop,

	.timer				= 0,
	.timer_point		= 0,
	.point_state		= 0,
	.visible_seconds	= 0,
	.buffer				= {0},

};


/* Functions -----------------------------------------------------------------*/

/**
 * 	@brief	Функция выполонения процесса
 */
static void loop(void)
{
	task_clock_t * task = &task_clock;

	switch(task->state)
	{
		case CASE_IDLE:
			break;

			
		case CASE_START:

			task->timer = HAL_GetTick() + HOURS_AND_MINUTES_TIME;
			task->timer_point = HAL_GetTick();
			task->visible_seconds = SECONDS_ERROR_STATE;						// Для синхронизации отображения информации на дисплее в соответстии режиму

			task->state = TASK_CLOCK_HOURS;
			break;


		case TASK_CLOCK_HOURS:

			if(task->visible_seconds != arm.rtc->time.seconds)
			{
				task->visible_seconds = arm.rtc->time.seconds;

				sprintf(task->buffer, "%02d%02d", module_ds3231m.time.hours, module_ds3231m.time.minutes);
				arm.indicator->send_text(task->buffer);
			}

			if(HAL_GetTick() > task->timer_point)
			{
				task->point_state ^= 0x01;
				task->timer_point = HAL_GetTick() + 500;
				arm.indicator->sign.point_enable(2, task->point_state);
			}

			if(HAL_GetTick() > task->timer)
			{
				task->timer = HAL_GetTick() + DATE_TIME;
				arm.indicator->sign.point_enable(2, false);
				task->visible_seconds = SECONDS_ERROR_STATE;
				task->state = TASK_CLOCK_DATE;
			}
			break;

			
		case TASK_CLOCK_DATE:

			if(task->visible_seconds != arm.rtc->time.seconds)
			{
				task->visible_seconds = arm.rtc->time.seconds;

				sprintf(task->buffer, "%02d%02d", module_ds3231m.time.date, module_ds3231m.time.month);
				arm.indicator->send_text(task->buffer);
			}


			if(HAL_GetTick() > task->timer)
			{
				task->timer = HAL_GetTick() + DAY_OF_WEEK_TIME;
				task->visible_seconds = SECONDS_ERROR_STATE;
				task->state = TASK_CLOCK_DAY;
			}
			break;


		case TASK_CLOCK_DAY:

			if(task->visible_seconds != arm.rtc->time.seconds)
			{
				task->visible_seconds = arm.rtc->time.seconds;

				sprintf(task->buffer, "  %02d", module_ds3231m.time.day);
				arm.indicator->send_text((char*)arm.rtc->time.day_name);
			}


			if(HAL_GetTick() > task->timer)
			{
				task->timer = HAL_GetTick() + YEAR_TIME;
				task->visible_seconds = SECONDS_ERROR_STATE;
				task->state = TASK_CLOCK_YEAR;
			}
			break;


		case TASK_CLOCK_YEAR:

			if(task->visible_seconds != arm.rtc->time.seconds)
			{
				task->visible_seconds = arm.rtc->time.seconds;

				sprintf(task->buffer, "%04d", module_ds3231m.time.year);
				arm.indicator->send_text(task->buffer);
			}


			if(HAL_GetTick() > task->timer)
			{
				task->state = CASE_START;
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
