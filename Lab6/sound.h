#ifndef sound_h
#define sound_h
#include <stdint.h>
#define C1   597   // 2093 Hz
#define B1   633   // 1975.5 Hz
#define BF1  670   // 1864.7 Hz
#define A1   710   // 1760 Hz
#define AF1  752   // 1661.2 Hz
#define G1   797   // 1568 Hz
#define GF1  845   // 1480 Hz
#define F1   895   // 1396.9 Hz
#define E1   948   // 1318.5 Hz
#define EF1  1004   // 1244.5 Hz
#define D1   1064   // 1174.7 Hz
#define DF1  1127   // 1108.7 Hz
#define C    1194   // 1046.5 Hz
#define B    1265   // 987.8 Hz
#define BF   1341   // 932.3 Hz
#define A    1420   // 880 Hz
#define AF   1505   // 830.6 Hz
#define G    1594   // 784 Hz
#define GF  1689   // 740 Hz
#define F   1790   // 698.5 Hz
#define E   1896   // 659.3 Hz
#define EF  2009   // 622.3 Hz
#define D   2128   // 587.3 Hz
#define DF  2255   // 554.4 Hz
#define C0  2389   // 523.3 Hz
#define B0  2531   // 493.9 Hz
#define BF0 2681   // 466.2 Hz
#define A0  2841   // 440 Hz
#define AF0 3010   // 415.3 Hz
#define G0  3189   // 392 Hz
#define GF0 3378   // 370 Hz
#define F0  3579   // 349.2 Hz
#define E0  3792   // 329.6 Hz
#define EF0 4018   // 311.1 Hz
#define D0  4257   // 293.7 Hz
#define DF0 4510   // 277.2 Hz
#define C7  4778   // 261.6 Hz
#define B7  5062   // 246.9 Hz
#define BF7 5363   // 233.1 Hz
#define A7  5682   // 220 Hz
#define AF7 6020   // 207.7 Hz
#define G7  6378   // 196 Hz
#define GF7 6757   // 185 Hz
#define F7  7159   // 174.6 Hz
#define E7  7584   // 164.8 Hz
#define EF7 8035   // 155.6 Hz
#define D7  8513   // 146.8 Hz
#define DF7 9019   // 138.6 Hz
#define C6  9556   // 130.8 Hz
static const uint8_t SinWave[64] = {				
  32,35,38,41,44,47,49,52,53,52,51,				
  50,49,48,50,52,53,51,52,52,51,53,				
  51,52,53,52,49,47,44,41,38,35,				
  32,29,26,23,20,17,15,12,10,8,				
  6,5,3,2,2,1,1,1,2,2,3,				
  5,6,8,10,12,15,17,20,23,26,29};		

static uint32_t Index = 0;

void SysTick_Handler(void);
void Sound_Init(void);
void Sound_Start(uint32_t period);

#endif
