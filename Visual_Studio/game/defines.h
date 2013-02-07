#ifndef DEFINES_H_
#define DEFINES_H_

#define DEBUG 

#define WIDTH 1024
#define HEIGHT 768

#define FPS 60U

#ifdef DEBUG
	#define VERSION "The Journey - 0.05 Alpha Build - " __DATE__ " " __TIME__ " - DEBUG Mode is enabled "
#else
	#define VERSION "The Journey - 0.05 Alpha Build - " __DATE__ " " __TIME__ " - DEBUG Mode is disabled "
#endif

// used in map.cpp
#define TILESIZE 32

// used in player.cpp
#define PLAYERSPEED 0.4f
#define COLLISIONTOLERANCE 4

#endif
