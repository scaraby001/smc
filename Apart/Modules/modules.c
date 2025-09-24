/**
  * @file   modules.c
  * @brief  Супервизор подключаемых модулей
  * 		Этот файл управляето модулями, подключаемыми к проекту
  */


/* Includes ------------------------------------------------------------------*/
#include "modules.h"


/**
  * @brief  Структура для доступа к модулям из процессов
  * 		Здесь присваиваются адреса модулей к пользовательским указателям
  */
modules_t arm = {

	.adc				= &module_adc,
	.indicator			= &module_indicator,
	.keyboard			= &module_keyboard,
	.leds				= &module_leds,
	.ms_time			= &module_ms_time,
	.rtc				= &module_ds3231m,
	.sound				= &module_sound,

};
