/**
	Vorantwortlich: Filip Menke
	Infotext: Hier wird das Intro dargestellt
*/
#include "intro.h"
#include "defines.h"
#include "schrift.h"
#include "savegame.h"
#include <ctime>

void Intro::Show(sf::RenderWindow& renderWindow){
	sf::Texture image;
	if(!image.loadFromFile(PATH"include/interface/splashscreen.png")){
        throw "Error: include/interface/splashscreen.png not found.";
	}
    
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    std::cout << "MaxRes: " << modes[0].width << "x" << modes[0].height << std::endl;

	sf::Sprite sprite(image);

	sprite.setOrigin((float)image.getSize().x/2,(float)image.getSize().y/2);    /// "Mittelpunkt" des Logos in die Mitte
	sprite.setPosition((float)ConfigFile::currentConfigFile->width/2.f,(float)ConfigFile::currentConfigFile->height/2.f);   /// Logo in der Mitte des Bildschirms

	renderWindow.clear(sf::Color(50,50,50));    /// Hintergrundfarbe im Intro
	renderWindow.draw(sprite);                  /// journey logo
	renderWindow.display();

	clock_t begin = clock();

	sf::Event currentEvent;
	for(;;){	// dann gibts keine warnung mehr im vs 2010 auf warning lv 4 :P
		while(renderWindow.pollEvent(currentEvent)){
			
            /// Intro wird beendet, wenn auf der Tastatur, der Maus oder dem Gamepad eine Taste gedrückt wird.
			if(currentEvent.type == sf::Event::KeyPressed || currentEvent.type == sf::Event::MouseButtonPressed || currentEvent.type == sf::Event::JoystickButtonPressed ){
				return;
			}
			else if(currentEvent.type == sf::Event::Closed){
				return;
			}
		}
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		//std::cout << elapsed_secs << std::endl;
        
        /// Wird 5 Sekunden keine Taste gedrückt, wird es auch übersprungen
		if( elapsed_secs > 5.f ){
			return;
		}
	}
}
