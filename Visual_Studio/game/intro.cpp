#include "intro.h"
#include "defines.h"
#include "schrift.h"

void Intro::Show(sf::RenderWindow& renderWindow)
{
	sf::Texture image;
	if(!image.loadFromFile(PATH"include/interface/splashscreen.png")){
        throw "Error: include/interface/splashscreen.png not found.";
	}

	sf::Sprite sprite(image);

	sprite.setOrigin((float)image.getSize().x/2,(float)image.getSize().y/2);    // mittelpunkt des logos in die mitte
	sprite.setPosition((float)WIDTH/2.f,(float)HEIGHT/2.f);                     // mitte des bildschirms

	renderWindow.clear(sf::Color(50,50,50));    // Hintergrundfarbe im Intro
	renderWindow.draw(sprite);                  // journey logo
	renderWindow.display();

	sf::Event currentEvent;
	while(true)
	{
		while(renderWindow.pollEvent(currentEvent))
		{
			if(currentEvent.type == sf::Event::KeyPressed || currentEvent.type == sf::Event::MouseButtonPressed || currentEvent.type == sf::Event::JoystickButtonPressed ){
				return;
			}else if(currentEvent.type == sf::Event::Closed){
				return;
			}
		}
	}
}
