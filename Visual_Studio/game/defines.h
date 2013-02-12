#ifndef DEFINES_H_
#define DEFINES_H_

#define DEBUG 

#define WIDTH 1024
#define HEIGHT 768

#define FPS 60U

#ifdef DEBUG
	#define VERSION "The Journey - 0.25 Alpha Build - " __DATE__ " " __TIME__ " - DEBUG Mode is enabled "
#else
	#define VERSION "The Journey - 0.25w Alpha Build - " __DATE__ " " __TIME__ " - DEBUG Mode is disabled "
#endif

// used in menues
#define BUTTONWIDTH 704
#define BUTTONHEIGHT 64

// used in map.cpp and player.cpp
#define TILESIZE 32

// used in player.cpp
#define PLAYERSPEED 0.2f
#define COLLISIONTOLERANCE 4
#define ANIMATIONSPEED 4.0f	// Mehr = langsamere Animation

#define BASEHEALTH 100
#define HEALTHPERLEVEL 10
#define BASEEXP 50
#define EXPMULTIPLICATOR 1.15f

#define SAVEGAME "save.txt"
#define CHECKSAVE 1
#define CHECKSUM 1337

#define DEFAULT_HEALTH 100
#define DEFAULT_LVL 1
#define DEFAULT_EXP 1

#define DEFAULT_F_NAME "Ashley"
#define DEFAULT_M_NAME "Fabi"	// in memories

#define DEFAULT_LEVEL 1
#define DEFAULT_POSX WIDTH/2
#define DEFAULT_POSY HEIGHT/2

#endif
