/*
 * My approach to writing the tests such that it found the error randomly, while
 * still completing in the shortest possible of time. To do this, I made a char
 * array that contained only the chars that were tested for to maximize 
 * potential for creating valid strings, and for moving through the states.
 * To come up with a char, I simply used mod against a call to rand such that it
 * gave me the index for a character from the array I made.
 * 
 * For creating valid strings, I allocated memory for the five character string
 * that would hopefully eventually become the word "reset" and added and extra 
 * spot for the null character. I then called my completed inputChar to get a 
 * random char and place it in each of the five spots in the array. Finally, I 
 * add in the null terminating character. 
 * 
 * In practice, running this does exactly what I want. Due to traversal of 
 * states being a simple matter of getting the right characters, the program's
 * state moves to nine incredibly fast. Then, all that's left is to iterate over
 * the random possibilities for strings until it comes up with the word "reset",
 * at which point the program prints "error" and exits.
 */