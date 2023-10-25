#include "stm32f10x.h"
#include "dac.h"
#include "sound.h"
#include "key.h"
//*********************************************************************************
//													Main Program																								
//*********************************************************************************
int main(void){       
  Key_Init();
  Sound_Init();
  while(1){    
	GPIOA->ODR = 0x0F;
		switch (Key_In()){																
			case 0x000E:				
				Sound_Start(C7); //PA0-Key0=261.6Hz
				break;
			case 0x000D:
				Sound_Start(E0); //PA1-Key1=329.6Hz
				break;
			case 0x000B:
				Sound_Start(G0); //PA2-Key2=392.0Hz
				break;
			case 0x0007:
				Sound_Start(A0); //PA3-Key3=440.0 Hz
				break;
			default:
				Sound_Start(0);
				break;
		}
  }             
}


