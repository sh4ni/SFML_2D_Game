#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SFML/Graphics.hpp>

class Character{
public:
	Character();
	~Character();

	void Update(float ElapsedTime);
	void Render(sf::RenderWindow &Window);

	// getter and settter
	float getPosX(void);
	float getPosY(void);
	void setPosition(float X, float Y);

	float getSpeed(void);
	void increaseSpeed(float speedValue);
	void decreaseSpeed(float speedValue);

	void setName(std::string Name);
	std::string getName(void);

	int getHealth(void);
	void setHealth(int Health);

	void setLvl(int Lvl);
	int getLvl(void);

	//void setBlockControl(bool block=false);


protected:
    sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect*** ColMap;
	sf::Vector2i MapSize;
	int Animation;

	float PosX;
	float PosY;
	float Speed;
	
	std::string Name; 

	int Health;
	int Lvl;
};

#endif