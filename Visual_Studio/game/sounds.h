#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <SFML/Audio.hpp>

class gameMusic{
public:
	gameMusic(){
		//Konstruktor	
	};
	~gameMusic(){
		//Destruktor
	};

	static sf::Music music;
};

#endif