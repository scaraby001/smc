/**
  * @file   adc.с
  * @brief  Модуль аналого-цифрового преобразования
  *			АЦП настроено таким образом, чтобы выполнять чтение выбранного канала
  *			с использованием одного ранка без применения DMA
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"


/* Variables -----------------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;

/* Functions -----------------------------------------------------------------*/

/**
 * @brief	Девиация двух значений
 * @param	value1 первая величина
 * @param	value2 вторая величина
 * @retval  девиация двух величин
 */
uint32_t deviation(uint32_t value1, uint32_t value2)
{
	uint32_t temp_value = 0;

	if (value1 > value2)
	{
		temp_value = value1 - value2;
	}
	else
	{
		temp_value = value2 - value1;
	}

	return temp_value;
}

/**
 * @brief	Медианное значение трёх значений
 * @param	a первая величина
 * @param	b вторая величина
 * @param	c третья величина
 * @retval  медиана
 */
uint32_t median_of_3(uint32_t a, uint32_t b, uint32_t c)
{
	uint32_t median;


	if ((a <= b) && (a <= c))
	{
		median = (b <= c) ? b : c;
	}
	else
	if ((b <= a) && (b <= c))
	{
		median = (a <= c) ? a : c;
	}
	else
	{
		median = (a <= b) ? a : b;
	}

	return median;
}

/**
 * @brief	Выбор канала для чтения АЦП
 * @note	Номер ранка определяется при насройке АЦП в CubeMX
 * @param	channel канал АЦП
 */
void adc_set_channel(uint8_t channel)
{
	 ADC_ChannelConfTypeDef sConfig = {0};
	 
	 sConfig.Channel = channel;
	 sConfig.Rank = 1;
	 HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/**
 * @brief	Запуск преобразования
 */
void adc_start(void)
{
	HAL_ADC_Start(&hadc1);
}

/**
 * @brief	Ожидание предварительно запущенного преобразования
 * @param	timeout время принудительного завершения ожидания
 * @retval  Результат преобразования
 */
uint32_t adc_wait_and_read(uint32_t timeout)
{
	HAL_ADC_PollForConversion(&hadc1, timeout);
	return HAL_ADC_GetValue(&hadc1);
}

/**
 * @brief	Запуск и ожидание преобразования
 * @note	Перед вызовом функции необходимо выбрать требуемый канал АЦП
 * @retval  Результат преобразования
 */
uint32_t adc_read(void)
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	return HAL_ADC_GetValue(&hadc1);
}

/**
 * @brief	Чтение выбранного канала АЦП N раз и усреднение по медианам
 * @param	channel канал АЦП
 * @param	n количество троекратных чтений
 * @retval  Результат преобразования
 */
uint32_t adc_read_middle_of_n_medians(uint32_t channel, uint32_t n)
{
	uint32_t a = 0;
	uint32_t b = 0;
	uint32_t c = 0;
	
	uint32_t sum = 0;
	
	adc_set_channel(channel);
	
	if (n == 0) n = 1;
	
	for (uint32_t i=0; i<n; i++)
	{
		a = adc_read();
		b = adc_read();
		c = adc_read();
		//
		sum += median_of_3(a, b, c);
	}
	
	return (sum / n);
}

/**
 * Параметры модуля управления АЦП
 */
module_adc_t module_adc = {

	.channel = adc_set_channel,
	.start = adc_start,
	.wait = adc_wait_and_read,
	.read = adc_read,
	.median = adc_read_middle_of_n_medians,

};
