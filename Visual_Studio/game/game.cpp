#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "defines.h"
#include "player.h"
#include "npc.h"
#include "schrift.h"
#include "map.h"

using namespace sf;

int Game() {
	try {

		RenderWindow window(VideoMode(WIDTH, HEIGHT), VERSION);
		Clock clock;
		Texture BackgroundTexture;
		Sprite BackgroundSprite;
		std::vector<Npc> npcs;

		Player P1("include/texture/player/player.png");
		Schrift myVersion(0,HEIGHT-10,VERSION);
		Schrift Texter(0,50,"Danner");
	
		BackgroundTexture.loadFromFile("include/texture/background.png");
		BackgroundSprite.setTexture(BackgroundTexture);
		BackgroundSprite.setPosition(0.f,0.f);

		Npc::loadTexture("include/texture/npc/npc.png");

		LoadMap("include/map/map1.txt");

		int npcCount = 0;

		int timeCount = 0;
		int random_x = 0;

		while (window.isOpen())
		{
			Event event;
		

			float ElapsedTime = (float)clock.restart().asMilliseconds();
			timeCount += (int)ElapsedTime;

			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			// Npc Erstellung 
			if(timeCount >= 100){
				random_x = (rand()%WIDTH)+1;
				Npc E1;
				E1.SetPosition(random_x,20.f);
				npcs.push_back(E1);
				timeCount = 0;
				npcCount++;
			}

			window.clear(Color(255,255,255));
			//window.draw(BackgroundSprite);
			DrawMap(window);

			for(int i=0; i< npcs.size(); i++){
				if(!npcs[i].Active){
					npcs.erase(npcs.begin() + i);
					npcCount--;
				}
				npcs[i].Update(window, ElapsedTime);
				npcs[i].Render(window);
			}
			std::cout << npcCount << std::endl;
			P1.Update(window, ElapsedTime);
			P1.Render(window);
			myVersion.Render(window);
			Texter.Render(window),
			window.display();
		}
	}
	catch(const char * ErrorCode){
		std::cout << ErrorCode << std::endl;
	}
	catch(...){
		std::cout << "Ausnahmefehler" << std::endl;
	}

    return 0;
}
