/* Original Author: Corwin Perren (perrenc)
 * File: lab4_4.c
 * Created: 2012 February 08, 21:05 by perrenc
 * Last modified: 2012 February 08, 23:23 by perrenc
 *
 * Purposefully used for making and correcting errors
 */

#include <stdio.h>

int main(int argc, char **argv) {

	/*creates the user input and incrementing variables*/
	int i;
	int loop;
	
	system("clear");
	printf("Enter an integer between 1 and 20:\n");
	scanf("%d", &i);

	/*errors and exits if i is greater than 20 or less than one*/
	if((1 > i) | (i > 20)){
		printf("You have entered an incorrect value.\n");
		return(-1);
	}
	/*prints ascending characters from one up to the user entered value*/
	for (loop = 1; (loop <= i); ++loop){
		printf("%d ", loop);
	}

	printf("\n");

	/*prints descending characters from the user entered value down to 1*/
	for(loop = i; ((loop <= i) & (loop != 0)); loop--){
		printf("%d ", loop);
	}
	printf("\n");
}

/*error listings
 * Removed semicolon at line 23: "expected ; before return"
 * Commented out variable i on line 14: "i undeclared (first use in this function.
		Each undeclared identifier is reported only once for each function it appears in."
 * Removed braces from the following if function: "expected } before else"

int extra() {
	if(x < 3) {
		printf("x is less than 3.\n");
		printf("you fart loudly.");
	else
		printf("x is not less than 3.\n");
		printf("et cetera\n");
}
}
 * Removed parentheses from line 27: "expected ( before loop
									  expected ; before { token"
 * Passed wrong type to line 28: It compiled, but output was empty.
 */
