/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: lab5.c
 * Created: 2012 February 16, 00:22 by perrenc
 * Last Modified: 2012 February 16, 00:22 by perrenc
 * 
 * This program asks the user for two values, counts up to it from zero, down to zero 
 * from it, and bewteen the two values. It then asks whether the user would like to 
 * run the program again.
 */
 
#include <stdio.h>

/* ------Begin Functions------ */

/* Begin user input functions */

/* Asks the user for a positive integer and returns it */
int userinput1(void){
	int a; // For temp user input storage
	system("clear");
	printf("Please enter your first positive integer: ");
	scanf("%d", &a);
	return a;
}	

/* Asks the user for another positive integer and returns it */
int userinput2(void){
	int a; // For temp user input storage
	printf("Please enter your second positive integer: ");
	scanf("%d", &a);
	return a;
}	

/* End user input functions */

/* Begin Math Functions */

/* Receives the first integer from the user and counts up from 0 to it with integers */
int count_up(int userin1){
	int loop; //Used for loop incrementing
	for(loop = 0 ; loop <= userin1 ; loop++){
		printf("%d ", loop);
	}
	printf("\n");
}	

/* Counts down to zero by integers from the first integer entered by the user */
int count_down(int userin1){
	int loop; //Used for loop incrementing
	for(loop = userin1 ; loop >= 0 ; loop--){
		printf("%d ", loop);
	}
	printf("\n");
}

/* Counts from the smaller number up to the larger one based on those entered by 
 * the user */
int count_between_up(int userin1, int userin2){
	int loop; //Used for loop incrementing
	for(loop = userin1 ; loop <= userin2 ; loop++){
		printf("%d ", loop);
	}
	printf("\n");
}

/* Counts down from the larger number to the smaller one from those entered by the user */
int count_between_down(int userin1, int userin2){
	int loop; //Used for loop incrementing
	for(loop = userin1 ; loop >= userin2 ; loop--){
		printf("%d ", loop);
	}
	printf("\n");
}

/* End Math Functions */

/* Begin Generic Functions */

/* Assigns the user inputs to variables, counts up, then down, then counts up or down
 * between the two entered number depending on which of the two is larger */
void task1(void){
	
	int userin1 = userinput1 ();
	int userin2 = userinput2 ();

	count_up (userin1);
	count_down (userin1);

	if (userin1 < userin2){
		count_between_up (userin1, userin2);
	}else if (userin1 > userin2){
		count_between_down (userin1, userin2);
	}		
}

/* Used for quitting or rerunning the program per user request */
int quittask1(void){
	
	int endtask1;

	printf("\nWould you like to run again? Type 0 for no or 1 for yes: ");
	scanf("%d", &endtask1);

	return endtask1;
}	
	
/* ------End Functions------ */

/* Runs task 1 from above and determines whether the program should exit or rerun based
 * on the return value from quittask1. To rerun, a goto is used since I could not get 
 * loops to work. */
int main(int argc, char **argv){

	begintask1: ;
	task1 ();
	int endtask1 = quittask1 ();
	if(endtask1 == 0){
		//Break
	}else if(endtask1 == 1){
		goto begintask1;
	}	

	return 0;
}
