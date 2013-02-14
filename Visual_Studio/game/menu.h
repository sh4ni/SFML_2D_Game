#ifndef _MENU_H_
#define _MENU_H_

#include <SFML/Graphics.hpp>
#include "schrift.h"
#include <list>

class MainMenu{
public:
	enum MenuResult { Nothing, Exit, Continue, Options, NewGame, NewGameGender, Female, Male };
	class MenuItem{
		public:
			sf::IntRect rect;
			sf::Sprite image;
			sf::Text text;
			MenuResult action;
			bool active;
		};
	MenuResult Show(sf::RenderWindow& renderWindow, bool newgame = false, bool gendermenu = false);

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window, bool gendermenu = false);
	MenuResult HandleClick(int x, int y);
	//MenuResult HandleKeyboard(int result);
	std::list<MenuItem> _menuItems;
	void Update(sf::RenderWindow &Window);
	int buttons;
	MenuItem* button;
	sf::Sprite spriteMenu;
	sf::Sprite spriteBackground;
	sf::Text Version;
};

#endif
