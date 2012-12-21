#include "game.h"

int main(void){

	game();

<<<<<<< HEAD
	return 0;
=======
int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), VERSION);
	Clock clock;
	Texture BackgroundTexture;
	Sprite BackgroundSprite;
	std::vector<Npc> npcs;

	Player P1("include/texture/player/player.png");
	Schrift myVersion("include/fonts/arial.ttf",0,HEIGHT-10,VERSION);
	Schrift Texter("include/fonts/arial.ttf",0,50,"Danner");
	
	BackgroundTexture.loadFromFile("include/texture/background.png");
	BackgroundSprite.setTexture(BackgroundTexture);
	BackgroundSprite.setPosition(0.f,0.f);

	Npc::loadTexture("include/texture/npc/npc.png");

	int timeCount = 0;
	float random_x;

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
			random_x =(float) (rand()%WIDTH)+1;
			Npc E1;
			E1.SetPosition(random_x,20.f);
			npcs.push_back(E1);
			timeCount = 0;
		}

        window.clear(Color(0,127,255));
		window.draw(BackgroundSprite);

		for(unsigned i=0; i< npcs.size(); i++){
			if(!npcs[i].Active){
				npcs.erase(npcs.begin() + i);
			}
			npcs[i].Update(window, ElapsedTime);
			npcs[i].Render(window);
		}

		P1.Update(window, ElapsedTime);
		P1.Render(window);
		myVersion.Render(window);
		Texter.Render(window),
        window.display();
    }

    return 0;
>>>>>>> 77ed9d78eef8cc77c0917932b04aab581c340223
}
