/**
  * @file   ds3231m.h
  * @brief  Модуль управления микросхемой часов реального времени
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DS3231M_H_
#define __DS3231M_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"

/* Defines -------------------------------------------------------------------*/
/**
 * @brief	Регистры микросхемы DS3231M
 * 			An STM32 HAL library written for the DS3231 real-time clock IC.
 * 			Library by @eepj www.github.com/eepj
 */
#define DS3231_I2C_ADDR 	0x68

#define DS3231_REG_SECOND 	0x00
#define DS3231_REG_MINUTE 	0x01
#define DS3231_REG_HOUR  	0x02
#define DS3231_REG_DOW    	0x03

#define DS3231_REG_DATE   	0x04
#define DS3231_REG_MONTH  	0x05
#define DS3231_CENTURY 		7
#define DS3231_REG_YEAR   	0x06

#define DS3231_A1_SECOND	0x07
#define DS3231_A1_MINUTE	0x08
#define DS3231_A1_HOUR		0x09
#define DS3231_A1_DATE		0x0a

#define DS3231_A2_MINUTE	0x0b
#define DS3231_A2_HOUR		0x0c
#define DS3231_A2_DATE		0x0d

#define DS3231_AXMY			7
#define DS3231_DYDT			6

#define DS3231_REG_CONTROL 	0x0e
#define DS3231_EOSC			7
#define DS3231_BBSQW		6
#define DS3231_CONV			5
#define DS3231_RS2			4
#define DS3231_RS1			3
#define DS3231_INTCN		2
#define DS3231_A2IE			1
#define DS3231_A1IE			0

#define DS3231_REG_STATUS	0x0f
#define DS3231_OSF			7
#define DS3231_EN32KHZ		3
#define DS3231_BSY			2
#define DS3231_A2F			1
#define DS3231_A1F			0

#define DS3231_AGING		0x10

#define DS3231_TEMP_MSB		0x11
#define DS3231_TEMP_LSB		0x12

#define DS3231_TIMEOUT		HAL_MAX_DELAY


/* Variables -----------------------------------------------------------------*/

/**
 * Структура модуля управления микросхемой часов реального времени
 */
typedef struct{

	struct
	{
		uint8_t seconds;
		uint8_t minutes;
		uint8_t hours;
		uint8_t day;
		const char * day_name;
		uint8_t date;
		uint8_t month;
		uint16_t year;
	}time;
	
	struct
	{
		void(*seconds)(uint8_t value);
		void(*minutes)(uint8_t value);
		void(*hours)(uint8_t value);
		void(*day)(uint8_t value);
		void(*date)(uint8_t value);
		void(*month)(uint8_t value);
		void(*year)(uint16_t value);
	}time_set;

	void(*isr_set)(uint8_t enable);		/*!<	Вкл./выкл. прерывания микросхемы 1с			*/
	void(*isr)(void);					/*!<	Обработка прерывания микросхемы 1с			*/

	
}module_ds3231m_t;

extern module_ds3231m_t module_ds3231m;			/*!<	Экземпляр модуля управления микросхемой часов реального времени	*/


#endif /* __DS3231M_H_ */
