/**
	Verantwortlich: Filip Menke <br>
	Infotext: Diese Klasse stellt die Schrift am Bildschirm dar. 
	Der Font Arial wird dabei nur einmal geladen (statische Variablen).
	Es ist möglich die Farbe, die größe und den Textstring individuell anzupassen.
*/
#include "schrift.h"
#include "defines.h"

#ifdef SYS_MACOS
#include "ResourcePath.hpp"
#endif

bool Schrift::FontLoaded = false;
sf::Font Schrift::font;

Schrift::Schrift(){
	//Standard Konstruktor
};
/// Überladener Standardkonstruktor
Schrift::Schrift(float X, float Y, sf::String myText, int size, sf::Uint8 color){
	this->Init(X,Y,myText,size,color);
}
/// Initalisierung der Schrift mit dem Standardfont Arial
void Schrift::Init(float X, float Y, sf::String myText, int size, sf::Uint8 color){
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
	printText.setPosition(X,Y);
}
/// Aktualisieren des Textes
void Schrift::Update(sf::String myText){
	printText.setString(myText);
}
/// Darstellung des Textes auf dem Bildschirm
void Schrift::Render(sf::RenderWindow &Window){
	Window.draw(printText);
}
