#ifndef _NPC_H_
#define _NPC_H_

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Npc{
public:
	void Update(RenderWindow &Window, float ElapsedTime);
	void Render(RenderWindow &Window);
	void SetPosition(float X, float Y);
	Npc();
	static void loadTexture(String text);
	static Texture texture;
	bool Active;
private:
	float Speed;
	Sprite sprite;
	
};



#endif