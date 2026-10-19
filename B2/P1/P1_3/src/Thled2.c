#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h> 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

osThreadId_t tid_Thled2;                        // thread id
int Init_Thled2 (void);  
void Thled2 (void *argument);                   // thread function
extern osThreadId_t tid_Thled3; 
GPIO_InitTypeDef led_ld2 = { 
        .Pin = GPIO_PIN_7,
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

mygpio_pin pinB7;
//mygpio_pin pinB7;
int Init_Thled2 (void) {
 
	// Initialize LEDS
    __HAL_RCC_GPIOB_CLK_ENABLE();
	pinB7.pin= led_ld2;
	pinB7.port=GPIOB;
	pinB7.delay=1500;
	pinB7.counter=1;
  tid_Thled2 = osThreadNew(Thled2, (void *)&pinB7, NULL);
  if (tid_Thled2 == NULL) {
    return(-1);
  }/*
	pinB7.pin= led_ld2;
	pinB7.port=GPIOB;
	pinB7.delay=2000;
  pinB7.counter=0;
	tid_Thread = osThreadNew(Thread, (void *)&pinB7, NULL);
  if (tid_Thread == NULL) {
    return(-1);
  }
 */
  return(0);
}
 
void Thled2 (void *argument) {
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
	
	//static uint32_t a=0;
	mygpio_pin *gpio = (mygpio_pin *)argument;
	HAL_GPIO_Init(gpio->port, &(gpio->pin));	
	while (1){	
		osThreadFlagsWait( // con 's'
		0x0001U,
		osFlagsWaitAny,
		osWaitForever);
		
		for(uint32_t a=0;a<=20;a++){
			HAL_GPIO_TogglePin(gpio->port, gpio->pin.Pin);
			osDelay(137);
			HAL_GPIO_TogglePin(gpio->port, gpio->pin.Pin);
			osDelay(137);
			
			if(a==15){
				//osDelay(1000);
				osThreadFlagsSet(
				tid_Thled3, 0x0002U);
			}
			
			if(a==20){
				a=0;
				osThreadFlagsWait( // con 's'
				0x0001U,
				osFlagsWaitAny,
				osWaitForever);
			}
				
		}



		
  }
}