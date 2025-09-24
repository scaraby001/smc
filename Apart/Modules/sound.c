/**
  * @file   sound.h
  * @brief  Модуль генерации и воспроизведения звуков с помощью резонатора
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "sound.h"


/* Defines -------------------------------------------------------------------*/

/**
 * @brief	Таймер в режиме PWM Genetation CH1
 * 			Частота после предделителя 2000000 Гц
 */
extern TIM_HandleTypeDef 	htim1;				/*!< Структура таймера генерации звука						*/
#define SOUND_TIMER_HTIM	&htim1				/*!< Адрес структуры таймера генерации звука				*/
#define SOUND_PWM_CHANNEL	TIM_CHANNEL_1		/*!< Канал ШИМ таймера генерации звука						*/

/**
 * @brief	Включение/выключение вывода, управляющего звуком
 */
#define SOUND_PIN_SET				HAL_GPIO_WritePin(SPEAKER_GPIO_Port, SPEAKER_Pin, GPIO_PIN_SET);
#define SOUND_PIN_RESET				HAL_GPIO_WritePin(SPEAKER_GPIO_Port, SPEAKER_Pin, GPIO_PIN_RESET);

/**
 * @brief	Рассчётные коэффициенты для вычисления значений регистров таймера
 * 			для формирования звуков заданной частоты и громкости
 */
#define SOUND_TIMER_MAX_FREQUENCY			2000000									// Частота таймера после предделителя
#define VOLUME_ADUST_COEFFICIENT			200										// Привязка значений громкости к коэффициенту заполнения

/* Variables -----------------------------------------------------------------*/

/**
 * @brief	Параметры звуков (частота, длительность / Гц, мс). Если частота = 0, то это пауза
 */
const uint16_t sound_none[4] = {0,80,0,0};
const uint16_t power_off[10] = {2332, 80, 2512, 80, 2839, 80, 0, 80,0,0};
const uint16_t power_on[8] = {2968, 80, 3265, 80, 0, 80,0,0};
const uint16_t error[10]	= {717,80, 0, 80, 717, 80, 0, 80,0,0};
const uint16_t long_beep[6] = {2176, 1000, 0, 80,0,0};
const uint16_t double_beep[10] = {2176, 80, 0, 80, 2176, 80, 0, 80,0,0};
const uint16_t beep[6] = {2176, 80, 0, 80,0,0};
const uint16_t low_beep[6] = {742, 80, 0, 80,0,0};
const uint16_t ok[8] = {1070, 200, 1209, 200, 0, 40,0,0};
const uint16_t casa_intro_1[22] = {784,125,0,40, 784,125,0,40, 784,125,0,40, 1046,437,0,40, 1318,250, 0,80, 0,0};
const uint16_t casa_intro_2[32] = {0, 250, 1046,250,0,40, 1046,125,0,40, 988,125,0,40, 988,125,0,40, 880,125,0,40, 880,125,0,40, 784,250,0,40, 0,0};
const uint16_t casa_outro_1[22] = {784,125,0,40, 784,125,0,40, 784,125,0,40, 988,437,0,40, 1175,250, 0,80, 0,0};
const uint16_t casa_outro_2[32] = {0, 250, 784,250,0,40, 784,125,0,40, 880,125,0,40, 880,125,0,40, 988,125,0,40, 988,125,0,40, 1046,250,0,40, 0,0};
const uint16_t victory[82] = {523,500,80,0,656,250,80,0,784,250,80,0,
							1047,125,80,0,1047,125,80,0,1047,125,80,0,1047,125,80,0,1047,125,80,0,1047,125,80,0,
							988,125,1047,125,
							1175,125,80,0,1175,125,80,0,1175,125,80,0,1175,125,80,0,1175,125,80,0,1175,125,80,0,
							1047,125,1175,125,1319,500,1175,500,
							1047,250,784,250,1047,250,
							0,80, 0,0
};

/**
 * @brief	Указатели на массивы с последовательностями звуков в мелодиях
 */
const uint16_t * melody_p[SOUNDS_QTY] = {
	sound_none,
	power_off,
	power_on,
	error,
	long_beep,
	double_beep,
	beep,
	low_beep,
	ok,
	casa_intro_1,
	casa_intro_2,
	casa_outro_1,
	casa_outro_2,
	victory,
};

/**
 * @brief	Параметры воспроизводимой мелодии
 */
typedef struct{
	const uint16_t * melody;
	uint8_t index;
}melody_t;

melody_t played = {
	.melody = sound_none,
	.index = 0,
};

volatile uint32_t sound_duty = 0;										/*!<	Длительность воспроизведения звука											*/
const uint8_t sound_level_values[SOUND_LEVELS_QTY] = {0, 1, 5, 20};		/*!<	Уровни громкости в % от Кз, составляющего 0.5 периода генерируемого звука	*/

/**
 * Экземпляр модуля генерации и воспроизведения звуков с помощью резонатора
 */
