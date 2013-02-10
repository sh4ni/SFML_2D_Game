#include "map.h"

/*IntRect Map::getRect(int x, int y){
	x /= TILESIZE;
	y /= TILESIZE;
	return CollisionMap[x][y];
}*/

void Map::Show(sf::RenderWindow& renderWindow, int LevelId, sf::View viewCamera, Savegame& currentSavegame){

	#ifdef DEBUG
		std::cout << "Lade Map Nr: " << LevelId << "." << std::endl;
	#endif
	// Hier wird die Textur f�r die Map geladen.
	static Texture LevelTexture;
	LevelTexture.loadFromFile("include/texture/world/overworld.png");		// Lade Texturedatei

	int MapSizeX = 0;
	int MapSizeY = 0;
	int LoadCounterX = 0;
	int LoadCounterY = 0;
	int TileType;
	TilePart** TileMap;
	IntRect*** CollisionMap;
	
	std::ostringstream FileName;	
	FileName << "include/map/map" << LevelId << ".txt";

	// Map Loader. Datei wird eingelesen und es werden dynamisch neue objekte erzeugt.
	std::ifstream openfile(FileName.str());
	if( openfile.is_open() ){
		openfile >> MapSizeX >> MapSizeY;

		TileMap = new TilePart*[MapSizeX];			// Map Speicher Dynamisch reservieren.
		for ( int i = 0 ; i < MapSizeX ; i++ ){		// Es ist nicht gew�hrleistet ob der Speicher an einem St�ck hintereinander ist.
			TileMap[i] = new TilePart[MapSizeY];
		}

		CollisionMap = new IntRect**[MapSizeX];
		for ( int i = 0 ; i < MapSizeX ; i++ ){
			CollisionMap[i] = new IntRect*[MapSizeY];
		}


		while( !openfile.eof() ){
			openfile >> TileType;
			IntRect subRect;
			subRect.height=subRect.width=TILESIZE;
			subRect.top=TileType/10*TILESIZE;
			subRect.left=TileType%10*TILESIZE;
			
			TileMap[LoadCounterX][LoadCounterY].TexturePart = new Sprite(LevelTexture,subRect);
			switch( TileType ){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 19:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;
					break;
			default:
				CollisionMap[LoadCounterX][LoadCounterY]=new IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);
				break;

			}
			/*if(TileType >10  ){
				CollisionMap[LoadCounterX][LoadCounterY]=new IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);
			}else{
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;
			}*/
			//CollisionMap[LoadCounterX][LoadCounterY] = IntRect(0,0,32,32) ;
			LoadCounterX++;
			if( LoadCounterX >= MapSizeX ){
				LoadCounterX = 0;
				LoadCounterY++;
			}
		}
		#ifdef DEBUG
			std::cout << "Map erfolgreich eingelesen." << std::endl;
		#endif
		std::ifstream closefile(FileName.str());
	}
	else {
		//throw("Map konnte nicht geoeffnet werden. Fehler: 01.1"); // 01.X = Texturenfehler allgemein 01.1 genau der hier
	}
	
	Clock clock;

	Player P1(1,CollisionMap,currentSavegame);
			
	P1.setHealth(currentSavegame.pHealth);
	P1.setExp(currentSavegame.pExp);
	P1.setLvl(currentSavegame.pLvl);
	

	float LastTime = 1.f;
	float ElapsedTime;
	float Frames;

	int CamX;
	int CamY;

	Schrift DisplayFPS(0,0,"FPS: 0",20);
	Schrift DisplayKoord(0,20,"X: 0 Y: 0",20);
	Schrift DisplaySpeed(0,40,"Speed: 0",20);
	Schrift DisplayHealth((WIDTH-115),0,"Health: 0",20);
	Schrift DisplayLvl((WIDTH-115),20,"Lvl: 0",20);

	while(true)
	{
		ElapsedTime = (float)clock.restart().asMilliseconds();

		Frames = 1.f /( ElapsedTime / 1000 );
		LastTime = ElapsedTime;

		// FPS
		std::ostringstream FPSText;
		FPSText.precision(0);
		FPSText << std::fixed << "FPS: " << Frames;
		DisplayFPS.Update(FPSText.str());

		// Kamera folgt dem Spieler...
		CamX = (int)P1.getPosX();
		CamY = (int)P1.getPosY();

		// Wenn ich schonmal die Spieler koordinaten schonmal hab, dann nutz ich sie hier noch schnell f�r die Koordinatenanzeige
		// und ruf aus performancegr�nden die getPos methode nicht 2 mal auf.
		std::ostringstream PlayerKoordText;
		PlayerKoordText.precision(0);
		PlayerKoordText << std::fixed << "X: " << CamX << " Y: " << CamY;
		DisplayKoord.Update(PlayerKoordText.str());

		std::ostringstream PlayerSpeedText;
		PlayerSpeedText.precision(1);
		PlayerSpeedText << std::fixed << "Speed: " << P1.getSpeed();
		DisplaySpeed.Update(PlayerSpeedText.str());

		std::ostringstream PlayerHealthText;
		PlayerHealthText.precision(0);
		PlayerHealthText << std::fixed << "Health: " << P1.getHealth();
		DisplayHealth.Update(PlayerHealthText.str());

		std::ostringstream PlayerLvlText;
		PlayerLvlText.precision(0);
		PlayerLvlText << std::fixed << "Level: " << P1.getLvl();
		DisplayLvl.Update(PlayerLvlText.str());

		// ...geht aber nicht �bers Kartenende hinaus
		if ( CamX < WIDTH/2 ) CamX = WIDTH/2;
		if ( CamY < HEIGHT/2 ) CamY = HEIGHT/2;
		if ( CamX > MapSizeX * TILESIZE - WIDTH/2 ) CamX = MapSizeX * TILESIZE - WIDTH/2;
		if ( CamY > MapSizeY * TILESIZE - HEIGHT/2 ) CamY = MapSizeY * TILESIZE - HEIGHT/2;

		renderWindow.setView(viewCamera);
		viewCamera.setCenter((float)CamX,(float)CamY);	// Alles was ab hier gerendert wird, bewegt sich mit der Kamera mit

		// Hier wird die Map gerendert.
		// Orientierung an der Kamera, damit nur sichtbare Sprites neu gezeichnet werden.
		for( int y = ((int)CamY-(HEIGHT/2))/TILESIZE ; y < ((int)CamY+(HEIGHT/2)+TILESIZE-1)/TILESIZE ; y++ ){		// abbruchbedingung k�nnte evtl noch minimal optimiert werden,
			for( int x = ((int)CamX-(WIDTH/2))/TILESIZE ; x < ((int)CamX+(WIDTH/2)+TILESIZE-1)/TILESIZE ; x++ ){	// sollte so aber recht gut funktionieren.
				TileMap[x][y].TexturePart->setPosition( (float)(x * TILESIZE), (float)(y * TILESIZE) );				// sollten aufnahmefehler beim bewegen auftreten, dann stimmt hier was nicht.
				renderWindow.draw(*TileMap[x][y].TexturePart);
			}
		}

		// Rendern des Spielers
		P1.Render(renderWindow);
		P1.Update(renderWindow, ElapsedTime);
		
		Event levelLoop;
		while(renderWindow.pollEvent(levelLoop)){
			if(levelLoop.type == Event::KeyPressed){
				if(levelLoop.key.code == Keyboard::Escape){
					bool quitGame = pause(renderWindow,viewCamera,levelLoop,P1,LevelId, currentSavegame);
					if(quitGame)
						return;
				}else if(levelLoop.key.code == Keyboard::F10) {
					sf::Image Screen = renderWindow.capture();
					if(Screen.saveToFile("screenshots\\screenshot-"__DATE__"-.png")){
						#ifdef DEBUG
							std::cout << " Screenshot gespeichert.. " << std::endl;	
						#endif
					}
				}else if(levelLoop.key.code == Keyboard::E){
					// player speed up
					P1.increaseSpeed(0.1f);
				}else if(levelLoop.key.code == Keyboard::Q){
					// player speed runter
					P1.decreaseSpeed(0.1f);
				}
			}else if(levelLoop.type == Event::LostFocus){
				#ifdef DEBUG
					std::cout << " Ausserhalb Fenster!.. " << std::endl;	
				#endif
					pause(renderWindow,viewCamera,levelLoop,P1,LevelId, currentSavegame);
			}else if(levelLoop.type == Event::Closed){
				return;
			}
		}

		renderWindow.setView(renderWindow.getDefaultView());	// Alles was ab hier gerendert wird, wird nicht mit der Kamera mit bewegt! z.b. das Interface
		
		DisplayFPS.Render(renderWindow); // FPS Anzeige
		DisplayKoord.Render(renderWindow); // Spielerkoordinaten Anzeige
		DisplaySpeed.Render(renderWindow); // Geschwindigkeit des Players
		DisplayHealth.Render(renderWindow); // Health Player
		DisplayLvl.Render(renderWindow); // Level of Player

		renderWindow.display();
	}
}

