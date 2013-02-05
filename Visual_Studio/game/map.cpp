#include "include.h"

void Map::Show(RenderWindow& renderWindow, int LevelId, View viewCamera){

	#if DEBUG == 1
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

	char FileName[30];
	sprintf(FileName,"include/map/map%d.txt",LevelId);

	// Map Loader. Datei wird eingelesen und es werden dynamisch neue objekte erzeugt.
	std::ifstream openfile(FileName);
	if( openfile.is_open() ){
		openfile >> MapSizeX >> MapSizeY;

		TileMap = new TilePart*[MapSizeX];			// Map Speicher Dynamisch reservieren.
		for ( int i = 0 ; i < MapSizeX ; i++ ){		// Es ist nicht gewährleistet ob der Speicher an einem Stück hintereinander ist.
			TileMap[i] = new TilePart[MapSizeY];
		}

		while( !openfile.eof() ){
			openfile >> TileType;
			IntRect subRect;
			subRect.height=subRect.width=TILESIZE;
			subRect.top=TileType/10*TILESIZE;
			subRect.left=TileType%10*TILESIZE;
			TileMap[LoadCounterX][LoadCounterY].TexturePart = new Sprite(LevelTexture,subRect);
			LoadCounterX++;
			if( LoadCounterX >= MapSizeX ){
				LoadCounterX = 0;
				LoadCounterY++;
			}
		}
		#if DEBUG == 1
			std::cout << "Map erfolgreich eingelesen." << std::endl;
		#endif
		std::ifstream closefile(FileName);
	}
	else {
		//throw("Map konnte nicht geoeffnet werden. Fehler: 01.1"); // 01.X = Texturenfehler allgemein 01.1 genau der hier
	}
	
	Clock clock;
	
	#if DEBUG == 1
		Player P1("include/texture/player/player_debug.png");
	#else
		Player P1("include/texture/player/player.png");
	#endif

	float LastTime = 1.f;
	float ElapsedTime;
	float Frames;
	char text[16];

	Schrift DisplayFPS(0,0,"Fps Anzeige",20);

	bool paused = false;
	while(true)
	{
		ElapsedTime = (float)clock.restart().asMilliseconds();

		Frames = 1.f /( ElapsedTime / 1000 );
		LastTime = ElapsedTime;
		
		sprintf(text,"FPS: %f",Frames);
		DisplayFPS.Update(text);
		// FPSText.SetPosition(window.ConvertCoords(20, 20, defaultCamera));
		
		float CamX = P1.getPosX();
		float CamY = P1.getPosY();

		// Kamera folgt dem Spieler, geht aber nicht übers Kartenende hinaus
		if ( CamX < WIDTH/2 ) CamX = WIDTH/2;
		if ( CamY < HEIGHT/2 ) CamY = HEIGHT/2;
		if ( CamX > MapSizeX * TILESIZE - WIDTH/2 ) CamX = float(MapSizeX * TILESIZE - WIDTH/2);
		if ( CamY > MapSizeY * TILESIZE - HEIGHT/2 ) CamY = float(MapSizeY * TILESIZE - HEIGHT/2);

		// Orientierung an der Kamera, damit nur sichtbare Sprites neu gezeichnet werden.
		for( int y = ((int)CamY-(HEIGHT/2))/TILESIZE ; y < ((int)CamY+(HEIGHT/2)+TILESIZE-1)/TILESIZE ; y++ ){		// abbruchbedingung könnte evtl noch minimal optimiert werden,
			for( int x = ((int)CamX-(WIDTH/2))/TILESIZE ; x < ((int)CamX+(WIDTH/2)+TILESIZE-1)/TILESIZE ; x++ ){	// sollte so aber recht gut funktionieren.
				TileMap[x][y].TexturePart->setPosition( x * TILESIZE, y * TILESIZE );								// sollten aufnahmefehler beim bewegen auftreten, dann stimmt hier was nicht.
				renderWindow.draw(*TileMap[x][y].TexturePart);
			}
		}
		DisplayFPS.Render(renderWindow);
		P1.Render(renderWindow);
		P1.Update(renderWindow, ElapsedTime);
		
		Event levelLoop;
		while(renderWindow.pollEvent(levelLoop)){
			if(levelLoop.type == Event::KeyPressed){
				if(levelLoop.key.code == Keyboard::Escape){
					pause(renderWindow,viewCamera,levelLoop,paused);
				}else if(levelLoop.key.code == Keyboard::End){
					return;
				}else if(levelLoop.key.code == Keyboard::F10) {
					sf::Image Screen = renderWindow.capture();
					if(Screen.saveToFile("screenshots\screenshot-"__DATE__"-.png")){
						#if DEBUG == 1
							std::cout << " Screenshot gespeichert.. " << std::endl;	
						#endif

					}
				}
			}else if(levelLoop.type == Event::LostFocus){
				#if DEBUG == 1
					std::cout << " Ausserhalb Fenster!.. " << std::endl;	
				#endif
				pause(renderWindow,viewCamera,levelLoop,paused);
			}else if(levelLoop.type == Event::Closed){
				return;
			}
			//sf::sleep(sf::microseconds(1000));
		}

		viewCamera.setCenter(CamX,CamY);
		
		renderWindow.setView(viewCamera);
		renderWindow.display();
	}
}

void Map::pause(RenderWindow& renderWindow, View viewCamera, Event levelLoop, bool paused){

	paused = true;
					
	Schrift Pause(viewCamera.getCenter().x-75.0,viewCamera.getCenter().y-25.0,"Pause",50);
	Pause.Render(renderWindow);
	renderWindow.display();
					
	while(paused){
		#if DEBUG == 1
			std::cout << " Pause " << std::endl;	
		#endif
		renderWindow.pollEvent(levelLoop); // sonst kann die pause nicht verlassen werden!
		if ((levelLoop.type == sf::Event::KeyPressed) && (levelLoop.key.code == sf::Keyboard::Escape)){
			paused = false;
			std::cout << paused << " Continue Playing.. " << std::endl;
		}else if(levelLoop.key.code == Keyboard::End){
			return;
		}
	}
}
