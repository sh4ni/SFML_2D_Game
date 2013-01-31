#include "include.h"

void Map::Show(RenderWindow& renderWindow, int LevelId, View view){
	
	std::cout << "Lade Map..." << LevelId << std::endl;

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
		std::cout << "Map erfolgreich Eingelesen" << std::endl;
		std::ifstream closefile(FileName);
	}
	else {
		//throw("Map konnte nicht geoeffnet werden. Fehler: 01.1"); // 01.X = Texturenfehler allgemein 01.1 genau der hier
	}
	
	
	Clock clock;
	
	Player P1("include/texture/player/player.png");
	
	Event currentEvent;

	float LastTime = 1.f;
	Schrift DisplayFPS(0,0,"Fps Anzeige");
	while(true)
	{
		float ElapsedTime = (float)clock.restart().asMilliseconds();

		float Frames = 1.f /( ElapsedTime / 1000 );
		LastTime = ElapsedTime;
		char text[15];
		//std::cout << "FPS: " << Frames << std::endl;
		sprintf(text,"FPS: %f",Frames);
		DisplayFPS.Update(text);
		
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
		
		
		while(renderWindow.pollEvent(currentEvent))
		{
			if(currentEvent.type == Event::KeyPressed){
				if(currentEvent.key.code == Keyboard::Escape){
					std::cout << " Pause " << std::endl;	
					getchar();
				}
			}
		}
		int CamX = P1.getPosX();
		int CamY = P1.getPosY();

		if ( CamX < WIDTH/2 ) CamX = WIDTH/2;
		if ( CamY < HEIGHT/2 ) CamY = HEIGHT/2;
		if ( CamX > MapSizeX * TILESIZE - WIDTH/2 ) CamX = MapSizeX * TILESIZE - WIDTH/2;
		if ( CamY > MapSizeY * TILESIZE - HEIGHT/2 ) CamY = MapSizeY * TILESIZE - HEIGHT/2;

		view.setCenter(CamX,CamY);
		
		renderWindow.setView(view);
		renderWindow.display();
	}
}
