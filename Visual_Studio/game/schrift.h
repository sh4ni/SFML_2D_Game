#ifndef _SCHRIFT_H_
#define _SCHRIFT_H_
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Schrift{
public:
	void Render		(RenderWindow &Window);
	Schrift			(int X, int Y, String myText, int size);
	void Update		(String myText);
private:
	Font font;
	Text printText;
	int X, Y;
	String myText;
};

#endif