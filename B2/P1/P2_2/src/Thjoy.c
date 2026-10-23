#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h> 

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

osThreadId_t tid_Thjoy;   // thread id
//extern osThreadId_t tid_Thled2;  
//extern osThreadId_t tid_Thled3; 
int Init_Thjoy (void);  
void Thjoy (void *argument);                   // thread function
void Timer_Callback(void *arg);

static GPIO_InitTypeDef GPIO_InitStruct ;
osTimerId_t tim_joy;


//GESTOS
uint8_t ARRIBA =0;
uint8_t DERECHA =0;
uint8_t ABAJO =0;
uint8_t IZQUIERDA =0;
uint8_t CENTRO =0;




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

GPIO_InitTypeDef led_ld3 = {
        .Pin = GPIO_PIN_14,
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
mygpio_pin pinB7;
mygpio_pin pinB14;

void EXTI15_10_IRQHandler(void){
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
		osThreadFlagsSet(tid_Thjoy, 0x0001U);
}

int Init_Thjoy (void) {
	
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	/*Configuracion joystick */
	/*Habilitacion reloj B*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
		/*Habilitacion reloj E*/
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	
	//Configuracion pines B
	// Puerto B PIN 10 ARRIBA -> 15
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// Puerto B PIN 11 DERECHA -> 16
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//Configuracion pines E
	// Puerto B PIN 12 ABAJO -> 12
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	// Puerto B PIN 14 IZQUIERDA -> 13
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// Puerto B PIN 15 CENTRO -> 14
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	

	
	
	// Initialize LEDS
	// Initialize LED 3
	pinB14.pin= led_ld3;
	pinB14.port=GPIOB;
	pinB14.delay=1500;
	pinB14.counter=1;
  //tid_Thjoy = osThreadNew(Thjoy, (void *)&pinB14, NULL);

		
	// Initialize LED 2
  
	pinB7.pin= led_ld2;
	pinB7.port=GPIOB;
	pinB7.delay=1500;
	pinB7.counter=1;
  //tid_Thjoy = osThreadNew(Thjoy, (void *)&pinB7, NULL);

			
	// Initialize LED  1
   
	pinB0.pin= led_ld1;
	pinB0.port=GPIOB;
	pinB0.delay=1500;
	pinB0.counter=1;
	
	HAL_GPIO_Init(pinB0.port,&(pinB0.pin));
	HAL_GPIO_Init(pinB7.port,&(pinB7.pin));
	HAL_GPIO_Init(pinB14.port,&(pinB14.pin));
	
	
  tim_joy=osTimerNew(Timer_Callback,osTimerOnce,NULL,NULL);

	
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

void Timer_Callback(void *arg) {
  
  if      (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_0,GPIO_PIN_SET ); // como toggle pero le especificas estado set =1 reset =0
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_7,GPIO_PIN_RESET );
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_14,GPIO_PIN_RESET );
		ARRIBA++;
	}
  else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_0,GPIO_PIN_RESET ); // como toggle pero le especificas estado set =1 reset =0
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_7,GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_14,GPIO_PIN_RESET );
		DERECHA++;
	}
  else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_0,GPIO_PIN_SET ); // como toggle pero le especificas estado set =1 reset =0
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_7,GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_14,GPIO_PIN_RESET );
		ABAJO++;
	}
  else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_0,GPIO_PIN_RESET ); // como toggle pero le especificas estado set =1 reset =0
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_7,GPIO_PIN_RESET );
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_14,GPIO_PIN_SET );
		IZQUIERDA++;
	}
  else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_0,GPIO_PIN_SET ); // como toggle pero le especificas estado set =1 reset =0
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_7,GPIO_PIN_RESET );
		HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_14,GPIO_PIN_SET );
		CENTRO++;
	}
	
	
}