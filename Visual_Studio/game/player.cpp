#include "defines.h"
#include "player.h"

Player::Player(String tex, IntRect*** CollisionMap){

	this->ColMap = CollisionMap;

	Speed = PLAYERSPEED;

	if(!texture.loadFromFile(tex)){
		std::cout << "Fehler beim Laden der Textur!" << std::endl;	
	}else{
		#ifdef DEBUG
			std::cout << "Textur wurde erfolgreich geladen!" << std::endl;
		#endif
	}

	//texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(16.f,32.f);
	sprite.setPosition(WIDTH/2,HEIGHT/2);
	//sprite.setScale(2.1f,2.1f); // player wird 110% groß skaliert

	Collision.height = TILESIZE*2;
	Collision.width = TILESIZE;
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
	
	Collision.left = x-TILESIZE/2;
	Collision.top = y-TILESIZE;

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)){
		x -= (Speed*ElapsedTime);
	}
	if(Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)){
		x += Speed*ElapsedTime;
	}
	if(Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)){
		y -= (Speed*ElapsedTime);
	}
	if(Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)){
		y += (Speed*ElapsedTime);
	}
	/*if(Keyboard::isKeyPressed(Keyboard::Escape)){
		exit(EXIT_SUCCESS);
	}*/
	int ty = ((int)y/TILESIZE)-1;
	int tx = ((int)x/TILESIZE)-1;
	for(int i=0;i<9;i++){
		if(ColMap[tx+(i%3)][ty+i/3] != NULL){
			if(Collision.intersects(*ColMap[tx+(i%3)][ty+i/3])){
				std::cout << ".";
			}
		}	
	}


	/*if ( Collision.intersects( *ColMap[(int)x/TILESIZE][(int)y/TILESIZE] ) ) {
			std::cout << "." ;
	}*/

	sprite.setPosition(x,y);

}

void Player::Render(RenderWindow &Window){
	Window.draw(sprite);
}