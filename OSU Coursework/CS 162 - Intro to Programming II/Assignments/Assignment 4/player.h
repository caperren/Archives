#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;


class Player{
public:
	friend class Rps;
	
	Player(){
		srand ( time(NULL) );
		wins(0);
		losses(0);
		}
		
		Player
	
	string guess(){
		int number;
		
		number = ((int)(rand() * 10))%3;
		if (number == 0){
			return "Rock";
		}
		else if (number == 1){
			return "Paper";
		}
		else if (number == 2){
			return "Scissors";
		}
		else {
			return "Dynamite";
		}
	}
	
	string userguess(){
		
/*Extra Challenge 1: 
	Get User input for one of the guesses*/
		
	}
	
	string wunderguess(){
		try{
			SimpleSerial wunderboard("COM3", 9600);
			string mystr = wunderboard.readLine();
			cout << "Wunderboard says: " << mystr << endl;
		}catch(boost::system::system_error& e){
			cout << "Error. << e.what() << endl;
		}
	
/*Extra Challenge 2: 
	Add the Players name to the object and it constructor. Add a function to get the name/change it*/

private:
	int wins;
	int losses;
	
};
