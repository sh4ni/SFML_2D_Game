#ifndef DEFINES_H_
#define DEFINES_H_

#define DEBUG 1

#define WIDTH 1024
#define HEIGHT 768

#define FPS 60U

#if DEBUG == 1
	#define VERSION "The Journey - 0.05 Alpha Build - " __DATE__ " " __TIME__ " DEBUGGING "
#else
	#define VERSION "The Journey - 0.05 Alpha Build - " __DATE__ " " __TIME__ " STABLE "
#endif


// used in map.cpp
#define TILESIZE 32

// used in player.cpp
#define PLAYERSPEED 0.4f


#define EXP_LVL_2 100
#define EXP_LVL_3 125
#define EXP_LVL_4 155


#endif