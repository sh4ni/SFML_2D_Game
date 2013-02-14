#include "schrift.h"
#include "defines.h"

bool Schrift::FontLoaded = false;
sf::Font Schrift::font;


Schrift::Schrift(int X, int Y, sf::String myText, int size, sf::Uint8 color){
	
	if( !FontLoaded ){
		if(!Schrift::font.loadFromFile(PATH"include/fo1nts/arial.ttf")){
			throw "error during loading the font!\a";
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
