#ifndef DEFINES_H_
#define DEFINES_H_

#if defined(_WIN32) || defined(__WIN32__)
	#define SYS_WINDOWS
#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
    #define SYS_MACOS
#endif

#define FPS 60U

#define DEBUGINFO				// Für Debug infos im code

#define VERSION "0.6.3 Alpha"	// intro und menü

#ifdef DEBUGINFO
	#define WINDOWTITLE "The Journey - " VERSION " Build - " __DATE__ " " __TIME__ " - DEBUG Mode is enabled"
#else
	#define WINDOWTITLE "The Journey - " VERSION " Build - " __DATE__ " " __TIME__
#endif

// path du /include folder
#ifdef SYS_MACOS
    #define PATH "/Users/Kimiko/Documents/Git/SFML_2D_Game/Visual_Studio/game/"
#else
    #define PATH ""
#endif

// used in menues
#define BUTTONWIDTH 704
#define BUTTONHEIGHT 64
#define GENDERBUTTON 128
#define OPTIONWIDTH 100
#define OPTIONHEIGHT 50
#define CONTROLLERAXISMENUE 75.f

// used in map.cpp and player.cpp
#define TILESIZE 32

// used in player.cpp
#define PLAYERSPEED 0.2f
#define COLLISIONTOLERANCE 4
#define ANIMATIONSPEED 2.8f         // Mehr = langsamere Animation
#define CONTROLLERTOLERANCE 22.f    // Analogstick nie komplett in der 0 possition
#define IDLEHEAL 85					// 
#define COOLDOWN 200

#define BASEHEALTH 100.f
#define HEALTHMULTIPLICATOR 1.1f
#define BASEEXP 50.f
#define EXPMULTIPLICATOR 1.15f


// used in monster.cpp
#define HOLDTIME 3.f
#define MOVETIME 0.5f
#define DETECTIONRADIUS 3
#define LOSTRADIUS 8

// ConfigFile
#define SETTINGS "settings.cfg"
#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_WINMODE "window"
#define DEFAULT_SOUND 1 // true

// ConfigFile - Controllersettings
#ifdef SYS_WINDOWS
	#define DEFAULT_A 0
	#define DEFAULT_B 1
	#define DEFAULT_X 2
	#define DEFAULT_Y 3
	#define DEFAULT_LB 4
	#define DEFAULT_RB 5
	#define DEFAULT_BACK 6
	#define DEFAULT_START 7
	#define DEFAULT_LS 8
	#define DEFAULT_RS 9
#else
	#define DEFAULT_A 0
	#define DEFAULT_B 1
	#define DEFAULT_X 2
	#define DEFAULT_Y 3
	#define DEFAULT_LB 4
	#define DEFAULT_RB 5
	#define DEFAULT_BACK 9
	#define DEFAULT_START 8
	#define DEFAULT_LS 6
	#define DEFAULT_RS 7
#endif

// SaveGame
#define SAVEGAME "savegame.bin"
#define DEFAULT_HEALTH 100
#define DEFAULT_LVL 1
#define DEFAULT_EXP 0
#define DEFAULT_F_NAME "Ashley"
#define DEFAULT_M_NAME "Fabi"	// in memories
#define DEFAULT_LEVEL "map1"
#define DEFAULT_POSX 512
#define DEFAULT_POSY 384
#define CHECKSUM 4815162342


#endif
