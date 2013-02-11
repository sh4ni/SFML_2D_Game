#ifndef DEFINES_H_
#define DEFINES_H_

#define DEBUG 

#define WIDTH 1024
#define HEIGHT 768

#define FPS 60U

#ifdef DEBUG
	#define VERSION "The Journey - 0.11 Alpha Build - " __DATE__ " " __TIME__ " - DEBUG Mode is enabled "
#else
	#define VERSION "The Journey - 0.11 Alpha Build - " __DATE__ " " __TIME__ " - DEBUG Mode is disabled "
#endif

// used in map.cpp
#define TILESIZE 32

// used in player.cpp
#define PLAYERSPEED 0.2f
#define COLLISIONTOLERANCE 4
#define ANIMATIONSPEED 64.0f



#define SAVEGAME "save.txt"
#define CHECKSUM 1337

#define DEFAULT_HEALTH 20
#define DEFAULT_LVL 1
#define DEFAULT_EXP 1



#define DEFAULT_LEVEL 2
#define DEFAULT_POSX WIDTH/2
#define DEFAULT_POSY HEIGHT/2

#endif
