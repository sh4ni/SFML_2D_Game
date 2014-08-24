/**
	Infotext: Wenn die Pause gestartet wird, färbt sich der Bildschirm dunkel transparent.
	Der Spieler kann hier durch Eingaben das Spiel a) F6 - Speichern b) F9 - Laden c) Leertaste - Beenden.
	Die Pause gibt einen Wert zurück. Dieser Rückgabewert entscheidet ob das Spiel schlussendlich
	beendet wird oder fortgesetzt wird.
*/
#include "pause.h"

bool Pause::Show(sf::RenderWindow& renderWindow, sf::View& viewCamera){

	float CenterX = viewCamera.getCenter().x;    // pausemenü muss hier gegengerechnet werden,
	float CenterY = viewCamera.getCenter().y;    // da sonst mapkoordinaten mit bildschirm koordinaten
                                                    // nicht übereinstimmen!
	// Hintergrund Box
	sf::RectangleShape Background(sf::Vector2f((float)ConfigFile::currentConfigFile->width, (float)ConfigFile::currentConfigFile->height));
    Background.setFillColor(sf::Color(0, 0, 0,100));
	Background.setPosition(CenterX-(float)ConfigFile::currentConfigFile->width/2.f,CenterY-(float)ConfigFile::currentConfigFile->height/2.f);

	// Pause Schriftzug
	Schrift Pause(CenterX,CenterY-100,"Paused",50);
	Pause.printText.setOrigin(Pause.printText.getGlobalBounds().width/2.f+1.f,0);	// Textbox zentrieren

	// Logo im Pausemen¸
	sf::Texture LogoImage;
	if(!LogoImage.loadFromFile(PATH"include/interface/splashscreen.png")){  // selbes bild wie im intro
		throw "Error: include/interface/splashscreen.png not found.";
	}
	sf::Sprite Logo(LogoImage);
	Logo.setOrigin((float)LogoImage.getSize().x/2.f,0);
	Logo.setPosition(CenterX,CenterY-300.f);

	// Hilfetext
	#ifdef DEBUGINFO
		Schrift HilfeTasten(CenterX-50.f,CenterY,"ESC:\n1\n2\n9\n0\nF6\nF9:\nSpace:",20);
		Schrift HilfeText(CenterX+70.f,CenterY,"Continue\nGrow EXP\nGet Damage\nEnable Player Input\nBlock Player Input\nSave\nLoad\nExit Game",20);
	#else
		Schrift HilfeTasten(CenterX-50.f,CenterY,"ESC:\nF6\nF9:\nSpace:",20);
		Schrift HilfeText(CenterX+70.f,CenterY,"Continue\nSave\nLoad\nExit Game",20);
	#endif
	HilfeTasten.printText.setOrigin(HilfeTasten.printText.getGlobalBounds().width/2.f+1.f,0);	// Textbox zentrieren
	HilfeText.printText.setOrigin(HilfeText.printText.getGlobalBounds().width/2.f+1.f,0);		// Textbox zentrieren

	// LOS! ZEICHNE ES, NIEDERER SKLAVE!
	renderWindow.draw(Background);
	renderWindow.draw(Logo);
	Pause.Render(renderWindow);
	HilfeTasten.Render(renderWindow);
	HilfeText.Render(renderWindow);

	renderWindow.display();

	sf::Event pauseLoop;
	while(renderWindow.waitEvent(pauseLoop)){
	
		Map::currentMap->currentMap->getClock()->restart();	// damit der Player nicht w‰hrend der Pause weiter laufen kann

		if(pauseLoop.type == sf::Event::KeyPressed || pauseLoop.type == sf::Event::JoystickButtonPressed ){
			if (pauseLoop.key.code == sf::Keyboard::Escape || pauseLoop.joystickButton.button == ConfigFile::currentConfigFile->controller_START ){
				#ifdef DEBUGINFO
					std::cout << "Continue Playing.." << std::endl;
				#endif
					return false;
			}else if(pauseLoop.key.code == sf::Keyboard::Space || pauseLoop.joystickButton.button == ConfigFile::currentConfigFile->controller_BACK ){
				#ifdef DEBUGINFO
					std::cout << "Quit Game!" << std::endl;
				#endif
					return true; // gebe true zurueck damit das spiel anschlieﬂend beendet wird
			}else if(pauseLoop.key.code == sf::Keyboard::F6){
				Savegame::currentSaveGame->saveSavegame();
			}else if(pauseLoop.key.code == sf::Keyboard::F9){
				Savegame::currentSaveGame->loadSavegame();
			}
		}else if(pauseLoop.type == sf::Event::Closed){
			return true;
		}
	}
	return false;	// das Spiel geht weiter
}