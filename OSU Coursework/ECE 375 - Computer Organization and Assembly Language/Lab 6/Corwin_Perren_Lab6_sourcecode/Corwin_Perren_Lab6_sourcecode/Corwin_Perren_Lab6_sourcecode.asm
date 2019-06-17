;***********************************************************
;*
;*	Corwin_Perren_Lab6_sourcecode.asm
;*
;*	This program uses external pin interrupts to emulate
;* 	the polling bumpbot routine.
;*
;*	This is the skeleton file for Lab 6 of ECE 375
;*
;***********************************************************
;*
;*	 Author: Corwin Perren
;*	   Date: 10/31/18
;*
;***********************************************************

.include "m128def.inc"			; Include definition file

;***********************************************************
;*	Internal Register Definitions and Constants
;***********************************************************
.def	mpr = r16				; Multipurpose register
.def	waitcnt = r17				; Wait Loop Counter
.def	ilcnt = r18				; Inner Loop Counter
.def	olcnt = r19				; Outer Loop Counter

.equ	WTime = 100				; Time to wait in wait loop, used to be 100 

.equ	WskrR = 0				; Right Whisker Input Bit [D]
.equ	WskrL = 1				; Left Whisker Input Bit [D]
.equ	EngEnR = 4				; Right Engine Enable Bit [B]
.equ	EngEnL = 7				; Left Engine Enable Bit [B]
.equ	EngDirR = 5				; Right Engine Direction Bit [B]
.equ	EngDirL = 6				; Left Engine Direction Bit [B]	

; Bits 1 and 3 set for INT0 and INT1 trigger on falling edge
.equ	InterruptsFallingEdge = (1 << ISC01) | (1 << ISC11)

; Bits 0 and 1 set for INT0 and INT1 interrupts enabled
.equ	InterruptMasksEnabled = (1 << INT0) | (1 << INT1)		
.equ	InterruptMasksDisabled = 0

; Setting these bits to one and writing them to the flag register clears them
.equ	InterruptFlagRegisterClear = (1 << INTF0) | (1 << INTF1)

;/////////////////////////////////////////////////////////////
;These macros are the values to make the TekBot Move.
;/////////////////////////////////////////////////////////////

.equ	MovFwd = (1<<EngDirR|1<<EngDirL)	; Move Forward Command
.equ	MovBck = $00				; Move Backward Command
.equ	TurnR = (1<<EngDirL)			; Turn Right Command
.equ	TurnL = (1<<EngDirR)			; Turn Left Command
.equ	Halt = (1<<EngEnR|1<<EngEnL)		; Halt Command

;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; Beginning of code segment

;***********************************************************
;*	Interrupt Vectors
;***********************************************************
.org	$0000					; Beginning of IVs
		rjmp 	INIT			; Reset interrupt

		; Set up interrupt vectors for any interrupts being used
.org INT0addr
	rcall HitRight
	reti

.org INT1addr
	rcall HitLeft
	reti
		; This is just an example:
;.org	$002E					; Analog Comparator IV
;		rcall	HandleAC		; Call function to handle interrupt
;		reti					; Return from interrupt

.org	$0046					; End of Interrupt Vectors

;***********************************************************
;*	Program Initialization
;***********************************************************
INIT:							; The initialization routine
		; Initialize Stack Pointer
		ldi		mpr, low(RAMEND)	; Init the 2 stack pointer registers
		out		SPL, mpr			
		ldi		mpr, high(RAMEND)
		out		SPH, mpr
		
		; Initialize Port B for output
		ldi mpr, ((1 << EngEnR) | (1 << EngDirR) | (1 << EngEnL) | (1 << EngDirL))
		out DDRB, mpr
		
		; Initialize Port D for input
		ldi mpr, ((0 << WskrR) | (0 << WskrL)) ; Not necessary, just being explicit
		out DDRD, mpr

		; Set pins to enable pullups
		ldi mpr, ((1 << WskrR) | (1 << WskrL))
		out PORTD, mpr

		; Initialize external interrupts
		; D0 is INT0, D1 is INT1
		; Set the Interrupt Sense Control to falling edge 
		ldi mpr, InterruptsFallingEdge
		sts EICRA, mpr

		; Configure the External Interrupt Mask
		ldi mpr, InterruptMasksEnabled
		out EIMSK, mpr

		; Turn on interrupts
		sei
		; NOTE: This must be the last thing to do in the INIT function

