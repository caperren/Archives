;***********************************************************
;*
;*	Enter Name of file here
;*
;*	Enter the description of the program here
;*
;*	This is the RECEIVE skeleton file for Lab 8 of ECE 375
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
.def	waitcnt = r17				; Wait Loop Counter
.def	ilcnt = r18				; Inner Loop Counter
.def	olcnt = r19				; Outer Loop Counter
.def	cmd_reg = r20			; Current command reg
.def	addr_reg = r21			; Current address reg
.def	freeze_sent_reg = r22	; State of freeze send
.def	freeze_count_reg = r23	; Freeze count

.equ	WTime = 100				; Time to wait in wait loop, used to be 100 

; Inputs on port d
.equ	WskrR = 0				; Right Whisker Input Bit
.equ	WskrL = 1				; Left Whisker Input Bit
.equ	RXD1  = 2				; RX Input Pin

; Drive pins
.equ	EngEnR = 4				; Right Engine Enable Bit
.equ	EngEnL = 7				; Left Engine Enable Bit
.equ	EngDirR = 5				; Right Engine Direction Bit
.equ	EngDirL = 6				; Left Engine Direction Bit

.equ	BotAddress = 0x1a		; (Enter your robot's address here (8 bits))

;/////////////////////////////////////////////////////////////
;These macros are the values to make the TekBot Move.
;/////////////////////////////////////////////////////////////
.equ	MovFwd =  (1<<EngDirR|1<<EngDirL)	;0b01100000 Move Forward Action Code
.equ	MovBck =  $00						;0b00000000 Move Backward Action Code
.equ	TurnR =   (1<<EngDirL)				;0b01000000 Turn Right Action Code
.equ	TurnL =   (1<<EngDirR)				;0b00100000 Turn Left Action Code
.equ	Halt =    (1<<EngEnR|1<<EngEnL)		;0b10010000 Halt Action Code

;/////////////////////////////////////////////////////////////
;These macros are the TekBot IR commands.
;/////////////////////////////////////////////////////////////
; INPUT
.equ	MovFwdCmd =  ($80|1<<(EngDirR-1)|1<<(EngDirL-1))	;0b10110000 Move Forward Action Code
.equ	MovBckCmd  =  ($80|$00)								;0b10000000 Move Backward Action Code
.equ	TurnRCmd  =   ($80|1<<(EngDirL-1))					;0b10100000 Turn Right Action Code
.equ	TurnLCmd  =   ($80|1<<(EngDirR-1))					;0b10010000 Turn Left Action Code
.equ	HaltCmd  =    ($80|1<<(EngEnR-1)|1<<(EngEnL-1))		;0b11001000 Halt Action Code
.equ	FreezeCmd = 0b11111000								;0b11111000 Freeze Action Code

; OUTPUT
.equ	FreezeOthersCmd = 0b01010101						;0b01010101 Broadcase freeze action code

; Store the upper and lower bytes for ubrr setting
.equ	ubrr_low = low(832)
.equ	ubrr_high = high(832)

; Bits 1 and 3 set for INT0 and INT1 trigger on falling edge
.equ	InterruptsFallingEdge = (1 << ISC01) | (1 << ISC11)

; Bits 0 and 1 set for INT0 and INT1 interrupts enabled
.equ	InterruptMasksEnabled = (1 << INT0) | (1 << INT1)		
.equ	InterruptMasksDisabled = 0

; Setting these bits to one and writing them to the flag register clears them
.equ	InterruptFlagRegisterClear = (1 << INTF0) | (1 << INTF1)

; Mask for commands vs addresses
.equ	cmd_addr_bit = 7

; Max freeze before offline
.equ freezes_to_perm_halt = 3

;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; Beginning of code segment

;***********************************************************
;*	Interrupt Vectors
;***********************************************************
.org	$0000					; Beginning of IVs
		rjmp 	INIT			; Reset interrupt

;Should have Interrupt vectors for:
;- Right whisker
.org INT0addr
	rjmp HitRight

;- Left whisker
.org INT1addr
	rjmp HitLeft

;- USART receive
.org URXC1addr
	rjmp RX1_DATA_RECEIVED

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
	; Set drive pins to outputs
	ldi mpr, ((1 << EngEnR) | (1 << EngDirR) | (1 << EngEnL) | (1 << EngDirL))
	out DDRB, mpr
	
	; Set whisker pins and rx pin to inputs
	ldi mpr, ((0 << WskrR) | (0 << WskrL) | (0 << RXD1))
	out DDRD, mpr

	; Set whisker pins to enable pullups
	ldi mpr, ((1 << WskrR) | (1 << WskrL))
	out PORTD, mpr
	
	; USART1
	; Set baudrate at 2400bps
	ldi mpr, ubrr_low
	sts UBRR1L, mpr

	ldi mpr, ubrr_high
	sts UBRR1H, mpr

	; Enable transmitter, reciever, and enable interrupt on receive
	ldi mpr, ((1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1))
	sts UCSR1B, mpr
	
	; Set frame format: 8 data bits, 2 stop bits
	; Double data rate 
	ldi mpr, (1 << U2X1)
	sts UCSR1A, mpr

	; 8N2 setting
	ldi mpr, ((1 << UCSZ10) | (1 << UCSZ11) | (1 << USBS1))
	sts UCSR1C, mpr

	; External Interrupts
	; D0 is INT0, D1 is INT1
	; Set the Interrupt Sense Control to falling edge 
	ldi mpr, InterruptsFallingEdge
	sts EICRA, mpr

	; Configure the External Interrupt Mask
	ldi mpr, InterruptMasksEnabled
	out EIMSK, mpr

	; Initialize TekBot Forward Movement
	ldi	cmd_reg, MovFwd		; Load Move Forward Command
	out	PORTB, cmd_reg		; Send command to motors

	; Flush usart rx
	rcall USART_Flush

	; Clear address register
	clr addr_reg
	clr freeze_count_reg

	; Enabled global interrupts
	sei

;***********************************************************
;*	Main Program
;***********************************************************
MAIN:
	rjmp	MAIN

;***********************************************************
;*	Functions and Subroutines
;***********************************************************
;----------------------------------------------------------------
; Sub:	SendFreezeOthers
; Desc:	Sends a tekbot command to freeze all other bots
;----------------------------------------------------------------
SendFreezeOthers:
	lds mpr, UCSR1A				; Load usart1 status
	sbrs mpr, UDRE1				; Check if data reg empty
	rjmp SendFreezeOthers		; Jump back until it is

	ldi mpr, FreezeOthersCmd	; Load freeze cmd
	sts UDR1, mpr				; Send command over serial
	ret

;----------------------------------------------------------------
; Sub:	USART_Flush
; Desc:	Clear RX buffer
;----------------------------------------------------------------
USART_Flush:
	lds mpr, UCSR1A				; Load status reg
	sbrs mpr, RXC1				; Check to see if there's data
	ret							; If not, return
	lds mpr, UDR1				; Otherwise, read in data
	rjmp USART_Flush			; And go check again

;----------------------------------------------------------------
; Sub:	RX1_DATA_RECEIVED
; Desc:	Processes incoming serial data on interrupt
;----------------------------------------------------------------
RX1_DATA_RECEIVED:
	cli							; Stop processing interrupts

	cpi freeze_count_reg, freezes_to_perm_halt	; Check if at limit
	breq RX1_DATA_RECV_END_NO_INT				; No interrrupt return

	lds mpr, UDR1				; Read in data

	; Check freeze
	cpi mpr, FreezeOthersCmd	; Check if we should be frozen
	brne RX1_DATA_RECV_ADDR		; Skip to process normal command

	; Check if it's our freeze
	cpi freeze_sent_reg, 0		; Check if we sent it
	breq RX1_PROCESS_FREEZE

	; Ignore our own command
	clr freeze_sent_reg			; Reset flag
	clr addr_reg				; Clear addr reg for next run
	rjmp RX1_DATA_RECV_END		; Go to end

RX1_PROCESS_FREEZE:
	; Process freeze
	ldi	mpr, Halt				; Load cmd to halt
	out PORTB, mpr				; Halt bot

	ldi mpr, 5					; Load 5 sec wait
RX1_FREEZE_LOOP:
	ldi	waitcnt, WTime			; Wait for 1 seconds
	rcall Wait					; Call wait function
	dec mpr						; Decrement counter
	brne RX1_FREEZE_LOOP		; If not done with wait, go back

	; Clear interrupt flags so no new interrupts until after
	ldi mpr, InterruptFlagRegisterClear
	out EIFR, mpr

	inc freeze_count_reg		; Inc count for freeze
	cpi freeze_count_reg, freezes_to_perm_halt	; Check if at limit
	breq RX1_DATA_RECV_END_NO_INT				; No interrrupt return

	; Not at limit, go back to last command
	rcall USART_Flush			; Clear any commands while frozen
	out PORTB, cmd_reg			; Go to prev state
	rjmp RX1_DATA_RECV_END		; Go to end

	; Check address
RX1_DATA_RECV_ADDR:
	sbrc mpr, cmd_addr_bit		; Check if is address
	rjmp RX1_DATA_RECV_COMMAND	; Process if was command

	mov addr_reg, mpr			; If addr, update addr reg
	rjmp RX1_DATA_RECV_END		; Go to end

	; Check command
RX1_DATA_RECV_COMMAND:
	cpi addr_reg, BotAddress	; Cmp current with our addr
	brne RX1_DATA_RECV_END		; If not ours, go to end
		
	; Check if it's freeze command
	cpi mpr, FreezeCmd
	brne RX1_REG_COMMAND

	rcall SendFreezeOthers		; Send freeze
	ldi freeze_sent_reg, 1		; Set that freeze sent

	clr addr_reg				; Clear addr reg for next run
	rjmp RX1_DATA_RECV_END		; Go to end

RX1_REG_COMMAND:
	; Otherwise it's normal command
	mov cmd_reg, mpr			; Set current command
	lsl cmd_reg					; Ours, remove cmd bit, make command
	out PORTB, cmd_reg			; Write command

	clr addr_reg				; Clear addr reg for next run

RX1_DATA_RECV_END:
	sei							; Reenable interrupts

RX1_DATA_RECV_END_NO_INT:
	reti

;----------------------------------------------------------------
; Sub:	HitRight
; Desc:	Processes the logic for when the right whisker is hit
;----------------------------------------------------------------
HitRight:
		cli

		cpi freeze_count_reg, freezes_to_perm_halt	; Check if at limit
		breq HitRightEnd							; No interrrupt return

		push	mpr			; Save mpr register
		push	waitcnt		; Save wait register
		in		mpr, SREG	; Save program state
		push	mpr			; Save program state

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

		; Restore drive state
		out		PORTB, cmd_reg	; Send saved command to port

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		rcall USART_Flush	; Clear out receive register

		pop		mpr			; Restore program state
		out		SREG, mpr	; Restore program state
		pop		waitcnt		; Restore wait register
		pop		mpr			; Restore mpr

		sei
HitRightEnd:
		reti				; Return from subroutine

;----------------------------------------------------------------
; Sub:	HitLeft
; Desc:	Processes the logic for when the left whisker is hit
;----------------------------------------------------------------
HitLeft:
		cli
		
		cpi freeze_count_reg, freezes_to_perm_halt	; Check if at limit
		breq HitLeftEnd								; No interrrupt return

		push	mpr			; Save mpr register
		push	waitcnt		; Save wait register
		in		mpr, SREG	; Save program state
		push	mpr			; Save program state

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

		; Restore drive state
		out		PORTB, cmd_reg	; Send saved command to port

		; Clear interrupt flags so no new interrupts until after
		ldi mpr, InterruptFlagRegisterClear
		out EIFR, mpr

		rcall USART_Flush	; Clear out receive register

		pop		mpr			; Restore program state
		out		SREG, mpr	; Restore program state
		pop		waitcnt		; Restore wait register
		pop		mpr			; Restore mpr

		sei
HitLeftEnd:
		reti				; Return from subroutine

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

;***********************************************************
;*	Additional Program Includes
;***********************************************************
