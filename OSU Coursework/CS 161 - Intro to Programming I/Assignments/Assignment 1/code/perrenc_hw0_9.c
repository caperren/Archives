/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw0_9.c
 * Created: Unknown by perrenc
 * Last Modified: 2012 February 21, 20:54 by perrenc
 *
 * Print the square and cube roots of values from 1 to 25.
 *
 */

#include <stdio.h>
#include <math.h>

int main(int argc, char **argv){

	int x; 		// Variable used for incrementing from 1 to 25
	double y; 	// Variable used for squares and cubes manipulation

	/*
	 * Increments x from 1 to 25 and prints the x value, its square root, and cube 
	 * root in column format, then ends.
	 */
	for(x = 1 ; x <= 25 ; x++){
		y = x;
		printf("%i	", x);
		y = sqrt (x); 		// Takes the square root of x	
		printf("%f	", y);
		y = pow(x, (1/3.0)); 	// Takes the cube root of x
		printf("%f	\n", y);
	}
}
