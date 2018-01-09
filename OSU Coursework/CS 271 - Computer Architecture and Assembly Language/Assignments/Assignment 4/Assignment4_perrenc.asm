;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TITLE Assignment 3						(main.asm)
;Author: Corwin Perren
;Date: 2/05/2014
;Description:
;	This program greets a user by name, takes in ten integers, sums 
;	non-negative ones, and computers the average of that sum before saying
;	goodbye to the user
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

INCLUDE Irvine32.inc

.data
ProgramTitle BYTE "CS 271 - Assignment 3",0
ProgrammerName BYTE "Written by: Corwin Perren",0

NamePrompt BYTE "By what name may I call you?",0
NamePrompt2 BYTE "Your name: ", 0

NameResponse BYTE "Welcome, ", 0
NameResponse2 BYTE ".", 0

EnterNumPrompt BYTE "Please enter ten numbers between 0 and 100.", 0
TooGreat BYTE "The number you entered was too large, please try again.", 0
IncorrectInputMessage BYTE "You did not enter a valid number.", 0

ListingPrint BYTE "Number ", 0
ListingPrint2 BYTE ":", 0

NoPosMessage BYTE "No Positive Integers Entered...", 0
NumPosMessage BYTE "Number of Positive Integers: ", 0
SumMessage BYTE "Sum of Positive Integers: ", 0
AvgMessage BYTE "Average of Positive Integers: ", 0

TermMessage BYTE "Goodbye, ", 0
TermMessage2 BYTE ".", 0 

TESTMessage BYTE "Equal to Zero", 0

NumEntries DWORD 10
MaxEntryVal	DWORD 100

StorArray DWORD 10 dup(0)
UserName BYTE 25 dup(0)

NumPos DWORD 0
FinalSum DWORD 0
FinalAvg DWORD 0


.code

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for writing string with newline
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Write_String_NL PROC uses EDX,
	StringPtr:Ptr BYTE								;Varible to store string offset pointer

	mov edx, StringPtr								;Moves the string to the proper register
	call WriteString								;Prints the string
	call Crlf										;Prints a new line

	ret
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
	INVOKE Write_String_NL, OFFSET ProgrammerName	;prints the programmer's name'
	call Crlf										;Prints a new line
	ret
PrintTitle ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for getting user's name and greeting them
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MeetAndGreet PROC
	INVOKE Write_String_NL, OFFSET NamePrompt		;Gets username prompt ready
				
	INVOKE Write_String, OFFSET NamePrompt2			;Gets second half of username prompt ready
	
	mov edx,OFFSET UserName							;Moves offset for username storage var into edx
	mov ecx,SIZEOF UserName							;Sets max number of characters to read in
	call ReadString									;Reads in username string from user

	call Clrscr										;Clears the screen to clean up output
	INVOKE Write_String, OFFSET NameResponse		;Prints the first part of the welcome message
	INVOKE Write_String, OFFSET UserName			;Prints the user's name
	INVOKE Write_String_NL, OFFSET NameResponse2	;Prints teh second half of the welcome message
	call Crlf										;Prints a new line

	ret
MeetAndGreet ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for printing what the user may enter
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintEnterNumPrompt PROC
	INVOKE Write_String_NL, OFFSET EnterNumPrompt	;Gets welcome message part 2 redy to print
	ret
PrintEnterNumPrompt ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for retreiving integer inputs from user
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GetNumbers PROC

	mov ecx, NumEntries								;Sets loop count variable
	mov esi, OFFSET StorArray						;Puts storage array into esi reg
	jmp EnterNum									;Jump to beginning of number entry

IncorrectInput:
	INVOKE Write_String_NL, OFFSET IncorrectInputMessage	;Displays incorrect input message
	jmp EnterNum									;Jumps to beginning of number entry
GreaterThanMax:
	INVOKE Write_String_NL, OFFSET TooGreat			;Displays number to great message
