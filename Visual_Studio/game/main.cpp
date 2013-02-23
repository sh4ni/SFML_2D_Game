#include "game.h"
#include <exception>
using namespace std;

class myexception: public exception{
	virtual const char* what() const throw(){
		return "the exception happened";
	}
} myex;

int main(void){
	try{
		Game::Init();
		/*while(1){
			int * myfail = new int[10000000];
			std::cout << "alloziiere speicher!" << std::endl;
		}*/
		
	}
	catch(const char * str){
		std::cout << str << std::endl;
		system("pause");
	}
	catch(bad_alloc& err){
		std::cout << err.what() << std::endl;
		system("pause");
	}
	
	catch(...){
		std::cout << "error" << std::endl;
		system("pause");
	}
	return 0;
}
