/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw0_1.c
 * Created: Unknown by perrenc
 * Last Modified: 2012 February 21,20:33 by perrenc
 *
 * Prints out integers and decimals with specific numbers of decimal points.
 *
 */

#include <stdio.h>

int main (int argc, char **argv) {

	/* 
	 * Creates integers and float variables, then assigns a computed value to them
	 */
	int a = (2 * 3);
	int b = (1 / 5);
	int c = (1 / 2);
	int d = (3 / 2);
	int e = (1 / 3);
	int f = (2 / 3);
	double g = (1.0 / 5);
	double h = (1.0 / 2);
	double i = (3.0 / 2);
	double j = (1.0 / 3);
	double k = (2.0 / 3);
	double l = (1 / 5.0);
	double m = (1 / 2.0);
	double n = (3 / 2.0);
	double o = (1 / 3.0);
	double p = (2 / 3.0);

	/* 
	 * Prints the integers with no decimal points and then prints the float values
	 * with 6 and 20 decimal points
	 */
	printf("a = %i\n", a);
	printf("b = %i\n", b);
	printf("c = %i\n", c);
	printf("d = %i\n", d);
	printf("e = %i\n", e);
	printf("f = %i\n", f);
	printf("g = %.06f\n    %.20f\n", g, g);
	printf("h = %.06f\n    %.20f\n", h, h);
	printf("i = %.06f\n    %.20f\n", i, i);
	printf("j = %.06f\n    %.20f\n", j, j);
	printf("k = %.06f\n    %.20f\n", k, k);
	printf("l = %.06f\n    %.20f\n", l, l);
	printf("m = %.06f\n    %.20f\n", m, m);
	printf("n = %.06f\n    %.20f\n", n, n);
	printf("o = %.06f\n    %.20f\n", o, o);
	printf("p = %.06f\n    %.20f\n", p, p);

}
