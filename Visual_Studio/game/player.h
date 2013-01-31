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
	float			getPosX(void);
	float			getPosY(void);
private:
	float Speed;
	Texture texture;
	Sprite sprite;
	float x;
	float y;
	/*
	
	current_life, max_life, 
	exp, level


	*/
};

#endif