#include "include.h"

void Game::Start(void)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	_mainWindow.create(VideoMode(WIDTH, HEIGHT), VERSION);
	//_mainWindow.setIcon(32,32,*windowIcon);
	
	// Hier folgt der Rest

	// Setze den Spielstatus auf Playing
	_gameState = Game::ShowingIntro;
	//_gameState = Game::Playing;
	// Solange das Spiel nicht beendet wird, f�hre GameLoop aus
	while(!IsExiting()){
		GameLoop();
	}
	// Wenn der GameLoop beendet wurde, schlie�e das Fenster
	_mainWindow.close();
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

void Game::GameLoop()
{
	View view  = _mainWindow.getView();
	
	Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	_mainWindow.setFramerateLimit(FPS);

	switch(_gameState){
		case Game::ShowingIntro:{
			std::cout << "Intro" << std::endl;
			ShowIntro();
		break;
		}
		case Game::ShowingMenu:{
			std::cout << "Menu" << std::endl;
			ShowMenu();
		break;
		}
		case Game::Playing:{
			// Hier wird die Map geladen
			std::cout << "Spiel - Map" << std::endl;
			ShowMap(1,view);				// 1 = Level 1 -> Hauptkarte / Oberwelt
		break;
		}
	}
}
void Game::ShowMap(int LevelId, View view){
	Map map;
	map.Show(_mainWindow, LevelId, view);
	_gameState = Game::ShowingMenu;
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