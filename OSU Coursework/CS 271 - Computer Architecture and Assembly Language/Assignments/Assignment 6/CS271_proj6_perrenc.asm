;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TITLE Assignment 6						(main.asm)
;Author: Corwin Perren
;Date: 3/13/2014
;Description:
;	This program runs all five programs from assignment 6 from a menu screen
;	These programs include a letter matrix generator, array row sum calculator,
;	a floating point comparator, circle area computer, and quadratic formula
;	calculator
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

INCLUDE Irvine32.inc

.data

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Program Variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 Global Relevant Variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ProgramTitle BYTE "CS 271 - Assignment 6",0
ProgrammerName BYTE "Written by: Corwin Perren",0

SelectInstructions BYTE "Please enter 1-6 to run desired program.", 0

MenuItem1 BYTE "1: Letter Matrix Generator (Chapter 9, Exercise 10)",0
MenuItem2 BYTE "2: Two Dimensional Array Row Summer (Chapter 9, Exercise 12)",0
MenuItem3 BYTE "3: Floating Point Comparator (Chapter 12, Exercise 1)",0
MenuItem4 BYTE "4: Area of Circle Calculator (Chapter 12, Exercise 5)",0
MenuItem5 BYTE "5: Quadratic Formula Calculator (Chapter 12, Exercise 6)",0
MenuItem6 BYTE "6: Exit",0

Selection BYTE "Selection: ",0

IncorrectNumberEntry BYTE "You did not enter 1-7. Press enter to try again.", 0
ReturnToSelect BYTE "Program Complete. Press enter to return to selection screen.", 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 Letter Matrix Generator Variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NumVowels DWORD 6
VowelArray BYTE "AEIOUY"
NumConsonants DWORD 20
ConsonantArray BYTE "BCDFGHJKLMNPQRSTVWXZ"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 Two Dimensional Array Row Summer Variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CurrentArrayContents BYTE "Array Contents:",0

TableRow1 BYTE "1  2  3  4  5",0								;For printing to display only
TableRow2 BYTE "6  7  8  9  10",0								;For printing to display only	
TableRow3 BYTE "11 12 13 14 15",0								;For printing to display only
TableRow4 BYTE "16 17 18 19 20",0								;For printing to display only
TableRow5 BYTE "21 22 23 24 25",0								;For printing to display only


SummerPleaseEnterIndex BYTE "Enter a row to sum between 1 and 5.",0
SummerIndexPrompt BYTE "Row: ",0

TableArrayIndex DWORD ?

TableArrayBYTE			BYTE 1,2,3,4,5							;Row 1
TableArrayRowSizeBYTE	= ($-TableArrayBYTE)					;Size of single row
						BYTE 6,7,8,9,10							;Row 2
						BYTE 11,12,13,14,15						;Row 3
						BYTE 16,17,18,19,20						;Row 4
						BYTE 21,22,23,24,25						;Row 5

TableArrayWORD			WORD 1,2,3,4,5							;Row 1
TableArrayRowSizeWORD	= ($-TableArrayWORD)					;Size of single row
						WORD 6,7,8,9,10							;Row 2
						WORD 11,12,13,14,15						;Row 3
						WORD 16,17,18,19,20						;Row 4
						WORD 21,22,23,24,25						;Row 5

TableArrayDWORD			DWORD 1,2,3,4,5							;Row 1
TableArrayRowSizeDWORD	= ($-TableArrayDWORD)					;Size of single row
						DWORD 6,7,8,9,10						;Row 2
						DWORD 11,12,13,14,15					;Row 3
						DWORD 16,17,18,19,20					;Row 4
						DWORD 21,22,23,24,25					;Row 5


SummerBYTE BYTE "Sum using BYTE array: ",0
SummerWORD BYTE "Sum using WORD array: ",0
SummerDWORD  BYTE"Sum using DWORD array: ",0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 Floating Point Comparator Variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
EnterTwoFloat BYTE "Please enter two floating point numbers to compare.",0
Float1Prompt BYTE "First Number : ", 0
Float2Prompt BYTE "Second Number: ", 0

FirstLargerThanFloat BYTE "The first value you entered was larger than the second.", 0
SecondLargerThanFloat BYTE "The second value you entered was larger than the first.", 0
EqualFloat BYTE "The numbers you entered were equal",0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 Area of Circle Calculator Variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
EnterRadius BYTE "Enter the radius of a circle to calculate it's area.",0
RadiusPrompt BYTE "Radius: ",0

