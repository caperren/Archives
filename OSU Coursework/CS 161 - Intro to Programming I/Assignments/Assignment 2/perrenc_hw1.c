/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw1.c
 * Created: 2012 February 17, 14:47 by perrenc
 * Last Modified: 2012 February 20, 17:07 by perrenc
 *
 * This program is the entirety of assigment 2 for ece151
 * and requires compiling with the -lm argument to include 
 * math linking.
 */
 
/* Includes */
#include <stdio.h>
#include <math.h>

/* Defines */

// For prob 1 ( 10.7 )
#define WEEK 5
#define DAY 7

/* --------------------Begin Functions-------------------- */

/* ********Problem 1******** */

/* 
 * This program takes a month of web page hit values and averages them per week day for 
 * the month.
 */
void prob1(void){

	system("clear"); 			//Clears the screen

	int dayloop; 				//For looping the day variable
	int weekloop; 				//For looping the week variable
	int mean[DAY] = {0, 0, 0, 0, 0, 0, 0}; 	//Array for storing mean values
	
	/* This is the array taken from the book. */
	int hits[WEEK][DAY] = {367, 654, 545, 556, 565, 526, 437, 
			       389, 689, 554, 526, 625, 537, 468,
			       429, 644, 586, 626, 652, 546, 493, 
			       449, 689, 597, 679, 696, 568, 522,
			       489, 729, 627, 729, 737, 598, 552};

	/* Fills the mean array with sums of the weekday values for the month. */
	for (weekloop = 0 ; weekloop < WEEK ; weekloop++){
		for (dayloop = 0 ; dayloop < DAY ; dayloop++){
			mean[dayloop] += hits[weekloop][dayloop];
		}
	}
	
	/* 
	 * Averages the values in mean for the number of weekdays summed in the previous
	 * step.
	 */
	for (dayloop = 0 ; dayloop < DAY ; dayloop++){
		mean[dayloop] /= WEEK;
	}

	/* Prints the mean values for each day of the week. */
	printf("The mean number of hits for Mondays of this month was    %d\n", mean[0]);
	printf("The mean number of hits for Tuesdays of this month was   %d\n", mean[1]);
	printf("The mean number of hits for Wednesdays of this month was %d\n", mean[2]);
	printf("The mean number of hits for Thursdays of this month was  %d\n", mean[3]);
	printf("The mean number of hits for Fridays of this month was    %d\n", mean[4]);
	printf("The mean number of hits for Saturdays of this month was  %d\n", mean[5]);
	printf("The mean number of hits for Sundays of this month was    %d\n", mean[6]);

	pause ();	//Waits for user to press enter before returning to the menu
}


/* ********Problem 2******* */

/* Calculates a centroid value for an array of pixels with intensities. */
void prob2(void){
	
	system("clear"); 		//Clears the screen

	int loop; 			//Used for loop incrementing
	double centroidx = 0; 		//Stores the centroid x value
	double centroidy = 0; 		//Stores the centroid y value
	double intensitytotal = 0; 	//Stores the intensity total
	
	/* Pixels locations and intensities from the book. */
	double pixel[][2] = {5.0, -4.0, 4.0, 3.0, -1.0, 6.0, -9.0, 5.0};
	double intensity[] = {.25, .57, .63, .1};

	/* 
	 * Loops through the pixel array and performs the summing for the centroid
	 * calculation and stores the values in centroidx and y.
	 */
	for (loop = 0 ; loop <= 3 ; loop++){
		centroidx += (pixel[loop][0]*intensity[loop]);
		centroidy += (pixel[loop][1]*intensity[loop]);
	}
	
	/*
	 * Loops through the intensity array and sums the four values and stores it in
	 * intensitytotal.
	 */
	for (loop = 0 ; loop <= 3 ; loop++){
		intensitytotal += intensity[loop];
	}	

	/*
	 * Divides the centroidx and y sums by the intensitytotal to complete the last 
	 * step of the centroid calculation.
	 */
	centroidx /= intensitytotal;
	centroidy /= intensitytotal;

	/* Prints the centroid value. */
	printf("Centroid = %.2lf, %.2lf", centroidx, centroidy);
	
	pause ();	//Waits for user input before returning to the menu.	
}

/* ********Problem 3******** */

/* 
 * Searches an array of binary values for a specific binary value and returns whether it
 * finds it.
 */
void prob3(void){

	system("clear"); 		//Clears the screen
	
	int loop;			//Used for looping
	int searchvalue = 0b01010001;	//Value that is searched
	int searchreturn;		//Stores binary search return value

	/* Random binary data. */
	int data[8] = {0b00000001, 0b00010001, 0b10010101, 0b01101110, 
		       0b11011011, 0b11111110, 0b00111101, 0b10011101};
	
	printf("---------- Data Array Values --------\n");	//Prints a header
	
	/* Prints the array. */
	for (loop = 0 ; loop < 8 ; loop++){
		printf("%d\n", data[loop]);
	}	

	/* Searches the array with the binary value specified above. */
	searchreturn = binarysearch (8, data, searchvalue);

	/*
	 * Prints a different message depending on whether the value searched was 
	 * found or not.
	 */
	if (searchreturn != -1){
		printf("\nThis binary value equivalent to %d was found in ", searchvalue); 
		printf("element %d of the array.\n", searchreturn);
	}else {
		printf("\nThis binary value equivalent to %d could not ", searchvalue);
		printf("be found in the array.");
	}
	
	pause (); 	//Watis for the user to press enter before returning to the menu
}