;***********************************************************
;*	Main Program
;***********************************************************
MAIN:							; The Main program

		; Initialize TekBot Forward Movement
		ldi	mpr, MovFwd		; Load Move Forward Command
		out	PORTB, mpr		; Send command to motors
		rjmp	MAIN			; Create an infinite while loop to signify the 
						; end of the program.

;***********************************************************
;*	Functions and Subroutines
;***********************************************************

;-----------------------------------------------------------
;	You will probably want several functions, one to handle the 
;	left whisker interrupt, one to handle the right whisker 
;	interrupt, and maybe a wait function
;------------------------------------------------------------

;----------------------------------------------------------------
; Sub:	HitRight
; Desc:	Handles functionality of the TekBot when the right whisker
;		is triggered.
;----------------------------------------------------------------
HitRight:
		cli

		push	mpr			; Save mpr register
		push	waitcnt			; Save wait register
		in		mpr, SREG	; Save program state
		push	mpr			;

		; Move Backwards for a second
		ldi		mpr, MovBck	; Load Move Backward command
		out		PORTB, mpr	; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Turn left for a second
		ldi		mpr, TurnL	; Load Turn Left Command
		out		PORTB, mpr	; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Move Forward again
		ldi		mpr, MovFwd	; Load Move Forward command
		out		PORTB, mpr	; Send command to port

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		pop		mpr		; Restore program state
		out		SREG, mpr	;
		pop		waitcnt		; Restore wait register
		pop		mpr		; Restore mpr

		sei
		ret				; Return from subroutine

;----------------------------------------------------------------
; Sub:	HitLeft
; Desc:	Handles functionality of the TekBot when the left whisker
;		is triggered.
;----------------------------------------------------------------
HitLeft:
		cli

		push	mpr			; Save mpr register
		push	waitcnt			; Save wait register
		in		mpr, SREG	; Save program state
		push	mpr			;

		; Move Backwards for a second
		ldi		mpr, MovBck	; Load Move Backward command
		out		PORTB, mpr	; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Turn right for a second
		ldi		mpr, TurnR	; Load Turn Left Command
		out		PORTB, mpr	; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Move Forward again
		ldi		mpr, MovFwd	; Load Move Forward command
		out		PORTB, mpr	; Send command to port

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		pop		mpr		; Restore program state
		out		SREG, mpr	;
		pop		waitcnt		; Restore wait register
		pop		mpr		; Restore mpr

		sei
		ret				; Return from subroutine

;----------------------------------------------------------------
; Sub:	Wait
; Desc:	A wait loop that is 16 + 159975*waitcnt cycles or roughly
;		waitcnt*10ms.  Just initialize wait for the specific amount
;		of time in 10ms intervals. Here is the general eqaution
;		for the number of clock cycles in the wait loop:
;			((3 * ilcnt + 3) * olcnt + 3) * waitcnt + 13 + call
;----------------------------------------------------------------
Wait:
		push	waitcnt			; Save wait register
		push	ilcnt			; Save ilcnt register
		push	olcnt			; Save olcnt register

Loop:	ldi		olcnt, 224		; load olcnt register
OLoop:	ldi		ilcnt, 237		; load ilcnt register
ILoop:	dec		ilcnt			; decrement ilcnt
		brne	ILoop			; Continue Inner Loop
		dec		olcnt		; decrement olcnt
		brne	OLoop			; Continue Outer Loop
		dec		waitcnt		; Decrement wait
		brne	Loop			; Continue Wait loop

		pop		olcnt		; Restore olcnt register
		pop		ilcnt		; Restore ilcnt register
		pop		waitcnt		; Restore wait register
		ret				; Return from subroutine

;***********************************************************
;*	Stored Program Data
;***********************************************************

; Enter any stored data you might need here

;***********************************************************
;*	Additional Program Includes
;***********************************************************
; There are no additional file includes for this program
