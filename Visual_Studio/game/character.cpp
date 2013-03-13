/**
	Verantwortlich: Filip Menke
	Infotext: 
*/
#include "character.h"

Character::Character(){
	
};
Character::~Character(){

};

float Character::getPosX(void){
	return this->PosX;
}

float Character::getPosY(void){
	return this->PosY;
}

void Character::setPosition( float x, float y){
    this->PosX = x;
    this->PosY = y;
	sprite.setPosition(x,y);
}

void Character::setMapSize( int xMax, int yMax){
	this->MapSize.x = xMax;
	this->MapSize.y = yMax;
}

float Character::getSpeed(void){
	return this->Speed;
}

void Character::increaseSpeed(float speedValue){
	if(this->Speed < 0.35f)
		this->Speed += speedValue;
}

void Character::decreaseSpeed(float speedValue){
	if(this->Speed <= 0.2f)
		this->Speed = 0.1f;
	else
		this->Speed -= speedValue;
}

void Character::setName(std::string Name){
	this->Name = Name;
}
std::string Character::getName(void){
	return this->Name;
}

int Character::getHealth(void){
	return this->Health;
}

void Character::setHealth(int Health){
	this->Health = Health;
}

void Character::setLvl(int Lvl){
	this->Lvl = Lvl;
}

int Character::getLvl(void){
	return this->Lvl;
}

void Character::setColMap(sf::IntRect***& ColMap){
	this->ColMap = ColMap;
};

/**
 Berechnet den Schaden, welchen dein Charakter bekommen kann.
 Der Schaden wird je nach Levelunterschied abgeschwächt oder verstärkt.
 Unterschied Maximal +- 10 Level und zwischen.
 Der "Gegner" ist immer der Angreifer.
 -10 Level: Schaden 0%
 0 Level: Schaden 100%
 +10 Level: Schaden 125%
 Wenn der Berechnete Schaden 0 ist, wird pauschal immer 1 Schaden zurückgegeben.
 */
int Character::calcDamage(int damage,int level){
	int levelDif = level-this->Lvl;
    float multi = 1.f;
    if(levelDif > 0){   // Gegner hat einen höhreren Level
		if(levelDif >10) levelDif = 10;
			multi += (float)levelDif*0.025f;
		}
		else if(levelDif < 0){ // Gegner hat einen niedrigeren Level
			if(levelDif < -10) levelDif = -10;
			multi += (float)levelDif*0.1f;
		}
		damage = (int)((float)damage * multi);
		if ( damage <= 0 ){
			damage = 1;
		}
	return damage;
}

/**
 Diese Methode ist für die Zahlen, die über die Charaktere hinweg schweben.
 z.B. Schaden, Erfahrungsgewinn, Levelaufstieg, usw.
 Für char type:
 m = Schaden am Monster
 p = Schaden am Player
 h = Heilung
 e = Erfahrung
 l = Level Up
 */
void Character::damageText(int damage, char type){
	Schrift E1;
	std::ostringstream dmgString;
	if( type == 'e') dmgString << "XP +";           // zusatztext für exp und levelup
	else if( type == 'l') dmgString << "LEVEL ";
		
	dmgString << damage;
	E1.Init(PosX,PosY,dmgString.str());
	E1.printText.setStyle(sf::Text::Bold);
	
	if( type == 'm') E1.printText.setColor(sf::Color(0xFF,0xFF,0x00));
	else if( type == 'p') E1.printText.setColor(sf::Color(0xFF,0x00,0x00));
	else if( type == 'h') E1.printText.setColor(sf::Color(0x00,0xFF,0x00));
	else if( type == 'e') E1.printText.setColor(sf::Color(0xFF,0x00,0xFF));
	else if( type == 'l'){
		E1.printText.setCharacterSize(100);
		E1.printText.setColor(sf::Color(0xFF,0x88,0x00));
	}
	
	E1.printText.setOrigin(E1.printText.getGlobalBounds().width/2, 0);
	dmgText.push_back(E1);
}

/**
 Für das Darstellen eines Charakters.
 Inaktive ("getötete") Charaktere sollen nicht neu gezeichnet werden.
 der dmgText wird in einem Vector gespeichert und soll auch angezeigt werden können,
 wenn ein Charakter inaktiv ist.
 */
void Character::Render(sf::RenderWindow &renderWindow, bool drawEnemy){
	if(drawEnemy) renderWindow.draw(sprite);

	for( unsigned i=0; i<dmgText.size(); i++){
		dmgText[i].printText.setPosition(dmgText[i].printText.getPosition().x, dmgText[i].printText.getPosition().y-1 );
		dmgText[i].printText.setColor(sf::Color(dmgText[i].printText.getColor().r,dmgText[i].printText.getColor().g,dmgText[i].printText.getColor().b,dmgText[i].printText.getColor().a-4));
		dmgText[i].Render(renderWindow);
		if( dmgText[i].printText.getColor().a < 10 ){
			dmgText.erase(dmgText.begin()+i);
		}
	}
}
