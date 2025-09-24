/**
  * @file   74ac138d.h
  * @brief  Управление микросхемой дешифратора
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __74AC138D_H_
#define __74AC138D_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Variables -----------------------------------------------------------------*/
/**
 * Параметры портов, подключенных к входам микросхемы
 */
typedef struct{

	GPIO_TypeDef *	adr_0_port;			// Порт A,B,C...
	uint16_t		adr_0_pin;			// Номер порта
	GPIO_TypeDef *	adr_1_port;
	uint16_t		adr_1_pin;
	GPIO_TypeDef *	cs_port;
	uint16_t		cs_pin;

}ports_74ac138d_t;

/**
 * Параметры микросхемы и управления её состоянием
 * Микросхем может быть несколько, поэтому структура содержит указатель на
 * структуру, содержащую порты и указатели на функции управления микросхемой
 * которые одинаковые для разных микросхем
 */
typedef struct{

	ports_74ac138d_t ports;
	
	void(*cs_enable)(uint8_t enable, ports_74ac138d_t * ports);					// Управление CS
	void(*select)(uint8_t row, ports_74ac138d_t * ports);						// Назначение активного вывода (адрес)

}module_74ac138d_t;


/* Functions -----------------------------------------------------------------*/
void cs_pin_74ac138d_enable(uint8_t enable, ports_74ac138d_t * ports);
void row_74ac138d_select(uint8_t row, ports_74ac138d_t * ports);


#endif /* __74AC138D_H_ */
