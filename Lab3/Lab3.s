; Dinh nghia cac dia chi thanh ghi 
GPIOA_BASE  EQU 0x40010800			; Dia chi co so PortA
RCC_BASE 	EQU 0x40021000			; Dia chi co so reset and clock 
GPIOA_CRL   EQU GPIOA_BASE + 0x00   ; Dia chi thanh ghi CRL
GPIOA_ODR   EQU GPIOA_BASE + 0x0C   ; Ðia chi thanh ghi ODR (output data register)
GPIOA_IDR   EQU GPIOA_BASE + 0x08   ; Ðia chi thanh ghi IDR (input data register)	
RCC_APB2ENR EQU RCC_BASE   + 0x18		; page  146 reference manual Bat clock

; Dinh nghia cac bien 
total_cycle         		EQU 500000  ; corresponding 0.5s = 5.10^8ns
duty_cycle_default_percent  EQU 3	    ; corresponding 30%
duty_cycle_increase_percent EQU 2       ; corresponding 20%
breath_cycle				EQU	10		; corresponding 0,01x10^9(ns) = 0,01s note
	
	

; Khai bao vung ma lenh
		AREA Lab3, CODE, READONLY
		ENTRY	; diem di vào trong code 
		EXPORT __main
		
__main
; Khoi tao
	;Bat bus cho PortA
    LDR R0, =RCC_APB2ENR   
    LDR R1, =0x00000004  ;bat clock PortA   
    STR R1, [R0]
	
	;Bat cau hinh thanh ghi PortA
    LDR R0, =GPIOA_CRL  
    LDR R1, =0x00020808  ;cau hinh cac chan PA0,PA2 input:MODE = 00 và CNF = 10 pullup or pulldown
						 ;cau hinh chan PA4 output: MODE = 11 max speed 50MHz và CNF = 00 general push-pull
    STR R1, [R0]
	
	
;---------------------------------
;	FUNCTION LOOP
;---------------------------------
loop  
; main engine goes here
	; change led
	  BL   LED_On				
      BL   delay_duty_cycle
      BL   LED_Off
      BL   delay_relax_cycle
	; breath led
	  BL   breath_handle_status
	  B    loop

;---------------------------------
;	FUNCTION TOGGLE LED
;---------------------------------
LED_On
	 LDR R1,=GPIOA_ODR
     LDR R0,[R1]
     ORR R0,R0,#0x10 ; bit 1 high
     STR R0,[R1]
     BX LR
LED_Off
     LDR R1,=GPIOA_ODR
     LDR R0,[R1]
     BIC R0,R0,#0x10 ; bit 1 low
     STR R0,[R1]
     BX LR

;---------------------------------
;	FUNCTION DELAY
;---------------------------------
delay_ns
	 NOP
     NOP
	 NOP
	 NOP
	 SUBS R0, R0, #1
	 BNE delay_ns
	 BX LR	 
delay_duty_cycle 
	 PUSH{LR}
	 BL  change_handle_status
	 CMP R4, #0
	 BEQ default_duty_cycle					;default_duty_cycle
	 LDR R0, =total_cycle					;no cahnge R0=50000000=5*10^7
	 MUL R0, R0, R4							;R0=5*10^7*3=1.5x10^8
	 BL delay_ns
	 POP{LR}
	 BX LR 
delay_relax_cycle
	 PUSH{LR}
	 BL  change_handle_status
	 CMP R4, #0
	 BEQ default_duty_cycle					;default_duty_cycle
	 LDR R0, =total_cycle					;no change R0=50000000=5*10^7
	 LDR R2, =10							; no change R1=10 ~100%
	 SUBS R2, R2, R4						;R1=10-3=7 ~70%
	 MUL R0, R0, R2 						;R0=5*10^7*7=1.5x10^8
	 BL delay_ns
	 POP{LR}
	 BX LR
	 
;---------------------------------
;	FUNCTION CHANGE_HANDLE_STATUS
;---------------------------------
change_handle_status						;Start Function
    PUSH {LR}								;Store in stack beacause use subroutines
    BL   change_read_status
    CMP  R1, #0
    BNE  change_wait_release
	B	 change_end
change_wait_release
    BL   change_read_status
    CMP  R1, #0
    BNE  change_wait_release 				
	BL   increase_duty_cycle
change_end	
    POP  {LR}								;Retrieve stack 
    BX LR									;End Function and return function call 
	
change_read_status							;read pin state 1 or 0
    LDR R0, =GPIOA_IDR
    LDR R1, [R0]
    AND R1, R1, #0x01 						;0x01(press) or 0(release) 			
    BX LR									;return function call	
increase_duty_cycle							;Start increase								
    LDR  R2, =duty_cycle_increase_percent
    ADD  R4, R4, R2 ; 
    CMP  R4, #10
    BHS  duty_cycle_modulo					;r4>10 and then do duty_cycle_modulo
	BX   LR									;End increase and return function call
duty_cycle_modulo
    SUBS R4, R4, #10 						;R4 = R4 - 100 (modulo 100)
    BX   LR									
default_duty_cycle
	LDR R4, =duty_cycle_default_percent 
	BX LR

	
