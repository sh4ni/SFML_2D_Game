#include "include.h"
void Intro::Show(RenderWindow & renderWindow)
{
	Texture image;
	if(image.loadFromFile("include/interface/intro.png") != true)
	{
		return;
	}

	Sprite sprite(image);
	
	renderWindow.draw(sprite);
	renderWindow.display();

	Event currentEvent;
	while(true)
	{
		while(renderWindow.pollEvent(currentEvent))
		{
			if(currentEvent.type == Event::KeyPressed || currentEvent.type == Event::MouseButtonPressed ){
				return;
			}else if(currentEvent.type == Event::Closed){
				return;
			}
		}
	}
}