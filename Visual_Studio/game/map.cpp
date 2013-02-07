#include "include.h"
#include <vector>

/*IntRect Map::getRect(int x, int y){
	x /= TILESIZE;
	y /= TILESIZE;
	return CollisionMap[x][y];
}*/

void Map::Show(RenderWindow& renderWindow, int LevelId, View viewCamera){

	#ifdef DEBUG
		std::cout << "Lade Map Nr: " << LevelId << "." << std::endl;
	#endif
	// Hier wird die Textur für die Map geladen.
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
		for ( int i = 0 ; i < MapSizeX ; i++ ){		// Es ist nicht gewährleistet ob der Speicher an einem Stück hintereinander ist.
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
			if(TileType >10  ){
				CollisionMap[LoadCounterX][LoadCounterY]=new IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);
			}else{
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;
			}
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
	
	#ifdef DEBUG
		Player P1("include/texture/player/player_female_debug.png",CollisionMap);
	#else
		Player P1("include/texture/player/player_female.png",CollisionMap);
	#endif

	float LastTime = 1.f;
	float ElapsedTime;
	float Frames;

	float CamX;
	float CamY;

	Schrift DisplayFPS(0,0,"FPS: 0",20);
	Schrift DisplayKoord(0,20,"X: 0 Y: 0",20);
	Schrift DisplaySpeed(0,40,"Speed: 0",20);


	bool paused = false;
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
		CamX = P1.getPosX();
		CamY = P1.getPosY();

		// Wenn ich schonmal die Spieler koordinaten schonmal hab, dann nutz ich sie hier noch schnell für die Koordinatenanzeige
		// und ruf aus performancegründen die getPos methode nicht 2 mal auf.
		std::ostringstream PlayerKoordText;
		PlayerKoordText.precision(1);
		PlayerKoordText << std::fixed << "X: " << CamX << " Y: " << CamY;
		DisplayKoord.Update(PlayerKoordText.str());

		std::ostringstream PlayerSpeedText;
		PlayerSpeedText.precision(1);
		PlayerSpeedText << std::fixed << "Speed: " << P1.getSpeed();
		DisplaySpeed.Update(PlayerSpeedText.str());

		// ...geht aber nicht übers Kartenende hinaus
		if ( CamX < WIDTH/2 ) CamX = WIDTH/2;
		if ( CamY < HEIGHT/2 ) CamY = HEIGHT/2;
		if ( CamX > MapSizeX * TILESIZE - WIDTH/2 ) CamX = float(MapSizeX * TILESIZE - WIDTH/2);
		if ( CamY > MapSizeY * TILESIZE - HEIGHT/2 ) CamY = float(MapSizeY * TILESIZE - HEIGHT/2);

		renderWindow.setView(viewCamera);
		viewCamera.setCenter(CamX,CamY);	// Alles was ab hier gerendert wird, bewegt sich mit der Kamera mit

		// Hier wird die Map gerendert.
		// Orientierung an der Kamera, damit nur sichtbare Sprites neu gezeichnet werden.
		for( int y = ((int)CamY-(HEIGHT/2))/TILESIZE ; y < ((int)CamY+(HEIGHT/2)+TILESIZE-1)/TILESIZE ; y++ ){		// abbruchbedingung könnte evtl noch minimal optimiert werden,
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
					bool test = pause(renderWindow,viewCamera,levelLoop,paused);
					if(test)
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
				pause(renderWindow,viewCamera,levelLoop,paused);
			}else if(levelLoop.type == Event::Closed){
				return;
			}
		}

		renderWindow.setView(renderWindow.getDefaultView());	// Alles was ab hier gerendert wird, wird nicht mit der Kamera mit bewegt! z.b. das Interface
		
		DisplayFPS.Render(renderWindow); // FPS Anzeige
		DisplayKoord.Render(renderWindow); // Spielerkoordinaten Anzeige
		DisplaySpeed.Render(renderWindow); // Geschwindigkeit des Players

		renderWindow.display();
	}
}

bool Map::pause(RenderWindow& renderWindow, View viewCamera, Event levelLoop, bool paused){

	paused = true;
					
	Schrift Pause((int)viewCamera.getCenter().x-75,(int)viewCamera.getCenter().y-25,"Pause",50);
	Pause.Render(renderWindow);
	renderWindow.display();
	
	#ifdef DEBUG
		std::cout << "Pause" << std::endl;	
	#endif
	while(paused){
		renderWindow.pollEvent(levelLoop); // sonst kann die pause nicht verlassen werden!
		if ((levelLoop.type == sf::Event::KeyPressed) && (levelLoop.key.code == sf::Keyboard::Escape)){
			paused = false;
			#ifdef DEBUG
				std::cout << "Continue Playing.." << std::endl;
			#endif
		}else if(levelLoop.key.code == Keyboard::Space){
			#ifdef DEBUG
				std::cout << "Spiel beenden!" << std::endl;
			#endif
			return true; // gebe true zurueck damit das spiel anschließend beendet wird
		}
	}
	return false; // gebe false zurueck damit das spiel nicht beendet wird, sondern weiter geht!
}