bool Map::load(Player& P1)
{
	std::cout << "loading savegame..";
	std::ifstream loadgame;
	loadgame.open(SAVEGAME, std::ios::binary);
	if(loadgame.is_open()){
		int tester;
	
		loadgame >> tester;
		
		std::cout << tester << std::endl;
	
		P1.setHealth(tester);
		P1.setLvl(50);

		loadgame >> tester;

		loadgame.close();
	}else{
		std::cout << "fehler beim �ffnen der datei!" << std::endl;
		return false;
	}

	return true;
}

bool Map::save(Player& P1, int LevelId)
{
	std::cout << "saving..\n";
	std::ofstream savegame;
	savegame.open(SAVEGAME, std::ios::binary);
	if(savegame.is_open()){
		// Health
		savegame << P1.getHealth() << std::endl;
		// Level
		savegame << P1.getLvl() << std::endl;
		// Exp
		savegame << P1.getExp() << std::endl;
		// Gender
		// Name


		// Map
		savegame << LevelId << std::endl;
		// Pos X auf Map
		savegame << P1.getPosX() << std::endl;
		// Pos Y auf Map
		savegame << P1.getPosY() << std::endl;

		savegame.close();
		std::cout << "saved..\n";
	}else{
		std::cout << "FAILURE by saving the game..\a\n";
	}
	
	return true;
}

