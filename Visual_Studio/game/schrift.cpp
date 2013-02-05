#include "schrift.h"
#include "defines.h"

Schrift::Schrift(float X, float Y, String myText, int size){
	
	if(!font.loadFromFile("include/fonts/arial.ttf"))
		std::cout << "Fehler beim Laden der Schrift!\a" << std::endl;
	else
		#if DEBUG == 1
			std::cout << "Font geladen!" << std::endl;
		#endif

	printText.setString(myText);
	printText.setFont(font);
	printText.setCharacterSize(size);
	printText.setColor(sf::Color(255,255,255));
	//printText.setOrigin(size,size);
	printText.setPosition(X,Y);
}

void Schrift::Update(String myText){
	printText.setString(myText);
}

void Schrift::Render(RenderWindow &Window){
	Window.draw(printText);
}
