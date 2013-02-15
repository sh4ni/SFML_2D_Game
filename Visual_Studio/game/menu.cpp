#include "menu.h"
#include "defines.h"
#include <iostream>

int MainMenu::animation = 0;

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

	// Men¸ und Hintergrund
	spriteMenu.setTexture(imageMenu);
	spriteBackground.setTexture(imageBackground);       // ich brauch das bild 2 mal, da es sich beim durchscrollen
	spriteBackgroundRepeat.setTexture(imageBackground); // wiederholen muss!

	spriteMenu.setOrigin((float)imageMenu.getSize().x/2.f,(float)imageMenu.getSize().y/2.f);
	spriteMenu.setPosition((float)WIDTH/2.f,(float)HEIGHT/2.f);
	
	sf::Font font;
	if(!font.loadFromFile(PATH"include/fonts/arial.ttf")){
        throw "Error: include/fonts/arial.ttf not found.";
	}

    sf::Texture imageGenderButton;
    sf::Texture imageButton;
    if(!imageButton.loadFromFile(PATH"include/interface/button.png")){  // standard button brauch ich in jedem menü
        throw "Error: include/interface/button.png not found.";
    }
    if( gendermenu ){           // ab hier das heldenauswahlmenü
        if(!imageGenderButton.loadFromFile(PATH"include/interface/genderbutton.png")){  // genderbutton nur im gendermenp
            throw "Error: include/interface/genderbutton.png not found.";
        }
        buttons = 3;                        // Wieviel buttons im geschlechtsauswahlscreen?
        if(!(button = new MenuItem[buttons])){
            throw "Error: Could not allocate space for 'MenuItem'"; // achtung! die ersten 2 buttons sind speziell!
        }                                                           // also auf sone ganz spezielle art und weise...
                                                                    // naja... sie sind eben ganz besonders!
        button[0].text.setString("Select your hero");
        button[0].text.setPosition(WIDTH/2,HEIGHT/2-120);   // na wenn meine ersten beiden buttons eh keinen text haben...
        button[0].text.setFont(font);                       // dann kann ich ihre texte ja für was anderes nehmen ^^
        button[0].text.setCharacterSize(40U);
        button[0].text.setColor(sf::Color(0U,0U,0U));
        button[0].text.setOrigin(button[0].text.getGlobalBounds().width/2+1,0);
        
        if(!newgame){               // warnung nur anzeigen, wenns einen alten spielstand zum überschreiben gibt.
            button[1].text.setString("Warning: This will overwrite your old savegame!");
            button[1].text.setPosition(WIDTH/2,HEIGHT/2+130);
            button[1].text.setFont(font);
            button[1].text.setCharacterSize(20U);
            button[1].text.setColor(sf::Color(200U,0U,0U));
            button[1].text.setOrigin(button[1].text.getGlobalBounds().width/2+1,0);
        }
        
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
                case 2:         // ab hier können mehr standard buttons eingefügt werden!
                    button[i].text.setString("Back");
                    button[i].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                    button[i].action = Menue;
                    break;
            }
            button[i].image.setPosition(button[i].rect.left,button[i].rect.top);
            _menuItems.push_back(button[i]);
        }
    }
    else {              // ab hier das hauptmenü!
        buttons = 4;	// Wieviele Buttons brauchen wir? Ja richtig! 4 verdammt noch mal!
        if(!(button = new MenuItem[buttons])){
            throw "Error: Could not allocate space for 'MenuItem'";
        }
        
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
                    button[i].text.setString("Exit");       // neue buttons können ohne probleme eingefügt werden!
                    button[i].action = Exit;
                    button[i].active = true;
                    break;
            }
            if( button[i].active ){	// Andere Textur und Textfarbe f¸r klickbare Buttons
                button[i].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                button[i].text.setColor(sf::Color(255U,255U,255U));
            }
            _menuItems.push_back(button[i]);
        }        
    }

    // version des spiels im menü
	Version.setPosition((float)WIDTH-5.f,(float)HEIGHT-5.f);
	Version.setString(VERSION);
	Version.setCharacterSize(16U);
	Version.setFont(font);
	Version.setColor(sf::Color(0U,0U,0U));
	Version.setOrigin(Version.getGlobalBounds().width,Version.getGlobalBounds().height);

	Update(window);

	return GetMenuResponse(window,gendermenu);
}

