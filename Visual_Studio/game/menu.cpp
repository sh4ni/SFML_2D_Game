#include "include.h"

void Menu::Show(RenderWindow& renderWindow){
	Texture background;
	if(background.loadFromFile("include/interface/menu.png") != true){
		return;
	}

	Sprite sprite(background);
	
	renderWindow.draw(sprite);
	
	Schrift Menu_Start(175,235,"Spiel Starten",30);
	Menu_Start.Render(renderWindow);

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

