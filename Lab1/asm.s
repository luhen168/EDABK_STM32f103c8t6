GPIOA_BASE  EQU 0x40010800
RCC_BASE 	EQU 0x40021000
GPIOA_CRL   EQU GPIOA_BASE + 0x00   ; Dia chi thanh ghi CRL
GPIOA_CRH   EQU GPIOA_BASE + 0x04   ; Ðia chi thanh ghi CRH
GPIOA_ODR   EQU GPIOA_BASE + 0x0C   ; Ðia chi thanh ghi ODR (output data register)
GPIOA_IDR   EQU GPIOA_BASE + 0x08   ; Ðia chi thanh ghi IDR (input data register)
GPIOA_BSRR  EQU GPIOA_BASE + 0x10   ; Ðia chi thanh ghi BSRR (bit set/reset register)
GPIOA_BRR   EQU GPIOA_BASE + 0x14   ; Ðia chi thanh ghi BRR (bit reset register)		
RCC_APB2ENR EQU RCC_BASE + 0x18		; page  146 reference manual Bat clock
	
		AREA asm, CODE, READONLY
		ENTRY
		EXPORT __main
		
__main
; Khoi tao
	;CNF|MODE(I/O)
	;= xx|xx
    LDR R0, =RCC_APB2ENR   			;Load address clock to R0
    LDR R1, =0x00000004     		;Enable clock A
    STR R1, [R0]					;Store address clock
	;Set pin:
	;Input PA0,1,2: MODE = 00 and CNF = 10(pullup/pulldown)-> 1000(binary) = 8(hexa) 
	;Output PA3: General purpose output push-pull  max speed 50 MHz.
    LDR R0, =GPIOA_CRL  			;Load address configuration register low to R0
    LDR R1, =0x00003888 			
    STR R1, [R0]					;Store address clock
	LDR R0, =GPIOA_ODR				;Load address output data register to R0
	LDR R1, =0x00					;set input PA0,1,2 is pull up by PxODR bit 1 OR pulldown bit 0
	STR R1, [R0]
	

loop
	LDR R0, =GPIOA_IDR
;read state
    LDR R1, [R0]					;Load to R1 corresponding read PA0
    AND R1, R1, #0x01   			;set PA0 is input pull up 

    LDR R2, [R0]					;Load to R1 corresponding read PA1	
    AND R2, R2, #0x02     			;set PA1 is input pull up 
	LSR	R2, R2, #0x01				;logic shift right 1 bit R2>>1 
	
    LDR R3, [R0]					;Load to R1 corresponding read PA2
    AND R3, R3, #0x04				;set PA2 is input pull up
	LSR R3, R3, #0x02				;logic shift right 2 bit R3>>2

;handle_event XOR(R1,R2,R3)
    EOR R4, R2, R1					;R4 =XOR(R2,R1)
    EOR R4, R4, R3					;R4 =XOR(R4,R3)

;make LED to show result
    LDR R0, =GPIOA_ODR     
    LSL R4, R4, #0x03				;logic shift left 3 bit R4<<3 to result in bit 4 pin PA3       
    STR R4, [R0]					;store value in output data register
	NOP
    B loop                 
    END                     

               