#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"
#include <string.h>

#ifndef __mod_LCD_H
#define __mod_LCD_H
	int Init_ThLCD (void);  
	int Init_th_LCD_test (void);
	void ThLCD (void *argument);                   // thread function
  void th_LCD_test (void *argument);
  void LCD_reset(void);
  void cb_spi(uint32_t event);
  void LCD_wr_cmd ( unsigned char cmd );
  void LCD_wr_data ( unsigned char data );
  void LCD_update ( void );
  void LCD_symbolToLocalBuffer_L1(uint8_t symbol);
  void LCD_symbolToLocalBuffer_L2(uint8_t symbol);

  void Escribe_L1 ( uint8_t * cadena );
  void Escribe_L2 ( uint8_t * cadena );
#endif