#include "sound.h"
#include "dac.h"


const uint8_t Bassoon[64] = {  			
  34,37,37,37,36,35,35,34,32,31,30,			
  31,34,43,57,61,51,32,15,7,8,13,			
  18,24,32,41,46,47,44,38,31,25,			
  19,15,13,17,24,33,40,43,39,33,			
  27,25,25,26,26,26,28,30,31,32,			
  33,34,37,39,38,36,35,35,35,34,32,31			
};  			

void Sound_Init(){
	
	DAC_Init();
	Index = 0;
	SysTick->LOAD = 0; 
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	
}

void Sound_Start(uint32_t period){
	
	SysTick->LOAD = period - 1;
	
}

void SysTick_Handler(void){   
	
  DAC_Out(Bassoon[Index]);    
	Index = (Index+1)%64;
	
}

