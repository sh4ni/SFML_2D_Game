#include "SplashScreen.h"
#include "game.h"
void SplashScreen::Show(sf::RenderWindow & renderWindow)
{
	Texture image;
	if(image.loadFromFile("include/texture/SplashScreen.png") != true)
	{
		return;
	}

	Sprite sprite(image);
	
	renderWindow.draw(sprite);
	renderWindow.display();

	Event event;
	while(true)
	{
		while(renderWindow.pollEvent(event))
		{
			if(event.type == Event::EventType::KeyPressed || event.type == Event::EventType::MouseButtonPressed ){
				return;
			}else if(event.type == Event::EventType::Closed){
				return;
			}
		}
	}
}