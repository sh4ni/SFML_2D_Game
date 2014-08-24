/**
	Infotext: Die Klasse Sound spielt die Ingame Musik ab.
	Sofern in der Konfigurationsdatei settings.cfg der Sound aktiviert (Sound = True) 
	ist wird ein Sound im Menü und den einzelnen Maps abgespielt. In der Pause wird die
	Soundausgabe pausiert. Weiterhin erfolgt kein Sound im Intro.
	Wird der Sound deaktiviert erfolgt keine Soundausgabe im gesamten Spiel / Menüs.

*/
#include "sounds.h"

gameMusic::gameMusic(){
	//Konstruktor	
};
gameMusic::~gameMusic(){
	//Destruktor
};

sf::Music gameMusic::music;