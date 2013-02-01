#include "defines.h"
#include "player.h"

Player::Player(String tex){
	Speed = PLAYERSPEED;

	if(!texture.loadFromFile(tex)){
		std::cout << "Fehler beim Laden der Textur!" << std::endl;	
	}else{
		#if DEBUG == 1
			std::cout << "Textur wurde erfolgreich geladen!" << std::endl;
		#endif
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(16.f,16.f);
	sprite.setPosition(WIDTH/2,HEIGHT/2);
	sprite.setScale(2.1f,2.1f); // player wird 110% groß skaliert
}

float Player::getPosX(void){
	return this->x;
}

float Player::getPosY(void){
	return this->y;
}

void Player::Update(RenderWindow &Window, float ElapsedTime){
	this->x = sprite.getPosition().x;
	this->y = sprite.getPosition().y;
	
	#if DEBUG == 1
		std::cout << x << " - " << y << std::endl;
	#endif

	if(Keyboard::isKeyPressed(Keyboard::Left)){
		sprite.setRotation(270.f);
		x -= (Speed*ElapsedTime);
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Right)){
		sprite.setRotation(90.f);
		x += (Speed*ElapsedTime);
	}
	if(Keyboard::isKeyPressed(Keyboard::Up)){
		sprite.setRotation(0.f);
		y -= (Speed*ElapsedTime);
	}
	if(Keyboard::isKeyPressed(Keyboard::Down)){
		sprite.setRotation(180.f);
		y += (Speed*ElapsedTime);
	}
	/*
	if(Keyboard::isKeyPressed(Keyboard::Escape)){
		exit(EXIT_SUCCESS);
	}
	*/
	sprite.setPosition(x,y);
}

void Player::Render(RenderWindow &Window){
	Window.draw(sprite);
}