#include "game.h"

int main(void){
	try{
		Game::Init();
	}
	catch(...){
		std::cout << "error" << std::endl;
	}
	return 0;
}
