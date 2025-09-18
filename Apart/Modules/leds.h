#ifndef __LEDS_H_
#define __LEDS_H_

#include "main.h"

//////////////////////////////////////////////////////////////////////////

// Состояния светодиода (mode)
typedef enum{

	LED_OFF,
	LED_ON,
	LED_BLINK,
	LED_2X_BLINK,

}led_mode_t;

// Порт, к которому подключен светодиод
typedef struct{

	GPIO_TypeDef * port;			// Порт A,B,C...
	uint16_t pin;							// Номер порта
	GPIO_PinState is_on;			// Состояние порта, при котором светодиод светится (GPIO_PIN_SET / GPIO_PIN_RESET)
	led_mode_t mode;					// Светится/не светится/мигает/мигает быстро
	
}led_port_t;

typedef struct{

	// Светодиоды, подключенные к устройству. Имена на усмотрение пользователя
	struct
	{
		led_port_t green1;
		led_port_t green2;		
		led_port_t yellow1;
		led_port_t yellow2;
		led_port_t red1;
		led_port_t red2;
		
		led_port_t null;						// Заглушка. Признак окончания списка светодиодов
	
	}led;

	void(*isr)(void);

}module_leds_t;

extern module_leds_t module_leds;

//////////////////////////////////////////////////////////////////////////

#endif /* __LEDS_H_ */
