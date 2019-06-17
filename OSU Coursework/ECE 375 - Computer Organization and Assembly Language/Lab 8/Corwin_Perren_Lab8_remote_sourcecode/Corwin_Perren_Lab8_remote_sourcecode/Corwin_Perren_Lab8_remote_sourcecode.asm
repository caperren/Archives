;***********************************************************
;*
;*	Enter Name of file here
;*
;*	Enter the description of the program here
;*
;*	This is the TRANSMIT skeleton file for Lab 8 of ECE 375
;*
;***********************************************************
;*
;*	 Author: Enter your name
;*	   Date: Enter Date
;*
;***********************************************************

.include "m128def.inc"			; Include definition file

;***********************************************************
;*	Internal Register Definitions and Constants
;***********************************************************
.def	mpr = r16				; Multi-Purpose Register
.def	waitcnt = r17			; Wait Loop Counter
.def	ilcnt = r18				; Inner Loop Counter
.def	olcnt = r19				; Outer Loop Counter
.def	speed_reg = r20			; Store of speed
.def	command_reg = r21		; Reg to store current command

.equ	WTime = 15				; Button debounce time

.equ	EngEnR = 4				; Right Engine Enable Bit
.equ	EngEnL = 7				; Left Engine Enable Bit
.equ	EngDirR = 5				; Right Engine Direction Bit
.equ	EngDirL = 6				; Left Engine Direction Bit

