#include "game.h"

void defaultSavegame(Savegame& mySavegame, bool corrput){
	// wenn der Spielstand korrput ist oder keiner vorhanden wird,
	// wird eine neuer erstellt mit vordefinierten defaultwerte

	#ifdef DEBUG
		if(corrput)
			std::cout << "The savegame is corrupt! Default savegame will be loaded..\a\n";
		else
			std::cout << "No savegame detected! Default savegame will be loaded..\a\n";
	#endif

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

		srand((unsigned int)time(NULL));
		int tmp = (rand() % 2);
		char pGender;
		std::string pName;
		if(tmp){
			pGender = 'M';
			pName = DEFAULT_M_NAME;
		}else{
			pGender = 'F';
			pName = DEFAULT_F_NAME;
		}

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

		#ifdef DEBUG
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
			defaultSavegame(mySavegame,true);

		#ifdef DEBUG
			std::cout << "Savegame successfully loaded.." << std::endl;
		#endif
		loadgame.close();

		Game::Start(mySavegame);	// Da ein Spielstand vorhanden ist Defaultmäßig der zweite Paramter false
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
	if(!Icon.loadFromFile("include/interface/icon.bmp"))
		std::cout << "Failed to load the Windowicon" << std::endl;
	else
		_mainWindow.setIcon(32,32,Icon.getPixelsPtr());

	_mainWindow.setFramerateLimit(FPS);
	_mainWindow.setVerticalSyncEnabled(true);

	// Setze den Spielstatus auf Intro -> Intro wird angezeigt
	_gameState = Game::ShowingIntro;
	
	// Solange das Spiel nicht beendet wird, führe GameLoop aus
	while(!IsExiting()){
		GameLoop(currentSavegame, newgame);
	}

	// Wenn der GameLoop beendet wurde, schließe das Fenster
	_mainWindow.close();
	//system("pause");
}

bool Game::IsExiting()
{
	// Wenn der Spielstatus auf Beenden gesetzt wird dann gebe ein True zurück, ansonsten ein False
	if(_gameState == Game::Exiting){
		return true;
	}else{ 
		return false;
	}
	// Möglichkeit das Spiel zu Speichern
}

void Game::GameLoop(Savegame& currentSavegame, bool newgame)
{
	sf::View viewCamera  = _mainWindow.getView();

	switch(_gameState){
		case Game::ShowingIntro:{
			#ifdef DEBUG
				std::cout << "Show the Intro" << std::endl;
			#endif
			ShowIntro();
		break;
		}
		case Game::ShowingMenu:{
			#ifdef DEBUG
				std::cout << "Show the Menu" << std::endl;
			#endif
			ShowMenu(newgame);
		break;
		}
		case Game::Playing:{
			// Hier wird die Map geladen
			#ifdef DEBUG
				std::cout << "Show the Map / Level" << std::endl;
			#endif
			ShowMap(viewCamera,currentSavegame);				
		break;
		}
		case Game::NewGame:{
			defaultSavegame(currentSavegame,false);
			ShowMap(viewCamera,currentSavegame);
		}
	}
}
void Game::ShowMap(sf::View viewCamera, Savegame& currentSavegame){
	Map map;
	map.Show(_mainWindow, currentSavegame.mLevelId, viewCamera, currentSavegame);
	#ifdef DEBUG
		std::cout << "_gameState = Exiting!" << std::endl;
	#endif
	_gameState = Exiting;
}

void Game::ShowIntro(){
	Intro intro;
	intro.Show(_mainWindow);
	_gameState = Game::ShowingMenu;
}


void Game::ShowMenu(bool newgame){
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow, newgame);
	switch(result)
	{
		case MainMenu::Exit:
			#ifdef DEBUG
				std::cout << "Menu -> Exit Button pressed" << std::endl;
			#endif
				_gameState = Exiting;
			break;
		case MainMenu::Continue:
			#ifdef DEBUG
				std::cout << "Menu -> Continue Button pressed " << std::endl;	
			#endif
				_gameState = Playing;	
			break;
		case MainMenu::NewGame:
			#ifdef DEBUG
				std::cout << "Menu -> NewGame Button pressed " << std::endl;	
			#endif
			_gameState = NewGame;
			break;
	}
}

// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;