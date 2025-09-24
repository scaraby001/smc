/**
  * @file   leds.h
  * @brief  Модуль управления светодиодами
  * 		Имена светодиодов назначаются пользователем и прописываются в структуре модуля
  * 		в разделе led
  * 		Вектор прерываний модуля должен вызываться в прерывании 1 раз в 1 мс
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LEDS_H_
#define __LEDS_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Defines -------------------------------------------------------------------*/

/*
 * Состояния светодиода (mode)
 */
typedef enum{

	LED_OFF,
	LED_ON,
	LED_BLINK,
	LED_2X_BLINK,

}led_mode_t;

/*
 * Порт, к которому подключен светодиод
 */
typedef struct{

	GPIO_TypeDef * port;			// Порт A,B,C...
	uint16_t pin;					// Номер порта
	GPIO_PinState is_on;			// Состояние порта, при котором светодиод светится (GPIO_PIN_SET / GPIO_PIN_RESET)
	led_mode_t mode;				// Светится/не светится/мигает/мигает быстро
	
}led_port_t;

/*
 * Светодиоды, подключенные к устройству
 * Имена светодиодов выбираются, исходя из пользовательских предпочтений
 */
typedef struct{

	struct
	{
		led_port_t green1;
		led_port_t green2;		
		led_port_t yellow1;
		led_port_t yellow2;
		led_port_t red1;
		led_port_t red2;
		
		led_port_t null;				/*!<  Заглушка. Признак окончания списка светодиодов			*/
	
	}led;

	void(*isr)(void);					/*!<  Вектор обработки прерывания управления светодиодами		*/

}module_leds_t;

extern module_leds_t module_leds;		/*	Экземпляр модуля управления светодиодами					*/

//////////////////////////////////////////////////////////////////////////

#endif /* __LEDS_H_ */
