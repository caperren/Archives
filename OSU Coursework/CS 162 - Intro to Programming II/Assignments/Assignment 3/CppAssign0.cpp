#include <iostream>
#include <string>
using namespace std;

int main(void){
		
	unsigned char exit;
	string response;
	
	enum question{
		first,
		second,
		third
	} currentquestion = first;
	
	while(exit != 1){
		switch(currentquestion){
			case first:
				
				cout << "First Question: Which is NOT an internal component of a desktop computer?\n\nA: Ram Module\nB: Power Supply\nC: Hard Drive\nD: Monitor\nPlease enter your selection: ";
				cin >> response;
				if((response.compare("a") == 0) | (response.compare("A") == 0)){
					cout << "\nIncorrect. Ram modules are an internal component.\nPlease try again..." << endl;
					cout << string(2, '\n');
				}else if((response.compare("b") == 0) | (response.compare("B") == 0)){
					cout << "\nIncorrect. Power supplies are an internal component.\nPlease try again..." << endl;
					cout << string(2, '\n');
				}else if((response.compare("c") == 0) | (response.compare("C") == 0)){
					cout << "\nIncorrect. Hard drives are an internal component.\nPlease try again..." << endl;
					cout << string(2, '\n');
				}else if((response.compare("d") == 0) | (response.compare("D") == 0)){
					cout << "\nCorrect! Monitor's are external components on desktops." << endl;
					cout << string(2, '\n');
					currentquestion = second;
				}else{
					cout << "You have entered an invalid response, please try again..." << endl;
					cout << string(2, '\n');
				}
				break;
			case second:
				cout << "Second Question: Which is a model number for an ivy bridge processor?\nA: i5-3570K\nB: i7-2600K\nC: i3-2120\nD: i5-2400\nPlease enter your selection: ";
				cin >> response;
				if((response.compare("a") == 0) | (response.compare("A") == 0)){
					cout << "\nCorrect!. This is an ivy bridge processor!" << endl;
					cout << string(2, '\n');
					return 0;
				}else if((response.compare("b") == 0) | (response.compare("B") == 0)){
					cout << "\nIncorrect. This is a Sandy Bridge processor.\nPlease try again..." << endl;
					cout << string(2, '\n');
				}else if((response.compare("c") == 0) | (response.compare("C") == 0)){
					cout << "\nIncorrect. This is a Sandy Bridge processor.\nPlease try again..." << endl;
					cout << string(2, '\n');
				}else if((response.compare("d") == 0) | (response.compare("D") == 0)){
					cout << "\nIncorrect. This is a Sandy Bridge processor.\nPlease try again..." << endl;
					cout << string(2, '\n');
				}else{
					cout << "\nYou have entered an invalid response, please try again..." << endl;
					cout << string(2, '\n');
				}
				break;
			default:
				cerr << "You should never see this. There has been an enum error." << endl;
		}
	}
	return 0;
}