CircleAreaCalculated BYTE "The area for this circle is: ",0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 Quadratic Formula Calculator Variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PleaseEnterCoeff BYTE "Please enter coefficients a, b, and c for a quadratic forumula in the form", 10, 13, "ax",253," + bx + c = 0 to calculate it's real roots.",0
RootsNonRealMsg BYTE "These coefficients result in non-real roots. Exiting.",0

Root1Msg BYTE "Root1: ",0
Root2Msg BYTE "Root2: ",0

CoeffA BYTE "a: ",0
CoeffB BYTE "b: ",0
CoeffC BYTE "c: ",0

Stores2 DWORD 2
Stores4 DWORD 4

FloatA REAL10 ?
FloatB REAL10 ?
FloatC REAL10 ?

AfterRootTaken REAL10 ?
Root1 REAL10 ?
Root2 REAL10 ?

.code
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Globally Useful Procedures
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for writing string with newline
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Write_String_NL PROC uses EDX,
	StringPtr:Ptr BYTE								;Varible to store string offset pointer

	mov edx, StringPtr								;Moves the string to the proper register
	call WriteString								;Prints the string
	call Crlf										;Prints a new line

	ret												;Return from procedure
Write_String_NL ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for writing string without newline
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Write_String PROC uses EDX,
	StringPtr:Ptr BYTE								;Variable to store string offset pointer

	mov edx, StringPtr								;Moves the string to the proper registers
	call WriteString								;Prints the string

	ret
Write_String ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for printing program title and programmer name
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintTitle PROC
	INVOKE Write_String_NL, OFFSET ProgramTitle		;Prints the program title
	INVOKE Write_String_NL, OFFSET ProgrammerName	;prints the programmer's name
	call Crlf										;Prints a new line
	INVOKE Write_String_NL, OFFSET SelectInstructions	;Prints the selection screen instructions
	call Crlf										;Prints a new line
	INVOKE Write_String_NL, OFFSET MenuItem1		;Prints menu item 1
	INVOKE Write_String_NL, OFFSET MenuItem2		;Prints menu item 2
	INVOKE Write_String_NL, OFFSET MenuItem3		;Prints menu item 3
	INVOKE Write_String_NL, OFFSET MenuItem4		;Prints menu item 4
	INVOKE Write_String_NL, OFFSET MenuItem5		;Prints menu item 2
	INVOKE Write_String_NL, OFFSET MenuItem6		;Prints menu item 3
	INVOKE Write_String, OFFSET Selection			;Print selction prompt
	ret
PrintTitle ENDP

ClearFPU PROC
	FFREE ST(0)
	FFREE ST(1)
	FFREE ST(2)
	FFREE ST(3)
	FFREE ST(4)
	FFREE ST(5)
	FFREE ST(6)
	FFREE ST(7)
	ret
ClearFPU ENDP
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Letter Matrix Generator Procedures
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure returns a random consonant
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GetRandomConsonant PROC uses EBX
	mov	EAX, NumConsonants							;Set up range for random generator
	mov	EBX, offset ConsonantArray					;Move ConsonantArray into ebp for dereferencing
	call RandomRange								;Get random number within range
	add	EBX, EAX									;Point to element in ebp
	mov	AL, [EBX]									;Store pointed to value in AL
	ret												;Return from procedure
GetRandomConsonant ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure returns a random vowel
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GetRandomVowel PROC uses EBX
	mov	EAX, NumVowels								;Set up range for random generator						
	mov	EBX, offset VowelArray						;Move VowelArray into ebp for dereferencing 
	call RandomRange								;Get random number within range
	add	EBX, EAX									;Point to element in ebp
	mov	AL, [EBX]									;Store pointed to value in AL
	ret												;Return from procedure
GetRandomVowel ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure prints the matrixes in proper rows with random chars
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintMatrix PROC uses EAX ECX EDX
	mov	ECX, 4										;Move column count into ECX
PrintLineLoop:										
	mov	EDX, 4										;Move row count into EDX
	call CrLf										;Print new line
PrintShortLoop:	
	mov	EAX, 2										;Put in range for RandomRange of 2 (0-1)
	call RandomRange								;Get random 0 to 1
	cmp	EAX, 0										;Check if zero
	jnz	GetConsonant								;If one, print consonant
	call GetRandomVowel								;Get Vowel from generator			
	jmp	WriteCharToMatrix							;Go print the character
GetConsonant:
	call GetRandomConsonant							;Get consonant from generator
WriteCharToMatrix:
	call WriteChar								    ;Print current character
	dec	EDX											;Move forward one column
	jnz	PrintShortLoop								;Repeat column print until at column end
	dec	ECX											;Move down one row
	jnz	PrintLineLoop								;Print out column on new row
	call CrLf									    ;Print a new line
	ret												;Return from procedure
