;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TITLE Assignment 2						(main.asm)
;Author: Corwin Perren
;Date: 1/28/2014
;Description:
;	This program will calculate and display the nth iteration of the fibonacci
;	sequence for n values between 1 and 46
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


INCLUDE Irvine32.inc
.data
ProgramTitle BYTE "CS 271 - Assignment 2",0
ProgrammersName BYTE "Programmed By: Corwin Perren",0

NamePrompt BYTE "By what name may I call you?",0
NamePrompt2 BYTE "Your name: ", 0

NameResponse BYTE "Welcome, ", 0
NameResponse2 BYTE ".", 0

FibPrompt BYTE "How many values of the fibonacci sequence would you like to calculate?", 0
FibPrompt2 BYTE "Enter a value from 1 to 46: ", 0
FibPromptIncorrect BYTE "Value was not between 1 and 46. Please try again.", 0

TabString BYTE 9, 0
ExtraTab DWORD 1346269

TryAgainMessage BYTE "Would you like to run the program again?", 0
TryAgainMessage2 BYTE "Answer (y/n): ", 0

IncorrectTryMessage BYTE "Please enter y or n....", 0

TermMessage BYTE "Goodbye, ", 0
TermMessage2 BYTE ".", 0 

FibMax DWORD 46
FibMin DWORD 1

UserName BYTE 25 dup(0)
KeyAnswer BYTE 2 dup(0)
NumFib DWORD ?

FibVal DWORD 1
FibVal2 DWORD 1

CurCol DWORD 0
ColMax DWORD 5

Yep BYTE "y", 0
Nope BYTE "n", 0

.code
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Simple procedure for printing a tab to the screen
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
InsertTab PROC
	mov edx,OFFSET TabString			;Moves the tab character into the buffer
	call WriteString					;Writes the tab to the screen
	ret									;Returns out of the procedure
InsertTab ENDP

