#ifndef SAVEGAME_H_
#define SAVEGAME_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "defines.h"
#include "md5.h"
#include "map.h"

class ConfigFile{
public:
	static ConfigFile * currentConfigFile;
	ConfigFile();
	~ConfigFile();
	int width;
	int height;
	std::string winmode;
	bool sound;

	// Controller Setting
	unsigned controller_A;
	unsigned controller_B;
	unsigned controller_X;
	unsigned controller_Y;
	unsigned controller_LB;
	unsigned controller_RB;
	unsigned controller_BACK;
	unsigned controller_START;
	unsigned controller_LS;
	unsigned controller_RS;

	void saveConfigFile(bool defaultConfig=false);
	void loadConfigFile();
};

class Savegame{
public:
	static Savegame * currentSaveGame;
	Savegame();
	~Savegame();
	int pHealth;
	int pLvl;
	int pExp;
	char pGender;
	std::string pName;

	std::string mLevelId;
	float mPosX;
	float mPosY;

	char direction;
	int chapter;	// 

	std::string checksum;

	void saveSavegame(bool defaultConfig=false);
	bool loadSavegame(bool init=false);
	
};

#endif