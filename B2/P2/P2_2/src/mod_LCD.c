#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"
/* SPI Driver */
extern ARM_DRIVER_SPI Driver_SPI0;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI0;


/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThLCD;                        // thread id
osThreadId_t tid_th_LCD_test; 

void ThLCD (void *argument);                   // thread function
void th_LCD_test (void *argument);
void LCD_reset(void);
void cb_spi(uint32_t event);

GPIO_InitTypeDef CS = { 
        .Pin = GPIO_PIN_14,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_MEDIUM
    };

GPIO_InitTypeDef A0 = { 
        .Pin = GPIO_PIN_13,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_MEDIUM
    };

GPIO_InitTypeDef RST = { 
        .Pin = GPIO_PIN_6,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_MEDIUM
    };

typedef struct  {
	  GPIO_InitTypeDef pin;
		GPIO_TypeDef *port;
		int delay;
		uint8_t counter;
} mygpio_pin;

mygpio_pin pinD14;
mygpio_pin pinF13;
mygpio_pin pinA6;
		
		
uint32_t spi_errors = 0;

int Init_ThLCD (void) {
 
  tid_ThLCD = osThreadNew(ThLCD, NULL, NULL);
  if (tid_ThLCD == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThLCD (void *argument) {
	LCD_reset();

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
 


// LCD_reset(void
void LCD_reset(void){
	/* Initialize the SPI driver */
	SPIdrv->Initialize(cb_spi);
	/* Power up the SPI peripheral */
	SPIdrv->PowerControl(ARM_POWER_FULL);
	/* Configure the SPI to Master, 8-bit mode @10000 kBits/sec */
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_DATA_BITS(8), 2000000);
	/* SS line = INACTIVE = HIGH */
	SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
	
	
	//Iniciar los pines de CS RST y A0
	__HAL_RCC_GPIOD_CLK_ENABLE();
	pinD14.pin= CS;
	pinD14.port=GPIOD;
	pinD14.delay=1500;
	pinD14.counter=1;
	HAL_GPIO_Init(pinD14.port,&pinD14.pin);
	
	__HAL_RCC_GPIOF_CLK_ENABLE();
	pinF13.pin= A0;
	pinF13.port=GPIOF;
	pinF13.delay=1500;
	pinF13.counter=1;
	HAL_GPIO_Init(pinF13.port,&pinF13.pin);
	
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	pinA6.pin= RST;
	pinA6.port=GPIOF;
	pinA6.delay=1500;
	pinA6.counter=1;
	HAL_GPIO_Init(pinA6.port,&pinA6.pin);
	
	// generar rst
	HAL_GPIO_TogglePin(pinA6.port,pinA6.pin.Pin);
	osDelay(1);
	HAL_GPIO_TogglePin(pinA6.port,pinA6.pin.Pin);
	osDelay(1);
}

void cb_spi(uint32_t event){
	if (event & ARM_SPI_EVENT_TRANSFER_COMPLETE) {
		osThreadFlagsSet(tid_Threaxxx, 0x0002U);
	}
	if (event & ARM_SPI_EVENT_DATA_LOST) {
		spi_errors++; // pérdida de datos
	}
	if (event & ARM_SPI_EVENT_MODE_FAULT) {
		spi_errors++; // conflicto de bus
	}
}