void MainMenu::Update(sf::RenderWindow& window){    // methode zum neu zeichnen des menüs,
	window.draw(spriteBackground);                  // da mittlerweile der hintergrund animiert ist
    window.draw(spriteBackgroundRepeat);            // und auch die buttons sich ändern
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
		if( menuItemRect.contains(x,y) ){   // wenn man einen button klickt...
			return (*it).action;            // ...geb seine hinterlegte funktion zurück!
		}
	}
	return Nothing;     // für die, die keine buttons treffen und daneben klicken :>
}

/*MainMenu::MenuResult MainMenu::HandleKeyboard(int code){
	return Nothing;
}*/

MainMenu::MenuResult MainMenu::GetMenuResponse(sf::RenderWindow& window, bool gendermenu){
	sf::Event menuEvent;
    
	int selected = 0;
	int *active;
	if(!(active = new int[buttons])){
        throw "Error: Could not allocate space for 'int'";
    }
	int activeButtons = 0;
	for( int i=0; i<buttons; i++){          // array, das alle aktiven buttons beinhaltet,
		if( button[i].active ){             // damit bei der tastatureingabe die inaktiven
			active[activeButtons++] = i;    // buttons übersprungen werden.
		}
	}
	while(true){
		while(window.pollEvent(menuEvent)){
            if( gendermenu ){
                switch(active[selected]){   // aktiver button wird anders dargestellt.
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
			if(menuEvent.type == sf::Event::MouseButtonPressed){    // menü mit maus steuern
				#ifdef DEBUGINFO
					std::cout << "x " << menuEvent.mouseButton.x << " -  y " << menuEvent.mouseButton.y << std::endl;
				#endif
				if(HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y) != Nothing) // wenn kein button gedr¸ckt wurde, mache nichts
					return HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y);
			}
/*           _                            _
        _.-'`4`-._                    _,-'5`'-._
     ,-'          `-.,____________,.-'    .-.   `-.
    /   .---.             ___            ( 3 )     \
   /  ,' ,-. `.     __   / 10\   __   .-. `-` .-.   \
  /   | | 6 | |    (_8) | / \ | (9_) ( 2 )   ( 1 )   \
 /    `. `-' ,'    __    \___/        `-` ,-. `-`     \
 |      `---`   ,-`  `-.       .---.     ( 0 )        |
 |             / -'11`- \    ,'  .  `.    `-`         |     Xbox 360 Controller Tastenbelegung
 |            | 13    14 |   | - 7 - |                |
 !             \ -.12,- /    `.  '  ,'                |
 |              `-.__,-'       `---`                  |
 |                  ________________                  |
 |             _,-'`                ``-._             |
 |          ,-'                          `-.          |
  \       ,'                                `.       /
   `.__,-'                                    `-.__*/
			else if(menuEvent.type == sf::Event::KeyPressed || menuEvent.type == sf::Event::JoystickButtonPressed ){
                if(menuEvent.key.code == sf::Keyboard::Return || (menuEvent.type == sf::Event::JoystickButtonPressed && menuEvent.joystickButton.button == 0) || menuEvent.joystickButton.button == 8 ){
					return button[active[selected]].action;
				}
                else if(menuEvent.key.code == sf::Keyboard::Escape || menuEvent.joystickButton.button == 1 || menuEvent.joystickButton.button == 9 ){
                    if(gendermenu){
                        return Menue;
                    }
                }
                else if(menuEvent.key.code == sf::Keyboard::Up || menuEvent.key.code == sf::Keyboard::Left || menuEvent.joystickButton.button == 11 || menuEvent.joystickButton.button == 13 ){
					if( selected > 0 ){
                        if( gendermenu ){
                            switch(active[selected]){   // damit die alten buttons wieder normal dargestellt werden.
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
				else if(menuEvent.key.code == sf::Keyboard::Down || menuEvent.key.code == sf::Keyboard::Right || menuEvent.joystickButton.button == 12 || menuEvent.joystickButton.button == 14 ){
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
				#ifdef DEBUGINFO
					std::cout << "button " << menuEvent.joystickButton.button << " pressed." << std::endl;
				#endif
			}
			else if(menuEvent.type == sf::Event::Closed){
				return Exit;
			}
		}
        spriteBackground.setPosition(-animation,0);         // hintergrundbild läuft durch
        spriteBackgroundRepeat.setPosition(-animation+spriteBackground.getGlobalBounds().width,0);
        if( animation > spriteBackground.getGlobalBounds().width ) animation = 0;   // zurücksetzen wenn bildbreite erreicht wurde.
        animation++;            // LAUF FORREST! LAUF!
		Update(window);
	}
}
