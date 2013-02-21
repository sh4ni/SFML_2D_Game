#include "savegame.h"

Savegame * Savegame::currentSaveGame;
ConfigFile * ConfigFile::currentConfigFile;

Savegame::Savegame(){
#ifdef DEBUGINFO
	std::cout << "konstruktor Savegame." << std::endl;
#endif
}
Savegame::~Savegame(){
#ifdef DEBUGINFO
	std::cout << "dekonstruktor Savegame." << std::endl;
#endif
}

void Savegame::saveSavegame(const char pGender, bool defaultConfig){
	// wenn der Spielstand korrput ist oder keiner vorhanden wird,
	// wird eine neuer erstellt mit vordefinierten defaultwerte
	
	if(defaultConfig)
		std::cout << "Default savegame will be loaded..\a\n";
	else
		std::cout << "Savegame will be saved!" << std::endl;
	
	std::ofstream defaultsavegame;
	defaultsavegame.open(PATH SAVEGAME, std::ios::trunc & std::ios::binary);
	if(defaultsavegame.is_open()){
		if(defaultConfig){
			// Health
			defaultsavegame << DEFAULT_HEALTH << std::endl;
			Savegame::currentSaveGame->pHealth = DEFAULT_HEALTH;

			// Level
			defaultsavegame << DEFAULT_LVL << std::endl;
			Savegame::currentSaveGame->pLvl = DEFAULT_LVL;

				// Exp
			defaultsavegame << DEFAULT_EXP << std::endl;
			Savegame::currentSaveGame->pExp = DEFAULT_EXP;

			std::string pName;
			if(pGender == 'M')
				pName = DEFAULT_M_NAME;
			else
				pName = DEFAULT_F_NAME;

			// Gender
			defaultsavegame << pGender << std::endl;
			Savegame::currentSaveGame->pGender = pGender;

			// Name
			defaultsavegame << pName << std::endl;
			Savegame::currentSaveGame->pName = pName;

			////////////////////////////////////////////////////

			// Map
			defaultsavegame << DEFAULT_LEVEL << std::endl;
			Savegame::currentSaveGame->mLevelId = DEFAULT_LEVEL;

			// Pos X auf Map
			defaultsavegame << DEFAULT_POSX << std::endl;
			Savegame::currentSaveGame->mPosX = DEFAULT_POSX;

			// Pos Y auf Map
			defaultsavegame << DEFAULT_POSY << std::endl;
			Savegame::currentSaveGame->mPosY = DEFAULT_POSY;

			std::stringstream ss;
			ss << (Savegame::currentSaveGame->pHealth - Savegame::currentSaveGame->pLvl + Savegame::currentSaveGame->pExp + Savegame::currentSaveGame->pGender + (int)Savegame::currentSaveGame->mPosX + (int)Savegame::currentSaveGame->mPosY + CHECKSUM);
			std::string checksum = md5(ss.str());
			// Änderung noch in Final!

			#ifdef DEBUGINFO
				std::cout << "Savegame Checksum -> " << checksum << std::endl;
			#endif
				
			defaultsavegame << checksum;	
			Savegame::currentSaveGame->checksum = checksum;
			
		}else{
			
			defaultsavegame << Map::currentMap->getPlayer()->getHealth() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getLvl() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getExp() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getGender() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getName() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getLevelId() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getPosX() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getPosY() << std::endl;
			
			std::stringstream ss;
			ss << (Map::currentMap->getPlayer()->getHealth() - Map::currentMap->getPlayer()->getLvl() + Map::currentMap->getPlayer()->getExp() + Map::currentMap->getPlayer()->getGender() + (int)Map::currentMap->getPlayer()->getPosX() + (int)Map::currentMap->getPlayer()->getPosY() + CHECKSUM);
			std::string checksum = md5(ss.str());
			// Änderung noch in Final!

			defaultsavegame << checksum << std::endl;
		}
		defaultsavegame.close();
	}
}



