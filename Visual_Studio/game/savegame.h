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

	void saveConfigFile(ConfigFile& myConfigFile, bool default=false);
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

	void saveSavegame(Savegame& mySavegame, const char pGender = 'M', bool default=true);
	bool loadSavegame(Savegame& mySavegame);
};

#endif