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
	TilePart TileMap[100][100];

	std::string MapPath = "include/map/map";
	std::string MapFileType = ".txt";
	char MapNumber[3];	
	sprintf(MapNumber,"%d",LevelId);
	
	std::string FileName = MapPath + MapNumber + MapFileType;

	std::ifstream openfile(FileName);
	if( openfile.is_open() ){
		openfile >> MapSizeX >> MapSizeY;
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
	Schrift DisplayFPS(0,0,"Fps Anzeige",20);

	bool paused = false;
	while(true)
	{
		float ElapsedTime = (float)clock.restart().asMilliseconds();

		float Frames = 1.f /( ElapsedTime / 1000 );
		LastTime = ElapsedTime;
		char text[15];
		//std::cout << "FPS: " << Frames << std::endl;
		sprintf(text,"FPS: %f",Frames);
		DisplayFPS.Update(text);
		// FPSText.SetPosition(window.ConvertCoords(20, 20, defaultCamera));
		
		for( int y = 0 ; y < MapSizeY ; y++ ){
			for( int x = 0 ; x < MapSizeX ; x++ ){
				TileMap[x][y].TexturePart->setPosition( x * TILESIZE, y * TILESIZE );
				renderWindow.draw(*TileMap[x][y].TexturePart);
				//std::cout << "Zeichne " << x << "/" << y << " Tile ID=" << MapFile[x][y] << "=" << std::endl;
			}
		}
		DisplayFPS.Render(renderWindow);
		P1.Render(renderWindow);
		P1.Update(renderWindow, ElapsedTime);
		
		Event levelLoop;
		while(renderWindow.pollEvent(levelLoop))
		{

			if(levelLoop.type == Event::KeyPressed){
				if(levelLoop.key.code == Keyboard::Escape){
					pause(renderWindow,viewCamera,levelLoop,paused);
				}else if(levelLoop.key.code == Keyboard::End){
					return;
				}else if(levelLoop.key.code == Keyboard::F10) {
					sf::Image Screen = renderWindow.capture();
					if(Screen.saveToFile("screenshots//screenshot-"__DATE__"-.png")){
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

		float CamX = P1.getPosX();
		float CamY = P1.getPosY();

		if ( CamX < WIDTH/2 ) CamX = WIDTH/2;
		if ( CamY < HEIGHT/2 ) CamY = HEIGHT/2;
		if ( CamX > MapSizeX * TILESIZE - WIDTH/2 ) CamX = float(MapSizeX * TILESIZE - WIDTH/2);
		if ( CamY > MapSizeY * TILESIZE - HEIGHT/2 ) CamY = float(MapSizeY * TILESIZE - HEIGHT/2);

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
