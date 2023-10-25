#include <stdint.h>
#include "stm32f10x.h"
//Bài này mình làm còn chua hoan chinh neu cac ban tham khao thi su dung CNF Pullup
//cho input
//*********************************************************************************
//													Decleration GPIO																									
//*********************************************************************************
#define GPIOA_CRL           (*(volatile uint32_t*)(GPIOA_BASE + 0x00))  // Ðia chi thanh ghi CRL
#define GPIOA_ODR           (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))  // Ðia chi thanh ghi ODR (output data register)
#define GPIOA_IDR           (*(volatile uint32_t*)(GPIOA_BASE + 0x08))  // Ðia chi thanh ghi IDR (input data register)
#define GPIOB_CRL           (*(volatile uint32_t*)(GPIOB_BASE + 0x00))  // Ðia chi thanh ghi CRL
#define GPIOB_IDR						(*(volatile	uint32_t*)(GPIOB_BASE + 0x08))
#define RCC_APB2ENR         (*(volatile uint32_t*)(RCC_BASE + 0x18))   
	
//*********************************************************************************
//													Decleration variable Global																									
//*********************************************************************************

// Linked data structure

struct State {
  uint32_t Out;
  uint32_t Time; 
  uint32_t Next[8];
};
typedef const struct State State_t;
#define goW   0
#define waitW 1
#define as1		2	
#define goS   3
#define waitS 4
#define as2		5
#define walk  6
#define on1		7
#define off1	8
#define on2		9
#define off2	10
#define as3   11

static State_t FSM[12]={
 {0x4C,50000000,{goW,waitW,goW,waitW,waitW,waitW,waitW,waitW}},
 {0x54,10000000,{goW,as1,as1,as1,as1,as1,as1,as1}},
 {0x64,10000000,{goS,walk,goS,walk,goS,goS,goS,goS}},
 {0x61,50000000,{goS,waitS,waitS,waitS,goS,waitS,waitS,waitS}},
 {0x62,10000000,{goS,as2,as2,as2,as2,as2,as2,as2}},
 {0x64,10000000,{goW,walk,goW,walk,goW,walk,goW,walk}},
 {0xA4,50000000,{on1,walk,on1,on1,on1,on1,on1,on1}},
 {0x62, 5000000,{off1,off1,off1,off1,off1,off1,off1,off1}},
 {0x24, 5000000,{on2,on2,on2,on2,on2,on2,on2,on2}},
 {0x64, 5000000,{off2,off2,off2,off2,off2,off2,off2,off2}},
 {0x24, 5000000,{as3,walk,as3,as3,as3,as3,as3,as3}},
 {0x64,10000000,{goW,walk,goW,goW,goS,goS,goW,goW}},
};
static uint32_t S;  // index to the current state
static uint32_t Input;


//*********************************************************************************
//													Decleration Function Prototype																									
//*********************************************************************************
void delay_ns(uint32_t ns);

//*********************************************************************************
//													Main Program																								
//*********************************************************************************
int main(void){
    // Bat bus cho PortA
		RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
		// Bat bus cho PortB
		RCC_APB2ENR |= RCC_APB2ENR_IOPBEN;
	
		//cau hinh chan PA0-7 output: MODE = 11 max speed 50MHz và CNF = 00 general push-pull
		GPIOA_CRL = 0x22222222;
		//cau hinh cac chan PB0-2 input:MODE = 00 và CNF = 10 pullup or pulldown
		GPIOB_CRL = 0x00008888;
		S = goW;
    while(1){
			GPIOA_ODR = (GPIOA_ODR&0x00)|FSM[S].Out;  // set lights
			delay_ns(FSM[S].Time);
			Input = GPIOB_IDR&0x0F;   // read sensors
			S = FSM[S].Next[Input]; 
    }
}

//*********************************************************************************
//													Function																									
//*********************************************************************************
void delay_ns(uint32_t ns) {
    while (ns--) {
      __NOP();
    }
}

