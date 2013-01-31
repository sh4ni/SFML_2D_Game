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

	std::string FileName = "include/map/map1.txt";
	std::ifstream openfile(FileName);
	if( openfile.is_open() ){
		openfile >> MapSizeX >> MapSizeY;
		while( !openfile.eof() ){
			openfile >> TileType;  /*MapFile[LoadCounterX][LoadCounterY];*/
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

	Schrift Texter(0,50,"Map");
	
	Event currentEvent;

	while(true)
	{
		float ElapsedTime = (float)clock.restart().asMilliseconds();
		
		renderWindow.clear(Color(255,0,255));
		//DrawMap(renderWindow);
		//RectangleShape Tile;
		for( int y = 0 ; y < MapSizeY ; y++ ){
			for( int x = 0 ; x < MapSizeX ; x++ ){
				TileMap[x][y].TexturePart->setPosition( x * TILESIZE, y * TILESIZE );
				//Tile.setSize( Vector2f( TILESIZE, TILESIZE ) );
				//Tile.setPosition( x * TILESIZE, y * TILESIZE );
				/*switch( MapFile[x][y] ){
				case 1:
					Tile.setFillColor( Color::Blue );
					break;
				case 2:
					Tile.setFillColor( Color::Green );
					break;
				case 3:
					Tile.setFillColor( Color::Magenta );
					break;
				default:
					//throw("Unbekannte Textur angefordert. Fehler 01.2");
					Tile.setFillColor( Color::Black );
					break;
				}*/
				renderWindow.draw(*TileMap[x][y].TexturePart);
				//std::cout << "Zeichne " << x << "/" << y << " Tile ID=" << MapFile[x][y] << "=" << std::endl;
			}
		}
		Texter.Render(renderWindow);
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
		view.setCenter(P1.getPosX(),P1.getPosY());
		
		renderWindow.setView(view);
		renderWindow.display();
	}
}

/*void Map::InitMap(std::string FileName){

}

void Map::DrawMap(RenderWindow &window){


}

void TextureLoader::LoadTexture(std::string TextureFile){
}
*/