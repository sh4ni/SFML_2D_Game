#include "include.h"

void Game::Start(void)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	_mainWindow.create(VideoMode(WIDTH, HEIGHT), VERSION,sf::Style::Titlebar);
	
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
	
	//Event currentEvent;
	//_mainWindow.pollEvent(currentEvent);
	_mainWindow.setFramerateLimit(FPS);
	_mainWindow.setVerticalSyncEnabled(true);

	switch(_gameState){
		case Game::ShowingIntro:{
			#if DEBUG 1
				std::cout << "Intro" << std::endl;
			#endif
			ShowIntro();
		break;
		}
		case Game::ShowingMenu:{
			#if DEBUG 1
				std::cout << "Menu" << std::endl;
			#endif
			ShowMenu();
		break;
		}
		case Game::Playing:{
			// Hier wird die Map geladen
			#if DEBUG 1
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
	_gameState = Game::Exiting;
}

void Game::ShowIntro(){
	Intro intro;
	intro.Show(_mainWindow);
	_gameState = Game::ShowingMenu;
}

void Game::ShowMenu(){
	Menu menu;
	menu.Show(_mainWindow);
	_gameState = Game::Playing;
}


// static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
RenderWindow Game::_mainWindow;