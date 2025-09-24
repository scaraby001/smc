/**
  * @file   ds3231m.h
  * @brief  Модуль управления микросхемой часов реального времени
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "ds3231m.h"


/* Defines -------------------------------------------------------------------*/

/**
 * @brief	Шина IIC
 * 			Режим работы Fast Mode, 400000 Гц
 * @note	Оязательно необходимо выводы контроллера перевести в режим Pull-up,
 * 			так как на плате не установлены подтягивающиие резисторы
 */
extern I2C_HandleTypeDef 	hi2c1;			/*!< Структура параметров шины IIC						*/
#define D3231M_IIC			&hi2c1			/*!< Адрес структуры параметров шины IIC				*/
#define DS3231_IIC_TIMEOUT	5000U			/*!< Максимальное время ожидания отклика шины			*/

/* Variables -----------------------------------------------------------------*/

const char * day_names[7] = {
	"pone",
	"utor",
	"sred",
	"4etu",
	"ptnc",
	"subb",
	"uosr"
};

/**
 * Экземпляр модуля управления микросхемой часов реального времени
 */
/* Functions */
void(ds3231m_seconds_set)(uint8_t value);
void(ds3231m_minutes_set)(uint8_t value);
void(ds3231m_hours_set)(uint8_t value);
void(ds3231m_day_set)(uint8_t value);
void(ds3231m_date_set)(uint8_t value);
void(ds3231m_month_set)(uint8_t value);
void(ds3231m_year_set)(uint16_t value);
void ds3231m_isr_set(uint8_t enable);
void ds3231m_isr(void);

/* Variables */
module_ds3231m_t module_ds3231m = {

	.time.seconds	= 0,
	.time.minutes	= 0,
	.time.hours		= 0,
	.time.day		= 0,
	.time.date		= 0,
	.time.month		= 0,
	.time.year		= 1900,

	.time_set.seconds	= ds3231m_seconds_set,
	.time_set.minutes	= ds3231m_minutes_set,
	.time_set.hours		= ds3231m_hours_set,
	.time_set.day		= ds3231m_day_set,
	.time_set.date		= ds3231m_date_set,
	.time_set.month		= ds3231m_month_set,
	.time_set.year		= ds3231m_year_set,

	.isr_set	= ds3231m_isr_set,
	.isr		= ds3231m_isr,

};


/* Functions -----------------------------------------------------------------*/
/**
 * @brief	Чтение регистра микросхемы DS3231M
 * @param	address адрес регистра
 * @retval	значение содержимого регистра
 */
uint8_t ds3231_read_register(uint8_t address)
{
	uint8_t value;
	HAL_I2C_Master_Transmit(D3231M_IIC, DS3231_I2C_ADDR << 1, &address, 1, DS3231_IIC_TIMEOUT);
	HAL_I2C_Master_Receive(D3231M_IIC, DS3231_I2C_ADDR << 1, &value, 1, DS3231_IIC_TIMEOUT);
	return value;
}

/**
 * @brief	Запись регистра микросхемы DS3231M
 * @param	address адрес регистра
 * @retval	значение содержимого регистра
 */
void ds3231m_write_register(uint8_t address, uint8_t value)
{
	uint8_t buffer[2] = { address, value };
	HAL_I2C_Master_Transmit(D3231M_IIC, DS3231_I2C_ADDR << 1, buffer, 2, DS3231_IIC_TIMEOUT);
}

/**
 * @brief	Генерация выходного сигнала SQW 1 Гц
 * @param	enable вкл./выкл. генерации микросхемой сигналов 1 Гц
 * 			Прерывание по этому сигналу обновляет параметры календаря
 */
void ds3231m_isr_set(uint8_t enable)
{
	uint8_t buffer;

	buffer = ds3231_read_register(DS3231_REG_CONTROL);

	if(enable)
	{
		buffer |= (1<<DS3231_BBSQW);
		buffer &= ~(1<<DS3231_INTCN);
	}
	else
	{
		buffer &= ~(1<<DS3231_BBSQW);
		buffer |= (1<<DS3231_INTCN);
	}

	ds3231m_write_register( DS3231_REG_CONTROL, buffer);
}

/**
 * @brief	Обработка прерывания 1 Гц
 * 			Обновление времени и календаря
 */