bool Savegame::loadSavegame(bool init){
	std::ifstream loadgame;
	loadgame.open(PATH SAVEGAME, std::ios::binary);
	if(loadgame.is_open()){
		std::cout << "Savegame detected! Loading..\n";
		loadgame >> Savegame::currentSaveGame->pHealth;
		loadgame >> Savegame::currentSaveGame->pLvl;
		loadgame >> Savegame::currentSaveGame->pExp;
		loadgame >> Savegame::currentSaveGame->pGender;
		loadgame >> Savegame::currentSaveGame->pName;
		loadgame >> Savegame::currentSaveGame->mLevelId;
		loadgame >> Savegame::currentSaveGame->mPosX;
		loadgame >> Savegame::currentSaveGame->mPosY;
		loadgame >> Savegame::currentSaveGame->checksum;
		if(!init){
			Map::currentMap->getPlayer()->setHealth(Savegame::currentSaveGame->pHealth);
			Map::currentMap->getPlayer()->setLvl(Savegame::currentSaveGame->pLvl);
			Map::currentMap->getPlayer()->setExp(Savegame::currentSaveGame->pExp);
			Map::currentMap->getPlayer()->setGender(Savegame::currentSaveGame->pGender);
			Map::currentMap->getPlayer()->setName(Savegame::currentSaveGame->pName);
			Map::currentMap->getPlayer()->setLevelId(Savegame::currentSaveGame->mLevelId);
			Map::currentMap->getPlayer()->setPosition(Savegame::currentSaveGame->mPosX,Savegame::currentSaveGame->mPosY);
		}

		loadgame.close();

		// bitte prüfen @filip
		std::stringstream ss;
			ss << (Savegame::currentSaveGame->pHealth - Savegame::currentSaveGame->pLvl + Savegame::currentSaveGame->pExp + Savegame::currentSaveGame->pGender + (int)Savegame::currentSaveGame->mPosX + (int)Savegame::currentSaveGame->mPosY + CHECKSUM);
			std::string s = md5(ss.str());
			
			//std::string checksum = md5(ss.str()+Savegame::currentSaveGame->mLevelId+mySavegame.pName);	// Nicht aktivieren! Erst in Final!
		if(Savegame::currentSaveGame->checksum.compare(s) == 0){
			std::cout << "Savegame okay...!\n";
			return true;
		}else{
			std::cout << "Savegame corrupt...!\a\n";
			saveSavegame();
			return false;
		}
	}
	return false;
}


ConfigFile::ConfigFile(){
#ifdef DEBUGINFO
	std::cout << "konstruktor ConfigFile." << std::endl;
#endif
};
ConfigFile::~ConfigFile(){
#ifdef DEBUGINFO
	std::cout << "dekonstruktor ConfigFile." << std::endl;
#endif
};

