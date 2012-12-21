#include "defines.h"
#include "player.h"

Player::Player(String tex){
	Speed = 0.4f;

	if(!texture.loadFromFile(tex)){
		std::cout << "Fehler beim Laden der Textur!" << std::endl;	
	}else{
		std::cout << "Textur wurde erfolgreich geladen!" << std::endl;
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(16.f,16.f);
	sprite.setPosition(WIDTH/2,HEIGHT/2);
}

void Player::Update(RenderWindow &Window, float ElapsedTime){
	float x = sprite.getPosition().x;
	float y = sprite.getPosition().y;

	if(Keyboard::isKeyPressed(Keyboard::Left)){
		sprite.setRotation(270.f);
		if( x <= -10.f){
			x = (float) Window.getSize().x;
		}else{
			x -= (Speed*ElapsedTime);
		}
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Right)){
		sprite.setRotation(90.f);
		if( x >= Window.getSize().x + 10.f){
			x = 0.f;
		}else{
			x += (Speed*ElapsedTime);
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Up)){
		sprite.setRotation(0.f);
		if( y <= -10.f){
			y = (float) Window.getSize().y;
		}else{
			y -= (Speed*ElapsedTime);
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Down)){
		sprite.setRotation(180.f);
		if( y >= Window.getSize().y + 10.f){
			y = 0.f;
		}else{
			y += (Speed*ElapsedTime);
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Escape)){
		exit(EXIT_SUCCESS);
	}
	sprite.setPosition(x,y);
}

void Player::Render(RenderWindow &Window){
	Window.draw(sprite);
}