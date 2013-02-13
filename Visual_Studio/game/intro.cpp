#include "intro.h"
#include "defines.h"
#include "schrift.h"

void Intro::Show(sf::RenderWindow& renderWindow)
{
	sf::Texture image;
	if(!image.loadFromFile(PATH"include/interface/splashscreen.png")){
		exit(1); // exeption werfen oder sonst was...
	}

	sf::Sprite sprite(image);

	sprite.setOrigin((float)image.getSize().x/2,(float)image.getSize().y/2);
	sprite.setPosition((float)WIDTH/2.f,(float)HEIGHT/2.f);

	Schrift Version(WIDTH-5,HEIGHT-5,VERSION,16);
	Version.printText.setOrigin(Version.printText.getGlobalBounds().width,Version.printText.getGlobalBounds().height);

	renderWindow.clear(sf::Color(50,50,50)); // Hintergrundfarbe im Intro
	renderWindow.draw(sprite);
	Version.Render(renderWindow);
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
