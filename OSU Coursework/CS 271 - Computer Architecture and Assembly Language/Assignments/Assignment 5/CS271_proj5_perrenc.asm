;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TITLE Assignment 5						(main.asm)
;Author: Corwin Perren
;Date: 3/3/2014
;Description:
;	This program runs all three programs from assignment 5 from a menu screen
;	These programs include an array reverser, frequency table generator, and
;	prime number generator
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

INCLUDE Irvine32.inc

.data

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Variables to change for testing program capabilities
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SieveArray BYTE 5000 dup(?)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Globally relevant vars
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ProgramTitle BYTE "CS 271 - Assignment 5",0
ProgrammerName BYTE "Written by: Corwin Perren",0

SelectInstructions BYTE "Please enter 1-4 to run desired program.", 0

MenuItem1 BYTE "1: Reverse Array In Place (Chapter 4, Exercise 5)",0
MenuItem2 BYTE "2: Frequency Table (Chapter 9, Exercise 6)",0
MenuItem3 BYTE "3: Sieve of Eratosthenes (Chapter 9, Exercise 7)",0
MenuItem4 BYTE "4: Exit",0

Selection BYTE "Selection: ",0

IncorrectNumberEntry BYTE "You did not enter 1-4. Press enter to try again.", 0
ReturnToSelect BYTE "Program Complete. Press enter to return to selection screen.", 0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Vars for Prog 1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ArrayToReverse BYTE 1,2,3,4,5,6,7,8,9,10, 11, 12, 13, 14
PressEnter  BYTE "Press enter to reverse the currently reversed array. Enter 1 to quit.", 0
OriginalArray BYTE "Original Array:", 0
ReversedArray BYTE "Reversed Array:", 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Vars for Prog 2
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
StringEntry BYTE "Please enter a string to create a frequency table for.", 0
StringThing BYTE "Your string: ", 0

StringArray BYTE 256 dup(?)
FrequencyTable DWORD 256 dup(?)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Vars for Prog 3
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
EnterPrimes1 BYTE "Press enter to calculate primes up to ", 0
EnterPrimes2 BYTE ".", 0


.code
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
	INVOKE Write_String, OFFSET Selection			;Print selction prompt
	ret
PrintTitle ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to fill array with zeros using stosb
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
FillWithZeros PROC 
	mov al, 0										;Set al with value we want to fill array with
	cld												;Clear Direction Flag
	mov edi, OFFSET SieveArray						;Point edi to array
	mov ecx, LENGTHOF SieveArray					;Set counter so instruction knows when to end
	dec ecx											;Subtract one because it's starting at zero instead of one
	rep stosb										;Repeats the fill operation until the array is full
	ret												;Returns from function
FillWithZeros ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to mark multiples of primes with ones
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MarkMultiples PROC

	mov esi, OFFSET SieveArray						;Move into esi to get easy access
	mov BYTE PTR [esi], 1							;Mark zero as non-prime
	mov BYTE PTR [esi+1], 1							;Mark one as non-prime
	mov eax, 2										;Set cursor position
	mov ebx, 2										;Multiplication value
	jmp Inner

Outer:
	pop eax											;Pop eax if we got here so the stack gets aligned and we know where we are
LookForOne:
	inc eax											;Go up by one
	cmp BYTE PTR [esi+eax], 1						;Check to see if the new value at that position of the array has been marked
	je LookForOne									;If so, increment again to find an unmarked one

	cmp eax, LENGTHOF SieveArray					;Once an unmarked is found, make sure it's not outside our array size
	jg MultEnd										;If it's out of array bounds, we're done so exit
	mov ebx, 2										;Otherwise, get ready to mark multiples of current prime starting with two times the prime
Inner:
	push eax										;Save our original prime we're using
	mul ebx											;Multiply prime by scaler
	cmp eax, LENGTHOF SieveArray					;Make sure new value is not out of array size
	jg Outer										;If it is, we're done with this prime

	mov BYTE PTR [esi+eax], 1						;Otherwise, mark the multiple with a one
	inc ebx											;Increase our scaler
	pop eax											;Restore the prime we're on

	jmp Inner										;Re-do for next scaler

MultEnd:
	ret												;Return from procedure
MarkMultiples ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to print out unmarked values in array, which are primes
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintPrimes PROC
	mov esi, OFFSET SieveArray						;Load array into esi to get ready to print
	mov eax, 0										;Set our cursor to zero
LookForZeros:
	inc eax											;Move cursor forward one
	cmp eax, LENGTHOF SieveArray					;Check if cursor is out of array bounds
	jg PrintExit									;If so, exit because we are done

	cmp BYTE PTR [esi+eax], 1						;If we're in bounds, check if the position is marked
	je LookForZeros									;If so, don't print and look at the next position
	call WriteDec									;If unmarked, print the prime number sotred here
	call Crlf										;Print a new line to make things look nice
	jmp LookForZeros								;Continue for the next array position

