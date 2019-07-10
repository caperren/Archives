/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw0_5.c
 * Created: Unknown by perrenc
 * Last Modified: 2012 February 21, 20:49 by perrenc
 *
 * Total all integers from 1 to whatever value is necessary to make x in the loop equal
 * to 1000.
 *
 */

#include <stdio.h>

int main(int argc, char **argv){

	int x;		// Variable used for incrementing
	int total; 	// Variable used for holding the total amount

	/* Increments x, then adds current x to the total until x is equal to 1000. */
	for(x = 1 ; x <= 1000 ; x++){
		total += x;
	}
	
	/* Prints the total after going through the loop. */
	printf("%d\n", total);
}
