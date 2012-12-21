#ifndef MAP_H_
#define MAP_H_

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

#define TILESIZE 32

void LoadMap(const char* FileName );
void DrawMap(RenderWindow &window);

#endif