PrintMatrix ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for running program1 functions
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program1 PROC
	call ClrScr
	call Randomize									;Seeds the random number generator
	mov	ECX, 5										;Set loop counter to 5 to display matrix five times
Program1Loop:
	call PrintMatrix								;Call procedure to print out one matrix
	loop Program1Loop								;Continue printing until 5 arrays are one screen
	ret												;Return from Program1 procedure
Program1 ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Two Dimensional Array Row Summer Procedures
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure calculates the sum for a row using stack passed parameters
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Calc_Row_Sum PROC
		
	mov ebp, esp									;Get access to the stack
	mov ebx, [ebp+4]								;Save users selected row index
	dec ebx											;Remove one to make math simpler
	
	mov edx, [ebp+8]								;Save table type
	
	mov ecx, [ebp+12]								;Setup counter with row length
	mov esi, [ebp+16]								;Move array into array reg


	cmp edx, 1										;Check if it's a BYTE
	je RowSumBYTE									;Jump if BYTE

	cmp edx, 2										;Check if it's a WORD
	je RowSumWORD									;Jump if WORD

	cmp edx, 4										;Check if DWORD
	je RowSumDWORD									;Jump if DWORD

RowSumBYTE:
	mov eax, ecx									;Move row length into eax
	mul ebx											;Mutiply by row index
	add esi, eax									;Add this offset to esi 
	mov eax, 0										;Zero accumulator
RowSumBYTELoop:
	add al, [esi]									;Add value to al
	inc esi											;Move to the next value in the array
	loop RowSumBYTELoop								;Loop until row finished
	jmp CalcRowSumEnd								;Jump when finished

RowSumWORD:
	push edx										;Save EDX to stack
	mov eax, ecx									;Set up multiplication of 2
	mul ebx											;Multiply user index by two (Now if WORD format)
	add esi, eax									;Move cursor to index location using offset
	pop edx											;Return EDX to reg

	mov ebx, edx									;Store TYPE in EBX
	mov edx, 0										;Zero upper half for multiplication
	mov eax, ecx									;Set lower half to row size
	div ebx											;Divide by TYPE
	mov ecx, eax									;Set this number in the loop count reg

	mov eax, 0										;Zero accumulator
RowSumWORDLoop:
	add ax, [esi]									;Add value to ax
	add esi, 2										;Move to the next value in the array
	loop RowSumWORDLoop								;Loop until row finished
	jmp CalcRowSumEnd								;Jump when finished

RowSumDWORD:
	push edx										;Save EDX to stack
	mov eax, ecx									;Set up multiplication of 4
	mul ebx											;Multiply user index by four (Now in DWORD format)
	add esi, eax									;Move cursor to index location using offset
	pop edx											;Return EDX to reg

	mov ebx, edx									;Store TYPE in EBX
	mov edx, 0										;Zero upper half for multiplication
	mov eax, ecx									;Set lower half to row size
	div ebx											;Divide by TYPE
	mov ecx, eax									;Set this number in the loop count reg

	mov eax, 0										;Zero accumulator
RowSumDWORDLoop:
	add eax, [esi]									;Add value to ax
	add esi, 4										;Move to the next value in the array
	loop RowSumDWORDLoop							;Loop until row finished

CalcRowSumEnd:
	ret												;Return from procedure
