/**
  * @file   leds.h
  * @brief  Модуль управления светодиодами
  * 		Имена светодиодов назначаются пользователем и прописываются в структуре модуля
  * 		в разделе led
  * 		Вектор прерываний модуля должен вызываться в прерывании 1 раз в 1 мс
  */


/* Includes ------------------------------------------------------------------*/
#include "leds.h"


/* Defines -------------------------------------------------------------------*/

/**
 * Управление временными характеристиками свечения светодиодов. Время инкрементирования 10 мс
 */
#define LED_PWM_PERIOD				80
#define LED_PWM_HALF_PERIOD			(LED_PWM_PERIOD / 2)
#define LED_BLINK_PWM				(LED_PWM_PERIOD / 2)
#define LED_2X_BLINK_PWM			(LED_PWM_PERIOD / 4)


/* Functions -----------------------------------------------------------------*/

static void leds_isr(void);		/*  Вектор обработки прерывания таймера						*/

/**
 * @brief	Параметры подключения светодиодов
 * 			Параметры подключения и первоначального состояния прописываются вручную из файла main.h
 */
module_leds_t module_leds = {

	.led.green1		=	{	GREEN1_GPIO_Port,		GREEN1_Pin,		GPIO_PIN_SET,	LED_OFF	},
	.led.green2		=	{	GREEN2_GPIO_Port,		GREEN2_Pin,		GPIO_PIN_SET,	LED_OFF	},
	.led.yellow1	=	{	YELLOW1_GPIO_Port,		YELLOW1_Pin,	GPIO_PIN_SET,	LED_OFF	},
	.led.yellow2	=	{	YELLOW2_GPIO_Port,		YELLOW2_Pin,	GPIO_PIN_SET,	LED_OFF	},
	.led.red1		=	{	RED1_GPIO_Port,			RED1_Pin,		GPIO_PIN_SET,	LED_OFF	},
	.led.red2		=	{	RED2_GPIO_Port,			RED2_Pin,		GPIO_PIN_SET,	LED_OFF	},

	.led.null		=	{	NULL,					0x0,			GPIO_PIN_RESET, LED_OFF	},

	.isr = leds_isr,					/*!<  Вектор обработки прерывания управления светодиодами		*/

};



/**
 * @brief	Обработчик прерывания (1 мс)
 *			Контроль состояния светодиодов выполняется каждые 10 мс
 *			В зависимости от режима свечения светодиода определяется его состояние
 *			и выполняется включение или выключение
 */
static void leds_isr(void)
{
	static uint8_t divider = 0;
	static uint8_t pwm = 0;
	GPIO_PinState state;
	
	if(++divider < 10) return; else divider = 0;
	
	if(++pwm > 99) pwm = 0;

	led_port_t * led = (led_port_t *)&module_leds.led;			// Указатель на параметры первого светодиода

	while(led->port)
	{
			switch(led->mode)
			{
				case LED_OFF:
					state = (GPIO_PinState)(led->is_on ^ 1);
					break;
		
				case LED_ON:
					state = led->is_on;
					break;

				case LED_BLINK:
					if(pwm < LED_BLINK_PWM)		state = led->is_on;		else	state = (GPIO_PinState)(led->is_on ^ 1);
					break;

				case LED_2X_BLINK:
					if			(pwm < LED_2X_BLINK_PWM)							state = led->is_on;
					else if		(pwm < LED_PWM_HALF_PERIOD)							state = (GPIO_PinState)(led->is_on ^ 1);
					else if		(pwm < (LED_PWM_HALF_PERIOD + LED_2X_BLINK_PWM) )	state = led->is_on;
					else															state = (GPIO_PinState)(led->is_on ^ 1);
					break;
					
				default:
					break;
			}
			
		HAL_GPIO_WritePin(led->port, led->pin, state);				// Изменение состояния вывода контроллера

		led++;														// Адрес следующего по списку светодиода
	}
}
