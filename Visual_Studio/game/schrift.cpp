#include "schrift.h"
#include "defines.h"

Schrift::Schrift(String path, float X, float Y, String myText){
	
	if(!font.loadFromFile(path))
		std::cout << "Fehler beim Laden!" << std::endl;
	else
		std::cout << "Font geladen!" << std::endl;

	printText.setString(myText);
	printText.setFont(font);
	printText.setCharacterSize(10);

	//printText.setPosition(0,HEIGHT-10);
	printText.setPosition(X,Y);
}

void Schrift::Render(RenderWindow &Window){
	Window.draw(printText);
}
		