/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: flush.c
 * Created: 2012 February 16, 03:11 by perrenc
 * Last Modified: 2012 February 16, 03:11 by perrenc
 * 
 * Has the user input some text, prints out the first character entered along with its
 * ascii code and then reruns until it receives a "%" character.
 *
 */
 
#include <stdio.h>

/* ------Begin Functions------ */

/*Has the user input text, uses getchar to store the first character, then loops through
 * getchar until the buffer is clear when it receives a "\n". */
char getchar_and_flush(){
	char foo; //Used for temp user input storage
	int loop; //Used for looping to clear the buffer

	printf("Please enter some text. Press enter to finish. To quit, type a \"%\" symbol. ");
	foo = getchar ();
	loop = foo;
	while(loop != '\n'){
		loop = getchar ();
	}	
	return foo;
}


/* ------End Functions------ */

/* Initalizes foo, clears the screen, then calls getchar_and_flush until the return 
 * character is "%" */
int main(int argc, char **argv){

	char foo; //Used to check the return value of getchar_and_flush for looping

	system("clear");

	while(foo != '%'){
		foo = getchar_and_flush();
		printf("\nThe first character was %c or %d in ASCII.\n\n", foo, foo);
	}
	return 0;
}
