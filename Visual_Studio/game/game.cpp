/**
	Verantwortlich: Filip Menke
	Infotext: Dies ist das Hauptspiel. Ab hier wird alles in die einzelnen Spielbereiche aufgeteilt.

	Die Game::Init wird aus der main.cpp aufgerufen.
	
	Eine Detalierte Beschreibung erfolgt in den jeweiligen Methoden.
	
	Die Variable _gameState speichert immer den jeweiligen Zustand des Spiels.
	
*/
#include "game.h"

void Game::Init(void)
{	
	/**
	Es erfolgt eine Unterscheidung ob ein Spielstand vorhanden ist oder nicht.
	Wenn kein Spielstand vorhanden ist oder der alte Spielstand manipuliert ist wird die Variable entsprechend gesetzt. Dies ist wichtig um 
	im Menü den Punkt "NewGame" anwählbar zu machen oder nicht.

	Jetzt wird Game::Start aufgerufen.
	*/
    // Random funktion wird im Spiel öfters verwendet
	srand((unsigned)time(NULL));
	// Do it!
	Savegame::currentSaveGame = new Savegame;
	ConfigFile::currentConfigFile = new ConfigFile;

	// Lade die Einstellungen des Spiels.. 
	ConfigFile::currentConfigFile->loadConfigFile();

	Game::continueGame = false;
	
	if(Savegame::currentSaveGame->loadSavegame(true)){
		Game::Start();
	}else{
		Game::isNewGame = true;
		Game::Start();
	}
}

void Game::Start()
{
	/**
		Hier wird das eigentliche Fenster erzeugt und dargestellt. Dabei wird die Bildschirmauflösung aus der Konfigurationsdatei geladen.
		Der _gameState wird auf ShowingIntro gesetzt. Damit wird das Intro angezeigt. Nachdem das Intro beendet wurde startet
		der GameLoop(). Der GameLoop läuft solange bis das Spiel beendet wird.
	*/
	/// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	
	/// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	sf::VideoMode bpp = sf::VideoMode::getDesktopMode();
	
	if(ConfigFile::currentConfigFile->winmode == "window")
		_mainWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height, bpp.bitsPerPixel), WINDOWTITLE, sf::Style::Close);
	else{
		_mainWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height, bpp.bitsPerPixel), WINDOWTITLE, sf::Style::Fullscreen);
		// Deaktiviert den Mauszeiger im Fenster - Klicken geht weiterhin..
		_mainWindow.setMouseCursorVisible(false);
	}
	
	/// Lade und setze das Fenstericon
	sf::Image Icon;
	if(!Icon.loadFromFile(PATH"include/interface/icon.bmp"))
		std::cout << "Failed to load the Windowicon" << std::endl;
	else
		_mainWindow.setIcon(32,32,Icon.getPixelsPtr());

	_mainWindow.setFramerateLimit(FPS);
	_mainWindow.setVerticalSyncEnabled(true);

	/// Setze den Spielstatus auf Intro -> Intro wird angezeigt
	_gameState = Game::ShowingIntro;
	
	/// Solange das Spiel nicht beendet wird, führe GameLoop aus
	while(!IsExiting()){
		GameLoop();
	}

	// Wenn der GameLoop beendet wurde, schließe das Fenster
	_mainWindow.close();
	//system("pause");
}

bool Game::IsExiting()
{
	/// Wenn der Spielstatus auf Beenden gesetzt wird dann gebe ein True zurück, ansonsten ein False
	if(_gameState == Game::Exiting){

		delete Savegame::currentSaveGame;
		delete ConfigFile::currentConfigFile;
		
		return true;
	}else{ 
		return false;
	}
}

void Game::GameLoop(){
	sf::View viewCamera = _mainWindow.getView();
	char gender;
	std::cout << "SOUND: " << ConfigFile::currentConfigFile->sound << std::endl;
	if( gameMusic::music.getStatus() != sf::Music::Playing && ConfigFile::currentConfigFile->sound == true && _gameState != Game::ShowingIntro){
		if(!gameMusic::music.openFromFile(PATH"include/sound/menu.ogg")) 
			throw "sound not loaded";
		gameMusic::music.play();
		gameMusic::music.setLoop(true);
	}
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
			ShowMenu();
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
        case Game::Options:
            #ifdef DEBUGINFO
                std::cout << "Show the Options Menu" << std::endl;
            #endif
            ShowMenuOptions();
            break;
		case Game::Playing:
			// Hier wird die Map geladen
			#ifdef DEBUGINFO
				std::cout << "Show the Map / Level" << std::endl;
			#endif
			ShowMap(viewCamera);
            break;
		case Game::Continue:
			ShowMap(viewCamera);
            break;
		case Game::NewGame:
			ShowMap(viewCamera);
			break;
		case Game::Paused:
			Game::continueGame = true;
			GamePaused(viewCamera);
			break;
        default:
            break;
	}
}

