;***********************************************************
;*
;*	Corwin_Perren_Lab4_challengecode.asm
;*
;*	This program loads a two line string from program memory,
;*  then cycles it around the lines of the display.
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
.def	mpr = r16				; Multipurpose register is
.def	mpr2 = r10
								
.def	dataloopcountreg = r23

.equ	WTime = 25				; Time to wait in wait loop

.equ	datamemstart = 0x0100

.equ	stringlen = 32

;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg						; Beginning of code segment

;***********************************************************
;*	Interrupt Vectors
;***********************************************************
.org	$0000					; Beginning of IVs
		rjmp INIT			; Reset interrupt

.org	$0046					; End of Interrupt Vectors

;***********************************************************
;*	Program Initialization
;***********************************************************
INIT:							; The initialization routine
	; Initialize Stack Pointer
	ldi		mpr, low(RAMEND)	; Load the low and high bytes of ram end to the stack pointer
	out		SPL, mpr			
	ldi		mpr, high(RAMEND)
	out		SPH, mpr

	; Initialize LCD Display
	rcall LCDInit				; Call the lcd init function			

	; Move strings from Program Memory to Data Memory
	ldi	ZL, low(STRING_BEG << 1)	; Low byte of first byte in string into ZL 
	ldi ZH, high(STRING_BEG << 1)   	; High byte of first byte in string into ZH 

	ldi YL, low(datamemstart)		; Low byte of data memory start into YL
	ldi YH, high(datamemstart)

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
		
	rcall	LCDWrite		; This writes the data mem to the display, 
; based on fixed mem addresses in LCDDriver.asm
		
	ldi	mpr, WTime		; Copied from lab 1, copy time into general reg
	mov	wait, mpr		; Copy into wait reg
		
	rcall	AVRWait			; Call wait subroutine
	rcall	ROTATE_TEXT		; Call subroutine to rotate text by one char
		
	rjmp	MAIN			; jump back to main and create an infinite
					; while loop.  Generally, every main program is an
					; infinite while loop, never let the main program
					; just run off

;-----------------------------------------------------------
; Func: ROTATE_TEXT
; Desc: This rotates a text string by one character, looping around the end
;-----------------------------------------------------------
ROTATE_TEXT:							; Begin a function with a label
		; Save variables by pushing them to the stack
		push mpr
		push mpr2
		push dataloopcountreg
		push XL
		push XH

		; Get pointer to start of data memory for the lcd
		ldi XL, low(datamemstart)
		ldi XH, high(datamemstart)
		
		ld mpr, X+ 				; Get the first element, point to next

		; Initialize counter
		ldi dataloopcountreg, stringlen	; Initialize count for loop to string length
		dec dataloopcountreg 			; The last one we want to take care of
; specifically, so dec to skip the first char

ROTATE_TEXT_LOOP:
		ld mpr2, X		; Save value in current text mem location
		st X, mpr		; Overwrite current text mem with char from prev location
		inc XL			; Move to next byte location
		mov mpr, mpr2		; Move stored prev char value into main mpr for overwrite
		dec dataloopcountreg		; Dec loop counter
		brne ROTATE_TEXT_LOOP		; If not done with all chars, loop

		; Go back to start
		ldi XL, low(datamemstart)   	; Repoint to beginning of string array
		ldi XH, high(datamemstart)

		; Store last element as first
		st X, mpr			; Overwrite first char with previous last character
		
		; Restore variables by popping them from the stack,
		; in reverse order
		pop XH
		pop XL
		pop dataloopcountreg
		pop mpr2
		pop mpr

		ret						; End a function with RET

; This was copied and modified from lab 1
;----------------------------------------------------------------
; Sub:	AVRWait
; Desc:	A wait loop that is 16 + 159975*waitcnt cycles or roughly 
;		waitcnt*10ms.  Just initialize wait for the specific amount 
;		of time in 10ms intervals. Here is the general eqaution
;		for the number of clock cycles in the wait loop:
;			((3 * ilcnt + 3) * olcnt + 3) * waitcnt + 13 + call
;----------------------------------------------------------------
AVRWait:
		push	wait			; Save wait register
		push	count			; Save ilcnt register
		push	line			; Save olcnt register

Loop:	ldi		line, 224		; load olcnt register
OLoop:	ldi		count, 237		; load ilcnt register
ILoop:	dec		count			; decrement ilcnt
		brne	ILoop			; Continue Inner Loop
		dec		line		; decrement olcnt
		brne	OLoop			; Continue Outer Loop
		dec		wait		; Decrement wait 
		brne	Loop			; Continue Wait loop	

		pop		line		; Restore olcnt register
		pop		count		; Restore ilcnt register
		pop		wait		; Restore wait register
		ret				; Return from subroutine

;***********************************************************
;*	Stored Program Data
;***********************************************************

;-----------------------------------------------------------
; An example of storing a string. Note the labels before and
; after the .DB directive; these can help to access the data
;-----------------------------------------------------------
STRING_BEG:
.DB		" Corwin Perren    Hello World!  "		; Declaring data in ProgMem
STRING_END:

;***********************************************************
;*	Additional Program Includes
;***********************************************************
.include "LCDDriver.asm"		; Include the LCD Driver
