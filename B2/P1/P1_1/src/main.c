/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 #include "stm32f4xx_hal.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
 static GPIO_InitTypeDef GPIO_InitStruct ;
 
__NO_RETURN static void app_main (void *argument) {
  (void)argument;
  // ...
  for (;;) {}
}
 
int main (void) {
 
  // System Initialization
	HAL_Init();
  SystemCoreClockUpdate();
  // ...
		__HAL_RCC_GPIOD_CLK_ENABLE () ;
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;

	GPIO_InitStruct.Pin = GPIO_PIN_13 ;
	
	HAL_GPIO_Init( GPIOD, &GPIO_InitStruct ) ;
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	extern int Init_Thread (void);
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
