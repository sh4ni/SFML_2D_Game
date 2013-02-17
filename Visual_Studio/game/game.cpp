#include "game.h"

void Game::Init(void)
{
	// Do it!
	Savegame mySavegame;
	ConfigFile myConfigFile;
	
	// Lade die Einstellungen des Spiels.. 
	myConfigFile.loadConfigFile(myConfigFile);

	// Prüfung fehtl noch ob der ORdner schon vorhanden ist @fil
	if(!system("mkdir screenshots"))
		throw "Failed to create the screenshot folder!";

	if(mySavegame.loadSavegame(mySavegame)){
		Game::Start(mySavegame,myConfigFile);
	}else{
		Game::Start(mySavegame,myConfigFile,true);
	}
}

void Game::Start(Savegame& currentSavegame, ConfigFile& currentConfigFile, bool newgame)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	sf::VideoMode bpp = sf::VideoMode::getDesktopMode();
	_mainWindow.create(sf::VideoMode(currentConfigFile.width, currentConfigFile.height, bpp.bitsPerPixel), WINDOWTITLE, sf::Style::Close);

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
	
	// Solange das Spiel nicht beendet wird, führe GameLoop aus
	while(!IsExiting()){
		GameLoop(currentSavegame, currentConfigFile, newgame);
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

void Game::GameLoop(Savegame& currentSavegame, ConfigFile& currentConfigFile, bool newgame)
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
			ShowMenu(currentConfigFile, newgame);
            break;
        case Game::ShowingGenderMenu:
			#ifdef DEBUGINFO
				std::cout << "Show the Gender Menu" << std::endl;
			#endif			
            gender = ShowMenuGender(currentConfigFile);
			std::cout << gender ;
			if(gender == 'M' || gender == 'F')	// somit wird kein neuer spielstand erzeugt, wenn man den zurück button im gender menü drückt!
				currentSavegame.saveSavegame(currentSavegame,gender,true);
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
	intro.Show(_mainWindow);
	_gameState = Game::ShowingMenu;
}


void Game::ShowMenu(ConfigFile& currentConfigFile, bool newgame){
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow, currentConfigFile, newgame);
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
            _gameState = ShowingGenderMenu;
        default:
            break;  // was soll passieren wenn man daneben klickt? - RICHTIG! NICHTS VERDAMMT!
	}
}

const char Game::ShowMenuGender(ConfigFile& currentConfigFile){
    MainMenu genderMenu;
    MainMenu::MenuResult result = genderMenu.Show(_mainWindow, currentConfigFile, false, true);
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
