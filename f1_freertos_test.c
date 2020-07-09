#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_utils.h>

//-------| Debug stuff |------------------------
#include "CL_CONFIG.h"
#include "CL_delay.h"
#include "CL_printMsg.h"
#include "CL_systemClockUpdate.h"


#include "FreeRTOS.h"
#include "task.h"
#define LED1	LL_GPIO_PIN_6
#define LED2	LL_GPIO_PIN_5
//--------| Free RTOS STUFF |------------------


void blink_led1(void *args);
void blink_led2(void *args);
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);
void vPortSVCHandler(void);
//--------| Prototypes |----------------------
void init_leds(void);
int main(void)
{
	
	setSysClockTo72();
	CL_printMsg_init_Default(false);
	CL_delay_init();
	init_leds();
	CL_printMsg("CL libs Initialized\n");
	

	// Create the LED tasks.
	xTaskCreate(blink_led1, "LED_blink_1", 128, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(blink_led2, "LED_blink_2", 128, NULL, configMAX_PRIORITIES - 1, NULL);
	// Start the scheduler.
	vTaskStartScheduler();


	for (;;)
	{

	}
}
void init_leds(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; 
	LL_GPIO_InitTypeDef leds_struct;
	LL_GPIO_StructInit(&leds_struct);
	
	leds_struct.Pin        = LED1 | LED2;
	leds_struct.Mode       = LL_GPIO_MODE_OUTPUT;
	leds_struct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
	leds_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	
	LL_GPIO_Init(GPIOB, &leds_struct);
	
	//test leds
	for(int i =  0 ; i < 5 ; i++)
	{
		LL_GPIO_TogglePin(GPIOB, LED1);
		delayMS(50);
		LL_GPIO_TogglePin(GPIOB, LED2);
		delayMS(50);
		LL_GPIO_TogglePin(GPIOB, LED1);
		delayMS(50);
		LL_GPIO_TogglePin(GPIOB, LED2);			
	}	
	
}

void blink_led1(void *args)
{
	while (1)
	{	
		LL_GPIO_TogglePin(GPIOB, LED1);
		CL_printMsg("\nTask1 p\n");
		vTaskDelay(pdMS_TO_TICKS(100));
		CL_printMsg("\nTask1 a\n");
	}
}

void blink_led2(void *args)
{
	while (1)
	{	
		LL_GPIO_TogglePin(GPIOB, LED2);
		CL_printMsg("\nTask2 p\n");
		vTaskDelay(pdMS_TO_TICKS(800));
		CL_printMsg("\nTask2 a\n");
	}
	
}