.equ	BotAddress = 0x1a		; (Enter your robot's address here (8 bits))

; Use these action codes between the remote and robot
; MSB = 1 thus:
; control signals are shifted right by one and ORed with 0b10000000 = $80
.equ	MovFwdCmd  =  ($80|1<<(EngDirR-1)|1<<(EngDirL-1))	;0b10110000 Move Forward Action Code
.equ	MovBckCmd  =  ($80|$00)								;0b10000000 Move Backward Action Code
.equ	TurnRCmd  =   ($80|1<<(EngDirL-1))					;0b10100000 Turn Right Action Code
.equ	TurnLCmd  =   ($80|1<<(EngDirR-1))					;0b10010000 Turn Left Action Code
.equ	HaltCmd  =    ($80|1<<(EngEnR-1)|1<<(EngEnL-1))		;0b11001000 Halt Action Code
.equ	FreezeCmd  =  0b11111000							;0b11111000 Freeze Action Code

; Input pins for sending control commands
; Note that I had to avoid using the pin
; that is the transmitter on port D
.equ	MoveFwdPin = 0	
.equ	MoveBackPin = 1
.equ	MoveLeftPin = 7
.equ	MoveRightPin = 6
.equ	MoveHaltPin = 5
.equ	FreezePin = 4

; Outputs on port D
.equ	TXD1 = 3

; Store the upper and lower bytes for ubrr setting
.equ	ubrr_low = low(832)
.equ	ubrr_high = high(832)

;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; Beginning of code segment

;***********************************************************
;*	Interrupt Vectors
;***********************************************************
.org	$0000					; Beginning of IVs
		rjmp 	INIT			; Reset interrupt

.org	$0046					; End of Interrupt Vectors

;***********************************************************
;*	Program Initialization
;***********************************************************
INIT:
	; Initialize the Stack Pointer
	ldi		mpr, low(RAMEND)	; Init the 2 stack pointer registers
	out		SPL, mpr			
	ldi		mpr, high(RAMEND)
	out		SPH, mpr

	; I/O Ports
	; Initialize Port D pins for input
	ldi mpr, ((0 << MoveFwdPin) | (0 << MoveBackPin) | (0 << MoveLeftPin) | (0 << MoveRightPin) | (0 << MoveHaltPin) | (1 << TXD1))
	out DDRD, mpr

	; Enable pullups on input pins
	ldi mpr, ((1 << MoveFwdPin) | (1 << MoveBackPin) | (1 << MoveLeftPin) | (1 << MoveRightPin) | (1 << MoveHaltPin) | (1 << FreezePin))
	out PORTD, mpr

	; USART1
	; Set baudrate at 2400bps
	ldi mpr, ubrr_low
	sts UBRR1L, mpr

	ldi mpr, ubrr_high
	sts UBRR1H, mpr

	; Enable transmitter
	ldi mpr, ((1 << TXEN1) | (1 << RXEN1))
	sts UCSR1B, mpr
	
	; Set frame format: 8 data bits, 2 stop bits
	; Double data rate 
	ldi mpr, (1 << U2X1)
	sts UCSR1A, mpr

	; 8N2 setting
	ldi mpr, ((1 << UCSZ10) | (1 << UCSZ11) | (1 << USBS1))
	sts UCSR1C, mpr

;***********************************************************
;*	Main Program
;***********************************************************
MAIN:
	in mpr, PIND				; Get current state of button pins

	sbrs mpr,  MoveFwdPin		; Check if forward pin pressed
	rcall SendMoveFwd			; Send move forward if pressed

	sbrs mpr,  MoveBackPin		; Check if back pin pressed
	rcall SendMoveBack			; Send move back if pressed

	sbrs mpr,  MoveLeftPin		; Check if left pin pressed
	rcall SendMoveLeft			; Send move left if pressed

	sbrs mpr,  MoveRightPin		; Check if right pin pressed
	rcall SendMoveRight			; Send move right if pressed

	sbrs mpr,  MoveHaltPin		; Check if halt pin pressed
	rcall SendMoveHalt			; Send move halt if pressed

	sbrs mpr,  FreezePin		; Check if freeze pin pressed
	rcall SendFreeze			; Send move freeze if pressed

	; Some debounce time for any button presses
	ldi		waitcnt, WTime		; Wait for 150 milliseconds
	rcall	Wait				; Call wait function

	rjmp MAIN

;***********************************************************
;*	Functions and Subroutines
;***********************************************************
SendAddress:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendAddress			; Jump back until it is

	ldi mpr, BotAddress			; Load the bot address
	sts UDR1, mpr				; Send the address over serial
	ret

SendCommand:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendCommand			; Jump back until it is

	sts UDR1, command_reg		; Send command over serial
	ret

SendMoveFwd:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendMoveFwd			; Jump back until it is

	rcall SendAddress			; Send the address

	ldi command_reg, MovFwdCmd 	; Load move forward command
	rcall SendCommand			; Call the subroutine to send it

	ldi mpr, 0xFF				; Set mpr to max val so next loop can set it
	ret

SendMoveBack:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendMoveFwd			; Jump back until it is

	rcall SendAddress			; Send the address

	ldi command_reg, MovBckCmd 	; Load move back command
	rcall SendCommand			; Call the subroutine to send it

	ldi mpr, 0xFF				; Set mpr to max val so next loop can set it
	ret

SendMoveLeft:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendMoveFwd			; Jump back until it is

	rcall SendAddress			; Send the address

	ldi command_reg, TurnLCmd 	; Load turn left command
	rcall SendCommand			; Call the subroutine to send it

	ldi mpr, 0xFF				; Set mpr to max val so next loop can set it
	ret

SendMoveRight:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendMoveFwd			; Jump back until it is

	rcall SendAddress			; Send the address

	ldi command_reg, TurnRCmd 	; Load turn right command
	rcall SendCommand			; Call the subroutine to send it

	ldi mpr, 0xFF				; Set mpr to max val so next loop can set it
	ret

SendMoveHalt:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendMoveFwd			; Jump back until it is

	rcall SendAddress			; Send the address

	ldi command_reg, HaltCmd 	; Load halt command
	rcall SendCommand			; Call the subroutine to send it

	ldi mpr, 0xFF				; Set mpr to max val so next loop can set it
	ret

SendFreeze:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendFreeze				; Jump back until it is

	rcall SendAddress			; Send the address

	ldi command_reg, FreezeCmd 	; Load halt command
	rcall SendCommand			; Call the subroutine to send it

	ldi mpr, 0xFF				; Set mpr to max val so next loop can set it
	ret

Wait:
		push	waitcnt			; Save wait register
		push	ilcnt			; Save ilcnt register
		push	olcnt			; Save olcnt register

Loop:	ldi		olcnt, 224		; load olcnt register
OLoop:	ldi		ilcnt, 237		; load ilcnt register
ILoop:	dec		ilcnt			; decrement ilcnt
		brne	ILoop			; Continue Inner Loop
		dec		olcnt			; decrement olcnt
		brne	OLoop			; Continue Outer Loop
		dec		waitcnt			; Decrement wait
		brne	Loop			; Continue Wait loop

		pop		olcnt			; Restore olcnt register
		pop		ilcnt			; Restore ilcnt register
		pop		waitcnt			; Restore wait register
		ret						; Return from subroutine

