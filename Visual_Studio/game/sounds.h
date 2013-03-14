#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <SFML/Audio.hpp>

class gameMusic{
public:
	gameMusic();
	~gameMusic();

	static sf::Music music;
};

#endif