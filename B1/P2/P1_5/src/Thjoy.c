#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h> 

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

osThreadId_t tid_Thread;   // thread id
extern osThreadId_t tid_Thled2;  
extern osThreadId_t tid_Thled3; 
int Init_Thread (void);  
void Thread (void *argument);                   // thread function
/*
GPIO_InitTypeDef led_ld1 = { 
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };

GPIO_InitTypeDef led_ld2 = {
        .Pin = GPIO_PIN_7,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };
typedef struct  {
	  GPIO_InitTypeDef pin;
		GPIO_TypeDef *port;
		int delay;
		uint8_t counter;
} mygpio_pin;

mygpio_pin pinB0;
//mygpio_pin pinB7;*/
/*
void Timerjoy_Callback(void *arg) {
	osThreadFlagsSet(tid_Thjoy, 0x0001U);
}*/

int Init_Thread (void) {
 
	  return(0);
}
 
void Thread (void *argument) {
	/*
	static uint32_t a=0;
  mygpio_pin *gpio = (mygpio_pin *)argument;
	HAL_GPIO_Init(gpio->port, &(gpio->pin));	
  while (1) {
    gpio->counter++;
		HAL_GPIO_TogglePin(gpio->port, gpio->pin.Pin);
		osDelay(gpio->delay);
	}
	*/
	osTimerId_t swtim_id;
	while (1){	
		
		osThreadFlagsWait(0x0002U,osFlagsWaitAny,osWaitForever);
		
		swtim_id = osTimerNew(
		,
		osTimerPeriodic, // o osTimerOnce
		NULL, NULL);

		
		osTimerStart(swtim_id, 1000U);

		
  }
}