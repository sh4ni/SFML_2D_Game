#include "intro.h"

void Intro::Show(sf::RenderWindow& renderWindow)
{
	sf::Texture image;
	if(image.loadFromFile("include/interface/intro.png") != true)
	{
		return;
	}

	sf::Sprite sprite(image);
	
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