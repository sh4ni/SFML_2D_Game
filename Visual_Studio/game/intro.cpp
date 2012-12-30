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

	Event event;
	while(true)
	{
		while(renderWindow.pollEvent(event))
		{
			if(event.type == Event::KeyPressed || event.type == Event::MouseButtonPressed ){
				return;
			}else if(event.type == Event::Closed){
				return;
			}
		}
	}
}