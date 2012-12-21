#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Player{
public:
	void Update		(RenderWindow &Window, float ElapsedTime);
	void Render		(RenderWindow &Window);
	Player			(String texture);
private:
	float Speed;
	Texture texture;
	Sprite sprite;

	/*
	
	current_life, max_life, 
	exp, level


	*/
};

#endif