main PROC
	call Clrscr							;Clears the screen

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Write title and programmers name to screen
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov	 edx,OFFSET ProgramTitle		;Gets title ready to print
	call WriteString					;Prints title
	call Crlf							;Prints a new line

	mov	 edx,OFFSET ProgrammersName		;Gets programmers name ready to print
	call WriteString					;Prints programmers name
	call Crlf							;Prints newline
	call Crlf							;Prints newline

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Prompt user for their name
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov	 edx,OFFSET NamePrompt			;Gets username prompt ready
	call WriteString					;Prints username prompt
	call Crlf							;Prints newline
				
	mov	 edx,OFFSET NamePrompt2			;Gets second half of username prompt ready
	call WriteString					;Prints username prompt part 2
	mov edx,OFFSET UserName				;Moves offset for username storage var into edx
	mov ecx,SIZEOF UserName				;Sets max number of characters to read in
	call ReadString						;Reads in username string from user
	call Crlf							;Prints newline
	call Crlf							;Prints newline
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Write user welcome message, with their name
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DoAgain:
	call Clrscr							;Clears the screen to clean up output
	mov	 edx,OFFSET NameResponse		;Gets the welcome message part 1 ready to print
	call WriteString					;Prints welcome message part 1
	mov edx,OFFSET UserName				;Gets username ready to print
	call WriteString					;Prints username
	mov	 edx,OFFSET NameResponse2		;Gets welcome message part 2 redy to print
	call WriteString					;Prints welcome message part 2
	call Crlf							;Prints a new line
	call Crlf							;Prints a new line

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Prompt user for number of fibonacci to calculate
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov	edx,OFFSET FibPrompt			;Gets prompt for fibonacci number ready
	call WriteString					;Prints fib prompt
	call Crlf							;Prints a new line
			
	jmp EnterNumber						;Jumps to enter number label to begin input

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;This prompt shows when the number enterred is not within 1 to 46
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IncorrectInput:
	call Crlf							;Prints a new line
	mov	 edx,OFFSET FibPromptIncorrect	;Gets incorrect range prompt ready to print
	call WriteString					;Prints incorrect range prompt
	call Crlf							;Prints a new line
		
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Main prompt and read-in of fibonacci value from user
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
EnterNumber:	
	mov	 edx,OFFSET FibPrompt2			;Get main number entry prompt ready to print
	call WriteString					;Prints main input prompt
	call ReadInt						;Reads in int from user

	cmp eax, FibMin						;Checks if input is less than 1
	jl IncorrectInput					;Jumps to incorrect input if true

	cmp eax, FibMax						;Checks if input is greater than 46
	jg IncorrectInput					;Jumps to incorrect input if true

	mov NumFib, eax						;Copies number to variable if in correct range
	call Crlf							;Prints a new line

	mov FibVal, 1						;Reset fib start vals
	mov FibVal2, 1						;Reset fib start vals
	mov CurCol, 0						;Reset current column position

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;This section prints the first fibonacci sequence value
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov eax, FibVal						;Gets first value of fibonacci sequence ready to print (1)
	call WriteInt						;Writes value to screen
	cmp NumFib, 1						;Checks if only this one value should be displayed
	je FibDone							;If true, jump to end of program
	inc CurCol							;Increment column index
	call InsertTab						;Prints tab for column alignment
	call InsertTab						;Prints tab for column alignment

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;This section prints the second fibonacci sequence value
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	Call WriteInt						;Prints the second value of the fibonacci sequence (also 1)
	cmp NumFib, 2						;Checks if only these two should be displayed
	je FibDone							;If true, jump to end of program

	inc CurCol							;Otherwise, increment column index
	call InsertTab						;Prints tab for column alignment
	call InsertTab						;Prints tab for column alignment
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Setup of registers for the impending loop
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov ecx, NumFib						;Sets loop ecx value
	sub ecx, 2							;Subtracts two because of the first two have already been printed

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Main fibonacci calculation and print loop
	;PSEUDO
	;newval = FibVal2 + Fibval
	;print newval
	;FibVal = FibVal2
	;FibVal2 = newval;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CalcFib:
	mov eax, FibVal2					;Moves most recent fibonacci value into one reg
	add eax, FibVal						;Adds second to last to most recent
	call WriteInt						;Prints this fibonacci value to screen

	push FibVal2						;Moves old most recent to temp spot for move
	pop FibVal							;Replaces second to last old val
	mov FibVal2, eax					;Moves new most recent val to FibVal2
	
	call InsertTab						;Prints tab for column alignment
	cmp eax, ExtraTab					;Check if output val requires extra tab on output
	jl OneExtra							;Jump to extra tab output label if necessary

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Return jump point for extra tab printing to make output alignment nice
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MidCalcFib:
	inc CurCol							;Increment column index
	mov edx, CurCol						;Place index in reg for compare
	cmp edx, ColMax						;Compare current col index with max
	jne CalcFibLoop						;Jump to rest of loop if newline not needed

	call Crlf							;Prints newline when needed
	mov CurCol, 0						;Resets current column index

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Jump location used for avoiding printing a newline from above
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CalcFibLoop:
	loop CalcFib						;Loops fib calc sequence until ecx counter 0
	jmp TryAgain						;Jumps to program end if loop done

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Jump location used for printing on screen tabs
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
OneExtra:
	call InsertTab						;Calls procedure to input tab when necessary
	jmp MidCalcFib						;Jumps back to where it was called

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;This section deals with jumping if the user wants to calculate more values
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IncorrectTry:
	call Crlf							;Prints a new line
	mov edx,OFFSET IncorrectTryMessage	;Gets ready to print the incorrect try message
	call Writestring					;Prints the incorrect try message

TryAgain:
	call Crlf							;Prints a new line
	call Crlf							;Prints a new line
	mov edx,OFFSET TryAgainMessage		;Gets ready to print the try again message
	call WriteString					;Prints the try again message
	call Crlf							;Prints a new line

	mov edx,OFFSET TryAgainMessage2		;Gets ready to print the try again message part 2
	call WriteString					;Prints the try again message 2
	mov edx,OFFSET KeyAnswer			;Assigns edx to offset of keyanswer for keyboard input
	mov ecx,SIZEOF KeyAnswer			;Sets number of characters to read in
	call ReadString						;Reads in keyboard input

	INVOKE Str_compare,OFFSET KeyAnswer,OFFSET Yep	;Uses a multi-input irvine procedure to do a string comparison
	je DoAgain							;Jump to DoAgain if the user answered 'y'

	INVOKE Str_compare,OFFSET KeyAnswer,OFFSET Nope	;Uses a multi-input irvine procedure to do a string comparison
	je FibDone							;Jumps to end if user answered 'n'

	jmp IncorrectTry					;Jumps to an incorrect input section if user enterred wrong values

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Section to show the termination message
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
FibDone:
	call Crlf							;Prints a new line
	call Crlf							;Prints a new line
	mov	 edx,OFFSET TermMessage			;Gets termination message part 1 ready to print
	call WriteString					;Prints termination message

	mov edx,OFFSET UserName				;Gets username ready to print
	call WriteString					;Prints username

	mov	 edx,OFFSET TermMessage2		;Gets termination message part 2 ready to print
	call WriteString					;Prints termination messag part 2

	call Crlf							;Prints a new line
	exit								;Ends main program
main ENDP
END main