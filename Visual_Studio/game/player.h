#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Player{
public:
	void Update		(RenderWindow &Window, float ElapsedTime);
	void Render		(RenderWindow &Window);
	Player			(String tex,IntRect*** CollisionMap);
	float			getPosX(void);
	float			getPosY(void);
private:
	float Speed;
	Texture texture;
	Sprite sprite;
	IntRect*** ColMap;
	IntRect Collision;
	float x;
	float y;
	//int current_live;

	
	/*
	
	Anfangswert von Leben auf = 10; -> max_life = 10;
	Beim Speichern wird der Wert gespeichert und beim laden wieder neu gesetzt
	wenn das Level steigt erhöht sich auch das Maximale Leben

	current_life, max_life, 
	exp, level


	*/
};

#endif