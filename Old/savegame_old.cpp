#include "game.h"

void defaultSavegame(Savegame& mySavegame, const char pGender = 'M'){
	// wenn der Spielstand korrput ist oder keiner vorhanden wird,
	// wird eine neuer erstellt mit vordefinierten defaultwerte

	std::cout << "No savegame detected! Default savegame will be loaded..\a\n";
	
	std::ofstream defaultsavegame;
	defaultsavegame.open(SAVEGAME, std::ios::trunc & std::ios::binary);
	if(defaultsavegame.is_open()){

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
		ss << (mySavegame.pHealth - mySavegame.pLvl + mySavegame.pExp + mySavegame.pGender + (int)mySavegame.mPosX + (int)mySavegame.mPosY + mySavegame.mLevelId + CHECKSUM);
		std::string checksum = md5(ss.str());

		#ifdef DEBUGINFO
			std::cout << "Savegame Checksum -> " << checksum << std::endl;
		#endif

		defaultsavegame << checksum;	
		mySavegame.checksum = checksum;

		defaultsavegame.close();
	}
}

void Game::Init(void)
{
	// Do it!
	Savegame mySavegame;

	std::ifstream loadgame;
	loadgame.open(SAVEGAME, std::ios::binary);
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
		ss << (mySavegame.pHealth - mySavegame.pLvl + mySavegame.pExp + mySavegame.pGender + (int)mySavegame.mPosX + (int)mySavegame.mPosY + mySavegame.mLevelId + CHECKSUM);
		std::string s = md5(ss.str());
		
		if(mySavegame.checksum.compare(s) == 0)
			std::cout << "Savegame okay...!\n";
		else
			//defaultSavegame(mySavegame,'M',true);
			Game::Start(mySavegame, true);

		#ifdef DEBUGINFO
			std::cout << "Savegame successfully loaded.." << std::endl;
		#endif
		loadgame.close();

		Game::Start(mySavegame);	// Da ein Spielstand vorhanden ist Defaultm��ig der zweite Paramter false
	}else{
		defaultSavegame(mySavegame,false);
	}
	Game::Start(mySavegame, true);
}

void Game::Start(Savegame& currentSavegame, bool newgame)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	_mainWindow.create(sf::VideoMode(WIDTH, HEIGHT), WINDOWTITLE, sf::Style::Titlebar);

	// Deaktiviert den Mauszeiger im Fenster - Klicken geht weiterhin..
	//_mainWindow.setMouseCursorVisible(false);

	// Lade und setze das Fenstericon
	sf::Image Icon;
	if(!Icon.loadFromFile(PATH"include/interface/icon.bmp"))
		std::cout << "Failed to load the Windowicon" << std::endl;
	else
		_mainWindow.setIcon(32,32,Icon.getPixelsPtr());

	_mainWindow.setFramerateLimit(FPS);
	_mainWindow.setVerticalSyncEnabled(true);

	// Setze den Spielstatus auf Intro -> Intro wird angezeigt
	_gameState = Game::ShowingIntro;
	
	// Solange das Spiel nicht beendet wird, f�hre GameLoop aus
	while(!IsExiting()){
		GameLoop(currentSavegame, newgame);
	}

	// Wenn der GameLoop beendet wurde, schlie�e das Fenster
	_mainWindow.close();
	//system("pause");
}

bool Game::IsExiting()
{
	// Wenn der Spielstatus auf Beenden gesetzt wird dann gebe ein True zur�ck, ansonsten ein False
	if(_gameState == Game::Exiting){
		return true;
	}else{ 
		return false;
	}
	// M�glichkeit das Spiel zu Speichern
}

void Game::GameLoop(Savegame& currentSavegame, bool newgame)
{
	sf::View viewCamera  = _mainWindow.getView();
	char gender;

	switch(_gameState){
		case Game::ShowingIntro:
			#ifdef DEBUGINFO
				std::cout << "Show the Intro" << std::endl;
			#endif
			ShowIntro();
		break;
		case Game::ShowingMenu:
			#ifdef DEBUGINFO
				std::cout << "Show the Menu" << std::endl;
			#endif
			ShowMenu(newgame);
            break;
        case Game::ShowingGenderMenu:
			#ifdef DEBUGINFO
				std::cout << "Show the Gender Menu" << std::endl;
			#endif			
            gender = ShowMenuGender();
			defaultSavegame(currentSavegame,gender);
		break;
		case Game::Playing:
			// Hier wird die Map geladen
			#ifdef DEBUGINFO
				std::cout << "Show the Map / Level" << std::endl;
			#endif
			ShowMap(viewCamera,currentSavegame);				
            break;
		case Game::NewGame:
			ShowMap(viewCamera,currentSavegame);
			break;
        default:
            break;
	}
}
void Game::ShowMap(sf::View viewCamera, Savegame& currentSavegame){
	Map map;
	map.Show(_mainWindow, currentSavegame.mLevelId, viewCamera, currentSavegame);
	#ifdef DEBUGINFO
		std::cout << "_gameState = Exiting!" << std::endl;
	#endif
	_gameState = Exiting;
}

void Game::ShowIntro(){
	Intro intro;
	_gameState = Game::ShowingMenu;
	intro.Show(_mainWindow);
}


void Game::ShowMenu(bool newgame){
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow, newgame);
	switch(result)
	{
		case MainMenu::Exit:
			#ifdef DEBUGINFO
				std::cout << "Menu -> Exit Button pressed" << std::endl;
			#endif
				_gameState = Exiting;
			break;
		case MainMenu::Continue:
			#ifdef DEBUGINFO
				std::cout << "Menu -> Continue Button pressed " << std::endl;	
			#endif
				_gameState = Playing;
			break;
		/*case MainMenu::NewGame:
			#ifdef DEBUGINFO
				std::cout << "Menu -> NewGame Button pressed " << std::endl;	
			#endif
			_gameState = NewGame;
			break;*/
        case MainMenu::NewGameGender:
            _gameState = ShowingGenderMenu;
        default:
            break;
	}
}

const char Game::ShowMenuGender(){
    MainMenu genderMenu;
    MainMenu::MenuResult result = genderMenu.Show(_mainWindow, false, true);
    switch (result) {
        case MainMenu::Female:
            _gameState = NewGame;
			return 'F';									
									// @f: weis grad nicht wie ich ab hier weiter machen soll...
            break;                  //  vlt f�llt dir was ein. ich w�sst auch grad nicht wie ich
        case MainMenu::Male:        // aufs savegame zugreifen kann. evtl das auch static machen?
            _gameState = NewGame;   // meh...
			return 'M';
			break;
        default:
			return 'M';
            break;
    }
}

// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;