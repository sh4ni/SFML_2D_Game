#ifndef SAVEGAME_H_
#define SAVEGAME_H_


#include <fstream>

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
	char pName[DEFAULT_NAME_LENGTH];

	int mLevelId;
	float mPosX;
	float mPosY;

	double checksum;


};

#endif