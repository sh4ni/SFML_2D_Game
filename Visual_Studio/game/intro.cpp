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
	

	sf::Sprite sprite(image);

	sprite.setOrigin((float)image.getSize().x/2,(float)image.getSize().y/2);    // mittelpunkt des logos in die mitte
	sprite.setPosition((float)ConfigFile::currentConfigFile->width/2.f,(float)ConfigFile::currentConfigFile->height/2.f);                     // mitte des bildschirms

	renderWindow.clear(sf::Color(50,50,50));    // Hintergrundfarbe im Intro
	renderWindow.draw(sprite);                  // journey logo
	renderWindow.display();

	clock_t begin = clock();

	sf::Event currentEvent;
	for(;;){	// dann gibts keine warnung mehr im vs 2010 auf warning lv 4 :P
		while(renderWindow.pollEvent(currentEvent)){
			if(currentEvent.type == sf::Event::KeyPressed || currentEvent.type == sf::Event::MouseButtonPressed || currentEvent.type == sf::Event::JoystickButtonPressed ){
				return;

			}
			else if(currentEvent.type == sf::Event::Closed){
				return;
			}
		}
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		std::cout << elapsed_secs << std::endl;
		if( elapsed_secs > 5.f ){
			return;
		}
	}
}
