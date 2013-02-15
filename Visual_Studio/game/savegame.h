#ifndef SAVEGAME_H_
#define SAVEGAME_H_


#include <fstream>
#include "md5.h"

class Savegame
{
public:
	Savegame()
	{

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


};

#endif