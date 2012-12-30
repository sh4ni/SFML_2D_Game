#include "include.h"

void Menu::Show(RenderWindow& renderWindow){
	Texture image;
	if(image.loadFromFile("include/interface/menu.png") != true){
		return;
	}

	Sprite sprite(image);
	
	renderWindow.draw(sprite);
	
	//Schrift Texter(0,50,"Danner");
	//Texter.Render(renderWindow);

	//////////////////////////////
	//////////////////////////////
	//////////////////////////////
	Clock clock;

	float ElapsedTime = (float)clock.restart().asMilliseconds();

	Player P1("include/texture/player/player.png");

	// Ist hier Falsch, da dass Fenster so nicht aktualisiert wird
	P1.Render(renderWindow);
	P1.Update(renderWindow, ElapsedTime);
	//////////////////////////////
	//////////////////////////////
	//////////////////////////////

	renderWindow.display();


	
	

	while(true)
	{
		Event event;
		while(renderWindow.pollEvent(event))
		{
			if(event.type == Event::MouseButtonPressed ){
				return;
			}else if(event.type == Event::Closed){
				return;
			}
		}
	}
}

