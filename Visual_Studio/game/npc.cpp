#include "npc.h"

// Wegen Static im Header, hier wird texture bekannt gemacht
/*
Texture Npc::texture;

Npc::Npc(){
	Speed = 0.5f;

	Active = true;

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(35.f,35.f);
	sprite.setPosition(400.f,400.f);
	//sprite.setRotation(180.f);
}

void Npc::SetPosition(float X, float Y){
	sprite.setPosition(X,Y);
}

void Npc::Update(RenderWindow &Window, float ElapsedTime){
	if(Active){
		float x = sprite.getPosition().x;
		float y = sprite.getPosition().y;
	
		if(y <= Window.getSize().y){
			y += (Speed*ElapsedTime);
		}else{
			Active = false;
		}
		
		sprite.setPosition(x,y);
	}
}

void Npc::Render(RenderWindow &Window){
	if(Active){
		Window.draw(sprite);
	}
}


void Npc::loadTexture(String text){
	if(!texture.loadFromFile(text)){
			std::cout << "Bilddatei konnte nicht gefunden werden!" << std::endl;
		}else{
			std::cout << "Bilddatei erfolgreich geladen!" << std::endl;
		}
}
*/