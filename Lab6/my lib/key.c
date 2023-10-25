#include "key.h"

void Key_Init(){
	GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();
	
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

uint32_t Key_In(){
	uint32_t input;
	input = ((GPIOB->IDR)>>6)&0x0F;
	return input;
}