Calc_Row_Sum ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Program2
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program2 PROC
	call ClrScr										;Clears the screen
	INVOKE Write_String_NL, OFFSET CurrentArrayContents	;Prints out array contents header
	INVOKE Write_String_NL, OFFSET TableRow1		;Prints table row 1
	INVOKE Write_String_NL, OFFSET TableRow2		;Prints table row 2
	INVOKE Write_String_NL, OFFSET TableRow3		;Prints table row 3
	INVOKE Write_String_NL, OFFSET TableRow4		;Prints table row 4
	INVOKE Write_String_NL, OFFSET TableRow5		;Prints table row 5

	INVOKE Write_String_NL, OFFSET SummerPleaseEnterIndex	;Asks user to enter index value
	INVOKE Write_String, OFFSET SummerIndexPrompt	;Row prompt for niceity
	call ReadInt									;Read in Index from user
	mov TableArrayIndex, EAX						;Moves user selection into variable
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Using BYTES
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push OFFSET TableArrayBYTE						;Pushes the offset for the table array (32-bit)
	
	mov eax, 0										;Clears the eax register
	mov eax, TableArrayRowSizeBYTE					;Moves the table array row size into eax
	push eax										;Pushes that number onto the stack	(32-bit)
	
	mov eax, 0										;Clears the eax register
	mov eax, TYPE TableArrayBYTE					;Moves the type of table array into eax
	push eax    									;Pushes the type of the array onto the stack (32-bit)
	
	push TableArrayIndex							;Pushes the index selected by the user onto the stack	(32-bit)
	call Calc_Row_Sum								;Calculates sum for row and prints it
	INVOKE Write_String, OFFSET SummerBYTE			;Prints out what array was used to sum
	call WriteInt									;Writes to display
	call Crlf										;Prints a new line
	add esp, 16										;Cleanup the stack (4 times 32-bit)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Using WORDS
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push OFFSET TableArrayWORD						;Pushes the offset for the table array (32-bit)
	
	mov eax, 0										;Clears the eax register
	mov eax, TableArrayRowSizeWORD					;Moves the table array row size into eax
	push eax										;Pushes that number onto the stack	(32-bit)
	
	mov eax, 0										;Clears the eax register
	mov eax, TYPE TableArrayWORD					;Moves the type of table array into eax
	push eax    									;Pushes the type of the array onto the stack (32-bit)
	
	push TableArrayIndex							;Pushes the index selected by the user onto the stack	(32-bit)
	call Calc_Row_Sum								;Calculates sum for row and prints it
	INVOKE Write_String, OFFSET SummerWORD			;Prints out what array was used to sum
	call WriteInt									;Writes sum to display
	call Crlf										;Prints a new line
	add esp, 16										;Cleanup the stack (4 times 32-bit)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Using DWORDS
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push OFFSET TableArrayDWORD						;Pushes the offset for the table array (32-bit)
	
	mov eax, 0										;Clears the eax register
	mov eax, TableArrayRowSizeDWORD					;Moves the table array row size into eax
	push eax										;Pushes that number onto the stack	(32-bit)
	
	mov eax, 0										;Clears the eax register
	mov eax, TYPE TableArrayDWORD					;Moves the type of table array into eax
	push eax    									;Pushes the type of the array onto the stack (32-bit)
	
	push TableArrayIndex							;Pushes the index selected by the user onto the stack	(32-bit)
	call Calc_Row_Sum								;Calculates sum for row and prints it
	INVOKE Write_String, OFFSET SummerDWORD			;Prints out what array was used to sum
	call WriteInt									;Writes sum to the screen

	add esp, 16										;Cleanup the stack (4 times 32-bit)
	ret 											;Return from Program2 procedure
Program2 ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Floating Point Comparator Procedures
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure runs program 3 functions
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program3 PROC
	call ClrScr
	INVOKE Write_String_NL, OFFSET EnterTwoFloat	;Prompt to enter two floating point numbers
	INVOKE Write_String, OFFSET Float1Prompt		;Prompt for first val
	call ReadFloat									;Read in value
	INVOKE Write_String, OFFSET Float2Prompt		;Prompt for second val
	call ReadFloat									;Read in value
	call CRLF										;Print newline
	FCOMI ST(0), ST(1)								;Compare second value entered to first
	ja SecondWasGreater								;If the second is above the first, it's greater so jump
	INVOKE Write_String_NL, OFFSET FirstLargerThanFloat	;Otherwise, the first was greater and we print a message saying so
	jmp Prog3Ending									;Then we jump to program end

SecondWasGreater:
	INVOKE Write_String_NL, OFFSET SecondLargerThanFloat	;In the case the second is greater than the first, we will print that instead

Prog3Ending:
	ret												;Return from Program3 procedure
Program3 ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Area of Circle Calculator Procedures
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure runs program 4 functions
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program4 PROC
	call ClrScr										;Clear the screen
	INVOKE Write_String_NL, OFFSET EnterRadius		;Prompt the user to enter the radius
	INVOKE Write_String, OFFSET RadiusPrompt		;Provide nice header
	FLDPI											;Load pi onto floating point unit stack
	call ReadFloat									;Read in float from user
	fld ST(0)										;Load a second radius onto floating point stack
	FMULP											;Mulitply and pop
	FMULP											;Multiply and pop 
	INVOKE Write_String, OFFSET CircleAreaCalculated	;Print header for final value
	call WriteFloat									;Write calculated value to screen
	FFREE ST(0)										;Free FPU stack
	ret												;Return from Program4 procedure
