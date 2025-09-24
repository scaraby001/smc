/**
  * @file   keyboard.c
  * @brief  Модуль чтения и обработки кнопок и клавиатуры
  *
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "74ac138d.h"
#include "keyboard.h"


/* Defines -------------------------------------------------------------------*/

#define LINES_QTY		3						/*!<	Количество строк матрицы клавиатуры					*/

#define KEY_PRESS_BOUNCE_TIME		50			/*!<	Время дребезга нажатия кнопки (мс)					*/
#define KEY_RELEASE_BOUNCE_TIME		50			/*!<	Время дребезга отпускания кнопки (мс)				*/

#define BUTTON_HOLD_TIME		800				/*!<	Время удержания кнопки до логического HOLD (мс)		*/
#define BUTTON_HOLD_TIME_QUICK	200				/*!<	Время ускоренного срабатывания HOLD (мс)			*/

/*
 * @brief	Макросы чтения состояния кнопок
 * 			Имена инициализируются в CubeMX и находятся в файле main.h
 * @param	GPIO_TypeDef* GPIO_port
 * @param	uint16_t GPIO_Pin
 * @retval	GPIO_PinState Состояние порта контроллера
 */
/*	Столбцы	клавиатурной матрицы	*/
#define READ_LINE_1	HAL_GPIO_ReadPin(LINE_1_GPIO_Port, LINE_1_Pin)
#define READ_LINE_2	HAL_GPIO_ReadPin(LINE_2_GPIO_Port, LINE_2_Pin)
#define READ_LINE_3	HAL_GPIO_ReadPin(LINE_3_GPIO_Port, LINE_3_Pin)
#define READ_LINE_4	HAL_GPIO_ReadPin(LINE_4_GPIO_Port, LINE_4_Pin)

/*	Отдельно стоящие кнопки			*/
#define READ_BUT_1	HAL_GPIO_ReadPin(BUT_1_GPIO_Port, BUT_1_Pin)
#define READ_BUT_2	HAL_GPIO_ReadPin(BUT_2_GPIO_Port, BUT_2_Pin)
#define READ_BUT_3	HAL_GPIO_ReadPin(BUT_3_GPIO_Port, BUT_3_Pin)
#define READ_BUT_4	HAL_GPIO_ReadPin(BUT_4_GPIO_Port, BUT_4_Pin)

/*	Синяя кнопка на плате NUCLEO	*/
#define READ_B1		HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)


/* Variables -----------------------------------------------------------------*/

/**
 * Экземпляр модуля дешифратора 74AC138D
 */
module_74ac138d_t keyboard_decoder = {

	.ports.adr_0_port		= IC3_A_GPIO_Port,
	.ports.adr_0_pin		= IC3_A_Pin,
	.ports.adr_1_port		= IC3_B_GPIO_Port,
	.ports.adr_1_pin		= IC3_B_Pin,
	.ports.cs_port			= IC3_ENABLE_GPIO_Port,
	.ports.cs_pin				= IC3_ENABLE_Pin,
	
	.cs_enable  = cs_pin_74ac138d_enable,
	.select			= row_74ac138d_select,
};

/**
 * @brief Временные метки (тайминги) начала воздействия на кнопки
 */
typedef struct{
	volatile uint32_t press[KEYBOARD_KEYS_QUANTITY];
	volatile uint32_t release[KEYBOARD_KEYS_QUANTITY];
	volatile uint32_t hold[KEYBOARD_KEYS_QUANTITY];
	volatile uint32_t hold_time[KEYBOARD_KEYS_QUANTITY];
}timing_t;

/**
 * @brief	Состояния кнопок
 * 			определяют момент наступления событий PRESS, RELEASE, HOLD
 * 			Количество кнопок не может превышать 32
 * @param	obsolete предшествующее побитное состояние кнопок
 * @param	actual текущее побитное состояние кнопок
 */
typedef struct{
	uint32_t obsolete;					/*!<	Предшествующее состояние кнопок								*/
	uint32_t actual;					/*!<	Текущее состояние кнопок									*/
	timing_t timing;					/*!<	Временные метки (тайминги) начала воздействия на кнопки		*/
}matrix_t;

matrix_t matrix = {
	.obsolete 	= 0x0,
	.actual 	= 0x0
};

/**
 * Экземпляр модуля чтения и обработки кнопок и клавиатуры
 */
/* Functions */
void isr_keyboard_routine(void);

/* Variables */
module_keyboard_t module_keyboard = {

	.key.code = KB_KEY_CODE_ERROR,
	.key.is_pressed = false,
	.key.is_holded = false,

	.isr = isr_keyboard_routine,
};


/* Functions -----------------------------------------------------------------*/

/**
 * @brief	Анализ побитного состояния буферов кнопок
 */
