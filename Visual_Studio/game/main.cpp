#include "game.h"

int main(void){
	try{
		Game::Init();
	}
	catch(const char * str){
		std::cout << str << std::endl;
	}
	catch(...){
		std::cout << "error" << std::endl;
	}
	return 0;
}
