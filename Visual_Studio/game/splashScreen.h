#ifndef _SPLASHSCREEN_H_
#define _SPLASHSCREEN_H_
#include <SFML/Graphics.hpp>
using namespace sf; 

class SplashScreen
{
public:
	void ShowFirst(sf::RenderWindow& window);
	void Show(sf::RenderWindow& window);
};

#endif