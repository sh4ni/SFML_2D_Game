#include "include.h"

void Game::Start(void)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten

	_mainWindow.create(VideoMode(WIDTH, HEIGHT), VERSION, Style::Titlebar);

	//_mainWindow.setIcon(32,32,*windowIcon);

	// Setze den Spielstatus auf Intro -> Intro wird angezeigt
	_gameState = Game::ShowingIntro;
	//_gameState = Game::Playing;
	// Solange das Spiel nicht beendet wird, führe GameLoop aus
	while(!IsExiting()){
		GameLoop();
	}
	
	// Wenn der GameLoop beendet wurde, schließe das Fenster
	_mainWindow.close();
	system("pause");
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

void Game::GameLoop()
{
	View viewCamera  = _mainWindow.getView();
	
	_mainWindow.setFramerateLimit(FPS);
	_mainWindow.setVerticalSyncEnabled(true);

	switch(_gameState){
		case Game::ShowingIntro:{
			#if DEBUG==1
				std::cout << "Intro" << std::endl;
			#endif
			ShowIntro();
		break;
		}
		case Game::ShowingMenu:{
			#if DEBUG == 1
				std::cout << "Menu" << std::endl;
			#endif
			ShowMenu();
		break;
		}
		case Game::Playing:{
			// Hier wird die Map geladen
			#if DEBUG == 1
				std::cout << "Spiel - Map" << std::endl;
			#endif
			ShowMap(1,viewCamera);				// 1 = Level 1 -> Hauptkarte / Oberwelt
		break;
		}
	}
}
void Game::ShowMap(int LevelId, View viewCamera){
	Map map;
	map.Show(_mainWindow, LevelId, viewCamera);
	#if DEBUG == 1
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
			#if DEBUG == 1
				std::cout << "Menu -> Exit Button gedrueckt" << std::endl;
				//getchar();
				//_gameState = ShowingMenu;
			//#else
				_gameState = Game::Exiting;
			#endif
			break;
		case MainMenu::Play:
			#if DEBUG == 1
				std::cout << "Menu -> Play Button gedrueckt " << std::endl;	
				//getchar();
				//_gameState = ShowingMenu;
			//#else
				_gameState = Playing;	
			#endif
			break;
		case MainMenu::Options:
			// code
			//_gameState = Options;
			break;
	}
}


// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
RenderWindow Game::_mainWindow;