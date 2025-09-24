/**
  * @file   74ac138d.с
  * @brief  Управление микросхемой дешифратора
  *			Микросхем может быть несколько, поэтому экземпляры объявляются
  *			в модулях, которые включают в себя такие микросхемы (клавиатура и четырехразрядный дисплей)
  */

/* Includes ------------------------------------------------------------------*/
#include "74ac138d.h"


/* Functions -----------------------------------------------------------------*/

/**
 * @brief	Управление выводом CS
 * @param	enable	состояние входа микросхемы true/false
 * @param	ports	указатель на параметры портов, покдлюченных к входам микросхемы
 */
void cs_pin_74ac138d_enable(uint8_t enable, ports_74ac138d_t * ports)
{
	if(enable)
	{
		HAL_GPIO_WritePin(ports->cs_port, ports->cs_pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(ports->cs_port, ports->cs_pin, GPIO_PIN_RESET);	
	}
}

/**
 * Управление адресными входами
 * @param	row		номер выхода микросхемы дешифратора
 * @param	ports	указатель на параметры портов, покдлюченных к входам микросхемы
 */
void row_74ac138d_select(uint8_t row, ports_74ac138d_t * ports)
{
	row &= 0x03;				// Старшие разряды обнуляются
	
	switch(row)
	{
		case 0:
			HAL_GPIO_WritePin(ports->adr_0_port, ports->adr_0_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ports->adr_1_port, ports->adr_1_pin, GPIO_PIN_RESET);
			break;
			
		case 1:
			HAL_GPIO_WritePin(ports->adr_0_port, ports->adr_0_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ports->adr_1_port, ports->adr_1_pin, GPIO_PIN_RESET);
			break;
			
		case 2:
			HAL_GPIO_WritePin(ports->adr_0_port, ports->adr_0_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ports->adr_1_port, ports->adr_1_pin, GPIO_PIN_SET);
			break;
			
		case 3:
			HAL_GPIO_WritePin(ports->adr_0_port, ports->adr_0_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ports->adr_1_port, ports->adr_1_pin, GPIO_PIN_SET);
			break;
	}
}

