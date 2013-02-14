#ifndef _SCHRIFT_H_
#define _SCHRIFT_H_
#include <SFML/Graphics.hpp>
#include <iostream>

class Schrift{
public:
	void Render		(sf::RenderWindow &Window);
	Schrift			(int X = 0, int Y = 0, sf::String myText = "", int size = 16, sf::Uint8 color = 255);
	void Update		(sf::String myText);
	sf::Text printText;
private:
	static bool FontLoaded;
	static sf::Font font;
	int X, Y;
	sf::String myText;
};

#endif
