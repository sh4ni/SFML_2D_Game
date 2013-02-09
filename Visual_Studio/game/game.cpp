//#include "include.h"
#include "game.h"


void Game::Init(void)
{
	// Do it!
	Savegame mySavegame;

	std::ifstream loadgame;
	loadgame.open("save.txt", std::ios::binary);
	if(loadgame.is_open()){
		std::cout << "Spielstand erkannt! Wird geladen..\n";
		
		loadgame >> mySavegame.pHealth;
		loadgame >> mySavegame.pLvl;
		loadgame >> mySavegame.pExp;

		loadgame >> mySavegame.mLevelId;
		loadgame >> mySavegame.mPosX;
		loadgame >> mySavegame.mPosY;
		
		#ifdef DEBUG
			std::cout << "Spielstand Erfolgreich geladen." << std::endl;
		#endif

		loadgame.close();
	}else{
		std::cout << "Kein Spielstand erkannt! Default Spielstand wird erstellt..\a\n";
		std::ofstream defaultsavegame;
		defaultsavegame.open("save.txt", std::ios::binary);
		if(defaultsavegame.is_open()){
			// Health
			defaultsavegame << 100 << std::endl;
			mySavegame.pHealth = 100;
			// Level
			defaultsavegame << 1 << std::endl;
			mySavegame.pLvl = 1;
			// Exp
			defaultsavegame << 1 << std::endl;
			mySavegame.pExp = 1;
			// Gender
			// Name

			// Map
			defaultsavegame << 2 << std::endl;
			mySavegame.mLevelId = 2;
			// Pos X auf Map
			defaultsavegame << WIDTH/2 << std::endl;
			mySavegame.mPosX = WIDTH/2;
			// Pos Y auf Map
			defaultsavegame << WIDTH/2 ;
			mySavegame.mPosY = HEIGHT/2;	

		}
	}
	Game::Start(mySavegame);
	
}

void Game::Start(Savegame& currentSavegame)
{
	std::cout << currentSavegame.mPosY << std::endl;
	system("pause");
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	_mainWindow.create(sf::VideoMode(WIDTH, HEIGHT), VERSION, sf::Style::Titlebar);

	// Lade und setze das Fenstericon
	sf::Image Icon;
	if(!Icon.loadFromFile("include/interface/icon.bmp"))
		std::cout << "Fehler beim Laden des Icons!" << std::endl;
	else
		_mainWindow.setIcon(32,32,Icon.getPixelsPtr());

	// Setze den Spielstatus auf Intro -> Intro wird angezeigt
	_gameState = Game::ShowingIntro;
	//_gameState = Game::Playing;
	// Solange das Spiel nicht beendet wird, führe GameLoop aus
	while(!IsExiting()){
		GameLoop(currentSavegame);
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

void Game::GameLoop(Savegame& currentSavegame)
{
	sf::View viewCamera  = _mainWindow.getView();
	
	_mainWindow.setFramerateLimit(FPS);
	_mainWindow.setVerticalSyncEnabled(true);

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
			ShowMenu();
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


void Game::ShowMenu(){
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow);
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
		case MainMenu::Options:
			// code
			//_gameState = Options;
			break;
	}
}

// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;