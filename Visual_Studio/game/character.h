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
	char Lvl;

	// not in use yet
	//int strenght;
	//int agility;
	//int vitality;

public:
	Character();
	~Character();

	void Init();
	//void Update(float ElapsedTime);

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
    
    int calcDamage(int damage,int level){
        int levelDif = level-this->Lvl;
        float multi = 1.f;
        if(levelDif > 0){   // Gegner hat einen höhreren Level
            if(levelDif >10) levelDif = 10;
            multi += (float)levelDif*0.025f;
        }
        else if(levelDif < 0){ // Gegner hat einen niedrigeren Level
            if(levelDif < -10) levelDif = -10;
            multi += (float)levelDif*0.1f;
        }
        damage = (int)((float)damage * multi);
        if ( damage <= 0 ){
            damage = 1;
        }
        return damage;
    }
    
    void damageText(int damage, char type){
        Schrift E1;
		std::ostringstream dmgString;
        if( type == 'e') dmgString << "XP +";
        else if( type == 'l') dmgString << "LEVEL ";
		dmgString << damage;
        E1.Init(PosX,PosY,dmgString.str());
        E1.printText.setStyle(sf::Text::Bold);
        if( type == 'm') E1.printText.setColor(sf::Color(0xFF,0xFF,0x00));
        else if( type == 'p') E1.printText.setColor(sf::Color(0xFF,0x00,0x00));
        else if( type == 'e') E1.printText.setColor(sf::Color(0xFF,0x00,0xFF));
        else if( type == 'l'){
            E1.printText.setCharacterSize(100);
            E1.printText.setColor(sf::Color(0xFF,0x88,0x00));
        }
        E1.printText.setOrigin(E1.printText.getGlobalBounds().width/2, 0);
        dmgText.push_back(E1);
    }
    
    void Render(sf::RenderWindow &Window, bool drawEnemy = true){
        if(drawEnemy) Window.draw(sprite);
        for( unsigned i=0; i<dmgText.size(); i++){
            dmgText[i].printText.setPosition(dmgText[i].printText.getPosition().x, dmgText[i].printText.getPosition().y-1 );
            dmgText[i].printText.setColor(sf::Color(dmgText[i].printText.getColor().r,dmgText[i].printText.getColor().g,dmgText[i].printText.getColor().b,dmgText[i].printText.getColor().a-4));
            dmgText[i].Render(Window);
            if( dmgText[i].printText.getColor().a < 10 ){
                dmgText.erase(dmgText.begin()+i);
            }
        }
    }

};

#endif