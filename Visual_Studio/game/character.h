#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SFML/Graphics.hpp>

class Character{
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
	int AttackPower;
	int Lvl;

	// not in use yet
	//int strenght;
	//int agility;
	//int vitality;

public:
	Character();
	~Character();

	void Init();
	void Update(float ElapsedTime);
	void Render(sf::RenderWindow& Window);

	// getter and settter
	float getPosX(void);
	float getPosY(void);
	void setPosition(float X, float Y);
	void setMapSize( int xMax, int yMax){
		this->MapSize.x = xMax;
		this->MapSize.y = yMax;
	}

	float getSpeed(void);
	void increaseSpeed(float speedValue);
	void decreaseSpeed(float speedValue);

	void setName(std::string Name);
	std::string getName(void);

	int getHealth(void);
	void setHealth(int Health);

	void setLvl(int Lvl);
	int getLvl(void);

	void setColMap(sf::IntRect***& ColMap);
};

#endif