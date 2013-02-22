#include "game.h"

void Game::Init(void)
{
	// Do it!
	Savegame::currentSaveGame = new Savegame;
	ConfigFile::currentConfigFile = new ConfigFile;

	// Lade die Einstellungen des Spiels.. 
	ConfigFile::currentConfigFile->loadConfigFile();
	
	// Prüfung fehtl noch ob der ORdner schon vorhanden ist @fil
	if(!system("mkdir screenshots"))
		throw "Failed to create the screenshot folder!";

	if(Savegame::currentSaveGame->loadSavegame(true)){
		Game::Start();
	}else{
		Game::Start(true);
	}
}

void Game::Start(bool newgame)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	sf::VideoMode bpp = sf::VideoMode::getDesktopMode();
	
	if(ConfigFile::currentConfigFile->winmode == "window")
		_mainWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height, bpp.bitsPerPixel), WINDOWTITLE, sf::Style::Close);
	else{
		_mainWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height, bpp.bitsPerPixel), WINDOWTITLE, sf::Style::Fullscreen);
		// Deaktiviert den Mauszeiger im Fenster - Klicken geht weiterhin..
		_mainWindow.setMouseCursorVisible(false);
	}
	
	

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
	
	// Solange das Spiel nicht beendet wird, führe GameLoop aus
	while(!IsExiting()){
		GameLoop(newgame);
	}

	// Wenn der GameLoop beendet wurde, schließe das Fenster
	_mainWindow.close();
	//system("pause");
}

bool Game::IsExiting()
{
	// Wenn der Spielstatus auf Beenden gesetzt wird dann gebe ein True zurück, ansonsten ein False
	if(_gameState == Game::Exiting){
		
		delete Savegame::currentSaveGame;
		delete ConfigFile::currentConfigFile;
		
		return true;
	}else{ 
		return false;
	}
	// Möglichkeit das Spiel zu Speichern
}

void Game::GameLoop(bool newgame)
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
			if(gender == 'M' || gender == 'F'){	// somit wird kein neuer spielstand erzeugt, wenn man den zurück button im gender menü drückt!
				Savegame::currentSaveGame->pGender = gender;	// speichere explizit hier das geschlecht, da die restlichen werte aus der defines geladen werden
				Savegame::currentSaveGame->saveSavegame(true); // true -> erzeuge einen neuen spielstand
			}
		break;
		case Game::Playing:
			// Hier wird die Map geladen
			#ifdef DEBUGINFO
				std::cout << "Show the Map / Level" << std::endl;
			#endif
			ShowMap(viewCamera);				
            break;
		case Game::Continue:
			ShowMap(viewCamera,true);
			break;
		case Game::NewGame:
			ShowMap(viewCamera);
			break;
		case Game::Paused:
			GamePaused(viewCamera);
			break;
        default:
            break;
	}
}
void Game::GamePaused(sf::View viewCamera){
	Pause PauseMenu;
	bool quitGame = PauseMenu.Show(_mainWindow, viewCamera);
	if(quitGame)
		_gameState = Exiting;
	else
		_gameState = Continue;

	// hier kann ich irgendwann zurück ins menü!
}

void Game::ShowMap(sf::View viewCamera, bool continueGame){
	static Map map;
	Map::currentMap = &map;

	if(!continueGame)
		map.init(Savegame::currentSaveGame->mLevelId);

	MapEvent newGameState = map.Show(_mainWindow, Savegame::currentSaveGame->mLevelId, viewCamera);

	
	if(newGameState.theReason == MapEvent::pause){
		_gameState = Paused;
		
	}
	else if(newGameState.theReason == MapEvent::exiting)
		_gameState = Exiting;
	else if(newGameState.theReason == MapEvent::dead){
		Savegame::currentSaveGame->loadSavegame();
		_gameState = ShowingMenu;
	}else if(newGameState.theReason == MapEvent::mapchange){
		Map::currentMap->getPlayer()->setLevelId(newGameState.newMapId);
		Map::currentMap->getPlayer()->setPosition(newGameState.newMapPosX,newGameState.newMapPosY);
		Savegame::currentSaveGame->saveSavegame();
		
		map.destory();	// delete alte map bevor die neue geladen wird
		
	}
	
	
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
        case MainMenu::NewGameGender:
			#ifdef DEBUGINFO
				std::cout << "Menu -> New Game Button pressed " << std::endl;	
			#endif
            _gameState = ShowingGenderMenu;
        default:
            break;  // was soll passieren wenn man daneben klickt? - RICHTIG! NICHTS VERDAMMT!
	}
}

const char Game::ShowMenuGender(){
    MainMenu genderMenu;
    MainMenu::MenuResult result = genderMenu.Show(_mainWindow, false, true);
    switch (result) {
        case MainMenu::Female:
            _gameState = NewGame;
			return 'F';									
            break;                 
        case MainMenu::Male:        
            _gameState = NewGame;   
			return 'M';
			break;
        case MainMenu::Menue:
            _gameState = ShowingMenu;
			return 'X';			// Gebe X Zurück damit nicht gespeichert wird, wenn man nicht direkt möchte (new game)
            break;
		default:
            break; 
    }
    return 'M';
}

// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;