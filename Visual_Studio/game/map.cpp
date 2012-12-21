#include "map.h"

int MapSizeX = 0, MapSizeY = 0;
int LoadCounterX = 0, LoadCounterY = 0;
int MapFile[100][100] = {0};

void LoadMap(const char* FileName ){
	std::ifstream openfile(FileName);
	if( openfile.is_open() ){
		openfile >> MapSizeX >> MapSizeY;
		while( !openfile.eof() ){
			openfile >> MapFile[LoadCounterX][LoadCounterY];
			LoadCounterX++;
			if( LoadCounterX >= MapSizeX ){
				LoadCounterX = 0;
				LoadCounterY++;
			}
		}
		std::cout << "Map Eingelesen" << std::endl;
		std::ifstream closefile(FileName);
	}
	else {
		throw("Map konnte nicht geoeffnet werden. Fehler: 01.1"); // 01.X = Texturenfehler allgemein 01.1 genau der hier
	}
}

void DrawMap(RenderWindow &window){
	RectangleShape Tile;
	for( int y = 0 ; y < MapSizeY ; y++ ){
		for( int x = 0 ; x < MapSizeX ; x++ ){
			Tile.setSize( Vector2f( TILESIZE, TILESIZE ) );
			Tile.setPosition( x * TILESIZE, y * TILESIZE );
			switch( MapFile[x][y] ){
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
			}
			window.draw(Tile);
			//std::cout << "Zeichne " << x << "/" << y << " Tile ID=" << MapFile[x][y] << "=" << std::endl;
		}
	}
}
