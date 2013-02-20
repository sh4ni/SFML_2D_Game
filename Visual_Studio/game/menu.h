#ifndef _MENU_H_
#define _MENU_H_

#include <SFML/Graphics.hpp>
#include "schrift.h"
#include "savegame.h"
#include <list>

class MainMenu{
public:
	enum MenuResult { Nothing, Exit, Continue, Options, NewGame, NewGameGender, Female, Male, Menue };
	class MenuItem{
		public:
			sf::IntRect rect;
			sf::Sprite image;
			sf::Text text;
			MenuResult action;
			bool active;
		};
	MenuResult Show(sf::RenderWindow& renderWindow, bool newgame = false, bool gendermenu = false);
    ~MainMenu(void){
        
    }

private:
    enum MenuButton{ NoButton, Up, Down, Enter, Back };
    MenuButton GetMenuButton(sf::Event menuEvent);
	MenuResult GetMenuResponse(sf::RenderWindow& window, bool gendermenu = false);
	MenuResult HandleClick(int x, int y);
	std::list<MenuItem> _menuItems;
	void Update(sf::RenderWindow &Window);
	int buttons = 0;
	MenuItem* button = 0;
	sf::Sprite spriteMenu;
	sf::Sprite spriteBackground;
	sf::Sprite spriteBackgroundRepeat;
	static int animation;      // laufvariable für das durchlaufende hintergrundbild
	sf::Text Version;
};

#endif