void keystroke_analysis(void)
{
	uint8_t pressed;					/*!<	Вспомогательное значение наступления события PRESSED			*/
	uint8_t release;					/*!<	Вспомогательное значение наступления события RELEASED			*/
	uint8_t holded;						/*!<	Вспомогательное значение наступления события HOLDED				*/
	uint8_t unplessed;					/*!<	Вспомогательное значение наступления события UNPRESSED			*/
	uint8_t error = false;				/*!<	Признак нажатия сразу двух. Игнорируется						*/

	// Проверка на то, что удерживаются не более одной кнопки
	for(uint8_t i=0;i<KEYBOARD_KEYS_QUANTITY;i++) error += ((matrix.actual>>i) & 1);
	if(error <2) error = false;

	for(uint8_t i=0;i<KEYBOARD_KEYS_QUANTITY;i++)
	{
		pressed		= ( !( (matrix.obsolete>>i) & 1) ) & ( (matrix.actual>>i) & 1);
		release		= ( !( (matrix.actual>>i) & 1) ) & ( (matrix.obsolete>>i) & 1);
		holded		= ( (matrix.obsolete>>i) & 1) & ( (matrix.actual>>i) & 1);
		unplessed	= ( !( (matrix.obsolete>>i) & 1) ) & ( !( (matrix.actual>>i) & 1) );

		if(error == false)		// Пока удерживается более одной кнопки, процессинг ставится на паузу
		{
			if(pressed)
			{
				matrix.timing.press[i] = HAL_GetTick();
			}

			if(release)
			{
				matrix.timing.release[i] = HAL_GetTick();
			}

			if(holded)
			{
				if( (HAL_GetTick() - matrix.timing.press[i]) > KEY_PRESS_BOUNCE_TIME)
				{
					// Дребезг закончился
					if(matrix.timing.hold[i] == false)
					{
						// Нажатие кнопки генерируется только один раз за весь период удержания
						module_keyboard.key.is_pressed = true;
						module_keyboard.key.code = i;
						matrix.timing.hold[i] = true;			// Флаг удержания кнопки
					}
				}

				if(matrix.timing.hold[i] == true)
				{
					matrix.timing.hold_time[i]++;											// Время удержания кнопки
				}

				if(matrix.timing.hold_time[i] == BUTTON_HOLD_TIME)							// Время до первого сигнала HOLD
				{
					// Удержание кнопки
					module_keyboard.key.is_holded = true;
					matrix.timing.hold_time[i] = BUTTON_HOLD_TIME - BUTTON_HOLD_TIME_QUICK;		// Время второго и последующих сигналов HOLD
				}
			}
		}

		if(unplessed)
		{
			// Кнопка в ненажатом состоянии
			if( (HAL_GetTick() - matrix.timing.release[i]) > KEY_RELEASE_BOUNCE_TIME)
			{
				// Кнопка считается ненажатой
				if(module_keyboard.key.code == i)
				{
					module_keyboard.key.code = KB_KEY_CODE_ERROR;
					module_keyboard.key.is_pressed = false;
					module_keyboard.key.is_holded = false;
				}

				matrix.timing.hold[i] = false;			// Флаг удержания кнопки
				matrix.timing.hold_time[i] = 0;			// Время удержания кнопки
			}
		}
	}

	matrix.obsolete = matrix.actual;
}

/**
 * @brief	Обработчик прерывания клавиатуры. Вызывается с периодом 1 мс
 * 			Формирует состояние матрицы клавиатуры в виде uint16_t,
 * 			где каждый бит отвечает за нажатие клавиши. Если клавиша нажата, то 1, не нажата 0
 * 			Нумеруются клавиши слева направо сверху вниз. 0-15
 */
void isr_keyboard_routine(void)
{
	static uint32_t buffer;
	static uint8_t cursor = 0;

	// Сброс буфера в начале опроса строк
	if(cursor == 0) buffer = 0;

	// Выбор строки клавиатуры
	keyboard_decoder.select(cursor, &keyboard_decoder.ports);

	// Чип-селект дешифратора IC3
	keyboard_decoder.cs_enable(true, &keyboard_decoder.ports);

	// Чтение состояния столбцов клавиатуры и запись состояний в буфер клавиатуры
	buffer |= READ_LINE_1<<(cursor * 4);
	buffer |= (READ_LINE_2<<1)<<(cursor * 4);
	buffer |= (READ_LINE_3<<2)<<(cursor * 4);
	buffer |= (READ_LINE_4<<3)<<(cursor * 4);

	// Номер столбца клавиатуры
	if(++cursor >= LINES_QTY)
	{
		// Чтение состояния четырёх отдельно стоящих кнопок
		buffer |= READ_BUT_1<<12;
		buffer |= READ_BUT_2<<13;
		buffer |= READ_BUT_3<<14;
		buffer |= READ_BUT_4<<15;

		// И ещё одной синей кнопки на плате NUCLEO
		buffer |= READ_B1<<16;

		matrix.actual = ~buffer;			// Состояние матрицы изменяется в конце опроса
		cursor = 0;

		keystroke_analysis();
	}
}
