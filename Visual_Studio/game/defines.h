#ifndef DEFINES_H_
#define DEFINES_H_

#define DEBUGINFO 

#define WIDTH 800
#define HEIGHT 600

#define FPS 60U

// Used in intro and menues
#define VERSION "0.29 Alpha"

#ifdef DEBUGINFO
	#define WINDOWTITLE "The Journey - " VERSION " Build - " __DATE__ " " __TIME__ " - DEBUG Mode is enabled"
#else
	#define WINDOWTITLE "The Journey - " VERSION " Build - " __DATE__ " " __TIME__ " - DEBUG Mode is disabled"
#endif

// path du /include folder
#define PATH ""

// used in menues
#define BUTTONWIDTH 704
#define BUTTONHEIGHT 64
#define GENDERBUTTON 128

// used in map.cpp and player.cpp
#define TILESIZE 32

// used in player.cpp
#define PLAYERSPEED 0.2f
#define COLLISIONTOLERANCE 4
#define ANIMATIONSPEED 2.8f         // Mehr = langsamere Animation
#define CONTROLLERTOLERANCE 22.f    // Analogstick nie komplett in der 0 possition
#define IDLEHEAL 85					// Alle mach das am Schluss
#define COOLDOWN 200

#define BASEHEALTH 100
#define HEALTHPERLEVEL 10
#define BASEEXP 50
#define EXPMULTIPLICATOR 1.15f

#define SAVEGAME "save.txt"
#define CHECKSAVE 1
#define CHECKSUM 1337

#define DEFAULT_HEALTH 100
#define DEFAULT_LVL 1
#define DEFAULT_EXP 0

#define DEFAULT_F_NAME "Ashley"
#define DEFAULT_M_NAME "Fabi"	// in memories

#define DEFAULT_LEVEL 1
#define DEFAULT_POSX WIDTH/2
#define DEFAULT_POSY HEIGHT/2

#endif
