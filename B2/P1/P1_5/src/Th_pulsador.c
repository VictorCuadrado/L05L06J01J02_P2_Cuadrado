#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h> 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

osThreadId_t tid_Thled3;                        // thread id
extern osThreadId_t tid_Thled2; 
int Init_Thled3 (void);  
void Thled3 (void *argument);                   // thread function

static GPIO_InitTypeDef GPIO_InitStruct ;

GPIO_InitTypeDef led_ld3 = { 
        .Pin = GPIO_PIN_14,
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
		
void EXTI15_10_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){

	}
}
typedef struct  {
	  GPIO_InitTypeDef pin;
		GPIO_TypeDef *port;
		int delay;
		uint8_t counter;
} mygpio_pin;

mygpio_pin pinB14;
//mygpio_pin pinB7;
int Init_Thled3 (void) {
 
	// Initialize LEDS
    __HAL_RCC_GPIOB_CLK_ENABLE();
	pinB14.pin= led_ld3;
	pinB14.port=GPIOB;
	pinB14.delay=1500;
	pinB14.counter=1;
  tid_Thled3 = osThreadNew(Thled3, (void *)&pinB14, NULL);
  if (tid_Thled3 == NULL) {
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
 
void Thled3 (void *argument) {
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
		/*Configuracion pulsador */
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	/*Habilitacion reloj pulsador*/
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	//GPIO_PULLDOWN //GPIO_NOPULL
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
	
	while (1){
		
		if


				
		}

}