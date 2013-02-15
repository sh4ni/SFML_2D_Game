#include "savegame.h"

void Savegame::saveSavegame(Savegame& mySavegame, const char pGender, bool default){
	// wenn der Spielstand korrput ist oder keiner vorhanden wird,
	// wird eine neuer erstellt mit vordefinierten defaultwerte
	
	std::cout << "No savegame detected! Default savegame will be loaded..\a\n";
	
	std::ofstream defaultsavegame;
	defaultsavegame.open(PATH SAVEGAME, std::ios::trunc & std::ios::binary);
	if(defaultsavegame.is_open()){
		if(default){
			// Health
			defaultsavegame << DEFAULT_HEALTH << std::endl;
			mySavegame.pHealth = DEFAULT_HEALTH;

			// Level
			defaultsavegame << DEFAULT_LVL << std::endl;
			mySavegame.pLvl = DEFAULT_LVL;

				// Exp
			defaultsavegame << DEFAULT_EXP << std::endl;
			mySavegame.pExp = DEFAULT_EXP;

			std::string pName;
			if(pGender == 'M')
				pName = DEFAULT_M_NAME;
			else
				pName = DEFAULT_F_NAME;

			// Gender
			defaultsavegame << pGender << std::endl;
			mySavegame.pGender = pGender;

			// Name
			defaultsavegame << pName << std::endl;
			mySavegame.pName = pName;

			////////////////////////////////////////////////////

			// Map
			defaultsavegame << DEFAULT_LEVEL << std::endl;
			mySavegame.mLevelId = DEFAULT_LEVEL;

			// Pos X auf Map
			defaultsavegame << DEFAULT_POSX << std::endl;
			mySavegame.mPosX = DEFAULT_POSX;

			// Pos Y auf Map
			defaultsavegame << DEFAULT_POSY << std::endl;
			mySavegame.mPosY = DEFAULT_POSY;

			std::stringstream ss;
			ss << (mySavegame.pHealth - mySavegame.pLvl + mySavegame.pExp + mySavegame.pGender + (int)mySavegame.mPosX + (int)mySavegame.mPosY + CHECKSUM);
			std::string checksum = md5(ss.str());
			//std::string checksum = md5(ss.str()+mySavegame.mLevelId+mySavegame.pName);	// Nicht aktivieren! Erst in Final!

			#ifdef DEBUGINFO
				std::cout << "Savegame Checksum -> " << checksum << std::endl;
			#endif

			defaultsavegame << checksum;	
			mySavegame.checksum = checksum;
			
		}else{
			defaultsavegame << mySavegame.pHealth << std::endl;
			defaultsavegame << mySavegame.pLvl << std::endl;
			defaultsavegame << mySavegame.pExp << std::endl;
			defaultsavegame << mySavegame.pGender << std::endl;
			defaultsavegame << mySavegame.pName << std::endl;
			defaultsavegame << mySavegame.mLevelId << std::endl;
			defaultsavegame << mySavegame.mPosX << std::endl;
			defaultsavegame << mySavegame.mPosY << std::endl;

			std::stringstream ss;
			ss << (mySavegame.pHealth - mySavegame.pLvl + mySavegame.pExp + mySavegame.pGender + (int)mySavegame.mPosX + (int)mySavegame.mPosY + CHECKSUM);
			std::string checksum = md5(ss.str());
			//std::string checksum = md5(ss.str()+mySavegame.mLevelId+mySavegame.pName);	// Nicht aktivieren! Erst in Final!

			defaultsavegame << checksum << std::endl;
		}
		defaultsavegame.close();
	}
}

bool Savegame::loadSavegame(Savegame& mySavegame){
	std::ifstream loadgame;
	loadgame.open(PATH SAVEGAME, std::ios::binary);
	if(loadgame.is_open()){
		std::cout << "Savegame detected! Loading..\n";
		loadgame >> mySavegame.pHealth;
		loadgame >> mySavegame.pLvl;
		loadgame >> mySavegame.pExp;
		loadgame >> mySavegame.pGender;
		loadgame >> mySavegame.pName;

		loadgame >> mySavegame.mLevelId;
		loadgame >> mySavegame.mPosX;
		loadgame >> mySavegame.mPosY;
		
		loadgame >> mySavegame.checksum;
		
		std::stringstream ss;
		ss << (mySavegame.pHealth - mySavegame.pLvl + mySavegame.pExp + mySavegame.pGender + (int)mySavegame.mPosX + (int)mySavegame.mPosY + CHECKSUM);
		std::string s = md5(ss.str());
		
		if(mySavegame.checksum.compare(s) == 0)
			std::cout << "Savegame okay...!\n";
		else
			return false;

		#ifdef DEBUGINFO
			std::cout << "Savegame successfully loaded.." << std::endl;
		#endif

		loadgame.close();
		
		return true;
	}else{
		saveSavegame(mySavegame);
	}
	return false;
}

void ConfigFile::saveConfigFile(ConfigFile& myConfigFile, bool default){
	std::ofstream configFile;
	configFile.open(PATH SETTINGS, std::ios::trunc);
	if(configFile.is_open()){
		if(default){
			#ifdef DEBUGINFO
				std::cout << "Default Config will be created.";
			#endif
			configFile << "WIDTH = " << DEFAULT_WIDTH << std::endl;
			myConfigFile.width = DEFAULT_WIDTH;
			configFile << "HEIGHT = " << DEFAULT_HEIGHT << std::endl;
			myConfigFile.height = DEFAULT_HEIGHT;
			configFile << "SCREENFOLDER = " << DEFAULT_SCREENSHOTFOLDER << std::endl;
			myConfigFile.screenfolder = DEFAULT_SCREENSHOTFOLDER;
		}else{
			#ifdef DEBUGINFO
				std::cout << "Config saved.";
			#endif
			configFile << "WIDTH = " << myConfigFile.width << std::endl;
			configFile << "HEIGHT = " << myConfigFile.height << std::endl;
			configFile << "SCREENFOLDER = " << myConfigFile.screenfolder << std::endl;
		}
	}
	configFile.close();
}

void ConfigFile::loadConfigFile(ConfigFile& myConfigFile){
	std::ifstream configFile;
	std::string line;
	configFile.open(PATH SETTINGS);
	if(configFile.is_open()){
		while(getline(configFile,line)){
			std::istringstream subline(line.substr(line.find("=") + 1));
			if(line.find("WIDTH") != -1)
				subline >> myConfigFile.width;
			else if(line.find("HEIGHT") != -1)
				subline >> myConfigFile.height;
			else if(line.find("SCREENFOLDER") != -1)
				subline >> myConfigFile.screenfolder;		
		}
		#ifdef DEBUGINFO
			std::cout << "Config successfully loaded.";
		#endif
	}else{
		saveConfigFile(myConfigFile,true);
	}
	configFile.close();
}