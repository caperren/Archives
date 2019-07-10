/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw0_2.c
 * Created: Unknown by perrenc
 * Last Modified: 2012 February 21, 20:36 by perrenc
 *
 * Uses printf to print integers the same way through two different methods.
 *
 */

#include <stdio.h>

int main (int argc, char **argv) {

	/* 
	 * Creates variables for holding integers between and including 1 to 4 assigns
	 * one of these values to each variable.
	 */
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	/* Prints the four integer values on one line using a single printf statement. */
	printf("%d, %d, %d, %d\n\n", a, b, c, d);
	
	/* 
	 * Prints the four integer values on the same line using four different printf 
	 * statements.
	 */
	printf("%d, ", a);
	printf("%d, ", b);
	printf("%d, ", c);
	printf("%d\n", d);
}
