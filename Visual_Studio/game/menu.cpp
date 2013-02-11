#include "menu.h"
#include "defines.h"
#include <iostream>

MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window, bool newgame)
{
	//Load menu image from file
	sf::Texture image;
	sf::Texture imageBackground;
	if(!image.loadFromFile("include/interface/menu.png")){
		exit(1); // fehlerbehebung fehlt noch...
	}
	if(!imageBackground.loadFromFile("include/interface/menu_back.png")){
		exit(1); // und noch mehr fehlerbehebung hier einsetzen :p
	}
	sf::Sprite sprite(image);
	sf:Sprite spriteBackground(imageBackground);

	sprite.setOrigin(image.getSize().x/2,image.getSize().y/2);
	sprite.setPosition(WIDTH/2,HEIGHT/2);
	
	Schrift PlayText(WIDTH/2-BUTTONWIDTH/2+5,225,"Continue",50);
	Schrift ExitText(WIDTH/2-BUTTONWIDTH/2+5,321,"Exit",50);

	//Setup clickable regions

	//Play menu item coordinates
	MenuItem playButton;
	
	playButton.rect.top= 225;
	playButton.rect.width = 288;
	playButton.rect.left = 160;
	playButton.rect.height = 862;

	playButton.action = Play;
	playButton.active = true;

	//Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.top = 321;
	exitButton.rect.width = 382;
	exitButton.rect.left = 160;
	exitButton.rect.height = 862;

	exitButton.action = Exit;

	_menuItems.push_back(playButton);
	_menuItems.push_back(exitButton);

	window.draw(spriteBackground);
	window.draw(sprite);

	PlayText.Render(window);
	ExitText.Render(window);

	window.display();
	return GetMenuResponse(window);
}

MainMenu::MenuResult MainMenu::HandleClick(int x, int y)
{
	std::list<MenuItem>::iterator it;

	for ( it = _menuItems.begin(); it != _menuItems.end(); it++)
	{
		sf::Rect<int> menuItemRect = (*it).rect;
		if( menuItemRect.width > y 
			&& menuItemRect.top < y 
			&& menuItemRect.left < x 
			&& menuItemRect.height > x)
			{
				return (*it).action;
			}
	}

	return Nothing;
}

MainMenu::MenuResult MainMenu::HandleKeyboard(int code)
{
	return Nothing;
}

MainMenu::MenuResult  MainMenu::GetMenuResponse(sf::RenderWindow& window)
{
	sf::Event menuEvent;

	while(true)
	{

		while(window.pollEvent(menuEvent))
		{
			if(menuEvent.type == sf::Event::MouseButtonPressed)
			{
				#ifdef DEBUG
					std::cout << "x " << menuEvent.mouseButton.x << " -  y " << menuEvent.mouseButton.y << std::endl;
				#endif

				return HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y);
			
			}else if(menuEvent.type == sf::Event::KeyPressed){
				return HandleClick(205,235);
				if(menuEvent.key.code == Keyboard::Down)
					return HandleKeyboard(2);
			}
			
			if(menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}
