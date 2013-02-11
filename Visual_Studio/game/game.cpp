#include "game.h"

void Game::Init(void)
{
	// Do it!
	Savegame mySavegame;

	std::ifstream loadgame;
	loadgame.open(SAVEGAME, std::ios::binary);
	if(loadgame.is_open()){
		std::cout << "Spielstand erkannt! Wird geladen..\n";
		
		loadgame >> mySavegame.pHealth;
		loadgame >> mySavegame.pLvl;
		loadgame >> mySavegame.pExp;
		loadgame >> mySavegame.pGender;

		loadgame >> mySavegame.mLevelId;
		loadgame >> mySavegame.mPosX;
		loadgame >> mySavegame.mPosY;
		
		loadgame >> mySavegame.checksum;

		if(mySavegame.checksum == ((mySavegame.pHealth - mySavegame.pLvl + mySavegame.pExp + mySavegame.pGender + mySavegame.mLevelId) + CHECKSUM))
			std::cout << "Spielstand okay...!\n";
		else{
			std::cout << "Spielstand korrupt..!\a\n";
			mySavegame.pHealth = DEFAULT_HEALTH;
			mySavegame.pLvl = DEFAULT_LVL;
			mySavegame.pExp = DEFAULT_EXP;
			mySavegame.pGender = 'M';

			mySavegame.mLevelId = DEFAULT_LEVEL;
			mySavegame.mPosX = DEFAULT_POSX;
			mySavegame.mPosY = DEFAULT_POSY;
		}

		#ifdef DEBUG
			std::cout << "Spielstand Erfolgreich geladen." << std::endl;
		#endif
		loadgame.close();

		Game::Start(mySavegame);	// Da ein Spielstand vorhanden ist Defaultmäßig der zweite Paramter false
	}else{
		std::cout << "Kein Spielstand erkannt! Default Spielstand wird erstellt..\a\n";
		std::ofstream defaultsavegame;
		defaultsavegame.open(SAVEGAME, std::ios::binary);
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
			// Gender
			srand((unsigned int)time(NULL));
			int tmp = (rand() % 2);
			char gender;
			if(tmp)
				gender = 'M';
			else
				gender = 'F';

			defaultsavegame << gender << std::endl;
			mySavegame.pGender = gender;
		
			// Name

			// Map
			defaultsavegame << DEFAULT_LEVEL << std::endl;
			mySavegame.mLevelId = DEFAULT_LEVEL;
			// Pos X auf Map
			defaultsavegame << DEFAULT_POSX << std::endl;
			mySavegame.mPosX = DEFAULT_POSX;
			// Pos Y auf Map
			defaultsavegame << DEFAULT_POSY << std::endl;
			mySavegame.mPosY = DEFAULT_POSY;	

			int checksum = (mySavegame.pHealth - mySavegame.pLvl + mySavegame.pExp + mySavegame.pGender + mySavegame.mLevelId) + CHECKSUM;
			std::cout << checksum << std::endl;

			defaultsavegame << checksum;
			mySavegame.checksum;

			defaultsavegame.close();

			Game::Start(mySavegame, true);
		}
	}
	Game::Start(mySavegame, true);
}

void Game::Start(Savegame& currentSavegame, bool newgame)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	_mainWindow.create(sf::VideoMode(WIDTH, HEIGHT), VERSION, sf::Style::Titlebar);

	// Deaktiviert den Mauszeiger im Fenster - Klicken geht weiterhin..
	//_mainWindow.setMouseCursorVisible(false);

	// Lade und setze das Fenstericon
	sf::Image Icon;
	if(!Icon.loadFromFile("include/interface/icon.bmp"))
		std::cout << "Fehler beim Laden des Icons!" << std::endl;
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
				std::cout << "Intro" << std::endl;
			#endif
			ShowIntro();
		break;
		}
		case Game::ShowingMenu:{
			#ifdef DEBUG
				std::cout << "Menu" << std::endl;
			#endif
			ShowMenu(newgame);
		break;
		}
		case Game::Playing:{
			// Hier wird die Map geladen
			#ifdef DEBUG
				std::cout << "Spiel - Map" << std::endl;
			#endif
			ShowMap(viewCamera,currentSavegame);				// 1 = Level 1 -> Hauptkarte / Oberwelt
		break;
		}
	}
}
void Game::ShowMap(sf::View viewCamera, Savegame& currentSavegame){
	Map map;
	map.Show(_mainWindow, currentSavegame.mLevelId, viewCamera, currentSavegame);
	#ifdef DEBUG
		std::cout << "Es wurde END gedrückt -> _gameState = Exiting!" << std::endl;
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
				std::cout << "Menu -> Exit Button gedrueckt" << std::endl;
				//getchar();
				//_gameState = ShowingMenu;
			#endif
				_gameState = Exiting;
			break;
		case MainMenu::Play:
			#ifdef DEBUG
				std::cout << "Menu -> Play Button gedrueckt " << std::endl;	
				//getchar();
				//_gameState = ShowingMenu;		
			#endif
				_gameState = Playing;	
			break;
		case MainMenu::Continue:
			// code
			//_gameState = Options;
			break;
	}
}

// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;