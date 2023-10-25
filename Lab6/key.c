#include <stdint.h>
#include "stm32f10x.h"
#include "key.h"

// **************Key_Init*********************
// Initialize piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: none
void Key_Init(void){ 
  // used in Lab 6
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL = 0x00008888;
}
// **************Key_In*********************
// Input from piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: 0 to 15 depending on keys
//   0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
uint32_t Key_In(void){ 
  // write this
  return GPIOA->IDR&0x0F; // replace this line
}

