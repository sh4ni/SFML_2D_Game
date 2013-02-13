#include "menu.h"
#include "defines.h"
#include <iostream>

MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window, bool newgame){
	//Load menu image from file
	sf::Texture imageMenu;
	sf::Texture imageBackground;
	sf::Texture imageButton;
	if(!imageMenu.loadFromFile(PATH"include/interface/menu.png")){
		exit(1); // fehlerbehebung fehlt noch...
	}
	if(!imageBackground.loadFromFile(PATH"include/interface/menu_back.png")){
		exit(1); // und noch mehr fehlerbehebung hier einsetzen :p
	}
	if(!imageButton.loadFromFile(PATH"include/interface/button.png")){
		exit(1); // .....
	}

	// Menü und Hintergrund
	spriteMenu.setTexture(imageMenu);
	spriteBackground.setTexture(imageBackground);

	spriteMenu.setOrigin((float)imageMenu.getSize().x/2.f,(float)imageMenu.getSize().y/2.f);
	spriteMenu.setPosition((float)WIDTH/2.f,(float)HEIGHT/2.f);
	
	sf::Font font;
	if(!font.loadFromFile(PATH"include/fonts/arial.ttf")){
		std::cout << "Failed to load the font!\a" << std::endl;
	}

	buttons = 5;	// Wieviele Buttons brauchen wir? Ja richtig! 5 verdammt noch mal!
	button = new MenuItem[buttons];

	for( int i=0; i<buttons; i++){
		button[i].image.setTexture(imageButton);
		button[i].image.setTextureRect(sf::IntRect(0,BUTTONHEIGHT*2,BUTTONWIDTH,BUTTONHEIGHT));
		button[i].image.setPosition((float)WIDTH/2.f-(float)BUTTONWIDTH/2.f,(float)HEIGHT/2.f-160.f+(float)i*(float)BUTTONHEIGHT*3.f/2.f);
		button[i].rect.left = WIDTH/2-BUTTONWIDTH/2;
		button[i].rect.top = HEIGHT/2-160+i*BUTTONHEIGHT*3/2;
		button[i].rect.width = BUTTONWIDTH;
		button[i].rect.height = BUTTONHEIGHT;
		button[i].text.setPosition((float)WIDTH/2.f-(float)BUTTONWIDTH/2.f+10.f,(float)HEIGHT/2.f-160.f+(float)i*(float)BUTTONHEIGHT*3.f/2.f+7.f);
		button[i].text.setFont(font);
		button[i].text.setCharacterSize(40U);
		button[i].text.setColor(sf::Color(100U,100U,100U));

		button[i].active = false;
		switch(i){
		case 0:
			button[i].text.setString("Continue");
			if(!newgame){
				button[i].active = true;
				button[i].action = Continue;
			}
			break;
		case 1:
			button[i].text.setString("New Game");
			button[i].active = true;
			button[i].action = NewGame;
			break;
		case 2:
			button[i].text.setString("Senseless Button");
			break;
		case 3:
			button[i].text.setString("Options");
			break;
		case 4:
			button[i].action = Exit;
			button[i].active = true;
			button[i].text.setString("Exit");
			break;
		}
		if( button[i].active ){	// Andere Textur und Textfarbe für klickbare Buttons
			button[i].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
			button[i].text.setColor(sf::Color(255U,255U,255U));
		}
		_menuItems.push_back(button[i]);
	}

	Version.setPosition((float)WIDTH-5.f,(float)HEIGHT-5.f);
	Version.setString(VERSION);
	Version.setCharacterSize(16U);
	Version.setFont(font);
	Version.setColor(sf::Color(0U,0U,0U));
	Version.setOrigin(Version.getGlobalBounds().width,Version.getGlobalBounds().height);

	Update(window);

	return GetMenuResponse(window);
}

void MainMenu::Update(sf::RenderWindow& window){
	window.draw(spriteBackground);
	window.draw(spriteMenu);
	for( int i=0; i<this->buttons; i++ ){
		window.draw(button[i].image);
		window.draw(button[i].text);
	}
	window.draw(Version);

	window.display();
}

MainMenu::MenuResult MainMenu::HandleClick(int x, int y){
	std::list<MenuItem>::iterator it;

	for ( it = _menuItems.begin(); it != _menuItems.end(); it++){
		sf::IntRect menuItemRect = (*it).rect;
		if( menuItemRect.contains(x,y) ){
			return (*it).action;
		}
	}
	return Nothing;
}

/*MainMenu::MenuResult MainMenu::HandleKeyboard(int code){
	return Nothing;
}*/

MainMenu::MenuResult  MainMenu::GetMenuResponse(sf::RenderWindow& window){
	sf::Event menuEvent;
	int selected = 0;
	int *active;
	active = new int[buttons];
	int activeButtons = 0;
	for( int i=0; i<buttons; i++){
		if( button[i].active ){
			active[activeButtons++] = i;
		}
	}
	while(true){
		while(window.pollEvent(menuEvent)){
			button[active[selected]].image.setTextureRect(sf::IntRect(0,BUTTONHEIGHT,BUTTONWIDTH,BUTTONHEIGHT));
			Update(window);
			if(menuEvent.type == sf::Event::MouseButtonPressed){
				#ifdef DEBUGINFO
					std::cout << "x " << menuEvent.mouseButton.x << " -  y " << menuEvent.mouseButton.y << std::endl;
				#endif
				return HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y);
			}
			else if(menuEvent.type == sf::Event::KeyPressed){
				if(menuEvent.key.code == sf::Keyboard::Return){
					return button[active[selected]].action;
				}
				else if(menuEvent.key.code == sf::Keyboard::Up){
					if( selected > 0 ){
						button[active[selected]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
						selected--;
					}
				}
				else if(menuEvent.key.code == sf::Keyboard::Down){
					if( selected < activeButtons-1 ){
						button[active[selected]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
						selected++;
					}
				}
			}
			else if(menuEvent.type == sf::Event::Closed){
				return Exit;
			}
		}
	}
}
