#include "schrift.h"
#include "defines.h"

bool Schrift::FontLoaded = false;
sf::Font Schrift::font;

// statische variablen, somit wird der font nur einmal geladen, egal wie oft die Schrift klasse
// benutzt wird.

Schrift::Schrift(int X, int Y, sf::String myText, int size, sf::Uint8 color){
	
	if( !FontLoaded ){
		if(!Schrift::font.loadFromFile(PATH"include/fonts/arial.ttf")){
			throw "Error: Font not found.";
		}
		else {
			FontLoaded = true;
			#ifdef DEBUGINFO
				std::cout << "Font loaded!" << std::endl;
			#endif
		}
	}

	printText.setString(myText);
	printText.setFont(this->font);
	printText.setCharacterSize(size);
	
	printText.setColor(sf::Color(color,color,color));
	printText.setPosition((float)X,(float)Y);
}

void Schrift::Update(sf::String myText){
	printText.setString(myText);
}

void Schrift::Render(sf::RenderWindow &Window){
	Window.draw(printText);
}
