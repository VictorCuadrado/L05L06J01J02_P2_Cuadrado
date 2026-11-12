#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h> 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

osThreadId_t tid_clock;                        // thread id
int Init_clock (void);  
void clock (void *argument);                   // thread function

uint8_t segundos=0;
uint8_t minutos=0;
uint8_t horas=0;

int Init_clock (void) {

  tid_clock = osThreadNew(clock, NULL, NULL);
  if (tid_clock == NULL) {
    return(-1);
  }

  return(0);
}
 
void clock (void *argument) {

  while (1) {
    for(uint8_t i=0; i<=60;i++){
			osDelay(1000); //esperar 1 segundo
			segundos++;
		}
		segundos=0;
		minutos++;
		if(minutos==60){
			horas++;
			minutos=0;
		}
		if(horas==24){
			horas=0;
		}
		

		
  }
}
