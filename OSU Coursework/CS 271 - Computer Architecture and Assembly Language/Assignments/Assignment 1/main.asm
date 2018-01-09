;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TITLE Assignment 1						(main.asm)
;Author: Corwin Perren
;Date: 1/19/2014
;Description:
;	This program will request two numbers from the user, perform common 
;	mathematical calculations on them, and print the results
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

INCLUDE Irvine32.inc
.data
ProgramTitle BYTE "CS 271 - Assignment 1",0dh,0ah,0
CreatorName	 BYTE "Creator: Corwin Peren",0dh,0ah,0

Prompt1 BYTE "Please enter the first number: ",0
Prompt2 BYTE "Please enter the second number: ",0

SumMessage	BYTE "Sum: ",0
DifMessage	BYTE "Difference: ",0
ProdMessage BYTE "Product: ",0
QuotMessage BYTE "Quotient: ",0
RemainMessage BYTE "Remainder: ",0

TermMessage BYTE "Computations Complete...",0dh,0ah,0

FirstNumber		DWORD ?
SecondNumber	DWORD ?

FinalSum		DWORD ?
FinalDif		DWORD ?
FinalProd		DWORD ?
FinalQuot		DWORD ?
FinalRemain		DWORD ?


.code

NineToFive PROC uses EAX
	mov eax, 9
	call WriteInt
	mov eax, 8
	call WriteInt
	mov eax, 7
	call WriteInt
	mov eax, 6
	call WriteInt
	mov eax, 5
	call WriteInt

	ret
NineToFive ENDP

main PROC

	call NineToFive
	exit
main ENDP

END main