;***********************************************************
;*
;*	Corwin_Perren_Lab7_sourcecode.asm
;*
;*	This program allows for speed control of the tekbot
;*  by using timer counters, with adjustment via buttons
;*
;*	This is the skeleton file for Lab 7 of ECE 375
;*
;***********************************************************
;*
;*	 Author: Corwin Perren
;*	   Date: 11/7/2018
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
.def	speed_reg = r20			; Store of speed
.def	mpr2 = r21

.equ	WTime = 15				; Button debounce time

.equ	max_level = 15			; Max speed level
.equ	min_level = 0			; Min speed level
.equ	speed_multiplier = 17	; Mutiplier for timer

.equ	sd_mask = 0b00001111	; Pins for speed display [B]
.equ	dr_mask = 0b11110000	; Pins for driving [B]

.equ	EngEnR = 4				; Right Engine Enable Bit [B]
.equ	EngEnL = 7				; Left Engine Enable Bit [B]
.equ	EngDirR = 5				; Right Engine Direction Bit [B]
.equ	EngDirL = 6				; Left Engine Direction Bit [B]	

.equ	upMax = 0				; Set speed to max bit [D]
.equ	upInc = 1				; Increase speed by one bit [D]
.equ	downInc = 2				; Decrease speed by one bit [D]
.equ	downMin = 3				; Decrease speed to min bit [D]

.equ	InterruptFlagRegisterClear = (1 << INTF0) | (1 << INTF1) | (1 << INTF2) | (1 << INTF3)

;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; beginning of code segment

;***********************************************************
;*	Interrupt Vectors
;***********************************************************
.org	$0000
		rjmp	INIT			; reset interrupt

		; place instructions in interrupt vectors here, if needed
.org INT0addr
	rjmp SPEEDMAX

.org INT1addr
	rjmp INCSPEED

.org INT2addr
	rjmp DECSPEED

.org INT3addr
	rjmp SPEEDMIN

.org	$0046					; end of interrupt vectors

;***********************************************************
;*	Program Initialization
;***********************************************************
INIT:
		; Initialize the Stack Pointer
		ldi		mpr, low(RAMEND)	; Init the 2 stack pointer registers
		out		SPL, mpr			
		ldi		mpr, high(RAMEND)
		out		SPH, mpr
		

		; Configure I/O ports	
		; Set drive pins and led display to outputs	
		ldi mpr, ((1 << EngEnR) | (1 << EngDirR) | (1 << EngEnL) | (1 << EngDirL) | sd_mask)
		out DDRB, mpr
		
		; Initialize Port D pins for input
		ldi mpr, ((0 << upMax) | (0 << upInc) | (0 << downInc) | (0 << downMin)) ; Not necessary, just being explicit
		out DDRD, mpr

		; Set input pins to enable pullups
		ldi mpr, ((1 << upMax) | (1 << upInc) | (1 << downInc) | (1 << downMin))
		out PORTD, mpr

		; Configure External Interrupts, if needed
		; Enable falling edge pin change interrupts on all input pins
		ldi mpr, ((1 << ISC01) | (1 << ISC11) | (1 << ISC21) | (1 << ISC31))
		sts EICRA, mpr

		; Set mask to enable the four interrupts
		ldi mpr, ((1 << INT0) | (1 << INT1) | (1 << INT2) | (1 << INT3))
		out EIMSK, mpr

		; Configure 8-bit Timer/Counters
		; Setup timer/counter 0
		ldi mpr, ((1 << WGM00) | (1 << COM01) | (1 << COM00) | (1 << WGM01) | ( 1 << CS00) | ( 1 << CS01) | ( 1 << CS02))
		out TCCR0, mpr

		; Setup timer/counter 2
		ldi mpr, ((1 << WGM20) | (1 << COM21) | (1 << COM20) | (1 << WGM21) | ( 1 << CS20) | ( 1 << CS22))
		out TCCR2, mpr

		; Set TekBot to Move Forward (1<<EngDirR|1<<EngDirL)
		ldi	mpr, (1 << EngDirR | 1 << EngDirL)
		out	PORTB, mpr

		; Set initial speed, display on Port B pins 3:0
		ldi speed_reg, min_level  ; Gets displayed on first loop run

		; Enable global interrupts (if any are used)
		sei

;***********************************************************
;*	Main Program
;***********************************************************
MAIN:
		in mpr, PINB				; Load current drive state
		ldi mpr2, dr_mask			; Load drive mask
		and mpr, mpr2				; And to clear all speed pins
		
		or mpr, speed_reg			; Or to set new speed display

		out PORTB, mpr				; Write display

		ldi mpr2, speed_multiplier	; Load multiplier
		mul speed_reg, mpr2			; Multiply for real speed

		out OCR0, r0				; Load low byte into counter
		out OCR2, r0				; Load low byte into other counter

		rjmp	MAIN				; Return to top of MAIN

;***********************************************************
;*	Functions and Subroutines
;***********************************************************
;-----------------------------------------------------------
; Func:	SPEEDMAX (ISR)
; Desc:	Sets the tekbot speed to its max, full movement
;-----------------------------------------------------------
SPEEDMAX:
		cli

		; If needed, save variables by pushing to the stack
		push waitcnt
		push mpr

		; Execute the function here
		ldi speed_reg, max_level

		; Wait for button to stabilize
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		; Restore any saved variables by popping from stack
		pop mpr
		pop waitcnt

		sei
		reti						; End a function with RET

;-----------------------------------------------------------
; Func:	INCSPEED (ISR)
; Desc:	Increases tekbot speed by 1, if not at max
;-----------------------------------------------------------
INCSPEED:	; Begin a function with a label
		cli

		; If needed, save variables by pushing to the stack
		push mpr
		push waitcnt

		; Execute the function here
		ldi mpr, max_level
		cpse speed_reg, mpr
		inc speed_reg
		
		; Wait for button to stabilize
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		; Restore any saved variables by popping from stack
		pop waitcnt
		pop mpr

		sei
		reti						; End a function with RET

;-----------------------------------------------------------
; Func:	DECSPEED (ISR)
; Desc:	Decreases tekbot speed by one, if not at min
;-----------------------------------------------------------
DECSPEED:	; Begin a function with a label
		cli

		; If needed, save variables by pushing to the stack
		push mpr
		push waitcnt

		; Execute the function here
		ldi mpr, min_level
		cpse speed_reg, mpr
		dec speed_reg
		
		; Wait for button to stabilize
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		; Restore any saved variables by popping from stack
		pop waitcnt
		pop mpr

		sei
		reti						; End a function with RET

;-----------------------------------------------------------
; Func:	SPEEDMIN (ISR)
; Desc:	Sets tekbot speed to min, no movement
;-----------------------------------------------------------
SPEEDMIN:	; Begin a function with a label
		cli

		; If needed, save variables by pushing to the stack
		push waitcnt
		push mpr

		; Execute the function here
		ldi speed_reg, min_level
		
		; Wait for button to stabilize
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		; Restore any saved variables by popping from stack
		pop mpr
		pop waitcnt

		sei
		reti						; End a function with RET

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