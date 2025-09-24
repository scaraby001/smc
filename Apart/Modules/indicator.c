/**
  * @file   indicator.h
  * @brief  Четырёхразрядный семисегмкнтный индикатор
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "indicator.h"
#include "74ac138d.h"
#include "74hc595d.h"
#include <stdbool.h>


/* Defines -------------------------------------------------------------------*/

#define BRIGHTNESS_VALUE_MAX	99


/* Variables -----------------------------------------------------------------*/

extern TIM_HandleTypeDef		htim10;								/*!< Структура таймера динамической индикации					*/
#define INDICATOR_TIMER			&htim10								/*!< Адрес структуры таймера динамической индикации				*/
#define INDICATOR_PWM_CHANNEL	TIM_CHANNEL_1						/*!< Канал ШИМ таймера динамической индикации					*/

/**
 *  Глифы символов семисегментного индикатора (ASCII)
 */
const uint8_t charToSegmDictionary[256] = {

		[' '] = 0x00,

		['0'] = 0xFC,
		['1'] = 0x60,
		['2'] = 0xDA,
		['3'] = 0xF2,
		['4'] = 0x66,
		['5'] = 0xB6,
		['6'] = 0xBE,
		['7'] = 0xE0,
		['8'] = 0xFE,
		['9'] = 0xF6,

		['a'] = 0xEE,
		['b'] = 0x3E,
		['c'] = 0x9C,
		['d'] = 0x7A,
		['e'] = 0x9E,
		['f'] = 0x8E,
		['g'] = 0xBE,
		['h'] = 0x6E,
		['i'] = 0x0C,
		['j'] = 0x70,
		['l'] = 0x1C,
		['n'] = 0x2A,
		['o'] = 0x3A,
		['p'] = 0xCE,
		['q'] = 0xE6,
		['r'] = 0x0A,
		['s'] = 0xB6,
		['t'] = 0x1E,
		['u'] = 0x7C,
		['y'] = 0x76,
		['z'] = 0xDA,

		['-'] = 0x02,
		['~'] = 0xC6,			// Градус цельсия
};

uint8_t buffer_char[INDICATOR_DIGITS_QTY] = {' ', ' ', ' ', ' '};				/*!< Буфер значений разрядов в коде ASCII			*/
uint8_t buffer_points[INDICATOR_DIGITS_QTY] = {false, false, false, false};		/*!< Буфер состояния десятичных точек				*/

/**
 * Экземпляр модуля дешифратора 74AC138D
 */
module_74ac138d_t indicator_decoder = {

	.ports.adr_0_port		= IC1_A_GPIO_Port,
	.ports.adr_0_pin		= IC1_A_Pin,
	.ports.adr_1_port		= IC1_B_GPIO_Port,
	.ports.adr_1_pin		= IC1_B_Pin,
	.ports.cs_port			= IC1_ENABLE_GPIO_Port,
	.ports.cs_pin			= IC1_ENABLE_Pin,

	.cs_enable 				= cs_pin_74ac138d_enable,
	.select					= row_74ac138d_select,
};

/**
 * Экземпляр модуля регистра 74HC595D
 */
module_74hc595d_t indicator_register = {

	.ports.data_port		= IC2_DATA_GPIO_Port,
	.ports.data_pin			= IC2_DATA_Pin,
	.ports.clk_port			= IC2_CLK_GPIO_Port,
	.ports.clk_pin			= IC2_CLK_Pin,
	.ports.update_port		= IC2_UPDATE_GPIO_Port,
	.ports.update_pin		= IC2_UPDATE_Pin,

	.send_byte = send_byte_to_74hc595d,
};

/**
 * Экземпляр модуля четырёхразрядного семисегментного индикатора
 */
/* Functions */
void indicator_timer_pwm_start(void);
void isr_indicator_pulse(void);
void isr_indicator_period(void);

void indicator_brightness_set(uint8_t value);

void put_code_into_buffer(uint8_t position, uint8_t code);
void set_decimal_point_enable(uint8_t position, uint8_t enable);
void buffer_char_left_shift(void);