/*
 * Function that compares an array of binary values against a search value and returns
 * whether the search term was found.
 */
int binarysearch(int arraysize, int data[], int search){

	int loop;	//Used for looping

	/* 
	 * Searches the array for a binary value and returns whether it was found or 
	 * not. 
	 */
	for (loop = 0 ; loop < arraysize ; loop++){
		if (search == data[loop]){
			return loop;
		}	
	}
	
	return -1;	//Returns if the search value is not found
}

/* ********Problem 5******** */

/* Performs matrix multiplication on two arrays the size of which the user can specify */ 
void prob5(void){

	system("clear"); 	//Clears the screen
	
	int arraysize; 		//Used for storing the array size
	int loop1; 		//Used for level one looping
	int loop2; 		//Used for level two looping
	int loop3; 		//Used for level three looping

	/* Allows the user to specify the size of the arrays to multiply. */
	printf("Please enter the number of columns and rows the arrays should have: ");
	scanf("%d", &arraysize);

	system("clear"); 			//Clears the screen

	int array1[arraysize][arraysize]; 	//First array
	int array2[arraysize][arraysize]; 	//Second array
	int array3[arraysize][arraysize]; 	//Third array with matrix multiplication

	/* 
	 * Fills arrays 1 and 2 with numbers based on where they are in the loops and fills 
	 * array three with zeros so we don't get random values.
	 */
	for (loop1 = 0 ; loop1 < arraysize ; loop1++){
		for (loop2 = 0 ; loop2 < arraysize ; loop2++){
			array1[loop1][loop2] = (2*((loop1+1)*(loop2+1)));
			array2[loop1][loop2] = (4*((loop1+1)*(loop2+2)));
			array3[loop1][loop2] = 0;
		}	
	}
	
	printf("This is array 1:\n"); 		//Prints a header for array 1

	/* Prints out array 1 */
	for (loop1 = 0 ; loop1 < arraysize ; loop1++){
		for (loop2 = 0 ; loop2 < arraysize ; loop2++){
			printf("%d 	", array1[loop1][loop2]);
		}
		printf("\n");
	}	
	
	printf("\nThis is array 2:\n"); 	//Prints a header for array 2

	/* Prints out array 2 */
	for (loop1 = 0 ; loop1 < arraysize ; loop1++){
		for (loop2 = 0 ; loop2 < arraysize ; loop2++){
			printf("%d 	", array2[loop1][loop2]);
		}
		printf("\n");
	}	
	
	/* 
	 * Performs matrix multiplication on array 1 and array 2 and stores it on 
	 * array 3. 
	 */	
	for (loop1 = 0 ; loop1 < arraysize ; loop1++){
		for (loop2 = 0 ; loop2 < arraysize ; loop2++){
			for (loop3 = 0 ; loop3 < arraysize ; loop3++){
				array3[loop1][loop2] += (array1[loop1][loop3] * 
							 array2[loop3][loop2]);
			}				 
		}
	}	
	
	/* Prints a header for array 3 */	
	printf("\nThis is array 3, which is the result of matrix multiplication on ");
	printf("array 1 and 2:\n");

	/* Prints out array 3 */
	for (loop1 = 0 ; loop1 < arraysize ; loop1++){
		for (loop2 = 0 ; loop2 < arraysize ; loop2++){
			printf("%d 	", array3[loop1][loop2]);
		}
		printf("\n");
	}	
	
	pause (); 	//Waits for the user to press enter before returning to the menu
}

/* ********Problem 6******** */

/* 
 * Calculates primes up to a user entered value by marking composites and printing 
 * what's left.
 */
