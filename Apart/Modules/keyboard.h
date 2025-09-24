/**
  * @file   keyboard.h
  * @brief  Модуль чтения и обработки кнопок и клавиатуры
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Defines -------------------------------------------------------------------*/

#define KB_KEY_CODE_ERROR					0xFF		// Признак неверного кода, которого нет в списке допустимых значений

/**
 * @brief Коды клавиш клавиатуры и отдельно стоящих кнопок
 *		  Кнопки нумеруются слева направо сверху вниз, исходя из схемотехники
 *		  Но для совместимости с пультом ДУ, номера адаптированы под пульт,
 *		  как в мобильном телефоне
 */
typedef enum{
	KB_KEY_CODE_1,				//	KEY_CODE_0,
	KB_KEY_CODE_2,				//	KEY_CODE_1,
	KB_KEY_CODE_3,				//	KEY_CODE_2,
	KB_KEY_CODE_UP,				//	KEY_CODE_3,
	KB_KEY_CODE_4,				//	KEY_CODE_4,
	KB_KEY_CODE_5,				//	KEY_CODE_5,
	KB_KEY_CODE_6,				//	KEY_CODE_6,
	KB_KEY_CODE_OK,				//	KEY_CODE_7,
	KB_KEY_CODE_7,				//	KEY_CODE_8,
	KB_KEY_CODE_8,				//	KEY_CODE_9,
	KB_KEY_CODE_9,				//	KEY_CODE_STAR,
	KB_KEY_CODE_DOWN,			//	KEY_CODE_HASH,
	KB_KEY_CODE_LEFT,			//	KEY_CODE_UP,
	KB_KEY_CODE_0,				//	KEY_CODE_DOWN,
	KB_KEY_CODE_RIGHT,			//	KEY_CODE_LEFT,
	KB_KEY_CODE_HASH,			//	KEY_CODE_RIGHT,
	KB_KEY_CODE_STAR,			//	KEY_CODE_OK,
	
	KEYBOARD_KEYS_QUANTITY,		// Количество кнопок
}keyboard_code_t;


/* Variables -----------------------------------------------------------------*/

/**
 * Структура модуля чтения и обрбаботки кнопок и клавиатуры
 */
typedef struct{

	struct
	{
		keyboard_code_t code;						/*!<	Код нажатой клавиши								*/
		uint8_t is_pressed;							/*!<	Признак того, что PRESS выполнен				*/
		uint8_t is_holded;							/*!<	Признак того, что HOLD выполнен					*/
	}key;

	void(*isr)(void);

}module_keyboard_t;


extern module_keyboard_t module_keyboard;			/*!<	Экземпляр модуля клавиатуры						*/

#endif /* __KEYBOARD_H_ */
