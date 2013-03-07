#include "savegame.h"

#ifdef SYS_MACOS
#include "ResourcePath.hpp"
#endif

Savegame * Savegame::currentSaveGame;
ConfigFile * ConfigFile::currentConfigFile;

// Prüfe ob eine Datei leer ist
bool is_empty(std::ifstream& myFile)
{
	// wenn die datei leer ist gebe ein true zurück
    return myFile.peek() == std::ifstream::traits_type::eof();
}

// Bereche die Checksume des Spielstandes anhand eines MD5 Alogrihtmus
std::string calc_checksum(){
	std::stringstream ss;
	ss << (Savegame::currentSaveGame->pHealth - Savegame::currentSaveGame->pLvl + Savegame::currentSaveGame->pExp - Savegame::currentSaveGame->pGender + (int)Savegame::currentSaveGame->mPosX - (int)Savegame::currentSaveGame->mPosY + CHECKSUM);
	std::string checksum = md5(ss.str());

#ifdef DEBUGINFO
	std::cout << "Savegame Checksum " << checksum << std::endl;
#endif

	return checksum;
}

Savegame::Savegame(){
#ifdef DEBUGINFO
	std::cout << "konstruktor Savegame." << std::endl;
#endif
}
Savegame::~Savegame(){
#ifdef DEBUGINFO
	std::cout << "destruktor Savegame." << std::endl;
#endif
}

void Savegame::saveSavegame(bool defaultSavegame){
	// wenn der Spielstand korrput ist oder keiner vorhanden wird,
	// wird eine neuer erstellt mit vordefinierten defaultwerte
	
	if(defaultSavegame)
		std::cout << "Default savegame will be loaded..\a\n";
	else
		std::cout << "Savegame will be saved!" << std::endl;
	
	std::ofstream defaultsavegame;
    std::string defaultSavegamePath = PATH SAVEGAME;
	defaultsavegame.open(defaultSavegamePath.c_str(), std::ios::trunc & std::ios::binary);
	if(defaultsavegame.is_open()){
		if(defaultSavegame){
			// Health
			defaultsavegame << DEFAULT_HEALTH << std::endl;
			Savegame::currentSaveGame->pHealth = DEFAULT_HEALTH;

			// Level
			defaultsavegame << DEFAULT_LVL << std::endl;
			Savegame::currentSaveGame->pLvl = DEFAULT_LVL;

			// Exp
			defaultsavegame << DEFAULT_EXP << std::endl;
			Savegame::currentSaveGame->pExp = DEFAULT_EXP;

			// Gender
			defaultsavegame << pGender << std::endl;
			Savegame::currentSaveGame->pGender = pGender;

			std::string pName;
			if(pGender == 'M')
				pName = DEFAULT_M_NAME;
			else
				pName = DEFAULT_F_NAME;

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

			///////////////////// CHECKSUM /////////////////////

			std::string checksum = calc_checksum();
			defaultsavegame << checksum;	
			Savegame::currentSaveGame->checksum = checksum;
			
		}else{
			// beim mapwechsel muss die neue Position gesetzt werden und alle werte aktualisiert werden vor dem speichern,
			// da sonst das Savegame korrupt wird!
			Savegame::currentSaveGame->pHealth = Map::currentMap->getPlayer()->getHealth();
			Savegame::currentSaveGame->pLvl = Map::currentMap->getPlayer()->getLvl();
			Savegame::currentSaveGame->pExp = Map::currentMap->getPlayer()->getExp();
			Savegame::currentSaveGame->pGender = Map::currentMap->getPlayer()->getGender();
			Savegame::currentSaveGame->pName = Map::currentMap->getPlayer()->getName();
			Savegame::currentSaveGame->mLevelId = Map::currentMap->getPlayer()->getLevelId();
			Savegame::currentSaveGame->mPosX = Map::currentMap->getPlayer()->getPosX();
			Savegame::currentSaveGame->mPosY = Map::currentMap->getPlayer()->getPosY();

			defaultsavegame << Map::currentMap->getPlayer()->getHealth() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getLvl() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getExp() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getGender() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getName() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getLevelId() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getPosX() << std::endl;
			defaultsavegame << Map::currentMap->getPlayer()->getPosY() << std::endl;
			
			std::string checksum = calc_checksum();
			defaultsavegame << checksum;
		}
		defaultsavegame.close();
	}
}


