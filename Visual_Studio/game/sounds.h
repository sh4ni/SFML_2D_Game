#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <SFML/Audio.hpp>

/**
 Klasse für die Musik während des Spielens.
 Sie ist statisch, da immer nur ein Titel gleichzeitig im Spiel abgespielt werden muss.
 */
class gameMusic{
public:
	gameMusic();
	~gameMusic();

	static sf::Music music;
};

#endif