void buffer_char_send_text(char * text);

/* Variables */
module_indicator_t module_indicator = {

	.timer.htim			= INDICATOR_TIMER,
	.timer.start		= indicator_timer_pwm_start,
	.timer.isr_pulse	= isr_indicator_pulse,
	.timer.isr_period	= isr_indicator_period,
	
	.brightness_set		= indicator_brightness_set,

	.graphic_buffer		= 0xFFFFFFFF,
	
	.sign.is_enable		= true,
	.sign.put_sign		= put_code_into_buffer,
	.sign.point_enable	= set_decimal_point_enable,
	.sign.shift			= buffer_char_left_shift,

	.send_text			= buffer_char_send_text,
};




/* Functions -----------------------------------------------------------------*/

/**
 * @brief Запуск ШИМ и прерываний таймера динамической индикации
 */
void indicator_timer_pwm_start(void)
{
	HAL_TIM_Base_Start_IT(INDICATOR_TIMER);
	HAL_TIM_PWM_Start_IT(INDICATOR_TIMER, INDICATOR_PWM_CHANNEL);
}

/**
 * @brief Обработка прерывания по окончанию импульса ШИМ таймера динамической индикации
 */
void isr_indicator_pulse(void)
{
	indicator_decoder.cs_enable(false, &indicator_decoder.ports);
}

/**
 * @brief Обработка прерывания по окончанию периода ШИМ таймера динамической индикации
 */
void isr_indicator_period(void)
{
	static uint8_t position = INDICATOR_DIGIT_0;
	uint8_t byte;

	//  Глифы берутся либо из массива символов, либо непосредственно из буфера сегментов
	if(module_indicator.sign.is_enable)
	{
		byte = ~charToSegmDictionary[buffer_char[position]];
		
		// Десятичные точки
		byte ^=  buffer_points[position];
	}
	else
	{
		byte = (uint8_t)(module_indicator.graphic_buffer>>(position * 8));
	}

	indicator_register.send_byte(byte, &indicator_register.ports);

	indicator_decoder.select(position, &indicator_decoder.ports);
	indicator_decoder.cs_enable(true, &indicator_decoder.ports);
	
	if(++position == INDICATOR_DIGITS_QTY) position = INDICATOR_DIGIT_0;
}

/**
 * @brief Значение яркости свечения индикаторов
 * @param value яркость 0-99%
 */
void indicator_brightness_set(uint8_t value)
{
	if(value > BRIGHTNESS_VALUE_MAX) value = BRIGHTNESS_VALUE_MAX;
	__HAL_TIM_SET_COMPARE(INDICATOR_TIMER, INDICATOR_PWM_CHANNEL, value);
}

/**
 * @brief Запись кода символа ASCII в буфер индикатора
 * @param position разряд 0-3
 * @param code код символа ASCII
 */
void put_code_into_buffer(uint8_t position, uint8_t code)
{
	if(position < INDICATOR_DIGITS_QTY)
	{
		buffer_char[position] = code;
	}
}

/**
 * @brief Вкл./выкл. десятичной точки в выбранном разряде индикатора
 * @param position разряд 0-3
 * @param enable признак свечения true/false
 */
void set_decimal_point_enable(uint8_t position, uint8_t enable)
{
	if(position < INDICATOR_DIGITS_QTY)
	{
		buffer_points[position] = enable ? true : false;
	}	
}

/**
 * @brief Сдвиг содержимого буфера кодов символов ASCII влево на один разряд
 */
void buffer_char_left_shift(void)
{
	for(uint8_t i=INDICATOR_DIGITS_QTY-1;i;i--)
	{
		buffer_char[i] = buffer_char[i-1];
	}
	buffer_char[0] = ' ';
}

/**
 * @brief Отправка текстовой строки в кодировке ASCII на индикатор
 * @param указатель на текстовую строку
 */
void buffer_char_send_text(char * text)
{
	while(*text)
	{
		buffer_char_left_shift();
		buffer_char[0] = (uint8_t)*text;

		text++;
	}
}
