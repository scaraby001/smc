/**
  * @file   74hc595d.с
  * @brief  Управление микросхемой сдвигового регистра
  *			Микросхем может быть несколько, поэтому экземпляры объявляются
  *			в модулях, которые включают в себя такие микросхемы (четырехразрядный дисплей)
  */

/* Includes ------------------------------------------------------------------*/
#include "74hc595d.h"


/* Functions -----------------------------------------------------------------*/

/**
 * @brief	Управление выводом CS
 * @param	byte	данные, записываемые в регистр
 * @param	ports	указатель на параметры портов, покдлюченных к входам микросхемы
 */
void send_byte_to_74hc595d(uint8_t byte, ports_74hc595d_t * ports)
{
	GPIO_PinState state;			// Временная переменная для формирования состояния входа данных микросхемы

	// Запись байта в регистр
	for(uint32_t i=0; i<8; i++)
	{
		// Формирование сигнала на входе данных микросхемы, в зависимости от значения в передаваемом байте данных
		state = (GPIO_PinState)(byte & 0x01);
		HAL_GPIO_WritePin(ports->data_port, ports->data_pin, state);

		// Формирование сигнала на входе CLOCK микросхемы
		HAL_GPIO_WritePin(ports->clk_port, ports->clk_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(ports->clk_port, ports->clk_pin, GPIO_PIN_RESET);
		
		byte = byte>>1;		// Следующий бит в передаваемом байте данных
	}

	// Выдача содержимого регистра на шину данных индикатора
	HAL_GPIO_WritePin(ports->update_port, ports->update_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ports->update_port, ports->update_pin, GPIO_PIN_RESET);
}

//////////////////////////////////////////////////////////////////////////
