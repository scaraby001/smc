/**
  * @file   modules.h
  * @brief  Супервизор подключаемых модулей
  *
  * 		Этот файл управляет модулями, подключаемыми к проекту.
  *
  * 		Для добавления модуля необходимо включить #include "<имя_файла_модуля>.h",
  * 		в структуре modules_t добавить указатель требуемого типа и назначить ему имя,
  * 		в экземпляре структуры arm присвоить этому указателю адрес на экземпляр модуля.
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODULES_H_
#define __MODULES_H_


/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "adc.h"							// АЦП
#include "ds3231m.h"						// Часы реального времени
#include "indicator.h"						// Четырёхразрядный семисегментный индикатор
#include "interrupt.h"						// Обработчики прерываний
#include "keyboard.h"						// Клавиатура + кнопка на плате NUCLEO
#include "leds.h"							// Одноцветные светодиоды
#include "ms_time.h"						// Прерывания 1 мс
#include "sound.h"							// Звук
#include "uart.h"							// Последовательный порт RS232


/**
  * @brief  Указатели на необходимые модули
  * 		Имена указателей выбираются по их функциональному назначению
  * 		Тип указателей объявляется в соответствующих файлах модулей
  */
typedef struct{

	module_adc_t			* adc;
	module_ds3231m_t		* rtc;
	module_indicator_t		* indicator;
	module_keyboard_t		* keyboard;
	module_leds_t			* leds;
	module_ms_time_t		* ms_time;
	module_sound_t 			* sound;

}modules_t;

extern modules_t arm;		/*	Экземпляр супервизора доступа к модулям			*/

#endif /* __MODULES_H_ */
