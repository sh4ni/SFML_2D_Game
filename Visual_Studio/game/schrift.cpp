#include "schrift.h"
#include "defines.h"

Schrift::Schrift(int X, int Y, String myText, int size){
	
	if(!font.loadFromFile("include/fonts/arial.ttf")){
		std::cout << "Fehler beim Laden der Schrift!\a" << std::endl;
	}

	#ifdef DEBUG
		std::cout << "Font geladen!" << std::endl;
	#endif

	printText.setString(myText);
	printText.setFont(font);
	printText.setCharacterSize(size);
	printText.setColor(sf::Color(255,255,255,250));
	printText.setPosition((float)X,(float)Y);
}

void Schrift::Update(String myText){
	printText.setString(myText);
}

void Schrift::Render(RenderWindow &Window){
	Window.draw(printText);
}
