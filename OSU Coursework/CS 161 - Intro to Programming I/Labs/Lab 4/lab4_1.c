/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: lab4_1.c
 * Created: 2012 February 6, 15:10 by perrenc
 * Last Modified: 2012 February 8, 23:00 by perrenc
 * 
 * Asks the user for five floating point values, and prints them
 * if all are entered correctly, else it ends the program.
 */
 
#include <stdio.h>

int main(int argc, char **argv) {

	double f1 = -1.0; //First user float value
	double f2 = -1.0; //Second user float value
	double f3 = -1.0; //Third user float value
	double f4 = -1.0; //Fourth user float value
	double f5 = -1.0; //Fifth user float value

// Clears the screen, then explains what needs to be entered, and what will be printed.
	system("clear");
	printf("I am going to ask you to enter 5 different positive floating point numbers.\n");
	printf("I will end the program as soon as you enter a negative value or a repeat number.\n");
	printf("I will congratulate you and print out the 5 numbers if you successfully complete the task.\n\n");

/* Has the user input five values, and quits the program if an entered number is negative
 * or if two numbers entered in a row are the same. If the values are entered correctly,
 * it prints out the five values the user entered.
 */
			printf("Please enter your first number. ");
			scanf("%lf", &f1);
			if(f1 < 0){
				return(0);
			}	
			printf("Please enter your second number. ");
			scanf("%lf", &f2);
			if((f2 == f1) | (f2 < 0)){
				return(0);
			}
			printf("Please enter your third number. ");
			scanf("%lf", &f3);
			if((f3 == f2) | (f3 < 0)){
				return(0);
			}
			printf("Please enter your fourth number. ");
			scanf("%lf", &f4);
			if((f4 == f3) | (f4 < 0)){
				return(0);
			}
			printf("Please enter your fifth number. ");
			scanf("%lf", &f5);
			if((f5 == f4) | (f5 < 0)){
				return(0);
			}
			printf("\nYour numbers were: %lf, %lf, %lf, %lf, %lf\n\n", f1, f2, f3, f4, f5);

}
