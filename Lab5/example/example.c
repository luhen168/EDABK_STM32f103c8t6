// Linked data structure
struct State {
  uint32_t Out;
  uint32_t Time; 
  uint32_t Next[4];};
typedef const struct State State_t;
#define goN   0
#define waitN 1
#define goE   2
#define waitE 3
State_t FSM[4]={
 {0x21,3000,{goN,waitN,goN,waitN}},
 {0x22, 500,{goE,goE,goE,goE}},
 {0x0C,3000,{goE,goE,waitE,waitE}},
 {0x14, 500,{goN,goN,goN,goN}}};
uint32_t S;  // index to the current state
uint32_t Input;
int main(void){ volatile uint32_t delay;
  PLL_Init();       // 80 MHz, Program 5.1
  SysTick_Init();   // Program 5.2
  SYSCTL_RCGCGPIO_R |= 0x12;   // B E
  delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
  GPIO_PORTE_DIR_R &= ~0x03;   // inputs on PE1-0
  GPIO_PORTE_DEN_R |= 0x03;    // enable digital on PE1-0
  GPIO_PORTB_DIR_R |= 0x3F;    // outputs on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;    // enable digital on PB5-0
  S = goN;  
  while(1){
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R&0xC0)|FSM[S].Out;  // set lights
    SysTick_Wait10ms(FSM[S].Time);
    Input = GPIO_PORTE_DATA_R&0x03;   // read sensors
    S = FSM[S].Next[Input]; 
  }
}