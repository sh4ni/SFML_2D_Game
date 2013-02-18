#include "savegame.h"

void Savegame::saveSavegame(Savegame& mySavegame, const char pGender, bool defaultConfig){
	// wenn der Spielstand korrput ist oder keiner vorhanden wird,
	// wird eine neuer erstellt mit vordefinierten defaultwerte
	
	if(defaultConfig)
		std::cout << "Default savegame will be loaded..\a\n";
	else
		std::cout << "Savegame will be loaded!" << std::endl;
	
	std::ofstream defaultsavegame;
	defaultsavegame.open(PATH SAVEGAME, std::ios::trunc & std::ios::binary);
	if(defaultsavegame.is_open()){
		if(defaultConfig){
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
			// Änderung noch in Final!

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
			// Änderung noch in Final!

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
		//std::string checksum = md5(ss.str()+mySavegame.mLevelId+mySavegame.pName);	// Nicht aktivieren! Erst in Final!
		loadgame.close();

		if(mySavegame.checksum.compare(s) == 0){
			std::cout << "Savegame okay...!\n";
			return true;
		}else{
			std::cout << "Savegame corrupt...!\a\n";
			saveSavegame(mySavegame);
			return false;
		}
		
	}else{
		saveSavegame(mySavegame);
	}
	return true;
}

void ConfigFile::saveConfigFile(ConfigFile& myConfigFile, bool defaultConfig){
	std::ofstream configFile;
	configFile.open(PATH SETTINGS, std::ios::trunc);
	if(configFile.is_open()){
		if(defaultConfig){
			#ifdef DEBUGINFO
				std::cout << "Default Config will be created.\n";
			#endif
			configFile << "### SCREEN SETTINGS ###" << std::endl;
			configFile << "WIDTH = " << DEFAULT_WIDTH << std::endl;
			myConfigFile.width = DEFAULT_WIDTH;

			configFile << "HEIGHT = " << DEFAULT_HEIGHT << std::endl;
			myConfigFile.height = DEFAULT_HEIGHT;

			configFile << "SCREENDIR = " << DEFAULT_SCREENSHOTDIR << std::endl;
			myConfigFile.screendirectory = DEFAULT_SCREENSHOTDIR;
			
			configFile << "\n### Controller Settings ###" << std::endl;
			#ifdef SFML_SYSTEM_WINDOWS
				configFile << "Win Settings" << std::endl;
			#else
				configFile << "Other OS Settings" << std::endl;
			#endif

			configFile << "A_BUTTON = " << DEFAULT_A << std::endl;
			myConfigFile.controller_A = DEFAULT_A;

			configFile << "B_BUTTON = " << DEFAULT_B << std::endl;
			myConfigFile.controller_B = DEFAULT_B;

			configFile << "X_BUTTON = " << DEFAULT_X << std::endl;
			myConfigFile.controller_X = DEFAULT_X;

			configFile << "Y_BUTTON = " << DEFAULT_Y << std::endl;
			myConfigFile.controller_Y = DEFAULT_Y;

			configFile << "LB_BUTTON = " << DEFAULT_LB << std::endl;
			myConfigFile.controller_LB = DEFAULT_LB;

			configFile << "RB_BUTTON = " << DEFAULT_RB << std::endl;
			myConfigFile.controller_RB = DEFAULT_RB;

			configFile << "BACK_BUTTON = " << DEFAULT_BACK << std::endl;
			myConfigFile.controller_BACK = DEFAULT_BACK;

			configFile << "START_BUTTON = " << DEFAULT_START << std::endl;
			myConfigFile.controller_START = DEFAULT_START;

			configFile << "LS_BUTTON = " << DEFAULT_LS << std::endl;
			myConfigFile.controller_LS = DEFAULT_LS;

			configFile << "RS_BUTTON = " << DEFAULT_RS << std::endl;
			myConfigFile.controller_RS = DEFAULT_RS;

			configFile << "### End of Configfile ###" << std::endl;

		}else{
			#ifdef DEBUGINFO
				std::cout << "Config saved.";
			#endif
			configFile << "WIDTH = " << myConfigFile.width << std::endl;
			configFile << "HEIGHT = " << myConfigFile.height << std::endl;
			//configFile << "SCREENFOLDER = " << myConfigFile.screenfolder << std::endl;
			// Controller Settings
		}
	}
	configFile.close();
}

void ConfigFile::loadConfigFile(ConfigFile& myConfigFile){
	std::ifstream configFile;
	std::string line;
	configFile.open(PATH SETTINGS);
	if(configFile.is_open()){
		while(configFile >> line){
			if(line == "WIDTH"){
				configFile.ignore(3);
				configFile >> myConfigFile.width;
			}else if(line == "HEIGHT"){
				configFile.ignore(3);
				configFile >> myConfigFile.height;
			}else if(line == "SCREENDIR"){
				configFile.ignore(3);
				configFile >> myConfigFile.screendirectory;
			}else if(line == "A_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_A;
				//if(!myConfigFile.controller_A > 0U && !myConfigFile.controller_A <31U)
					//throw "Error in ConfigFile!";
			}else if(line == "B_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_B;
			}else if(line == "X_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_X;
			}else if(line == "Y_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_Y;
			}else if(line == "LB_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_LB;
			}else if(line == "RB_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_RB;
			}else if(line == "BACK_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_BACK;
			}else if(line == "START_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_START;
			}else if(line == "LS_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_LS;
			}else if(line == "RS_BUTTON"){
				configFile.ignore(3);
				configFile >> myConfigFile.controller_RS;
			}
		}
		
		#ifdef DEBUGINFO
			std::cout << "CONFIG - Values" << std::endl;
			std::cout << "WIDTH " << myConfigFile.width << std::endl;
			std::cout << "HEIGHT " << myConfigFile.height << std::endl;
			std::cout << "SCREENDIR " << myConfigFile.screendirectory << std::endl;
			std::cout << "A " << myConfigFile.controller_A << std::endl;
			std::cout << "B " << myConfigFile.controller_B << std::endl;
			std::cout << "X " << myConfigFile.controller_X << std::endl;
			std::cout << "Y " << myConfigFile.controller_Y << std::endl;
			std::cout << "LB " << myConfigFile.controller_LB << std::endl;
			std::cout << "RB " << myConfigFile.controller_RB << std::endl;
			std::cout << "BACK " << myConfigFile.controller_BACK << std::endl;
			std::cout << "START " << myConfigFile.controller_START << std::endl;
			std::cout << "LAXIS " << myConfigFile.controller_LS << std::endl;
			std::cout << "RAXIS " << myConfigFile.controller_RS << std::endl;
			std::cout << "CONFIG - END " << std::endl;
		
			std::cout << "Config successfully loaded.\n";
		#endif
	}else{
		saveConfigFile(myConfigFile,true);
	}
	configFile.close();
}