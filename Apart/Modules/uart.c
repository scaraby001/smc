/**
  * @file   uart.c
  * @brief  Модуль отправки сообщений в UART
  * 		Для Keil необходимо закомментировать #define __CUBEIDE__
  * 		и поставить галочку в Options for Target/Target/Use MicroLIB
  */


/* Includes ------------------------------------------------------------------*/
#include "uart.h"


/* Variables -----------------------------------------------------------------*/
extern UART_HandleTypeDef	huart2;
#define MESSAGES_UART		&huart2


/* Defines -------------------------------------------------------------------*/
#define __CUBEIDE__
#define UART_SEND_MESSAGE_DELAY	5000


/**
  * @brief  Перенаправление printf() в UART
  */
#ifdef __CUBEIDE__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(MESSAGES_UART, (uint8_t *)&ch, 1, UART_SEND_MESSAGE_DELAY);
  return ch;
}
