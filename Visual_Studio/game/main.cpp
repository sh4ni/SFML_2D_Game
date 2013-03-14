/**
	Verantwortlich: Filip Menke, Daniel Maier <br>
	Infotext: Einstiegsdatei ins Spiel durch aufruf von Game::Init()
	Die Exception Handler fangen Fehler ab und verhindern einen möglichen Spielabsturz.
*/
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
	}
	catch(const char * str){
		std::cout << "sorry, an error occurs " << str << std::endl;
#ifdef SYS_WINDOWS
		system("pause");
#endif
	}
	catch(bad_alloc& err){
		std::cout << err.what() << std::endl;
#ifdef SYS_WINDOWS
		system("pause");
#endif
	}
	
	catch(...){
		std::cout << "error" << std::endl;
#ifdef SYS_WINDOWS
		system("pause");
#endif
	}
	return 0;
}
