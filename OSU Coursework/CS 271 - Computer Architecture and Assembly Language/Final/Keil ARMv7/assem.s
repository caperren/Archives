	AREA ConstrainProg, CODE, READWRITE
	ENTRY 
Start 
	ADR sp, Base ;Point to the base of the stack
	LDR r0, LowerVal ;Load the lower value into r0
	LDR r1, UpperVal ;Load the upper value in r1
	LDR r2, ParamToConstrain ;Load the value to constrain into r2
	STR r0,[sp,#-4] ;Load r0 onto the stack pointer
	STR r1,[sp,#-4] ;Load r1 onto the stack pointer
	STR r2,[sp,#-4] ;Load r2 onto the stack pointer
	BL Constrain ;Call constrain procedure
	LDR r2, [sp] ;Load constrained value back into r2
	;r2 should now contain 2000
	ADD sp, sp, #8 ;Fix the stack pointer
Constrain
	STR LR, [sp, #-4] ;Store return address
	LDR r5, [sp, #12] ;Retrieve lower
	LDR r4, [sp, #8] ;Retrieve upper
	LDR r3, [sp, #4] ;Retrieve val to constrain
	CMP r3, r4 ;Check if higher than max
	LDRHI r3, [sp, #8] ;If so, load max
	CMP r3, r5 ;Check if lower than min
	LDRLO r3, [sp, #12] ;If so, load min
	STR r3, [sp, #4] ;Overwrite constrained val
	LDR PC, [sp], #4 ;Return from procedure
ParamToConstrain DCD 2200 ;Value to constrain
UpperVal DCD 2000  ;Value to constrain
LowerVal DCD 1000  ;Value to constrain
Base DCD 0xAAAAAAAA
	END