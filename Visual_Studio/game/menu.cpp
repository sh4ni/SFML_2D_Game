#include "menu.h"
#include "defines.h"
#include <iostream>

MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window, bool newgame){
	//Load menu image from file
	sf::Texture imageMenu;
	sf::Texture imageBackground;
	sf::Texture imageButton;
	if(!imageMenu.loadFromFile("include/interface/menu.png")){
		exit(1); // fehlerbehebung fehlt noch...
	}
	if(!imageBackground.loadFromFile("include/interface/menu_back.png")){
		exit(1); // und noch mehr fehlerbehebung hier einsetzen :p
	}
	if(!imageButton.loadFromFile("include/interface/button.png")){
		exit(1); // .....
	}

	// Menü und Hintergrund
	sf::Sprite sprite(imageMenu);
	sf::Sprite spriteBackground(imageBackground);
	sprite.setOrigin((float)imageMenu.getSize().x/2.f,(float)imageMenu.getSize().y/2.f);
	sprite.setPosition((float)WIDTH/2.f,(float)HEIGHT/2.f);
	window.draw(spriteBackground);
	window.draw(sprite);
	
	sf::Font font;
	if(!font.loadFromFile("include/fonts/arial.ttf")){
		std::cout << "Fehler beim Laden der Schrift!\a" << std::endl;
	}

	MenuItem button[5];		// Es werden 4 Buttons erzeugt!

	int buttons = sizeof(button)/sizeof(button[0]);
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
			if(!newgame){
				button[i].active = true;
				button[i].action = Continue;
			}
			button[i].text.setString("Continue");
			break;
		case 1:
			button[i].text.setString("New Game");
			button[i].active = true;
			button[i].action = NewGame;
			break;
		case 2:
			button[i].text.setString("Map Editor");
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
		window.draw(button[i].image);
		window.draw(button[i].text);
	}

	Schrift Version(WIDTH-5,HEIGHT-5,VERSION,16,0);
	Version.printText.setOrigin(Version.printText.getGlobalBounds().width,Version.printText.getGlobalBounds().height);
	Version.Render(window);

	window.display();
	return GetMenuResponse(window);
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

MainMenu::MenuResult MainMenu::HandleKeyboard(int code){
	return Nothing;
}

MainMenu::MenuResult  MainMenu::GetMenuResponse(sf::RenderWindow& window){
	sf::Event menuEvent;
	while(true){
		while(window.pollEvent(menuEvent)){

			if(menuEvent.type == sf::Event::MouseButtonPressed){
				#ifdef DEBUG
					std::cout << "x " << menuEvent.mouseButton.x << " -  y " << menuEvent.mouseButton.y << std::endl;
				#endif
				return HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y);

			}
			else if(menuEvent.type == sf::Event::KeyPressed){
				return HandleClick(205,235);
				if(menuEvent.key.code == sf::Keyboard::Down)
					return HandleKeyboard(2);
			}
			
			if(menuEvent.type == sf::Event::Closed){
				return Exit;
			}
		}
	}
}
