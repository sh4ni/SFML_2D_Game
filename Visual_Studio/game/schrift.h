#ifndef _SCHRIFT_H_
#define _SCHRIFT_H_
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Schrift{
public:
	void Render		(RenderWindow &Window);
	Schrift			(int X, int Y, String myText, int size, int color = 255);
	void Update		(String myText);
	Text printText;
private:
	Font font;
	int X, Y;
	String myText;
};

#endif