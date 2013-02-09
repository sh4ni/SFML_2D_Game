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

	int mLevelId;
	int mPosX;
	int mPosY;

};

#endif