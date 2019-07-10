/** 
Change Machine Source File for Lab2 in CS 151
Made by Corwin Perren

Takes a money amount as an input and tells you the appropriate number of each bill and 
coin less than or equal to $20 increments to make up total amount.
*/

#include <stdio.h> /** Include the stdio.h file to allow for printf() to be used*/

/** @brief Main Function
		@param void This function does not accept any input variables
		@return This function would return an error code to the OS if needed.
*/
int main (int argc, char **argv) {

/** Original money amount and all bill/coin variables. x is used for manipulation of 
the original amount. 
*/
	double i = 00.00; /* initial input variable */
	int twenties = 0; /* $20 variable */		
	int tens = 0; /* $10 variable */
	int fives = 0; /* $5 variable */
	int ones = 0; /* $1 variable */
	int quarters = 0; /* $.25 variable */
	int dimes = 0; /* $.1 variable */
	int nickels = 0; /* $.05 variable */
	int pennies = 0; /* $.01 variable */
	int x = 0; /* used for the running total as the code progresses */
	
	system("clear"); /** Clears the screen */
	
	/** Prompts the user for a numerical input, stores it, multiplies it by 100, then
	stores that value as an integer on x. 
	*/	
	printf("Please enter the amount of money to make change for: ");
	scanf("%lf",&i);
	i = i*100 ;
	x = (int)i;
	
	/** Takes the money amount, determines the number of each bill or coin, and subtracts
	that number of bills from the total amount to calculate the next amount.
	*/
	twenties = x/2000;
	x = x-twenties*2000;
	tens = x/1000;
	x = x-tens*1000;
	fives = x/500;
	x = x-fives*500;
	ones = x/100;
	x = x-ones*100;
	quarters = x/25;
	x = x-quarters*25;
	dimes = x/10;
	x = x-dimes*10;
	nickels = x/5;
	x = x-nickels*5;
	pennies = x/1;
	
	system("clear"); /** Clears the screen */
	
	/** Print the original input amount, then the number of bills/coins needed only
	if that number is not zero.
	*/
	printf("%.2lf 	- Original Amount\n\n", (i/100));
	if (twenties != 0){
		printf("%i 	- Twenties\n",twenties);
	}if (tens != 0){
		printf("%i 	- Tens\n",tens);
	}if (fives != 0){
		printf("%i 	- Fives\n",fives);
	}if (ones != 0){
		printf("%i 	- Ones\n",ones);
	}if (quarters != 0){
		printf("%i 	- Quarters\n",quarters);
	}if (dimes != 0){
		printf("%i 	- Dimes\n",dimes);
	}if	(nickels != 0){
		printf("%i 	- Nickels\n",nickels);
	}if (pennies != 0){
		printf("%i 	- Pennies\n",pennies);
	}
	
	return (0);
}
