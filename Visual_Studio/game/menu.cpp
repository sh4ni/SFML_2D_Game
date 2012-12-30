#include "include.h"

void Menu::Show(RenderWindow& renderWindow){
	Texture image;
	if(image.loadFromFile("include/texture/menu.png") != true){
		return;
	}

	Sprite sprite(image);
	
	renderWindow.draw(sprite);
	
	//Schrift Texter(0,50,"Danner");
	//Texter.Render(renderWindow);

	Player P1("include/texture/player/player.png");
	P1.Render(renderWindow);

	renderWindow.display();


	
	

	while(true)
	{
		Event event;
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