void ConfigFile::saveConfigFile(bool defaultConfig){
	std::ofstream configFile;
	configFile.open(PATH SETTINGS, std::ios::trunc);
	if(configFile.is_open()){
		if(defaultConfig){
			#ifdef DEBUGINFO
				std::cout << "Default Config will be created.\n";
			#endif
			configFile << "### SCREEN SETTINGS ###" << std::endl;
			configFile << "WIDTH = " << DEFAULT_WIDTH << std::endl;
			ConfigFile::currentConfigFile->width = DEFAULT_WIDTH;

			configFile << "HEIGHT = " << DEFAULT_HEIGHT << std::endl;
			ConfigFile::currentConfigFile->height = DEFAULT_HEIGHT;

			configFile << "SCREENDIR = " << DEFAULT_SCREENSHOTDIR << std::endl;
			ConfigFile::currentConfigFile->screendirectory = DEFAULT_SCREENSHOTDIR;
			
			configFile << "\n### Controller Settings ###" << std::endl;
			#ifdef SYS_WINDOWS
				configFile << "Win Settings" << std::endl;
			#else
				configFile << "Other OS Settings" << std::endl;
			#endif

			configFile << "A_BUTTON = " << DEFAULT_A << std::endl;
			ConfigFile::currentConfigFile->controller_A = DEFAULT_A;

			configFile << "B_BUTTON = " << DEFAULT_B << std::endl;
			ConfigFile::currentConfigFile->controller_B = DEFAULT_B;

			configFile << "X_BUTTON = " << DEFAULT_X << std::endl;
			ConfigFile::currentConfigFile->controller_X = DEFAULT_X;

			configFile << "Y_BUTTON = " << DEFAULT_Y << std::endl;
			ConfigFile::currentConfigFile->controller_Y = DEFAULT_Y;

			configFile << "LB_BUTTON = " << DEFAULT_LB << std::endl;
			ConfigFile::currentConfigFile->controller_LB = DEFAULT_LB;

			configFile << "RB_BUTTON = " << DEFAULT_RB << std::endl;
			ConfigFile::currentConfigFile->controller_RB = DEFAULT_RB;

			configFile << "BACK_BUTTON = " << DEFAULT_BACK << std::endl;
			ConfigFile::currentConfigFile->controller_BACK = DEFAULT_BACK;

			configFile << "START_BUTTON = " << DEFAULT_START << std::endl;
			ConfigFile::currentConfigFile->controller_START = DEFAULT_START;

			configFile << "LS_BUTTON = " << DEFAULT_LS << std::endl;
			ConfigFile::currentConfigFile->controller_LS = DEFAULT_LS;

			configFile << "RS_BUTTON = " << DEFAULT_RS << std::endl;
			ConfigFile::currentConfigFile->controller_RS = DEFAULT_RS;

			configFile << "### End of Configfile ###" << std::endl;

		}else{
			#ifdef DEBUGINFO
				std::cout << "Config saved.";
			#endif
			configFile << "WIDTH = " << ConfigFile::currentConfigFile->width << std::endl;
			configFile << "HEIGHT = " << ConfigFile::currentConfigFile->height << std::endl;
			//configFile << "SCREENFOLDER = " << myConfigFile.screenfolder << std::endl;
			// Controller Settings
		}
	}
	configFile.close();
}

void ConfigFile::loadConfigFile(){
	
	std::ifstream configFile;
	std::string line;
	configFile.open(PATH SETTINGS);
	if(configFile.is_open()){
		while(configFile >> line){
			if(line == "WIDTH"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->width;
			}else if(line == "HEIGHT"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->height;
			}else if(line == "SCREENDIR"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->screendirectory;
			}else if(line == "A_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_A;
			}else if(line == "B_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_B;
			}else if(line == "X_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_X;
			}else if(line == "Y_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_Y;
			}else if(line == "LB_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_LB;
			}else if(line == "RB_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_RB;
			}else if(line == "BACK_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_BACK;
			}else if(line == "START_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_START;
			}else if(line == "LS_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_LS;
			}else if(line == "RS_BUTTON"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->controller_RS;
			}
		}
		
		#ifdef DEBUGINFO
			std::cout << "CONFIG - Values" << std::endl;
			std::cout << "WIDTH " << ConfigFile::currentConfigFile->width << std::endl;
			std::cout << "HEIGHT " << ConfigFile::currentConfigFile->height << std::endl;
			std::cout << "SCREENDIR " << ConfigFile::currentConfigFile->screendirectory << std::endl;
			std::cout << "A " << ConfigFile::currentConfigFile->controller_A << std::endl;
			std::cout << "B " << ConfigFile::currentConfigFile->controller_B << std::endl;
			std::cout << "X " << ConfigFile::currentConfigFile->controller_X << std::endl;
			std::cout << "Y " << ConfigFile::currentConfigFile->controller_Y << std::endl;
			std::cout << "LB " << ConfigFile::currentConfigFile->controller_LB << std::endl;
			std::cout << "RB " << ConfigFile::currentConfigFile->controller_RB << std::endl;
			std::cout << "BACK " << ConfigFile::currentConfigFile->controller_BACK << std::endl;
			std::cout << "START " << ConfigFile::currentConfigFile->controller_START << std::endl;
			std::cout << "LAXIS " << ConfigFile::currentConfigFile->controller_LS << std::endl;
			std::cout << "RAXIS " << ConfigFile::currentConfigFile->controller_RS << std::endl;
			std::cout << "CONFIG - END " << std::endl;
			std::cout << "Config successfully loaded.\n";
		#endif
	}else{
		saveConfigFile(true);
	}
	configFile.close();
}