#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h> 

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

osThreadId_t tid_Thjoy;   // thread id
extern osThreadId_t tid_Thled2;  
extern osThreadId_t tid_Thled3; 
int Init_Thjoy (void);  
void Thjoy (void *argument);                   // thread function
void Timer_Callback(void *arg);

osThreadId_t tid_Thjoy;
osTimerId_t tim_joy;
uint8_t gesto = 0;


GPIO_InitTypeDef led_ld1 = { 
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };
/*
GPIO_InitTypeDef led_ld2 = {
        .Pin = GPIO_PIN_7,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };*/
typedef struct  {
	  GPIO_InitTypeDef pin;
		GPIO_TypeDef *port;
		int delay;
		uint8_t counter;
} mygpio_pin;

//mygpio_pin pinB0;
//mygpio_pin pinB7;
int Init_Thjoy (void) {
 
	// Initialize LEDS
  tim_joy=osTimerNew(Timer_Callback,osTimerOnce,NULL,NULL);
	  if (tim_joy == NULL) {
    return(-1);
  }

	
  tid_Thjoy = osThreadNew(Thjoy,NULL, NULL);
  if (tid_Thjoy == NULL) {
    return(-1);
  }
  return(0);
}
 
void Thjoy (void *argument) {

	while (1){	
    osThreadFlagsWait(0x0001U, osFlagsWaitAny, osWaitForever);
    osTimerStart(tim_joy, 50);   /* reinicia el timer en cada rebote */
  }
}

static void Timer_Callback(void *arg) {
  
  if      (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET) {
		gesto += 1;
	}
  else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET) {
		gesto += 2;
	}
  else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == GPIO_PIN_SET) {
		gesto += 3;
	}
  else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == GPIO_PIN_SET) {
		gesto += 4;
	}
  else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == GPIO_PIN_SET) {
		gesto += 5;
	}
	
	
}