PrintExit:
	ret												;Return from procedure
PrintPrimes ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to run program 3
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program3 PROC
	call ClrScr										;Clears the screen
	INVOKE Write_String, OFFSET EnterPrimes1		;Prints prompt part one
	mov eax, LENGTHOF SieveArray					;Moves number of primes to calculate to print
	call WriteDec									;Prints number of primes to calculate
	INVOKE Write_String_NL, OFFSET EnterPrimes2		;Prints end of prompt line
	call ReadInt									;Waits for any key
	call FillWithZeros								;Fills the sieve array with zeros using stosb method
	call MarkMultiples								;Marks positions that are multiples of primes
	call PrintPrimes								;Prints remaining prime numbers
	ret												;Returns from procedure
Program3 ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to determine the character frequency of a string
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Get_Frequencies PROC InputString:PTR BYTE, OutputTable:PTR DWORD
	mov esi, InputString							;Moves string to parse into esi
	mov edi, OutputTable							;Moves output table into edi

FreqLoop:
	mov eax, 0										;Zeros eax to make things work right
	mov al, BYTE PTR [esi]							;Moves ascii decimal value into eax
	mov ecx, eax									;Saves current decimal value
	mov ebx, 4										;Multiply eax by four because it's stored in DWORD and not BYTE array
	mul ebx											;Actual multiplication
	add DWORD PTR [edi+eax], 1						;Increments frequency table position by one for this character
	
	cmp ecx, 0										;Checks if we've hit the NUL character
	je FreqDone										;Exit if it's found because we're done

	inc esi											;Otherwise, move to the next string array position
	jmp FreqLoop									;Do above again for next character

FreqDone:
	ret												;Return from procedure
Get_Frequencies ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to read in string from user
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GetNewString PROC StringPtr:PTR BYTE
	mov edx, StringPtr								;Move string array into edx for read in
	mov ecx, 256									;Read in a max of 256 characters
	call ReadString									;Read in string from user

	ret												;Exit from procedure
GetNewString ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to print out the frequency table
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintFreqTable PROC TablePtr:PTR DWORD
	mov esi, TablePtr								;Move frequency table into esi
	mov ecx, 256									;Set number of elements
	mov ebx, 4										;Set size of storage elements
	call DumpMem									;Dump memory for the array
	ret
PrintFreqTable ENDP

ZeroProg2 PROC
	mov eax, 0										;Set al with value we want to fill array with
	cld												;Clear Direction Flag
	mov edi, OFFSET StringArray						;Point edi to array
	mov ecx, 256									;Set counter so instruction knows when to end
	dec ecx											;Subtract one because it's starting at zero instead of one
	rep stosb										;Repeats the fill operation until the array is full

	mov eax, 0										;Set al with value we want to fill array with
	cld												;Clear Direction Flag
	mov edi, OFFSET FrequencyTable					;Point edi to array
	mov ecx, 1024									;Set counter so instruction knows when to end
	dec ecx											;Subtract one because it's starting at zero instead of one
	rep stosb										;Repeats the fill operation until the array is full
	
	ret												;Returns from function
ZeroProg2 ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to run program 2
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program2 PROC
	call ClrScr										;Clear the screen
	call ZeroProg2									;Zeros string and freq table array
	INVOKE Write_String_NL, OFFSET StringEntry		;Prompts the user to enter a string
	INVOKE Write_String, OFFSET StringThing			;Line prompt for above
	INVOKE GetNewString, OFFSET StringArray			;Reads in user string
	INVOKE Get_Frequencies, OFFSET StringArray, OFFSET FrequencyTable	;Runs frequency procedure on input string
	INVOKE PrintFreqTable, OFFSET FrequencyTable	;Prints out the new frequency table
	ret												;Returns from procedure
Program2 ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure to run program 1
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Program1 PROC
	call ClrScr										;Clears the screen
	INVOKE Write_String_NL, OFFSET OriginalArray	;Prints header for original array

	mov eax, TYPE ArrayToReverse					;Moves the array type into eax 

	cmp eax, 1										;Checks if it's a BYTE array
	je ITSBYTE										;Jumps to BYTE section if so
		
	cmp	eax, 2										;Checks if WORD array
	je ITSWORD										;Jumps to WORD section if so

	cmp eax, 4										;Check if DWORD array
	je ITSDWORD										;Jumps to DWORD section if so

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Run for BYTE Array
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ITSBYTE:	 
	mov esi, OFFSET ArrayToReverse					;Moves array into esi
	mov ecx, LENGTHOF ArrayToReverse				;Moves number of elements in array into ecx
	mov ebx, TYPE ArrayToReverse					;Moves type of array into ebx
	call DumpMem									;Shows original array

	mov esi, OFFSET ArrayToReverse					;Re-moves array into esi
	mov edi, OFFSET ArrayToReverse + SIZEOF ArrayToReverse - TYPE ArrayToReverse	;Moves the pointer to the last value of the array into edi
	mov eax, LENGTHOF ArrayToReverse				;Moves number of elements in array into eax
	mov edx, 0										;Sets remainder to zero by default
	mov ebx, 2										;Moves two into ebx
	div ebx											;Divides number of elements by two
	mov ecx, eax									;Puts that number into our loop counter

