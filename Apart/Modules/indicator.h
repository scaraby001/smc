/**
  * @file   indicator.h
  * @brief  Четырёхразрядный семисегмкнтный индикатор
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INDICATOR_H_
#define __INDICATOR_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Defines -------------------------------------------------------------------*/

/**
 * @ brief Порядковые номера разрядов индикатора от младшего к старшему
 */
enum{
	INDICATOR_DIGIT_0,
	INDICATOR_DIGIT_1,
	INDICATOR_DIGIT_2,
	INDICATOR_DIGIT_3,
	
	INDICATOR_DIGITS_QTY,
};


/* Variables -----------------------------------------------------------------*/

/**
 * @brief Структура модуля черытёхразрядного семисегментного индикатора
 */
typedef struct{

	struct
	{
		TIM_HandleTypeDef *	htim;			/*!< Указатель на параметры таймера динамической индикации						*/
		void(*start)(void);					/*!< Включение таймера															*/
		void(*isr_pulse)(void);				/*!< Обработчик прерывания окончания импульса ШИМ таймера						*/
		void(*isr_period)(void);			/*!< Обработчик прерывания окончания периода ШИМ таймера						*/
	}timer;
	
	void(*brightness_set)(uint8_t value);	/*!< Значения Кз яркости свечения индикатора 0-99%								*/

	uint32_t graphic_buffer;				/*!< Низкоуровневое состояние индикатора: 4 байта, проецируемые на сегменты		*/
	
	struct
	{
		uint8_t is_enable;											/*!< Вкл./выкл. буфера кодов символов					*/
		void(*put_sign)(uint8_t position, uint8_t code);			/*!< Вывод символа в кодировке ASCII на индикатор		*/
		void(*point_enable)(uint8_t position, uint8_t enable);		/*!< Вкл./выкл. десятичной точки на индикаторе			*/
		void(*shift)(void);											/*!< Сдвиг содержимого буфера влево на 1 символ
																		 с вводом пробела в младший разряд					*/
	}sign;

	void(*send_text)(char * text);			/*!< Отправка текстовой строки на индикатор										*/

}module_indicator_t;


extern module_indicator_t module_indicator;		/*	Экземпляр модуля четырёхразрядного семисегментного индикатора			*/

#endif /* __INDICATOR_H_ */