Program4 ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Quadratic Formula Calculator Procedures
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure reads in float coefficients from user
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GetCoefficients PROC
	INVOKE Write_String_NL, OFFSET PleaseEnterCoeff	;Asks user to enter coefficients
	call Crlf										;Prints a new line
	
	INVOKE Write_String, OFFSET CoeffA				;Asks for a
	call ReadFloat									;Reads in a
	
	INVOKE Write_String, OFFSET CoeffB				;Asks for b
	call ReadFloat									;Reads in b
	
	INVOKE Write_String, OFFSET CoeffC				;Asks for c
	call ReadFloat									;Reads in c

	FSTP FloatC										;Store coeff c in var
	FSTP FloatB 									;Store coeff b in var
	FSTP FloatA 									;Store coeff a in var
	ret												;Return from procedure
GetCoefficients ENDP


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure runs program5 functions
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program5 PROC
	call ClrScr										;Clear the screen
	call GetCoefficients							;Reading in coefficients from user

	FLD FLoatB										;Load b
	FMUL ST(0), ST(0)								;Square b
	FLD FloatA										;Load a
	FLD FloatC										;Load c
	FIMUL Stores4									;Multiply c by 4
	FMULP ST(1), ST(0)								;End result is 4*a*c
	FSUBP ST(1), ST(0)								;End result is b^2-4ac
	FLDZ											;Load zero onto stack for comparison
	FCOMIP ST(0), ST(1)								;Check if roots are going to be real or not
	ja NonRealRoots									;Jump to message if roots are non-real and exit
	FSQRT											;Take square root as it's real
	FSTP AfterRootTaken								;Store for use later on other root

	FLD FloatB										;Load b
	FCHS											;Negate b
	FLD AfterRootTaken								;Load rooted val
	FSUB											;Subtract rooted val from negated b (-b - sqrt(b^2-4ac))
	FLD FloatA										;Load a
	FIMUL Stores2									;Multiply a by 2
	FDIV											;Divide numerator by denominator
	FSTP Root1										;Store first root value

	FLD FloatB										;Load b
	FCHS											;Negate b
	FLD AfterRootTaken								;Load rooted val
	FADD											;Add rooted val from negated b (-b - sqrt(b^2-4ac))
	FLD FloatA										;Load a
	FIMUL Stores2									;Multiply a by 2
	FDIV											;Divide numerator by denominator
	FSTP Root2										;Store second root value

	call Crlf

	INVOKE Write_String, OFFSET Root1Msg			;Print Root1 Header
	FLD Root1										;Load root1 into register
	call WriteFloat									;Print root1
	FFREE ST(0)										;Free FPU register
	call Crlf										;Print new line

	INVOKE Write_String, OFFSET Root2Msg			;Print root2 header
	FLD Root2										;Load root2 into register
	call WriteFloat									;Print root2
	FFREE ST(0)										;Free FPU register
	call Crlf										;Print a new line

	jmp Program5Ending								;Jump to program end

		
NonRealRoots:
	FFREE ST(0)										;Free up register so it can be used again
	INVOKE Write_String_NL, OFFSET RootsNonRealMsg	;Print message saying there are no real roots

Program5Ending:
	ret												;Return from Program5 procedure
Program5 ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                               Program Main Function
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Main Procedure
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main PROC
	FINIT											;Initialize floating point unit
MainTitle:
	call ClrScr										;Clear screen
	call PrintTitle									;Print the title screen
	call ReadInt									;Read in user selection

	cmp eax, 1										;Check if user selected entry item 1
	je Prog1Jmp										;If so, jump to that program

	cmp eax, 2										;Check if user selected entry item 2
	je Prog2Jmp										;If so, jump to that program

	cmp eax, 3										;Check if user selected entry item 3
	je Prog3Jmp										;If so, jump to that program

	cmp eax, 4										;Check if user selected entry item 4
	je Prog4Jmp										;If so, jump to that program

	cmp eax, 5										;Check if user selected entry item 5
	je Prog5Jmp										;If so, jump to that program

	cmp eax, 6										;Check if user selected exit
	je MainExit										;If so, jump to end of program


	call ClrScr										;Clear screeen
	INVOKE Write_String_NL, OFFSET IncorrectNumberEntry	;Scold user for not entering one to six
	call ReadInt									;Wait for enter key
	jmp MainTitle									;Jump back to main screen

Prog1Jmp:
	call Program1									;Call program1
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
Prog2Jmp:
	call Program2									;Call program2
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
Prog3Jmp:
	call ClearFPU									;Empties FPU Registers to keep things from crashing
	call Program3									;Call program3
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
Prog4Jmp:
	call ClearFPU									;Empties FPU Registers to keep things from crashing
	call Program4									;Call program4
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
Prog5Jmp:
	call ClearFPU									;Empties FPU Registers to keep things from crashing
	call Program5									;Call program5
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
MainExit:
	exit
main ENDP

END main