;---------------------------------
;	FUNCTION BREATH LED
;---------------------------------
;Su dung cac thanh ghi R5,6,7,8 de
breath_handle_status
	PUSH{LR}
	BL breath_read_status
	CMP R1, #0
	BNE read_array
	B breath_end
read_array	
    LDR R5, =0      		;
loop_array					;
	CMP R5, #200			; 200 byte
    BHS read_array			;  
    LDR R6, =SinTable  		; R6 = &SinTable
    LDRH R7, [R5, R6]    	; R7 = SinTable[R5] luu 2byte thap
    BL LED_On				;
    BL breath_duty_cycle	;
    BL LED_Off				;
    BL breath_relax_cycle	;
    ADD R5, R5, #2     		; tang them 2byte =16bit
	BL breath_read_status	;	
	CMP R1, #0				;
	BNE loop_array         	;
breath_end					;
    POP {LR}				;
    BX LR					;

	

breath_read_status			;
	LDR R0, =GPIOA_IDR		;
	LDR R1, [R0]			;
	AND R1, R1, #0x04		; 0x04(press) or 0(release) 
	BX  LR					;
breath_duty_cycle			;
	PUSH{LR}				;
	LDR R0, =breath_cycle	;
	MUL R0, R0, R7			; R0=5*10^7*3=1.5x10^8
	BL delay_ns				;
	POP{LR}					;
	BX LR 					;
breath_relax_cycle			;
	PUSH{LR}				;
	LDR R0, =breath_cycle	; 
	LDR R2, =10000		    ; 
	SUBS R2, R2, R7			; 
	MUL R0, R0, R2 		    ; 
	BL delay_ns
	POP{LR}
	BX LR
	
	LTORG	; Cap phat bo nho cho bien va hang so 

	     
 ALIGN 4   
; 256 points with values from 100 to 9900      
PerlinTable
     DCW 5880,6225,5345,3584,3545,674,5115,598,7795,3737,3775,2129,7527,9020,368,8713,5459,1478,4043,1248,2741,5536,406
     DCW 3890,1516,9288,904,483,980,7373,330,5766,9555,4694,9058,2971,100,1095,7641,2473,3698,9747,8484,7871,4579,1440
     DCW 521,1325,2282,6876,1363,3469,9173,5804,2244,3430,6761,866,4885,5306,6646,6531,2703,6799,2933,6416,2818,5230,5421
     DCW 1938,1134,6455,3048,5689,6148,8943,3277,4349,8866,4770,2397,8177,5191,8905,8522,4120,3622,1670,2205,1861,9479
     DCW 1631,9441,4005,5574,2167,2588,1057,2512,6263,138,8369,3163,2895,8101,3009,5153,7259,8063,3507,789,6570,7756,7603
     DCW 5268,5077,4541,7297,6187,3392,6378,3928,4273,7680,6723,7220,215,2550,2091,8407,8752,9670,4847,4809,291,7833,1555
     DCW 5727,4617,4923,9862,3239,3354,8216,8024,7986,2359,8790,1899,713,2320,751,7067,7335,1172,1708,8637,7105,6608,8254
     DCW 4655,9594,5919,177,1784,5995,6340,2780,8560,5957,3966,6034,6493,1746,6684,445,5038,942,1593,9785,827,3852,4234
     DCW 4311,3124,4426,8675,8981,6914,7182,4388,4081,8445,9517,3813,8828,9709,1402,9364,7488,9211,8139,5613,559,7412
     DCW 6952,6302,9326,3201,2052,5651,9096,9632,636,9249,4196,1976,7450,8292,1287,7029,7718,4158,6110,7144,3316,7909
     DCW 6838,4502,4732,2014,1823,4962,253,5842,9823,5383,9134,7948,3660,8598,4464,2665,1210,1019,2856,9402,5498,5000
     DCW 7565,3086,2627,8330,2435,6072,6991
; 100 numbers from 0 to 10000
; sinusoidal shape
  ALIGN 4
SinTable 
  DCW  5000, 5308, 5614, 5918, 6219, 6514, 6804, 7086, 7361, 7626
  DCW  7880, 8123, 8354, 8572, 8776, 8964, 9137, 9294, 9434, 9556
  DCW  9660, 9746, 9813, 9861, 9890, 9900, 9890, 9861, 9813, 9746
  DCW  9660, 9556, 9434, 9294, 9137, 8964, 8776, 8572, 8354, 8123
  DCW  7880, 7626, 7361, 7086, 6804, 6514, 6219, 5918, 5614, 5308
  DCW  5000, 4692, 4386, 4082, 3781, 3486, 3196, 2914, 2639, 2374
  DCW  2120, 1877, 1646, 1428, 1224, 1036,  863,  706,  566,  444
  DCW   340,  254,  187,  139,  110,  100,  110,  139,  187,  254
  DCW   340,  444,  566,  706,  863, 1036, 1224, 1428, 1646, 1877
  DCW  2120, 2374, 2639, 2914, 3196, 3486, 3781, 4082, 4386, 4692  
      
     ALIGN      ; make sure the end of this section is aligned		 
    END
               