;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Alfred Lohmann
;* Version            : V1.0
;* Date               : 15.07.2012
;* Description        : This is a simple main.
;					  : The output is send to UART 1. Open Serial Window when 
;					  : when debugging. Select UART #1 in Serial Window selection.
;					  :
;					  : Replace this main with yours.
;
;*******************************************************************************

	IMPORT 	Init_TI_Board		; Initialize the serial line
	;IMPORT	initHW				; Init Timer
	IMPORT	puts				; C output function
	IMPORT	TFT_puts			; TFT output function

;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	
	AREA MyData, DATA, align = 2
	
text	DCB	"Hallo TI-Labor.\n\r",0

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************

	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; main subroutine
;--------------------------------------------
	EXPORT main [CODE]
	
main	PROC

		BL	Init_TI_Board	; Initialize the serial line to TTY
							; for compatability to out TI-C-Board
		;BL	initHW			; Timer init
		
		LDR	r0,=text
		BL	puts			; call C output method
		
		LDR r0,=text
		BL	TFT_puts		; call TFT output method
		
		
forever	b	forever		; nowhere to retun if main ends		
		ENDP
	
		ALIGN
       
		END
		