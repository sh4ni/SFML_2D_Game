#include "intro.h"
#include "defines.h"

void Intro::Show(sf::RenderWindow& renderWindow)
{
	sf::Texture image;
	if(!image.loadFromFile("include/interface/splashscreen.png")){
		exit(1); // exeption werfen oder sonst was...
	}

	sf::Sprite sprite(image);

	sprite.setOrigin(image.getSize().x/2,image.getSize().y/2);
	sprite.setPosition(WIDTH/2,HEIGHT/2);

	renderWindow.clear(sf::Color(50,50,50)); // Hintergrundfarbe im Intro
	renderWindow.draw(sprite);
	renderWindow.display();

	sf::Event currentEvent;
	while(true)
	{
		while(renderWindow.pollEvent(currentEvent))
		{
			if(currentEvent.type == sf::Event::KeyPressed || currentEvent.type == sf::Event::MouseButtonPressed ){
				return;
			}else if(currentEvent.type == sf::Event::Closed){
				return;
			}
		}
	}
}