void Game::GamePaused(sf::View viewCamera){
	Pause PauseMenu;
	gameMusic::music.pause();
	bool quitGame = PauseMenu.Show(_mainWindow, viewCamera);
	if(quitGame)
		_gameState = Exiting;
	else{
		if(ConfigFile::currentConfigFile->sound == true)
			gameMusic::music.play();

		_gameState = Continue;
	}
	// hier kann ich irgendwann zurück ins men¸!
}

void Game::ShowMap(sf::View viewCamera){
	static Map map;
	Map::currentMap = &map;

	std::cout << "continue " << Game::continueGame << std::endl;

	if(!Game::continueGame){	// wenn das spiel nach dem sterben fortgesetzt wird (mit altem spielstand) muss die map nicht erneut initalisiert werden, da sie es schon ist
		map.init(Savegame::currentSaveGame->mLevelId);
        map.getPlayer()->Init();    // der player aber schon :D
        map.initInterface();
    }else{
		Game::continueGame = false;
	}

	MapEvent newGameState = map.Show(_mainWindow, Savegame::currentSaveGame->mLevelId, viewCamera);
	
	if(newGameState.theReason == MapEvent::pause)
		_gameState = Paused;
	else if(newGameState.theReason == MapEvent::exiting)
		_gameState = Exiting;
	else if(newGameState.theReason == MapEvent::dead){
		Savegame::currentSaveGame->loadSavegame();
		_gameState = ShowingMenu;
	}else if(newGameState.theReason == MapEvent::mapchange){
		Map::currentMap->getPlayer()->setLevelId(newGameState.newMapId);
		Map::currentMap->getPlayer()->setPosition(newGameState.newMapPosX,newGameState.newMapPosY);
		Savegame::currentSaveGame->saveSavegame();

		std::cout << "   newGameState - X:" << newGameState.newMapPosX << " Y:" << newGameState.newMapPosY << std::endl;
		map.destory();	// delete alte map bevor die neue geladen wird
	}
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
            break;
        case MainMenu::Options:
            #ifdef DEBUGINFO
                std::cout << "Menu -> Options Button pressed " << std::endl;
            #endif
            _gameState = Options;
        default:
            break;  // was soll passieren wenn man daneben klickt? - RICHTIG! NICHTS VERDAMMT!
	}
}

const char Game::ShowMenuGender(){
    MainMenu genderMenu;
    MainMenu::MenuResult result = genderMenu.Show(_mainWindow, 'G');
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

void Game::ShowMenuOptions(){
    MainMenu optionsMenu;
    MainMenu::MenuResult result = optionsMenu.Show(_mainWindow, 'O');
    switch (result) {
        case MainMenu::Menue:
            _gameState = ShowingMenu;
            break;
        case MainMenu::Sound:
            if(ConfigFile::currentConfigFile->sound == false){
                ConfigFile::currentConfigFile->sound = 1;
				gameMusic::music.play();
            }
            else {
                ConfigFile::currentConfigFile->sound = 0;
				gameMusic::music.stop();
            }
            ConfigFile::currentConfigFile->saveConfigFile();
            break;
        case MainMenu::Fullscreen:
            if(ConfigFile::currentConfigFile->winmode == "window"){
                _mainWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height), WINDOWTITLE, sf::Style::Fullscreen);
                //window.setMouseCursorVisible(false);
                ConfigFile::currentConfigFile->winmode = "fullscreen";
                ConfigFile::currentConfigFile->saveConfigFile();
            }else{
                _mainWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height), WINDOWTITLE, sf::Style::Close);
                //window.setMouseCursorVisible(true);
                ConfigFile::currentConfigFile->winmode = "window";
                ConfigFile::currentConfigFile->saveConfigFile();
            }
            break;
        case MainMenu::DefaultSettings:
            if((ConfigFile::currentConfigFile->width != DEFAULT_WIDTH)||
               (ConfigFile::currentConfigFile->height != DEFAULT_HEIGHT)||
               (ConfigFile::currentConfigFile->winmode != DEFAULT_WINMODE)){
                _mainWindow.create(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), WINDOWTITLE, sf::Style::Close);
            }
            ConfigFile::currentConfigFile->saveConfigFile(true);
            break;
		default:
            break;
    }
}

// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
bool Game::isNewGame;
bool Game::continueGame;