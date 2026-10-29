#include "cmsis_os2.h"                          // CMSIS RTOS header file
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThLCD;                        // thread id
osThreadId_t tid_th_LCD_test; 
 
void ThLCD (void *argument);                   // thread function
void th_LCD_test (void *argument);


int Init_ThLCD (void) {
 
  tid_ThLCD = osThreadNew(ThLCD, NULL, NULL);
  if (tid_ThLCD == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThLCD (void *argument) {
 
  while (1) {
    ; // Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}


int Init_th_LCD_test (void) {
 
  tid_ThLCD = osThreadNew(ThLCD, NULL, NULL);
  if (tid_ThLCD == NULL) {
    return(-1);
  }
 
  return(0);
}

void th_LCD_test (void *argument) {
 
  while (1) {
    ; // Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
 