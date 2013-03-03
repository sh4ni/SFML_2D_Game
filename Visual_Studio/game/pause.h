#ifndef _PAUSE_H_
#define _PAUSE_H_

#include <SFML/Graphics.hpp>
#include "defines.h"
#include "schrift.h"
#include "savegame.h"

class Pause{
public:
	bool Show(sf::RenderWindow& renderWindow, sf::View& viewCamera);
};

#endif