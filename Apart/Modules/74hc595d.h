/**
  * @file   74hc595d.h
  * @brief  Управление микросхемой сдвигового регистра
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __74HC595D_H_
#define __74HC595D_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Variables -----------------------------------------------------------------*/
/**
 * Параметры портов, подключенных к входам микросхемы
 */
typedef struct{

	GPIO_TypeDef *	data_port;						// Порт A,B,C...
	uint16_t		data_pin;						// Номер порта
	GPIO_TypeDef *	clk_port;
	uint16_t		clk_pin;
	GPIO_TypeDef *	update_port;
	uint16_t		update_pin;

}ports_74hc595d_t;

/**
 * Параметры микросхемы и управления её состоянием
 * Микросхем может быть несколько, поэтому структура содержит указатель на
 * структуру, содержащую порты и указатели на функции управления микросхемой
 * которые одинаковые для разных микросхем
 */
typedef struct{

	ports_74hc595d_t ports;
	
	void(*send_byte)(uint8_t byte, ports_74hc595d_t * ports);

}module_74hc595d_t;


/* Functions -----------------------------------------------------------------*/
void send_byte_to_74hc595d(uint8_t byte, ports_74hc595d_t * ports);


#endif /* __74HC595D_H_ */
