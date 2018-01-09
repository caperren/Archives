// g++ serial.c++ -lboost_system -lws2_32 -D _WIN32_WINNT=0x0501

#include <iostream>
#include "SimpleSerial.h"
#include "conio.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{

	string inputstr;
	cout << "Lab 5 Code Started" << endl;
	SimpleSerial wunderboard("COM5", 9600);
	
	while(1){
		try { 
			cout << wunderboard.readLine() << endl;
			if(kbhit()){
				cin >> inputstr;
				wunderboard.writeString(inputstr);
				if((inputstr.compare("Exit") == 0) | (inputstr.compare("EXIT") == 0) | (inputstr.compare("exit") == 0)){
					return 0;
				}
			}
			
		
		} catch(boost::system::system_error& e)
		{
			cout<<"Error: "<<e.what()<<endl;
			return 1;
		}
	}
}