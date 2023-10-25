#include <stdint.h>
#include "stm32f10x.h"
#include "dac.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	//PortB7-0
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;     //enable clock APB2ENR
	GPIOB->CRL = 0x33333333;								//have reset  = 0x44444444		
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){
	GPIOB->ODR = data;
}
