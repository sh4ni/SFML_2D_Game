#ifndef SAVEGAME_H_
#define SAVEGAME_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "defines.h"
#include "md5.h"

class ConfigFile{
public:
	ConfigFile(){
	};
	~ConfigFile(){
	};
	int width;
	int height;
	std::string screenfolder;

	// Controller Setting
	unsigned controller_A;
	unsigned controller_B;
	unsigned controller_X;
	unsigned controller_Y;
	unsigned controller_LB;
	unsigned controller_RB;
	unsigned controller_BACK;
	unsigned controller_START;
	unsigned controller_LAXIS;
	unsigned controller_RAXIS;

	void saveConfigFile(ConfigFile& myConfigFile, bool defaultConfig=false);
	void loadConfigFile(ConfigFile& myConfigFile);
};

class Savegame{
public:
	Savegame(){
	}
	~Savegame(){
	}
	int pHealth;
	int pLvl;
	int pExp;
	char pGender;
	std::string pName;

	std::string mLevelId;
	float mPosX;
	float mPosY;

	std::string checksum;

	void saveSavegame(Savegame& mySavegame, const char pGender = 'M', bool defaultConfig=false);
	bool loadSavegame(Savegame& mySavegame);
};

#endif