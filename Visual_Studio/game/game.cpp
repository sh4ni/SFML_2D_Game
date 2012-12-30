#include "include.h"

void Game::Start(void)
{
	// Wenn der Spielstatus uninitalisiert, verlasse die Methode
	if(_gameState != Uninitialized) return;
	// Erzeuge ein neues Fenster mit den in der defines.h hinterlegten Werten
	_mainWindow.create(VideoMode(WIDTH, HEIGHT), VERSION);


	// Hier folgt der Rest


	// Setze den Spielstatus auf Playing
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
	Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	_mainWindow.setFramerateLimit(FPS);

	switch(_gameState){
		case Game::ShowingIntro:{
			ShowIntro();
		break;
		}
		case Game::ShowingMenu:
		{
			ShowMenu();
		break;
		}
		case Game::Playing:{
			_mainWindow.clear(Color(255,0,255));
			_mainWindow.display();
			
			if(currentEvent.type == Event::Closed) _gameState = Game::Exiting;
			if(currentEvent.type == Event::KeyPressed)
			{
				if(currentEvent.key.code == Keyboard::Escape) _gameState = Game::ShowingMenu;
			}
		break;
		}
	}
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
Game::GameState Game::_gameState = Uninitialized;
RenderWindow Game::_mainWindow;