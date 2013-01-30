#include "include.h"




void Map::Show(RenderWindow& renderWindow, int LevelId){
	
	//LoadMap("include/map/map1.txt");
	std::cout << "lade Map " << LevelId << std::endl;

	// Hier wird die Textur für die Map geladen.
	static Texture LevelTexture;
	LevelTexture.loadFromFile("include/texture/world/overworld.png");		// Lade Texturedatei

	int MapSizeX = 0;
	int MapSizeY = 0;
	int LoadCounterX = 0;
	int LoadCounterY = 0;
	int TileType;
	//int MapFile[100][100] = {0};
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

	
	Schrift Texter(0,50,"Map");
	Texter.Render(renderWindow);


	renderWindow.display();

	while(true)
	{
		Event event;
		while(renderWindow.pollEvent(event))
		{
			if(event.type == Event::MouseButtonPressed || event.type == Event::KeyPressed){
				return;
			}else if(event.type == Event::Closed){
				return;
			}
		}
	}
}

/*void Map::InitMap(std::string FileName){

}

void Map::DrawMap(RenderWindow &window){


}

void TextureLoader::LoadTexture(std::string TextureFile){
}
*/