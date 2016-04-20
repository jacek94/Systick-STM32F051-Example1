#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#define MillisecondsIT 1e3
#define GLed GPIO_Pin_9
#define BLed GPIO_Pin_8
#define LEDGPIO GPIOC
#define PushButton GPIO_Pin_0
#define PushButtonGPIO GPIOA


volatile uint32_t Milliseconds = 0, Seconds = 0;


void SysTick_Handler(void){
	Milliseconds++;
	if(Milliseconds%1000 == 999){
		Seconds++;
	}
}


void DelayMil(uint32_t MilS){
	volatile uint32_t MSStart = Milliseconds;
	while((Milliseconds - MSStart)<MilS) asm volatile("nop");
}


void DelaySec(uint32_t S){
	volatile uint32_t Ss = Seconds;
	while((Seconds - Ss)<S) asm volatile("nop");
}


GPIO_InitTypeDef G;

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/MillisecondsIT);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);


	G.GPIO_Pin = BLed | GLed;
	G.GPIO_Mode = GPIO_Mode_OUT;
	G.GPIO_PuPd = GPIO_PuPd_NOPULL;
	G.GPIO_Speed = GPIO_Speed_Level_1;
	G.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LEDGPIO, &G);


	G.GPIO_Pin = PushButton;
	G.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(PushButtonGPIO, &G);


	uint8_t ToggleVar = 1;


	Milliseconds = Seconds = 0;

	while(1)
	{

		ToggleVar = ~ToggleVar;


		if(ToggleVar == 1){
			GPIO_SetBits(LEDGPIO, GLed);
			GPIO_ResetBits(LEDGPIO, BLed);
		}
		else{
			GPIO_ResetBits(LEDGPIO, GLed);
			GPIO_SetBits(LEDGPIO, BLed);
		}

		if(GPIO_ReadInputDataBit(PushButtonGPIO, PushButton) == SET){
			DelaySec(1);
		}
		else{
			DelayMil(100);
		}
	}
}