EnterNum:	
	INVOKE Write_String, OFFSET ListingPrint		;Prints number entry listing
	
	mov eax, NumEntries								;Moves total entries to eax
	inc eax											;Increments by one to start at 1 rather than 0
	sub eax, ecx									;Subracts current loop var to determine count val
	call WriteInt									;Writes for number entry listing

	INVOKE Write_String, OFFSET ListingPrint2		;Finishes priting number entry listing

	call ReadInt									;Reads in int from user
	jo IncorrectInput								;Re-do previous if incorrect input
	cmp eax, MaxEntryVal							;Check if number too great
	jg GreaterThanMax								;Jump if number too great

	mov [esi], eax									;Move number if valid into array
	add esi, 4										;Increment array pointer

	loop EnterNum									;Loop unti ecx equals 0
	ret
GetNumbers ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for determining number of positive integers, summing them, and
	;calculating their average
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CalcStuff PROC
	mov ecx, NumEntries								;Puts loop counter in
	mov esi, OFFSET StorArray						;Gets array ready to loop through
	mov eax, 0										;Makes sum register zero

ArrayLoop:
	mov edx, [esi]									;Read out one array value
	add esi, 4										;Increment array pointer
	cmp edx, 0										;Check if negative
	jl ArrayLoopEnd									;Jump to ignore if negative

	add eax, edx									;If non-negative, add to sum reg
	inc NumPos										;Increment number of positive found


ArrayLoopEnd:
	loop ArrayLoop									;Loop until array empty

	cmp NumPos, 0									;Check if any positive found
	jne MoreThanZero								;Jump if there are positive integers

	Invoke Write_String_NL, OFFSET NoPosMessage		;Write no positive intergers message
	jmp QuickEnd									;End program because no positive integers
	
MoreThanZero:
	mov FinalSum, eax								;Move positive integers sum to variable
	mov edx, 0										;Zero upper of numerator
	mov ebx, NumPos									;Put value in lower of numerator
	div ebx											;Divide sum by number of positive integers
	mov FinalAvg, eax								;Take result which is average and store in varible
	
	call Crlf										;Print a new line
	ret
CalcStuff ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Prints the number of positive integers
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintNumPos PROC
	Invoke Write_String, OFFSET NumPosMessage		;Print string for number of positive integers
	mov eax, NumPos									;Move varaible into printing register
	call WriteInt									;Print number of positive integers
	call Crlf										;Print a new line
	ret
PrintNumPos ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Prints the sum of the positive integers
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintSumOf PROC
	Invoke Write_String, OFFSET SumMessage			;Print the message for the sum
	mov eax, FinalSum								;Move sum into printing register
	call WriteInt									;Print sum
	call Crlf										;Print new line
	ret
PrintSumOf ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Prints the average of the positive integers 
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintAverageOf PROC
	Invoke Write_String, OFFSET AvgMessage			;Print message for the average
	mov eax, FinalAvg								;Move average into printing register
	call WriteInt									;Print the average
	call Crlf										;Print a new line
	ret
PrintAverageOf ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Procedure for printing the termination message
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintTerm PROC
	Invoke Write_String, OFFSET TermMessage			;Print the first part of the termination message
	Invoke Write_String, OFFSET UserName			;Print the user's name
	Invoke Write_String_NL, OFFSET TermMessage2		;Print the second part of the termination message
	ret
PrintTerm ENDP

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Main Procedure
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main PROC
	call PrintTitle									;Prints title
	call MeetAndGreet								;Greets user by name
	call PrintEnterNumPrompt						;Print prompt for program use
	call GetNumbers									;Retrieves user integer entries
	call CalcStuff									;Performs math on numbers entered
	call PrintNumPos								;Prints number of positive integers
	call PrintSumOf									;Prints sum of positive integers
	call PrintAverageOf								;Prints average of positive integers
QuickEnd::
	call PrintTerm									;Prints termination message
	
	exit
main ENDP

END main