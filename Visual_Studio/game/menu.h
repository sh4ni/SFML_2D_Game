#ifndef _MENU_H_
#define _MENU_H_

#include <SFML/Graphics.hpp>
#include "schrift.h"
#include <list>

class MainMenu{
public:
	enum MenuResult { Nothing, Exit, Continue, Options, Play };	
	class MenuItem{
		public:
			sf::IntRect rect;
			sf::Sprite image;
			sf::Text text;
			MenuResult action;
			bool active;
		};
	MenuResult Show(RenderWindow& renderWindow, bool newgame = false);

private:
	MenuResult GetMenuResponse(RenderWindow& window);
	MenuResult HandleClick(int x, int y);
	MenuResult HandleKeyboard(int result);
	std::list<MenuItem> _menuItems;
};

#endif
