#include "menu.h"
#include "defines.h"
#include <iostream>

int MainMenu::animation = 0;

MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& renderWindow, bool newgame, char menuType){
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
	spriteMenu.setPosition((float)ConfigFile::currentConfigFile->width/2.f,(float)ConfigFile::currentConfigFile->height/2.f);
	
	sf::Font font;
	if(!font.loadFromFile(PATH"include/fonts/arial.ttf")){
        throw "Error: include/fonts/arial.ttf not found.";
	}

    sf::Texture imageOtherButton;   // im neuen spiel, die spieler buttons. bei den optionen, der optionen button
    sf::Texture imageButton;
    if(!imageButton.loadFromFile(PATH"include/interface/button.png")){  // standard button brauch ich in jedem menü
        throw "Error: include/interface/button.png not found.";
    }
    
    // ========================= NEUES SPIEL =========================
    if( menuType == 'G' ){           // ab hier das heldenauswahlmenü
        if(!imageOtherButton.loadFromFile(PATH"include/interface/genderbutton.png")){  // genderbutton nur im gendermenp
            throw "Error: include/interface/genderbutton.png not found.";
        }
        buttons = 3;                                    // Wieviel buttons im geschlechtsauswahlscreen?
        button = new MenuItem[buttons];                 // achtung! die ersten 2 buttons sind speziell!
                                                        // also auf sone ganz spezielle art und weise...
        button[0].text.setString("Select your hero");   // naja... sie sind eben ganz besonders!
        button[0].text.setPosition((float)ConfigFile::currentConfigFile->width/2,(float)ConfigFile::currentConfigFile->height/2-120);   // na wenn meine ersten beiden buttons eh keinen text haben...
        button[0].text.setFont(font);                       // dann kann ich ihre texte ja für was anderes nehmen ^^
        button[0].text.setCharacterSize(40U);
        button[0].text.setColor(sf::Color(0U,0U,0U));
        button[0].text.setOrigin(button[0].text.getGlobalBounds().width/2+1,0);
        
        if(!newgame){               // warnung nur anzeigen, wenns einen alten spielstand zum überschreiben gibt.
            button[1].text.setString("Warning: This will overwrite your old savegame!");
            button[1].text.setPosition((float)ConfigFile::currentConfigFile->width/2,(float)ConfigFile::currentConfigFile->height/2+130);
            button[1].text.setFont(font);
            button[1].text.setCharacterSize(20U);
            button[1].text.setColor(sf::Color(200U,0U,0U));
            button[1].text.setOrigin(button[1].text.getGlobalBounds().width/2+1,0);
        }
        
        for( int i=0; i<buttons; i++){
            if( i<2 ){
                button[i].rect.left = ConfigFile::currentConfigFile->width/2-GENDERBUTTON/2+(i?100:-100);
                button[i].rect.top = ConfigFile::currentConfigFile->height/2-25;
                button[i].rect.width = GENDERBUTTON;
                button[i].rect.height = GENDERBUTTON;
                button[i].image.setTexture(imageOtherButton);
            }
            else {
                button[i].rect.left = ConfigFile::currentConfigFile->width/2-BUTTONWIDTH/2;
                button[i].rect.top = ConfigFile::currentConfigFile->height/2-160+(i+2)*BUTTONHEIGHT*3/2;
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
            button[i].image.setPosition((float)button[i].rect.left,(float)button[i].rect.top);
            _menuItems.push_back(button[i]);
        }
    }
    
    // ========================= OPTIONEN =========================
    else if( menuType == 'O' ){
        if(!imageOtherButton.loadFromFile(PATH"include/interface/options.png")){  // optionen button
            throw "Error: include/interface/options.png not found.";
        }
        buttons = 4;
        button = new MenuItem[buttons];
        for( int i=0; i<buttons; i++){
            button[i].rect.left = ConfigFile::currentConfigFile->width/2-BUTTONWIDTH/2;
            button[i].rect.top = ConfigFile::currentConfigFile->height/2-160+((i<3)?i:i+1)*BUTTONHEIGHT*3/2;
            button[i].rect.width = BUTTONWIDTH;
            button[i].rect.height = BUTTONHEIGHT;
            button[i].image.setTexture(imageButton);
            button[i].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
            button[i].image.setPosition((float)button[i].rect.left,(float)button[i].rect.top);
            button[i].text.setPosition(button[i].rect.left+10.f,button[i].rect.top+7.f);
            button[i].text.setFont(font);
            button[i].text.setCharacterSize(40U);
            button[i].text.setColor(sf::Color(255U,255U,255U));
            if(i<2){
                button[i].image2.setTexture(imageOtherButton);
                button[i].image2.setPosition((float)button[i].rect.left+BUTTONWIDTH-OPTIONWIDTH-7,(float)button[i].rect.top+7);

            }
            button[i].active = true;
            switch(i){
                case 0:
                    button[i].text.setString("Fullscreen");
                    button[i].action = Fullscreen;
                    if(ConfigFile::currentConfigFile->winmode == "window"){
                        button[i].image2.setTextureRect(sf::IntRect(0,0,OPTIONWIDTH,OPTIONHEIGHT));
                    }
                    else {
                        button[i].image2.setTextureRect(sf::IntRect(0,OPTIONHEIGHT,OPTIONWIDTH,OPTIONHEIGHT));
                    }
                    break;
                case 1:
                    button[i].text.setString("Sound");
                    button[i].action = Sound;
                    if(ConfigFile::currentConfigFile->sound == false){
                        button[i].image2.setTextureRect(sf::IntRect(0,0,OPTIONWIDTH,OPTIONHEIGHT));
                    }
                    else {
                        button[i].image2.setTextureRect(sf::IntRect(0,OPTIONHEIGHT,OPTIONWIDTH,OPTIONHEIGHT));
                    }
                    break;
                case 2:
                    button[i].text.setString("Load default settings");
                    button[i].action = DefaultSettings;
                    break;
                case 3:
                    button[i].text.setString("Back");
                    button[i].action = Menue;
                    break;
            }
            _menuItems.push_back(button[i]);
        }

    }
    
    // ========================= HAUPTMENÜ =========================
    else {              // ab hier das hauptmenü!
        buttons = 4;	// Wieviele Buttons brauchen wir? Ja richtig! 4 verdammt noch mal!
        button = new MenuItem[buttons];
        
        for( int i=0; i<buttons; i++){
            button[i].rect.left = ConfigFile::currentConfigFile->width/2-BUTTONWIDTH/2;
            button[i].rect.top = ConfigFile::currentConfigFile->height/2-160+((i<3)?i:i+1)*BUTTONHEIGHT*3/2;   // ((i<3)?i:i+1) = nach den 1. 3 buttons ein button abstand
            button[i].rect.width = BUTTONWIDTH;
            button[i].rect.height = BUTTONHEIGHT;
            button[i].image.setTexture(imageButton);
            button[i].image.setTextureRect(sf::IntRect(0,BUTTONHEIGHT*2,BUTTONWIDTH,BUTTONHEIGHT));
            button[i].image.setPosition((float)button[i].rect.left,(float)button[i].rect.top);
            button[i].text.setPosition((float)button[i].rect.left+10.f,(float)button[i].rect.top+7.f);
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
                    button[i].action = Options;
                    button[i].active = true;
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
	Version.setPosition((float)ConfigFile::currentConfigFile->width-5.f,(float)ConfigFile::currentConfigFile->height-5.f);
	Version.setString(VERSION);
	Version.setCharacterSize(16U);
	Version.setFont(font);
	Version.setColor(sf::Color(0U,0U,0U));
	Version.setOrigin(Version.getGlobalBounds().width,Version.getGlobalBounds().height);
	
	Update(renderWindow);

	return GetMenuResponse(renderWindow,menuType);
}

void MainMenu::Update(sf::RenderWindow& renderWindow){    // methode zum neu zeichnen des menüs,

    spriteBackground.setPosition((float)-animation,0.f);         // hintergrundbild läuft durch
    spriteBackgroundRepeat.setPosition(-animation+spriteBackground.getGlobalBounds().width,0);
    if( animation > spriteBackground.getGlobalBounds().width ) animation = 0;   // zurücksetzen wenn bildbreite erreicht wurde.
    animation++;            // LAUF FORREST! LAUF!

	renderWindow.draw(spriteBackgroundRepeat);            // da mittlerweile der hintergrund animiert ist
    renderWindow.draw(spriteBackground);                  // und auch die buttons sich ändern
	renderWindow.draw(spriteMenu);
	for( int i=0; i<this->buttons; i++ ){
		renderWindow.draw(button[i].image);
		renderWindow.draw(button[i].text);
        if( button[i].image2.getPosition().x ){
            renderWindow.draw(button[i].image2);
        }
	}
	renderWindow.draw(Version);

	renderWindow.display();
}

MainMenu::MenuResult MainMenu::HandleClick(int x, int y){
	std::list<MenuItem>::iterator it;
	for ( it = _menuItems.begin(); it != _menuItems.end(); it++){
		sf::IntRect menuItemRect = (*it).rect;
		if( menuItemRect.contains(x,y) ){   // wenn man einen button klickt...
			return (*it).action;            // ...geb seine hinterlegte funktion zurück!
		}
	}
	return Nothing;     // f¸r die, die keine buttons treffen und daneben klicken :>
}

MainMenu::MenuButton MainMenu::GetMenuButton(sf::Event menuEvent){
    
    // Tastatur
    if (menuEvent.type == sf::Event::KeyPressed){
        if(menuEvent.key.code == sf::Keyboard::Return){
            return Enter;
        }
        else if(menuEvent.key.code == sf::Keyboard::Escape){
            return Back;
        }
        else if(menuEvent.key.code == sf::Keyboard::Up || menuEvent.key.code == sf::Keyboard::Left){
            return Up;
        }
        else if(menuEvent.key.code == sf::Keyboard::Down || menuEvent.key.code == sf::Keyboard::Right){
            return Down;
        }
    }
    
    // Gamepad
    else if (menuEvent.type == sf::Event::JoystickButtonPressed){
        if(menuEvent.joystickButton.button == ConfigFile::currentConfigFile->controller_A || menuEvent.joystickButton.button == ConfigFile::currentConfigFile->controller_START){
            return Enter;
        }
        else if(menuEvent.joystickButton.button == ConfigFile::currentConfigFile->controller_B || menuEvent.joystickButton.button == ConfigFile::currentConfigFile->controller_BACK){
            return Back;
        }
#ifdef SYS_MACOS
        else if(menuEvent.joystickButton.button == 11 || menuEvent.joystickButton.button == 13){
            return Up;
        }
        else if(menuEvent.joystickButton.button == 12 || menuEvent.joystickButton.button == 14){
            return Down;
        }
#endif
    }

    return NoButton;
}

MainMenu::MenuResult MainMenu::GetMenuResponse(sf::RenderWindow& renderWindow, char menuType){

	sf::Event menuEvent;
    MenuButton CheckAxis = NoButton;
    bool blockAxis = false;
	int selected = 0;
	int *active = 0;
	if(!(active = new int[buttons])){
        throw "Error: Could not allocate space for 'int'";
    }
	int activeButtons = 0;
	for( int i=0; i<buttons; i++){          // array, das alle aktiven buttons beinhaltet,
		if( button[i].active ){             // damit bei der tastatureingabe die inaktiven
			active[activeButtons++] = i;    // buttons ¸bersprungen werden.
		}
	}
	while(4+8+15+16+23+42==108){	// LOST
        if( sf::Joystick::isConnected(0) ){
            if(!blockAxis){
                if( (sf::Joystick::getAxisPosition(0,sf::Joystick::Y) < -CONTROLLERAXISMENUE) ||
                   (sf::Joystick::getAxisPosition(0,sf::Joystick::X) < -CONTROLLERAXISMENUE) ||
                   (sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) < -CONTROLLERAXISMENUE) ||
                   (sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) < -CONTROLLERAXISMENUE) ){
                    CheckAxis = Up;
                    blockAxis = true;
                }
                else if( (sf::Joystick::getAxisPosition(0,sf::Joystick::Y) > CONTROLLERAXISMENUE) ||
                        (sf::Joystick::getAxisPosition(0,sf::Joystick::X) > CONTROLLERAXISMENUE) ||
                        (sf::Joystick::getAxisPosition(0,sf::Joystick::PovY) > CONTROLLERAXISMENUE) ||
                        (sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) > CONTROLLERAXISMENUE) ){
                    CheckAxis = Down;
                    blockAxis = true;
                }
            }
            else if ( (sf::Joystick::getAxisPosition(0,sf::Joystick::Y) > -CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2) ||
                     (sf::Joystick::getAxisPosition(0,sf::Joystick::X) > -CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2) ||
                     (sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) > -CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2) ||
                     (sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) > -CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2) ||
                     (sf::Joystick::getAxisPosition(0,sf::Joystick::Y) < CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2) ||
                     (sf::Joystick::getAxisPosition(0,sf::Joystick::X) < CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2) ||
                     (sf::Joystick::getAxisPosition(0,sf::Joystick::PovY) < CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2) ||
                     (sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) < CONTROLLERAXISMENUE-CONTROLLERAXISMENUE/2)) {
                CheckAxis = NoButton;
                blockAxis = false;
            }
        }
		while(renderWindow.pollEvent(menuEvent)){
            if( menuType == 'G' ){
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
			if(menuEvent.type == sf::Event::MouseMoved){
				//std::cout << menuEvent.mouseMove.x << " " << menuEvent.mouseMove.y << std::endl;
				for( int i=0; i<activeButtons; i++){
					if( button[active[i]].rect.contains(menuEvent.mouseMove.x,menuEvent.mouseMove.y) ){
						int selectedOld = selected;
						selected = i;
						if( selectedOld != selected ){
							if(menuType == 'G'){
								switch(active[selectedOld]){		// evtl kˆnnte man das hier besser lˆsen ... so oft wie ich die alten buttons neu render... evtl bei jedem "‰ndern" alles neu machen.
									case 0:
										button[active[selectedOld]].image.setTextureRect(sf::IntRect(0,0,GENDERBUTTON,GENDERBUTTON));
										break;
									case 1:
										button[active[selectedOld]].image.setTextureRect(sf::IntRect(0,GENDERBUTTON,GENDERBUTTON,GENDERBUTTON));
										break;
									case 2:
										button[active[selectedOld]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
										break;
								}
							}
							else {
								button[active[selectedOld]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
							}
						}
					}
				}
			}
			else if(menuEvent.type == sf::Event::MouseButtonPressed){    // men¸ mit maus steuern
				#ifdef DEBUGINFO
					std::cout << "x " << menuEvent.mouseButton.x << " -  y " << menuEvent.mouseButton.y << std::endl;
				#endif
				if(HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y) != Nothing) // wenn kein button gedr¸ckt wurde, mache nichts
                    delete [] active;
					return HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y);
			}
/*           _                            _										            _                            _
        _.-'`4`-._                    _,-'5`'-._								       _.-'`4`-._                    _,-'5`'-._
     ,-'          `-.,____________,.-'    .-.   `-.								    ,-'          `-.,____________,.-'    .-.   `-.
    /   .---.             ___            ( 3 )     \							   /   .---.             ___            ( 3 )     \
   /  ,' ,-. `.     __   / 10\   __   .-. `-` .-.   \							  /  ,' ,-. `.     __   / X \   __   .-. `-` .-.   \
  /   | | 6 | |    (_9) | / \ | (8_) ( 2 )   ( 1 )   \							 /   | | 8 | |    (_6) | / \ | (7_) ( 2 )   ( 1 )   \
 /    `. `-' ,'    __    \___/        `-` ,-. `-`     \							/    `. `-' ,'    __    \___/        `-` ,-. `-`     \
 |      `---`   ,-`  `-.       .---.     ( 0 )        |		Tastenbelegung		|      `---`   ,-`  `-.       .---.     ( 0 )        |
 |             / -'11`- \    ,'  .  `.    `-`         |   Xbox 360 Controller	|             / -'  `- \    ,'  .  `.    `-`         | DPad = Axen
 |            | 13    14 |   | - 7 - |                |		<-- MacOSX			|            |          |   | - 9 - |                |	PovX
 !             \ -.12,- /    `.  '  ,'                |		   Windows	-->		!             \ -.  ,- /    `.  '  ,'                |	PovY
 |              `-.__,-'       `---`                  |							|              `-.__,-'       `---`                  |
 |                  ________________                  |							|                  ________________                  |
 |             _,-'`                ``-._             |							|             _,-'`                ``-._             |
 |          ,-'                          `-.          |							|          ,-'                          `-.          |
  \       ,'                                `.       /							 \       ,'                                `.       /
   `.__,-'                                    `-.__.`							  `.__,-'                                    `-.__*/
			else if(menuEvent.type == sf::Event::KeyPressed || menuEvent.type == sf::Event::JoystickButtonPressed || !CheckAxis==NoButton ){
                if(GetMenuButton(menuEvent) == Enter){
					int temp = active[selected];
                    delete [] active;
					return button[temp].action;
				}														
				else if(GetMenuButton(menuEvent) == Back){
                    if(menuType != 'M'){
                        delete [] active;
                        return Menue;
                    }
                    else {
                        button[active[selected]].image.setTextureRect(sf::IntRect(0,0,BUTTONWIDTH,BUTTONHEIGHT));
                        selected = activeButtons-1;
                    }
                }
                else if( (GetMenuButton(menuEvent) == Up) || (CheckAxis == Up) ){
					if( selected > 0 ){
                        if( menuType == 'G' ){
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
				else if( (GetMenuButton(menuEvent) == Down) || (CheckAxis == Down) ){
					if( selected < activeButtons-1 ){
                        if( menuType == 'G' ){
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
                CheckAxis = NoButton;
				#ifdef DEBUGINFO
					std::cout << "button " << menuEvent.joystickButton.button << " pressed." << std::endl;
				#endif
			}
			else if(menuEvent.type == sf::Event::Closed){
                delete [] active;
				return Exit;
			}
		}
		Update(renderWindow);
	}
}
