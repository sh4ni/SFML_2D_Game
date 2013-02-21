#include "pause.h"

/*
Wenn die Pause gestartet wird,
färbt sich der Bildschirm dunkel transparent.
Der Spieler kann hier durch Eingaben das Spiel a) Speichern b) Laden c) Beenden
Die Pause gibt einen Wert zurück. Dieser Rückgabewert entscheidet ob das Spiel schlussendlich
beendet wird oder fortgesetzt wird.

*/

bool Pause::Show(sf::RenderWindow& renderWindow, sf::View viewCamera){
	
	int CenterX = (int)viewCamera.getCenter().x;    // pausemenŸ muss hier gegengerechnet werden,
	int CenterY = (int)viewCamera.getCenter().y;    // da sonst mapkoordinaten mit bildschirm koordinaten
                                                    // nicht Ÿbereinstimmen!
	// Hintergrund Box
	sf::RectangleShape Background(sf::Vector2f(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height));
    Background.setFillColor(sf::Color(0, 0, 0,100));
	Background.setPosition((float)CenterX-(float)ConfigFile::currentConfigFile->width/2.f,(float)CenterY-(float)ConfigFile::currentConfigFile->height/2.f);

	// Pause Schriftzug
	Schrift Pause(CenterX,CenterY-100,"Paused",50);
	Pause.printText.setOrigin(Pause.printText.getGlobalBounds().width/2.f+1.f,0);	// Textbox zentrieren

	// Logo im Pausemenü
	sf::Texture LogoImage;
	if(!LogoImage.loadFromFile(PATH"include/interface/splashscreen.png")){  // selbes bild wie im intro
		throw "Error: include/interface/splashscreen.png not found.";
	}
	sf::Sprite Logo(LogoImage);
	Logo.setOrigin((float)LogoImage.getSize().x/2.f,0);
	Logo.setPosition((float)CenterX,(float)CenterY-300.f);

	// Hilfetext
	Schrift HilfeTasten(CenterX-50,CenterY,"ESC:\nF6\nF9:\nSpace:",20);
	Schrift HilfeText(CenterX+70,CenterY,"Continue\nSave\nLoad\nExit Game",20);
	HilfeTasten.printText.setOrigin(HilfeTasten.printText.getGlobalBounds().width/2.f+1.f,0);	// Textbox zentrieren
	HilfeText.printText.setOrigin(HilfeText.printText.getGlobalBounds().width/2.f+1.f,0);		// Textbox zentrieren

	// LOS! ZEICHNE ES NIEDERER SKLAVE!
	renderWindow.draw(Background);
	renderWindow.draw(Logo);
	Pause.Render(renderWindow);
	HilfeTasten.Render(renderWindow);
	HilfeText.Render(renderWindow);

	renderWindow.display();

	sf::Event pauseLoop;
	while(renderWindow.waitEvent(pauseLoop)){
	
		Map::currentMap->currentMap->getClock()->restart();	// damit der Player nicht während der Pause weiter laufen kann

		if(pauseLoop.type == sf::Event::KeyPressed || pauseLoop.type == sf::Event::JoystickButtonPressed ){
			if (pauseLoop.key.code == sf::Keyboard::Escape || pauseLoop.joystickButton.button == ConfigFile::currentConfigFile->controller_START ){
				#ifdef DEBUGINFO
					std::cout << "Continue Playing.." << std::endl;
				#endif
					return false;
			}else if(pauseLoop.key.code == sf::Keyboard::Space || pauseLoop.joystickButton.button == ConfigFile::currentConfigFile->controller_BACK || pauseLoop.type == sf::Event::Closed){
				#ifdef DEBUGINFO
					std::cout << "Quit Game!" << std::endl;
				#endif
					return true; // gebe true zurueck damit das spiel anschließend beendet wird
			}else if(pauseLoop.key.code == sf::Keyboard::F6){
				Savegame::currentSaveGame->saveSavegame();
			}else if(pauseLoop.key.code == sf::Keyboard::F7){
				Savegame::currentSaveGame->loadSavegame();
			}
		}
	}
	return false;	// das Spiel geht weiter
}