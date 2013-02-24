#ifndef _MENU_H_
#define _MENU_H_

#include <SFML/Graphics.hpp>
#include "schrift.h"
#include "savegame.h"
#include <list>

class MainMenu{
public:
	enum MenuResult { Nothing, Exit, Continue, Options, NewGame, NewGameGender, Female, Male, Menue, Sound, Fullscreen, DefaultSettings };
	class MenuItem{
    public:
        sf::IntRect rect;
        sf::Sprite image;
        sf::Text text;
        MenuResult action;
        bool active;
        sf::Sprite image2;
	};
	MenuResult Show(sf::RenderWindow& renderWindow, bool newgame = false, char menuType = 'M');
	MainMenu(){
		button = NULL;
	}
    ~MainMenu(void){
		#ifdef DEBUGINFO
			std::cout << "destruktor menu" << std::endl;
		#endif
	
        if(button != NULL){
			delete [] button;
		}
    }

private:
    enum MenuButton{ NoButton, Up, Down, Enter, Back };
    MenuButton GetMenuButton(sf::Event menuEvent);
	MenuResult GetMenuResponse(sf::RenderWindow& renderWindow, char menuType = 'M');
	MenuResult HandleClick(int x, int y);
	std::list<MenuItem> _menuItems;
	void Update(sf::RenderWindow &Window);
	int buttons;
	MenuItem* button;
	sf::Sprite spriteMenu;
	sf::Sprite spriteBackground;
	sf::Sprite spriteBackgroundRepeat;
	static int animation;      // laufvariable für das durchlaufende hintergrundbild
	sf::Text Version;
};

#endif