/* Functions */
void sound_level_set(sound_level_t level);
sound_level_t sound_level_get(void);

void ISR_sound_duration(void);
void sound_buffer_put_byte(sound_index_t value);

void sound_boost_enable(uint8_t enable);

/* Variables */
module_sound_t module_sound = {
	.volume.set = sound_level_set,
	.volume.get = sound_level_get,
	.play = sound_buffer_put_byte,
	.isr = ISR_sound_duration
};


/* Functions -----------------------------------------------------------------*/

/**
 * @brief	Включение/выключение BOOST
 */
//void sound_boost_enable(uint8_t enable)
//{
//	if(enable)		HAL_GPIO_WritePin(BOOST_GPIO_Port, BOOST_Pin, GPIO_PIN_SET);
//	else			HAL_GPIO_WritePin(BOOST_GPIO_Port, BOOST_Pin, GPIO_PIN_RESET);
//}

/**
 * @brief	Включение звукового таймера
 */
void sound_timer_on(void)
{
	HAL_TIM_PWM_Start(SOUND_TIMER_HTIM, SOUND_PWM_CHANNEL);
}

/**
 * @brief	Выключение звукового таймера
 */
void sound_timer_off(void)
{
	__HAL_TIM_SET_COMPARE(SOUND_TIMER_HTIM, SOUND_PWM_CHANNEL, 0);
}

/**
 * @brief	Настройка параметров звукового таймера
 * @param	frequency частота звука, Гц
 * @param	volume громкость звука, H-M-L
 *
 */
void sound_timer_set(uint32_t frequency, uint32_t volume)
{
	sound_timer_off();

	if(frequency)
	{
		frequency = SOUND_TIMER_MAX_FREQUENCY/frequency;
		volume = (frequency * volume) / VOLUME_ADUST_COEFFICIENT;
		__HAL_TIM_SET_COMPARE(SOUND_TIMER_HTIM, SOUND_PWM_CHANNEL, volume);
		__HAL_TIM_SET_AUTORELOAD(SOUND_TIMER_HTIM, frequency);			
		
		sound_timer_on();
	}
}

/**
 * @brief	Кольцевой буфер последовательности проигрывания мелодий
 * @note	Размер буфера обязательно должен быть кратным степени двойки!
 */
/* Defines */
#define SOUND_BUFF_SIZE	8
#define SOUND_BUFF_MASK	(SOUND_BUFF_SIZE-1)

/* Variables */
uint8_t sound_buffer[SOUND_BUFF_SIZE] = {0};					/*!<		Кольцевой буфер очереди воспроизводимых мелодий		*/
uint8_t sound_buffer_put_index = 0;								/*!<		Входящий индекс кольцевого буфера					*/
uint8_t sound_buffer_get_index = 0;								/*!<		Исходящий индекс кольцевого буфера					*/
sound_level_t sound_level = SOUND_LEVEL_HIGH;					/*!<		Глобальный уровень громкости						*/

/* Functions */
/**
 * @brief	Запись номера мелодии в кольцевой буфер очереди воспроизведения
 */
void sound_buffer_put_byte(sound_index_t value)
{
	if(sound_level == SOUND_LEVEL_MUTE)	return;

	sound_buffer[sound_buffer_put_index++ & SOUND_BUFF_MASK] = value;
}

/**
 * @brief	Чтение номера мелодии из кольцевого буфера очереди воспроизведения
 */
sound_index_t sound_buffer_get_byte(void)
{
	return sound_buffer[sound_buffer_get_index++ & SOUND_BUFF_MASK];
}

/**
 * @brief	Количество номеров мелодий в кольцевом буфере очереди воспроизведения
 */
uint8_t sound_buffer_count(void)
{
	return (sound_buffer_put_index - sound_buffer_get_index) & SOUND_BUFF_MASK;
}


/**
 * @brief	Установка уровня громкости
 */
void sound_level_set(sound_level_t level)
{
	sound_level = level;
	
	if(level == SOUND_LEVEL_MUTE) sound_timer_off();					// Для случая, если изменение громкости выполняется при включенном таймере
}

/**
 * @brief	Текущий уровень громкости
 */
sound_level_t sound_level_get(void)
{
	return sound_level;
}

/**
 * @brief	Обработчик прерываний контроля длительности воспроизведения звуков
 */
void ISR_sound_duration(void)
{
	if(sound_duty) sound_duty--;

	if(sound_duty) return;			// Воспроизводится очередной звук мелодии

	if(sound_level == SOUND_LEVEL_MUTE) return;
	
	if( (played.melody[played.index] == 0) && (played.melody[played.index+1] == 0) )
	{
		// Опрос звукового буфера
		if (sound_buffer_count())
		{
			played.melody = melody_p[sound_buffer_get_byte()];
			played.index = 0;
		}			
	}
	else
	{
		// Очередная нота в мелодии
		sound_timer_set(played.melody[played.index], sound_level_values[sound_level]);
		sound_duty = played.melody[played.index+1];
		
		played.index +=2;
	}
}
