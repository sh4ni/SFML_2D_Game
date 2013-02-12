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
	sprite.setOrigin(imageMenu.getSize().x/2,imageMenu.getSize().y/2);
	sprite.setPosition(WIDTH/2,HEIGHT/2);
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
		button[i].image.setPosition(WIDTH/2-BUTTONWIDTH/2,HEIGHT/2-160+i*BUTTONHEIGHT*3/2);
		button[i].rect.left = WIDTH/2-BUTTONWIDTH/2;
		button[i].rect.top = HEIGHT/2-160+i*BUTTONHEIGHT*3/2;
		button[i].rect.width = BUTTONWIDTH;
		button[i].rect.height = BUTTONHEIGHT;
		button[i].text.setPosition(WIDTH/2-BUTTONWIDTH/2+10,HEIGHT/2-160+i*BUTTONHEIGHT*3/2+7);
		button[i].text.setFont(font);
		button[i].text.setCharacterSize(40);
		button[i].text.setColor(sf::Color(100,100,100));

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
			button[i].text.setColor(sf::Color(255,255,255));
		}
		_menuItems.push_back(button[i]);
		window.draw(button[i].image);
		window.draw(button[i].text);
	}

	/*Schrift PlayText(WIDTH/2-BUTTONWIDTH/2+5,225,"Continue",50);
	Schrift ExitText(WIDTH/2-BUTTONWIDTH/2+5,321,"Exit",50);
	PlayText.Render(window);
	ExitText.Render(window);*/

	/*MenuItem buttonContinue;
	MenuItem buttonNewGame;
	MenuItem buttonExit;

	buttonContinue.image.setTexture(imageButton);
	buttonNewGame.image.setTexture(imageButton);
	buttonExit.image.setTexture(imageButton);*/
	
	//.setTexture(texture);
	//.setOrigin(16.f,32.f);
	//.setTextureRect(sf::IntRect(0,0,TILESIZE,TILESIZE*2));
	//.setPosition(currentSavegame.mPosX,currentSavegame.mPosY);
	
	//Setup clickable regions

	//Play menu item coordinates
	/*MenuItem playButton;
	
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
	_menuItems.push_back(exitButton);*/

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
		/*if( menuItemRect.width > y 
			&& menuItemRect.top < y 
			&& menuItemRect.left < x 
			&& menuItemRect.height > x)
			{
				return (*it).action;
			}*/
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
				if(menuEvent.key.code == Keyboard::Down)
					return HandleKeyboard(2);
			}
			
			if(menuEvent.type == sf::Event::Closed){
				return Exit;
			}
		}
	}
}
