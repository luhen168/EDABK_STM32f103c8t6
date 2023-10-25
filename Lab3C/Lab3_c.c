#include <stdint.h>
#include "stm32f10x.h"

//*********************************************************************************
//													Decleration GPIO																									
//*********************************************************************************
#define GPIOA_CRL           (*(volatile uint32_t*)(GPIOA_BASE + 0x00))  // Ðia chi thanh ghi CRL
#define GPIOA_ODR           (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))  // Ðia chi thanh ghi ODR (output data register)
#define GPIOA_IDR           (*(volatile uint32_t*)(GPIOA_BASE + 0x08))  // Ðia chi thanh ghi IDR (input data register)
#define RCC_APB2ENR         (*(volatile uint32_t*)(RCC_BASE + 0x18))   
	
//*********************************************************************************
//													Decleration variable Global																									
//*********************************************************************************
static uint32_t total_cycle = 500000;
static uint32_t duty_cycle_default_percent = 3;
static uint32_t duty_cycle_increase_percent = 2;
static uint32_t breath_cycle = 10;
static uint32_t breath_default ;

//*********************************************************************************
//													Decleration Function Prototype																									
//*********************************************************************************
void LED_On(void);
void LED_Off(void);
void delay_ns(uint32_t ns);
void delay_duty_cycle(void);
void delay_relax_cycle(void);
void change_handle_status(void);
uint32_t change_read_status(void);
void increase_duty_cycle(void);
void breath_handle_status(void);
uint32_t breath_read_status(void);
void breath_duty_cycle(void);
void breath_relax_cycle(void);

//*********************************************************************************
//													Main Program																								
//*********************************************************************************
int main(void){
    // Bat bus cho PortA
		RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
		//cau hinh cac chan PA0,PA2 input:MODE = 00 và CNF = 10 pullup or pulldown
		//cau hinh chan PA4 output: MODE = 11 max speed 50MHz và CNF = 00 general push-pull
		GPIOA_CRL = 0x00020808;

    while(1){
        // Change led
        LED_On();
        delay_duty_cycle();
        LED_Off();
        delay_relax_cycle();

        // Breath led
        breath_handle_status();
    }
}

//*********************************************************************************
//													CHANGE Function																									
//*********************************************************************************
void LED_On() {
	GPIOA_ODR |= 0x10;  // Bit 1 high
}

void LED_Off() {
	GPIOA_ODR &= 0x00;  // Bit 1 low
}

void delay_ns(uint32_t ns) {
    while (ns--) {
      __NOP();
    }
}

void delay_duty_cycle() {
		uint32_t delay;
    if (duty_cycle_default_percent == 3) {
				change_handle_status();
			  delay = total_cycle * duty_cycle_default_percent;
        delay_ns(delay);
    } else {
				change_handle_status();
        delay = total_cycle * duty_cycle_default_percent;
        delay_ns(delay);
    }
}

void delay_relax_cycle() {
		uint32_t delay;
    if (duty_cycle_default_percent == 3) {
				change_handle_status();
			  delay = total_cycle * (10 - duty_cycle_default_percent);
        delay_ns(delay);
    } else {
				change_handle_status();
        delay = total_cycle * (10 - duty_cycle_default_percent);
        delay_ns(delay);
    }
}

void change_handle_status() {
	  if(change_read_status() != 0){
				while (change_read_status() != 0){
						 change_read_status();
				}
        increase_duty_cycle();
		}
}

uint32_t change_read_status() {
    return GPIOA_IDR & 0x01;
}

void increase_duty_cycle() {
    duty_cycle_default_percent += duty_cycle_increase_percent;
    if (duty_cycle_default_percent >= 10) {
        duty_cycle_default_percent = duty_cycle_default_percent - 10;
    }
}


//*********************************************************************************
//													BREATH FUNCTION																									
//*********************************************************************************
void breath_handle_status() {
		uint16_t SinTable[100] = {
        5000, 5308, 5614, 5918, 6219, 6514, 6804, 7086, 7361, 7626,
        7880, 8123, 8354, 8572, 8776, 8964, 9137, 9294, 9434, 9556,
        9660, 9746, 9813, 9861, 9890, 9900, 9890, 9861, 9813, 9746,
        9660, 9556, 9434, 9294, 9137, 8964, 8776, 8572, 8354, 8123,
        7880, 7626, 7361, 7086, 6804, 6514, 6219, 5918, 5614, 5308,
        5000, 4692, 4386, 4082, 3781, 3486, 3196, 2914, 2639, 2374,
        2120, 1877, 1646, 1428, 1224, 1036,  863,  706,  566,  444,
          340,  254,  187,  139,  110,  100,  110,  139,  187,  254,
          340,  444,  566,  706,  863, 1036, 1224, 1428, 1646, 1877,
         2120, 2374, 2639, 2914, 3196, 3486, 3781, 4082, 4386, 4692
				};
    while(breath_read_status() != 0) {
				uint16_t i = 0;
        for( ; i < 100; i++) {
				    breath_default = SinTable[i];
            LED_On();
            breath_duty_cycle();
            LED_Off();
            breath_relax_cycle();
            if(breath_read_status() == 0) {
                break;
            }
        }
    }
}

uint32_t breath_read_status() {
    return GPIOA_IDR & 0x04;
}

void breath_duty_cycle() {
		uint32_t delay = breath_cycle*breath_default;
    delay_ns(delay);
}

void breath_relax_cycle() { 
		uint32_t delay = breath_cycle*(10000-breath_default);
    delay_ns(delay);
}