void prob6(void){

	system("clear");	//Clears the screen

	int userin1; 		//Used for user input
	int loop1; 		//Used for level one loops
	int loop2;		//Used for level two loops
	int loop3;		//Used for level three loops
	int total = 0;		//Used for totaling the number of primes

	/* 
	 * Asks the user for a number to calculate prime number to, stores it, and 
	 * clears the screen.
	 */
	printf("Enter the value you want primes calculated to: ");
	scanf("%d", &userin1);
	system("clear");

	int sieve[(userin1-1)];		//Creates an array for holding sieve values in

	/* Fills the array with integers from 2 to the user entered value */
	for(loop1 = 2 ; loop1 <= userin1 ; loop1++){
		sieve[(loop1-2)] = loop1;
	}

	/* Sets all composites in the array to zero. */
	for(loop1 = 0 ; loop1 < userin1; loop1++){
		for(loop2 = 2; loop2 < userin1; loop2++){ 		
			for(loop3 = loop2; loop3 < userin1; loop3++){
				if ((loop2*loop3) == sieve[loop1]){
					sieve[loop1] = 0;
				}	
			}	
		}
	}
	
	/*
	 * Prints out all values in the array that aren't zero, which leaves you with
	 * only prime numbers.
	 */
	printf("Here are the prime numbers from 0 to the number you entered: \n\n");
	for(loop1 = 0 ; loop1 < (userin1-1) ; loop1++){
		if(sieve[loop1] != 0){
			printf("%d\n", sieve[loop1]);
			total++;	//Increments total when something is printed
		}	
	}

	/* Prints the number of primes. */
	printf("\nThere were %d primes in this list. ", total);
	
	pause ();	//Waits until the user presses enter, then returns to the menu
}


/* ********Problem 7******* */

/* 
 * This takes in a string, then copies each value to another string and adds a null 
 * character before returning the copied string.
 */
char str_copy(char *s2, const char *s1){

	int loop; 		//Used for looping

	/*
	 * Copies each character in string1 to the same place in string2 until it
	 * encounters the null character.
	 */
	for (loop = 0 ; s1[loop] != '\0' ; loop++){
		s2[loop] = s1[loop];
	}

	s2[(loop++)] = '\0';	//Adds the null character back in
	return *s2;		//Returns the copied string
}

/* Takes a user entered string and copies it to another string. */
void prob7(void){
	char string1[100];	//Storage for string1
	char string2[100];	//Storage for string2
	int loop; 		//Used for looping

	system("clear");	//Clears the screen

	/* 
	 * Asks the user to enter a string, clears the buffer, and stores the string on 
	 * string1.
	 */
	printf("Please enter a string:\n");
	while(getchar() != '\n'){
		; //VOID
	}	
	fgets(string1, 100, stdin); 

	/* 
	 * Replaces the newline character fgets scans in with a null character to make
	 * the array into a string.
	 */
	for (loop = 0 ; loop < 100 ; loop++){
		if (string1[loop] == '\n'){
			string1[loop] = '\0';
			break;
		}
	}
	
	printf("\nThis is string 1:\n%s\n\n", string1); 	//Prints string1 header
	
	str_copy(string2, string1); 				//Copies str1 to str2
	
	printf("This is string 2 after being copied:\n%s\n", string2);	//Prints header

	/* 
	 * This is a modified version of my pause function that accounts for the fact 
	 * that a buffer flush already happened. Essentially, this removes the extra
	 * getchar so that you don't have to hit enter twice to return to the menu.
	 */
	printf("\n\nPress enter to return to the menu.");
	while(getchar() != '\n'){
		; 	//VOID
	}
}	


/* Generic Functions */

/* Pauses until the user presses enter, then clears the buffer in case they type 
 * somthing before they hit enter */
int pause(void){
	printf("\n\nPress enter to return to the menu.");
	getchar ();
	while('\n' != getchar()){
		;	//VOID
	}	
}

/* --------------------End Functions-------------------- */

int main(int argc, char **argv){
	
	int menu; 				// Used for selecting a menu item

	beginning: ; 				// Used for goto statements in switch

	system("clear"); 			//Clears the screen

	/* Creates the list of possible selections for the user menu. */
	printf("Please select which homework problem you would like to run:\n");
	printf("1 -- Problem 1 ( 10.7 )\n");
	printf("2 -- Problem 2 ( 10.10 )\n");	
	printf("3 -- Problem 3 ( 10.30 )\n");
	printf("4 -- Problem 5 ( Matrix Multiplication )\n");
	printf("5 -- Problem 6 ( Sieve of Eratosthenes )\n");
	printf("6 -- Problem 7 ( 11.6 )\n");
	printf("7 -- Exit Program\n\n");
	printf("Enter Selection: ");
	scanf("%d", &menu);

	/* 
	 * Runs a particular program based on the user's input. Goto's are used since
	 * switch is a much cleaner way to impliment the menu selection and I could not
	 * find an alternative way.
	 */
	switch(menu){
		case 1:
			prob1 (); 		//Runs problem 1
			goto beginning; 	//Goes back to the menu after running
		case 2:
			prob2 (); 		//Runs problem 2
			goto beginning; 	//Goes back to the menu after running
		case 3:
			prob3 (); 		//Runs problem 3
			goto beginning; 	//Goes back to the menu after running
		case 4:
			prob5 (); 		//Runs problem 5
			goto beginning; 	//Goes back to the menu after running
		case 5:
			prob6 (); 		//Runs problem 6
			goto beginning; 	//Goes back to the menu after running
		case 6:
			prob7 ();		//Runs problem 7
			goto beginning; 	//Goes back to the menu after running
		case 7:
			system("clear");	//Clears the screen
			return 0;		//Ends the program
			
	}

}
