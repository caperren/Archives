/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: division.c
 * Created: 2012 February 8, 16:58 by perrenc
 * Last Modified: 2012 February 8, 23:15 by perrenc
 *
 * Has the user enter two integers, swaps them if the second one entered is larger than
 * the first one, then divides the first by the second, first mod second, and the first
 * by the second as doubles. It then prints out these values along with the equation.
 */
 
#include <stdio.h>

int main(int argc, char **argv) {

	int userint1; //User input 1
	int userint2; //User input 2
	int swap; //Used for swapping input 1 and 2 if 2 is larger than 1
	double double1; //Used for making a double of userint1
	double double2; //Used for making a double of userint2

//Clears the screen, and asks the user to enter the two integers
	system("clear");
	printf("Please enter an integer: ");
	scanf("%d", &userint1);
	printf("Please enter a second integer: ");
	scanf("%d", &userint2);

//Swaps userint1 and userint2 if userint2 is larger than userint1
	if(userint2 > userint1){
		swap = userint1;
		userint1 = userint2;
		userint2 = swap;
	}
	
//Prints and error and ends the program if you try and divide by zero	
	if(userint2 == 0){
		printf("\nYou cannot divide by zero, program ending...\n\n");
		return(1);
	}	

//Prints userint1/userint2 and userint1 mod userint2
	printf("\n%d / %d = %d\n", userint1, userint2, (userint1 / userint2));
	printf("%d %% %d = %d\n", userint1, userint2, (userint1 % userint2));

//Converts userint1 and 2 to doubles
	double1 = userint1;
	double2 = userint2;

//Prints double1/double2
	printf("%lf / %lf = %lf\n\n", double1, double2, (double1 / double2));
	
}
