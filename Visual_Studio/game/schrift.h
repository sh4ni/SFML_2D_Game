#ifndef _SCHRIFT_H_
#define _SCHRIFT_H_
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;	// OMG :( ohne das ding is der code VOLL mit fehlern ._. -k

class Schrift{
public:
	void Render		(sf::RenderWindow &Window);
	Schrift			(int X, int Y, sf::String myText, int size, int color = 255);
	void Update		(sf::String myText);
	sf::Text printText;
private:
	static bool FontLoaded;
	static sf::Font font;
	int X, Y;
	sf::String myText;
};

#endif