bool Savegame::loadSavegame(bool init){
	std::ifstream loadgame;
    std::string defaultSavegamePath = PATH SAVEGAME;
	loadgame.open(defaultSavegamePath.c_str(), std::ios::binary);
	
	if(!is_empty(loadgame) && loadgame.is_open()){

		// gehe in die letzte position der datei
		// wenn tellg keinen wert zurück gibt ist die datei leer und ein default save wird erstellt
		// ansonsten springe zum beginn der datei und lese diese ein
		

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
		loadgame.close();

		if(!init){ // wenn das spiel NICHT aus dem Init Bereich der game.cpp geladen wurde
			Map::currentMap->getPlayer()->setHealth(Savegame::currentSaveGame->pHealth);
			Map::currentMap->getPlayer()->setLvl(Savegame::currentSaveGame->pLvl);
			Map::currentMap->getPlayer()->setExp(Savegame::currentSaveGame->pExp);
			Map::currentMap->getPlayer()->setGender(Savegame::currentSaveGame->pGender);
			Map::currentMap->getPlayer()->setName(Savegame::currentSaveGame->pName);
			Map::currentMap->getPlayer()->setLevelId(Savegame::currentSaveGame->mLevelId);
			Map::currentMap->getPlayer()->setPosition(Savegame::currentSaveGame->mPosX,Savegame::currentSaveGame->mPosY);
		}

		std::string checksum = calc_checksum();

		if(Savegame::currentSaveGame->checksum.compare(checksum) == 0){
			std::cout << "Savegame okay...!\n";
			return true;
		}else{
			std::cout << "Savegame corrupt...!\a\n";
			saveSavegame(true); // default savegame will be created
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
	std::cout << "destruktor ConfigFile." << std::endl;
#endif
};

void ConfigFile::saveConfigFile(bool defaultConfig){
	std::ofstream configFile;
    std::string defaultSettingsPath = PATH SETTINGS;
	configFile.open(defaultSettingsPath.c_str(), std::ios::trunc);
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
			configFile << "WINMODE = " << DEFAULT_WINMODE << std::endl;
			ConfigFile::currentConfigFile->winmode = DEFAULT_WINMODE;

			configFile << "\n### Sound Settings ###" << std::endl;
			configFile << "SOUND = " << DEFAULT_SOUND << std::endl;
			ConfigFile::currentConfigFile->sound = DEFAULT_SOUND;
			
			configFile << "\n### Controller Settings ###" << std::endl;
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
			configFile << "### SCREEN SETTINGS ###" << std::endl;
			configFile << "WIDTH = " << ConfigFile::currentConfigFile->width << std::endl;
			configFile << "HEIGHT = " << ConfigFile::currentConfigFile->height << std::endl;
			configFile << "WINMODE = " << ConfigFile::currentConfigFile->winmode << std::endl;
		
			configFile << "\n### Sound Settings ###" << std::endl;
			configFile << "SOUND = " << ConfigFile::currentConfigFile->sound << std::endl;
		
			configFile << "\n### Controller Settings ###" << std::endl;
			configFile << "A_BUTTON = " << ConfigFile::currentConfigFile->controller_A << std::endl;
			configFile << "B_BUTTON = " << ConfigFile::currentConfigFile->controller_B << std::endl;
			configFile << "X_BUTTON = " << ConfigFile::currentConfigFile->controller_X << std::endl;
			configFile << "Y_BUTTON = " << ConfigFile::currentConfigFile->controller_Y << std::endl;
			configFile << "LB_BUTTON = " << ConfigFile::currentConfigFile->controller_LB << std::endl;
			configFile << "RB_BUTTON = " << ConfigFile::currentConfigFile->controller_RB << std::endl;
			configFile << "BACK_BUTTON = " << ConfigFile::currentConfigFile->controller_BACK << std::endl;
			configFile << "START_BUTTON = " << ConfigFile::currentConfigFile->controller_START << std::endl;
			configFile << "LS_BUTTON = " << ConfigFile::currentConfigFile->controller_LS << std::endl;
			configFile << "RS_BUTTON = " << ConfigFile::currentConfigFile->controller_RS << std::endl;

			configFile << "### End of Configfile ###" << std::endl;
			#ifdef DEBUGINFO
				std::cout << "Running Config saved.";
			#endif
		}
	}
	configFile.close();
}

void ConfigFile::loadConfigFile(){
	std::ifstream configFile;
	std::string line;
    std::string defaultSettingsPath = PATH SETTINGS;
	configFile.open(defaultSettingsPath.c_str());
	if(!is_empty(configFile) && configFile.is_open()){
		while(configFile >> line){
			if(line == "WIDTH"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->width;
			}else if(line == "HEIGHT"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->height;
			}else if(line == "WINMODE"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->winmode;
			}else if(line == "SOUND"){
				configFile.ignore(3);
				configFile >> ConfigFile::currentConfigFile->sound;
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
		configFile.close();
		#ifdef DEBUGINFO
			std::cout << "CONFIG - Loaded Values" << std::endl;
			std::cout << "WIDTH " << ConfigFile::currentConfigFile->width << std::endl;
			std::cout << "HEIGHT " << ConfigFile::currentConfigFile->height << std::endl;
			std::cout << "WINMODE " << ConfigFile::currentConfigFile->winmode << std::endl;
			std::cout << "SOUND " << ConfigFile::currentConfigFile->sound<< std::endl;
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
		saveConfigFile(true); // speichere die default config
	}
}