void ds3231m_isr(void)
{
	uint8_t buffer[7];

	HAL_I2C_Master_Transmit(D3231M_IIC, DS3231_I2C_ADDR << 1, DS3231_REG_SECOND, 1, DS3231_IIC_TIMEOUT);
	HAL_I2C_Master_Receive(D3231M_IIC, DS3231_I2C_ADDR << 1, buffer, 7, DS3231_IIC_TIMEOUT);

	module_ds3231m.time.seconds	= ((buffer[DS3231_REG_SECOND]>>4) & 0x0F) * 10 + (buffer[DS3231_REG_SECOND] & 0x0F);
	module_ds3231m.time.minutes	= ((buffer[DS3231_REG_MINUTE]>>4) & 0x0F) * 10 + (buffer[DS3231_REG_MINUTE] & 0x0F);
	module_ds3231m.time.hours	= ((buffer[DS3231_REG_HOUR]>>5) & 0x01) * 20 + ((buffer[DS3231_REG_HOUR]>>4) & 0x01) * 10 + (buffer[DS3231_REG_HOUR] & 0x0F);
	module_ds3231m.time.day		= buffer[DS3231_REG_DOW];
	module_ds3231m.time.day_name = day_names[buffer[DS3231_REG_DOW] - 1];						// Дни недели 1-7
	module_ds3231m.time.date	= ((buffer[DS3231_REG_DATE]>>4) & 0x0F) * 10 + (buffer[DS3231_REG_DATE] & 0x0F);
	module_ds3231m.time.month	= ((buffer[DS3231_REG_MONTH]>>4) & 0x03) * 10 + (buffer[DS3231_REG_MONTH] & 0x0F);

	if(buffer[DS3231_REG_MONTH] & 0x80)
		module_ds3231m.time.year = 2000;
	else
		module_ds3231m.time.year = 1900;

	module_ds3231m.time.year += ( ( (buffer[DS3231_REG_YEAR]>>4) & 0x0F) * 10 + (buffer[DS3231_REG_YEAR] & 0x0F) );

//	printf("\n\r%02d:%02d:%02d\n\r", module_ds3231m.time.hours, module_ds3231m.time.minutes, module_ds3231m.time.seconds);
//	printf("Day = %d\n\r", module_ds3231m.time.day);
//	printf("%02d.%02d.%04d\n\r", module_ds3231m.time.date, module_ds3231m.time.month, module_ds3231m.time.year);

}

/**
 * @brief Установка значения секунд
 * @param value 0-59
 */
void(ds3231m_seconds_set)(uint8_t value)
{
	uint8_t buffer;

	if(value < 60)
	{
		buffer = (value / 10 << 4) + value % 10;
		ds3231m_write_register(DS3231_REG_SECOND, buffer);
	}
}

/**
 * @brief Установка значения минут
 * @param value 0-59
 */
void(ds3231m_minutes_set)(uint8_t value)
{
	uint8_t buffer;

	if(value < 60)
	{
		buffer = (value / 10 << 4) + value % 10;
		ds3231m_write_register(DS3231_REG_MINUTE, buffer);
	}
}

/**
 * @brief Установка значения часов
 * @param value 0-23
 */
void(ds3231m_hours_set)(uint8_t value)
{
	uint8_t buffer;

	if(value < 24)
	{
		buffer = (value / 20 << 5) + (value % 20 / 10 << 4) + value % 10;
		ds3231m_write_register(DS3231_REG_HOUR, buffer);
	}
}

/**
 * @brief Установка значения дня недели
 * @param value
 */
void(ds3231m_day_set)(uint8_t value)
{
	uint8_t buffer = value;

	if(value && (value < 8) )
	{
		ds3231m_write_register(DS3231_REG_DOW, buffer);
	}
}

/**
 * @brief Установка значения числа
 * @param value
 */
void(ds3231m_date_set)(uint8_t value)
{
	uint8_t buffer;

	if(value && (value < 32) )
	{
		buffer = (value / 10 << 4) + value % 10;
		ds3231m_write_register(DS3231_REG_DATE, buffer);
	}
}

/**
 * @brief Установка значения месяца
 * @param value
 */
void(ds3231m_month_set)(uint8_t value)
{
	uint8_t buffer;

	buffer = ds3231_read_register(DS3231_REG_MONTH) & 0x80;		// Запомнить состояние бита "век"

	if(value && (value < 13) )
	{
		buffer += ( (value / 10 << 4) + value % 10 );
		ds3231m_write_register(DS3231_REG_MONTH, buffer);
	}
}

/**
 * @brief Установка значения года
 * @param value 1900-2099
 */
void(ds3231m_year_set)(uint16_t value)
{
	uint8_t buffer;

	buffer = ds3231_read_register(DS3231_REG_MONTH);

	if(value / 2000)
	{
		buffer |= (1<<DS3231_CENTURY);
	}
	else
	{
		buffer &= ~(1<<DS3231_CENTURY);
	}

	ds3231m_write_register(DS3231_REG_MONTH, buffer);

	buffer = (value % 100 / 10 << 4) + value % 10;
	ds3231m_write_register(DS3231_REG_YEAR, buffer);
}




