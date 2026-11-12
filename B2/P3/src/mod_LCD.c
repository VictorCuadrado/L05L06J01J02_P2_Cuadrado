#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"
#include "Arial12x12.h"
/* SPI Driver */
extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;


/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThLCD;                        // thread id
osThreadId_t tid_th_LCD_test; 

void ThLCD (void *argument);                   // thread function
void th_LCD_test (void *argument);
void LCD_reset(void);
void cb_spi(uint32_t event);
void LCD_wr_cmd ( unsigned char cmd );
void LCD_wr_data ( unsigned char data );
void LCD_update ( void );
void LCD_symbolToLocalBuffer_L1(uint8_t symbol);

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
		
char buffer[512];		
uint32_t spi_errors = 0;

void LCD_update ( void )
{
//		buffer[0]= 0x00;
//		buffer[1]= 0x00;
//		buffer[2]= 0xFE;
//		for (int i = 3; i <= 8; i++) {
//			buffer[i] = 0x22;
//		}
//		buffer[9]= 0xFE;
//		buffer[10]= 0x00;
//		buffer[11]= 0x00;
		
    int i;
    LCD_wr_cmd (0x00); // 4 bits de la parte baja de la dirección a 0
    LCD_wr_cmd (0x10); // 4 bits de la parte alta de la dirección a 0
    LCD_wr_cmd (0xB0); // Página 0
    for (i = 0; i < 128; i++) {
        LCD_wr_data (buffer[i]);
    }
    LCD_wr_cmd (0x00); // 4 bits de la parte baja de la dirección a 0
    LCD_wr_cmd (0x10); // 4 bits de la parte alta de la dirección a 0
    LCD_wr_cmd (0xB1); // Página 1
    for (i = 128; i < 256; i++) {
        LCD_wr_data (buffer[i]);
    }
    LCD_wr_cmd (0x00);
    LCD_wr_cmd (0x10);
    LCD_wr_cmd (0xB2); // Página 2
    for (i = 256; i < 384; i++) {
        LCD_wr_data (buffer[i]);
    }
    LCD_wr_cmd (0x00);
    LCD_wr_cmd (0x10);
    LCD_wr_cmd (0xB3); // Página 3
    for (i = 384; i < 512; i++) {
        LCD_wr_data (buffer[i]);
    }
}

int Init_ThLCD (void) {
	
  tid_ThLCD = osThreadNew(ThLCD, NULL, NULL);
  if (tid_ThLCD == NULL) {
    return(-1);
  }
 
  return(0);
}

int LCD_init (void){
	LCD_wr_cmd(0xAE);
	LCD_wr_cmd(0xA2);
	LCD_wr_cmd(0xA0);
	LCD_wr_cmd(0xC8);
	LCD_wr_cmd(0x22);
	LCD_wr_cmd(0x2F);
	LCD_wr_cmd(0x40);
	LCD_wr_cmd(0xAF);
	LCD_wr_cmd(0x81);
	LCD_wr_cmd(0x08); // ¿¿¿¿
	LCD_wr_cmd(0xA4);
	LCD_wr_cmd(0xA6);
	
	return(0);
}
 
void ThLCD (void *argument) {
	LCD_reset();
	LCD_init();
	LCD_update();
  while (1) {
    ; // Insert thread code here...
		osThreadFlagsWait( 0x0002U, osFlagsWaitAny, osWaitForever);
		LCD_symbolToLocalBuffer_L1('H');
		LCD_update();
		//osThreadYield();                            // suspend thread
  }
}



int Init_th_LCD_test (void) {
 
  tid_th_LCD_test = osThreadNew(th_LCD_test, NULL, NULL);
  if (tid_th_LCD_test == NULL) {
    return(-1);
  }
 
  return(0);
}

void LCD_symbolToLocalBuffer_L1(uint8_t symbol) {

    static uint16_t positionL1 = 0;
    uint8_t i, value1, value2;
    uint16_t offset = 0;

    offset = 25 * (symbol - ' ');

    for (i = 0; i < 12; i++) {

        value1 = Arial12x12[offset + i * 2 + 1];
        value2 = Arial12x12[offset + i * 2 + 2];

        buffer[i + positionL1] = value1;
        buffer[i + 128 + positionL1] = value2;
    }

    positionL1 = positionL1 + Arial12x12[offset];
}
void th_LCD_test (void *argument) {
 
  while (1) {
    ; // Insert thread code here...
		osDelay(1);
		osThreadFlagsSet(tid_ThLCD, 0x0002U);
		//ThLCD(buffer);
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
	pinA6.port=GPIOA;
	pinA6.delay=1500;
	pinA6.counter=1;
	HAL_GPIO_Init(pinA6.port,&pinA6.pin);
	
	// generar rst
	HAL_GPIO_WritePin(pinA6.port,pinA6.pin.Pin,GPIO_PIN_RESET);
	osDelay(1);
	HAL_GPIO_WritePin(pinA6.port,pinA6.pin.Pin,GPIO_PIN_SET);
	osDelay(1);
}

void cb_spi(uint32_t event){
	if (event & ARM_SPI_EVENT_TRANSFER_COMPLETE) {
		osThreadFlagsSet(tid_ThLCD, 0x0001U);
	}
	if (event & ARM_SPI_EVENT_DATA_LOST) {
		spi_errors++; // pérdida de datos
	}
	if (event & ARM_SPI_EVENT_MODE_FAULT) {
		spi_errors++; // conflicto de bus
	}
}


// LCD_wr_cmd : Función que escribe un dato en el LCD.

void LCD_wr_data ( unsigned char data ){
// Seleccionar CS = 0;
	HAL_GPIO_WritePin(pinD14.port,pinD14.pin.Pin,GPIO_PIN_RESET);
// Seleccionar A0 = 1;
	HAL_GPIO_WritePin(pinF13.port,pinF13.pin.Pin,GPIO_PIN_SET);
// Escribir un dato (data) usando la función SPIDrv -> Send ();
	SPIdrv->Send(&data,sizeof(data));
// Esperar al flag enviado desde la cb_spi () de transferencia
	osThreadFlagsWait( 0x0001U, osFlagsWaitAny, osWaitForever);
// finalizada;
// Seleccionar CS = 1;
	HAL_GPIO_WritePin(pinD14.port,pinD14.pin.Pin,GPIO_PIN_SET);
		
}

// LCD_wr_cmd : Función que escribe un comando en el LCD.

void LCD_wr_cmd ( unsigned char cmd ){
// Seleccionar CS = 0;
	HAL_GPIO_WritePin(pinD14.port,pinD14.pin.Pin,GPIO_PIN_RESET);
// Seleccionar A0 = 0;
	HAL_GPIO_WritePin(pinF13.port,pinF13.pin.Pin,GPIO_PIN_RESET);
// Escribir un comando (cmd) usando la función SPIDrv -> Send ();
	SPIdrv->Send(&cmd,sizeof(cmd));
// Esperar al flag enviado desde la cb_spi () de transferencia
	osThreadFlagsWait( 0x0001U, osFlagsWaitAny, osWaitForever);
// finalizada;
	HAL_GPIO_WritePin(pinD14.port,pinD14.pin.Pin,GPIO_PIN_SET);
}