#include "include.h"

void Menu::Show(RenderWindow& renderWindow){
	Texture image;
	if(image.loadFromFile("include/interface/menu.png") != true){
		return;
	}

	Sprite sprite(image);
	
	renderWindow.draw(sprite);
	
	Schrift Texter(0,50,"Menu");
	Texter.Render(renderWindow);

	renderWindow.display();

	while(true)
	{
		Event event;
		while(renderWindow.pollEvent(event))
		{
			if(event.type == Event::KeyPressed){
				return;
			}else if(event.type == Event::Closed){
				return;
			}
		}
	}
}

