;***********************************************************
;*
;*	Corwin_Perren_Lab4_sourcecode.asm
;*
;*	This program loads a two line string from program memory
;*      into data memory, then shows it on an LCD
;*
;*	This is the skeleton file for Lab 4 of ECE 375
;*
;***********************************************************
;*
;*	 Author: Corwin Perren
;*	   Date: 10/17/2018
;*
;***********************************************************

.include "m128def.inc"			; Include definition file

;***********************************************************
;*	Internal Register Definitions and Constants
;***********************************************************
.def	mpr = r16	; Multipurpose register is required for LCD Driver
.def	dataloopcountreg = r23

.equ	datamemstart = 0x0100

.equ	stringlen = 32

;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg				; Beginning of code segment

;***********************************************************
;*	Interrupt Vectors
;***********************************************************
.org	$0000			; Beginning of IVs
		rjmp INIT	; Reset interrupt

.org	$0046			; End of Interrupt Vectors

;***********************************************************
;*	Program Initialization
;***********************************************************
INIT:							; The initialization routine
	; Initialize Stack Pointer
	ldi		mpr, low(RAMEND) 	; Load the low and high bytes of ram end to the 
  	; stack pointer
	out		SPL, mpr			
	ldi		mpr, high(RAMEND)
	out		SPH, mpr

	; Initialize LCD Display
	rcall LCDInit				; Call the lcd init function			

	; Move strings from Program Memory to Data Memory
	ldi	ZL, low(STRING_BEG << 1) 	; Low byte of first byte in string into ZL 
	ldi ZH, high(STRING_BEG << 1)    	; High byte of first byte in string into ZH 

	ldi YL, low(datamemstart)	  	; Low byte of data memory start into YL
	ldi YH, high(datamemstart)		; High byte of data memory start into YH

	ldi dataloopcountreg, stringlen	; Initialize count for loop to string length

INIT_MEMCOPYLOOP:				; Loop to read data from progmem to datamem
	lpm mpr, Z+				; Get byte from address pointed to be Z, 
; store in reg, move to next byte
	
st Y+, mpr				; Store byte from reg into data mem address 
; pointed to by Y, then move Y to next open spot 		
	dec dataloopcountreg			; Decrement count as we're done with byte
	brne INIT_MEMCOPYLOOP			; If we haven't read in the whole string, loop again
			
		
	; NOTE that there is no RET or RJMP from INIT, this
	; is because the next instruction executed is the
	; first instruction of the main program

;***********************************************************
;*	Main Program
;***********************************************************
MAIN:							; The Main program
	; Display the strings on the LCD Display
		
	rcall LCDWrite			; This writes the data mem to the display, based on 
; fixed mem addresses in LCDDriver.asm

	rjmp	MAIN			; jump back to main and create an infinite
					; while loop.  Generally, every main program is an
					; infinite while loop, never let the main program
					; just run off

;***********************************************************
;*	Stored Program Data
;***********************************************************

;-----------------------------------------------------------
; An example of storing a string. Note the labels before and
; after the .DB directive; these can help to access the data
;-----------------------------------------------------------
STRING_BEG:
.DB		" Corwin Perren    Hello World!  "	; Declaring data in ProgMem
STRING_END:

;***********************************************************
;*	Additional Program Includes
;***********************************************************
.include "LCDDriver.asm"		; Include the LCD Driver