bool Map::pause(RenderWindow& renderWindow, View viewCamera, Event levelLoop, Player& P1, int LevelId, Savegame& currentSavegame){
	

	//Map::load(P1);

	#ifdef DEBUG
		std::cout << "Pause gestartet.." << std::endl;	
	#endif

	sf::Texture Image;
	Image.loadFromFile("include/interface/pause.png");
	sf::Sprite sprite(Image);
	
	sprite.setScale(1.1f,1.1f);
	sprite.setPosition(viewCamera.getCenter().x-(WIDTH/2),viewCamera.getCenter().y-(HEIGHT/2));
	
	sprite.setColor(sf::Color(0,0,0,100));
	renderWindow.draw(sprite);

	Schrift Pause((int)viewCamera.getCenter().x-75,(int)viewCamera.getCenter().y-25,"Pause",50);
	Pause.Render(renderWindow);
	renderWindow.display();
	
	while(renderWindow.waitEvent(levelLoop)){
		if(levelLoop.type == sf::Event::KeyPressed){
			if (levelLoop.key.code == sf::Keyboard::Escape){
				#ifdef DEBUG
					std::cout << "Continue Playing.." << std::endl;
				#endif
					return false; // gebe false zurueck damit das spiel nicht beendet wird, sondern weiter geht!
			}else if(levelLoop.key.code == Keyboard::Space){
				#ifdef DEBUG
					std::cout << "Spiel beenden!" << std::endl;
				#endif
					return true; // gebe true zurueck damit das spiel anschlie�end beendet wird
			}else if(levelLoop.key.code == Keyboard::F6){
				Map::save(P1, LevelId);
			}else if(levelLoop.key.code == Keyboard::F9){
				Map::load(P1);
			}
		}
	}
	return false; // gebe false zurueck damit das spiel nicht beendet wird, sondern weiter geht!
}
