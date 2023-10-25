#include <stdint.h>
#include "dac.h"
#include "stm32f10x.h"
#include "sound.h"
#include "key.h"
// **************Sound_Init*********************
void Sound_Init(void){
	DAC_Init();
	//SysTick	
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL  = 0;
	NVIC_EnableIRQ(SysTick_IRQn);
  NVIC_SetPriority(SysTick_IRQn,0);
	SysTick->CTRL = SysTick_CTRL_ENABLE|SysTick_CTRL_TICKINT|SysTick_CTRL_CLKSOURCE;
}

// **************Sound_Start*********************
void Sound_Start(uint32_t period){
	SysTick->LOAD = period-1;
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
	Index=(Index+1)%64;	
	DAC_Out(SinWave[Index]);							// Output digital values to DAC every period 
}



