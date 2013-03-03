#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SFML/Graphics.hpp>
#include <sstream>
#include "schrift.h"

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
    
    std::vector<Schrift> dmgText;

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
    
    void damageText(int damage, char type){
        Schrift E1;
		std::ostringstream dmgString;
		dmgString << damage;
        E1.Init(PosX,PosY,dmgString.str());
        if( type == 'p') E1.printText.setColor(sf::Color(255,0,0));
        else if( type == 'e') E1.printText.setColor(sf::Color(255,0,255));
        E1.printText.setOrigin(E1.printText.getGlobalBounds().width/2, 0);
        dmgText.push_back(E1);
    }
    
    void Render(sf::RenderWindow &Window){
        Window.draw(sprite);
        for( int i=0; i<dmgText.size(); i++){
            dmgText[i].printText.setPosition(dmgText[i].printText.getPosition().x, dmgText[i].printText.getPosition().y-1 );
            dmgText[i].printText.setColor(sf::Color(255,255,255,dmgText[i].printText.getColor().a-4));
            dmgText[i].Render(Window);
            if( dmgText[i].printText.getColor().a < 10 ){
                dmgText.erase(dmgText.begin()+i);
            }
        }
    }

};

#endif