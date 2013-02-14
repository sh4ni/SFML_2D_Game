#include "menu.h"
#include "defines.h"
#include <iostream>

MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window, bool newgame, bool gendermenu){
	//Load menu image from file
	sf::Texture imageMenu;
	sf::Texture imageBackground;
	if(!imageMenu.loadFromFile(PATH"include/interface/menu.png")){
        throw "Error: include/interface/menu.png not found.";
	}
	if(!imageBackground.loadFromFile(PATH"include/interface/menu_back.png")){
        throw "Error: include/interface/menu_back.png not found.";
	}

	// Menü und Hintergrund
	spriteMenu.setTexture(imageMenu);
	spriteBackground.setTexture(imageBackground);

	spriteMenu.setOrigin((float)imageMenu.getSize().x/2.f,(float)imageMenu.getSize().y/2.f);
	spriteMenu.setPosition((float)WIDTH/2.f,(float)HEIGHT/2.f);
	
	sf::Font font;
	if(!font.loadFromFile(PATH"include/fonts/arial.ttf")){
        throw "Error: include/fonts/arial.ttf not found.";
	}

    sf::Texture imageGenderButton;
    sf::Texture imageButton;
    if(!imageButton.loadFromFile(PATH"include/interface/button.png")){
        throw "Error: include/interface/button.png not found.";
    }
    if( gendermenu ){
        if(!imageGenderButton.loadFromFile(PATH"include/interface/genderbutton.png")){
            throw "Error: include/interface/genderbutton.png not found.";
        }
        buttons = 3;    // Wieviel buttons im geschlechtsauswahlscreen? achtung! die ersten 2 buttons sind speziell!
        button = new MenuItem[buttons];
        
        button[0].text.setString("Select your hero");
        button[0].text.setPosition(WIDTH/2,HEIGHT/2-120);
        button[0].text.setFont(font);
        button[0].text.setCharacterSize(40U);
        button[0].text.setColor(sf::Color(0U,0U,0U));
        button[0].text.setOrigin(button[0].text.getGlobalBounds().width/2+1,0);
        
        for( int i=0; i<buttons; i++){
            if( i<2 ){
                button[i].rect.left = WIDTH/2-GENDERBUTTON/2+(i?100:-100);
                button[i].rect.top = HEIGHT/2-25;
                button[i].rect.width = GENDERBUTTON;
                button[i].rect.height = GENDERBUTTON;
                button[i].image.setTexture(imageGenderButton);
            }
            else {
                button[i].rect.left = WIDTH/2-BUTTONWIDTH/2;
                button[i].rect.top = HEIGHT/2-160+(i+2)*BUTTONHEIGHT*3/2;
                button[i].rect.width = BUTTONWIDTH;
                button[i].rect.height = BUTTONHEIGHT;
                button[i].image.setTexture(imageButton);
                button[i].text.setPosition(button[i].rect.left+10.f,button[i].rect.top+7.f);
                button[i].text.setFont(font);
                button[i].text.setCharacterSize(40U);
                button[i].text.setColor(sf::Color(255U,255U,255U));
            }
            button[i].active = true;
            switch(i){
                case 0:
                    button[i].image.setTextureRect(sf::IntRect(0,0,GENDERBUTTON,GENDERBUTTON));
                    button[i].action = Female;
                    break;
                case 1:
                    button[i].image.setTextureRect(sf::IntRect(0,GENDERBUTTON,GENDERBUTTON,GENDERBUTTON));
                    button[i].action = Male;
                    break;
                case 2:
                    button[i].text.setString("Back");
                    button[i].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                    button[i].action = Menue;
                    break;
            }
            button[i].image.setPosition(button[i].rect.left,button[i].rect.top);
            _menuItems.push_back(button[i]);
        }
    }
    else {
        buttons = 4;	// Wieviele Buttons brauchen wir? Ja richtig! 5 verdammt noch mal!
        button = new MenuItem[buttons];
        
        for( int i=0; i<buttons; i++){
            button[i].rect.left = WIDTH/2-BUTTONWIDTH/2;
            button[i].rect.top = HEIGHT/2-160+((i<3)?i:i+1)*BUTTONHEIGHT*3/2;   // ((i<3)?i:i+1) = nach den 1. 3 buttons ein button abstand
            button[i].rect.width = BUTTONWIDTH;
            button[i].rect.height = BUTTONHEIGHT;
            button[i].image.setTexture(imageButton);
            button[i].image.setTextureRect(sf::IntRect(0,BUTTONHEIGHT*2,BUTTONWIDTH,BUTTONHEIGHT));
            button[i].image.setPosition(button[i].rect.left,button[i].rect.top);
            button[i].text.setPosition(button[i].rect.left+10.f,button[i].rect.top+7.f);
            button[i].text.setFont(font);
            button[i].text.setCharacterSize(40U);
            button[i].text.setColor(sf::Color(100U,100U,100U));
            
            button[i].active = false;
            switch(i){
                case 0:
                    button[i].text.setString("Continue");
                    if(!newgame){
                        button[i].action = Continue;
                        button[i].active = true;
                    }
                    break;
                case 1:
                    button[i].text.setString("New Game");
                    button[i].action = NewGameGender;
                    button[i].active = true;
                    break;
                case 2:
                    button[i].text.setString("Options");
                    break;
                case 3:
                    button[i].text.setString("Exit");
                    button[i].action = Exit;
                    button[i].active = true;
                    break;
            }
            if( button[i].active ){	// Andere Textur und Textfarbe für klickbare Buttons
                button[i].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                button[i].text.setColor(sf::Color(255U,255U,255U));
            }
            _menuItems.push_back(button[i]);
        }        
    }

	Version.setPosition((float)WIDTH-5.f,(float)HEIGHT-5.f);
	Version.setString(VERSION);
	Version.setCharacterSize(16U);
	Version.setFont(font);
	Version.setColor(sf::Color(0U,0U,0U));
	Version.setOrigin(Version.getGlobalBounds().width,Version.getGlobalBounds().height);

	Update(window);

	return GetMenuResponse(window,gendermenu);
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

MainMenu::MenuResult MainMenu::GetMenuResponse(sf::RenderWindow& window, bool gendermenu){
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
            if( gendermenu ){
                switch(active[selected]){
                    case 0:
                        button[active[selected]].image.setTextureRect(sf::IntRect(GENDERBUTTON,0,GENDERBUTTON,GENDERBUTTON));
                        break;
                    case 1:
                        button[active[selected]].image.setTextureRect(sf::IntRect(GENDERBUTTON,GENDERBUTTON,GENDERBUTTON,GENDERBUTTON));
                        break;
                    case 2:
                        button[active[selected]].image.setTextureRect(sf::IntRect(0,BUTTONHEIGHT,BUTTONWIDTH,BUTTONHEIGHT));
                        break;
                }
            }
            else {
                button[active[selected]].image.setTextureRect(sf::IntRect(0,BUTTONHEIGHT,BUTTONWIDTH,BUTTONHEIGHT));                
            }
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
				else if(menuEvent.key.code == sf::Keyboard::Up || menuEvent.key.code == sf::Keyboard::Left){
					if( selected > 0 ){
                        if( gendermenu ){
                            switch(active[selected]){
                                case 0:
                                    button[active[selected]].image.setTextureRect(sf::IntRect(0,0,GENDERBUTTON,GENDERBUTTON));
                                    break;
                                case 1:
                                    button[active[selected]].image.setTextureRect(sf::IntRect(0,GENDERBUTTON,GENDERBUTTON,GENDERBUTTON));
                                    break;
                                case 2:
                                    button[active[selected]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                                    break;
                            }
                        }
                        else {
                            button[active[selected]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                        }
						selected--;
					}
				}
				else if(menuEvent.key.code == sf::Keyboard::Down || menuEvent.key.code == sf::Keyboard::Right){
					if( selected < activeButtons-1 ){
                        if( gendermenu ){
                            switch(active[selected]){
                                case 0:
                                    button[active[selected]].image.setTextureRect(sf::IntRect(0,0,GENDERBUTTON,GENDERBUTTON));
                                    break;
                                case 1:
                                    button[active[selected]].image.setTextureRect(sf::IntRect(0,GENDERBUTTON,GENDERBUTTON,GENDERBUTTON));
                                    break;
                                case 2:
                                    button[active[selected]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                                    break;
                            }
                        }
                        else {
                            button[active[selected]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                        }
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
