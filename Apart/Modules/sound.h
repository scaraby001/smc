/**
  * @file   sound.h
  * @brief  Модуль генерации и воспроизведения звуков с помощью резонатора
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOUND_H_
#define __SOUND_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Variables -----------------------------------------------------------------*/

/**
 * @brief	Уровень громкости
 */
typedef enum{
	SOUND_LEVEL_MUTE,
	SOUND_LEVEL_LOW,
	SOUND_LEVEL_MIDDLE,
	SOUND_LEVEL_HIGH,
	
	SOUND_LEVELS_QTY
}sound_level_t;

/**
 * @brief	Звуки и мелодии
 */
typedef enum{
	SOUND_NONE,
	SOUND_POWER_OFF,
	SOUND_POWER_ON,
	SOUND_ERROR,
	SOUND_LONG_BEEP,
	SOUND_DOUBLE_BEEP,
	SOUND_BEEP,
	SOUND_LOW_BEEP,
	SOUND_OK,
	SOUND_CASA_INTRO_1,
	SOUND_CASA_INTRO_2,
	SOUND_CASA_OUTRO_1,
	SOUND_CASA_OUTRO_2,
	SOUND_VICTORY,
 
	SOUNDS_QTY
}sound_index_t;

/**
 * Структура модуля генерации и воспроизведения звуков с помощью резонатора
 */
typedef struct{

	struct
	{
		void(*set)(sound_level_t value);	/*!<	Установка уровня громкости													*/
		sound_level_t(*get)(void);			/*!<	Текущий уровень громкости													*/
	}volume;
	
	void(*play)(sound_index_t value);		/*!<	Постановка мелодии в буфер воспроизведения									*/
	void(*isr)(void);						/*!<	Обработчик прерываний воспроизведения звуков								*/

}module_sound_t;

extern module_sound_t module_sound;			/*!<	Экземпляр модуля генерации и воспроизведения звуков с помощью резонатора	*/

#endif /* __SOUND_H_ */
