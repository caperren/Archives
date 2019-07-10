/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw0_4.c
 * Created: Unknown by perrenc
 * Last Modified: 2012 February 21, 20:41 by perrenc
 *
 * Prints out integers from 10 down by using a for loop and if statement.
 *
 */

#include <stdio.h>

int main (int argc, char **argv) {

	/* Creates a variable for manipulation and sets it to an inital value of 10. */
	int x = 10;

	/* 
	 * Takes x and prints its value and a comma from 10 to 2, then when the if 
	 * statement falsifies, it prints the final value with a new line and exits.
	 */ 
	for( x ; x >= 1 ; x -= 1) {
		if (x != 1){
			printf("%d, ", x);
		}else{
			printf("%d\n", x);
		}
	}
}