ReverseBYTE:
	mov bl, [esi]									;Move data at beginning cursor to tmp storage
	mov dl, [edi]									;Move data at end cursor to tmp storage
	mov [esi], dl									;Move end data to beginning
	mov [edi], bl									;Move beginning data to end

	add esi, TYPE ArrayToReverse					;Move beginning cursor forward
	sub edi, TYPE ArrayToReverse					;Move ending cursor back
	loop ReverseBYTE								;Loop until count expired

	jmp Prog1Ret									;Jump to procedure end when done

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Run for WORD Array
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ITSWORD:
	mov esi, OFFSET ArrayToReverse					;Moves array into esi
	mov ecx, LENGTHOF ArrayToReverse				;Moves number of elements in array into ecx
	mov ebx, TYPE ArrayToReverse					;Moves type of array into ebx
	call DumpMem									;Shows original array

	mov esi, OFFSET ArrayToReverse					;Re-moves array into esi
	mov edi, OFFSET ArrayToReverse + SIZEOF ArrayToReverse - TYPE ArrayToReverse	;Moves the pointer to the last value of the array into edi
	mov eax, LENGTHOF ArrayToReverse				;Moves number of elements in array into eax
	mov edx, 0										;Sets remainder to zero by default
	mov ebx, 2										;Moves two into ebx
	div ebx											;Divides number of elements by two
	mov ecx, eax									;Puts that number into our loop counter

ReverseWORD:
	mov bx, [esi]									;Move data at beginning cursor to tmp storage
	mov dx, [edi]									;Move data at end cursor to tmp storage
	mov [esi], dx									;Move end data to beginning
	mov [edi], bx									;Move beginning data to end

	add esi, TYPE ArrayToReverse					;Move beginning cursor forward
	sub edi, TYPE ArrayToReverse					;Move ending cursor back
	loop ReverseWORD								;Loop until count expired

	jmp Prog1Ret


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Run for DWORD Array
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ITSDWORD:
	mov esi, OFFSET ArrayToReverse					;Moves array into esi
	mov ecx, LENGTHOF ArrayToReverse				;Moves number of elements in array into ecx
	mov ebx, TYPE ArrayToReverse					;Moves type of array into ebx
	call DumpMem									;Shows original array

	mov esi, OFFSET ArrayToReverse					;Re-moves array into esi
	mov edi, OFFSET ArrayToReverse + SIZEOF ArrayToReverse - TYPE ArrayToReverse	;Moves the pointer to the last value of the array into edi
	mov eax, LENGTHOF ArrayToReverse				;Moves number of elements in array into eax
	mov edx, 0										;Sets remainder to zero by default
	mov ebx, 2										;Moves two into ebx
	div ebx											;Divides number of elements by two
	mov ecx, eax									;Puts that number into our loop counter

ReverseDWORD:
	mov ebx, [esi]									;Move data at beginning cursor to tmp storage
	mov edx, [edi]									;Move data at end cursor to tmp storage
	mov [esi], edx									;Move end data to beginning
	mov [edi], ebx									;Move beginning data to end

	add esi, TYPE ArrayToReverse					;Move beginning cursor forward
	sub edi, TYPE ArrayToReverse					;Move ending cursor back
	loop ReverseWORD								;Loop until count expired



Prog1Ret:
	call Crlf										;Print a new line
	call Crlf										;Print a second new line
	INVOKE Write_String_NL, OFFSET ReversedArray	;Print header to say this is the reversed array
	mov esi, OFFSET ArrayToReverse					;Move array into esi
	mov ecx, LENGTHOF ArrayToReverse				;Set number of elements
	mov ebx, TYPE ArrayToReverse					;Set size of elements
	call DumpMem									;Display array

	ret												;Return from procedure
Program1 ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Main Procedure
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main PROC
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

	cmp eax, 4										;Check if user selected exit
	je MainExit										;If so, jump to end of program

	call ClrScr										;Clear screeen
	INVOKE Write_String_NL, OFFSET IncorrectNumberEntry	;Scold user for not entering one to four
	call ReadInt									;Wait for enter key
	jmp MainTitle									;Jump back to main screen

Prog1Jmp:
	call Program1									;Call array reverse program
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
Prog2Jmp:
	call Program2									;Call String frequency program
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
Prog3Jmp:
	call Program3									;Call prime generator program
	call Crlf										;Print new line
	INVOKE Write_String_NL, OFFSET ReturnToSelect	;Print hit enter to return to screen
	call ReadInt									;Wait for enter
	jmp MainTitle									;Jump to main screen
MainExit:
	exit
main ENDP

END main