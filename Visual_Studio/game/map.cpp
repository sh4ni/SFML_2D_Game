#include "map.h"

sf::Texture Map::LevelTexture;
Map * Map::currentMap;

Map::Map(){
	std::cout << "konstruktor MAP!" << std::endl;
}

void Map::init(std::string LevelId){

	#ifdef DEBUGINFO
		std::cout << "Load Map : " << LevelId << std::endl;
	#endif
	//renderWindow.setMouseCursorVisible(false);
	
	this->MapSizeX = 0;
	this->MapSizeY = 0;
	int LoadCounterX = 0;
	int LoadCounterY = 0;
	this->TileMap = 0;
	this->CollisionMap = 0;
	this->MapLevelMin = 1;
	this->MapLevelMax = 1;
	this->monsterCounter = 0;
    
    this->nextMap[0] = LevelId;
    this->nextMap[1] = LevelId;
    this->nextMap[2] = LevelId;
    this->nextMap[3] = LevelId;
	
	this->FileName = PATH"include/map/" + LevelId + ".txt";

	// Map Loader. Datei wird eingelesen und es werden dynamisch neue objekte erzeugt.
	std::ifstream openfile(FileName.c_str());
	if( openfile.is_open() ){
		openfile >> this->MapSizeX >> this->MapSizeY >> this->mapTheme >> this->mapMusic >> this->MapLevelMin >> this->MapLevelMax;
		
		TileMap = new TilePart*[MapSizeX];			// Map Speicher Dynamisch reservieren.
		for ( int i = 0 ; i < MapSizeX ; i++ ){		// Es ist nicht gewährleistet ob der Speicher an einem Stück hintereinander ist.
			TileMap[i] = new TilePart[MapSizeY];
		}

		CollisionMap = new sf::IntRect**[MapSizeX];
		for ( int i = 0 ; i < MapSizeX ; i++ ){
			CollisionMap[i] = new sf::IntRect*[MapSizeY];
		}


		while( LoadCounterY < MapSizeY ){
			openfile >> TileType;
			sf::IntRect subRect;
			subRect.height=subRect.width=TILESIZE;
			subRect.top=TileType/10*TILESIZE;   // zeile
			subRect.left=TileType%10*TILESIZE;  // spalte
			
			TileMap[LoadCounterX][LoadCounterY].EnemyId = 0;
			TileMap[LoadCounterX][LoadCounterY].Teleport = 0;
			TileMap[LoadCounterX][LoadCounterY].TexturePart = new sf::Sprite(LevelTexture,subRect);
			switch( TileType ){		// alle blöcke OHNE kollision!
			case  0: case  1: case  2: case  3: case  4: case  5: case  6: case  7: case  8: case  9:
			case 10: case 12: case 13: case 14: case 15: case 16: case 17: case 19:
			case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29:
			case 30: case 31: case 32: case 35: case 36:
			case 41: case 42:
			case 50: case 51: case 52: case 57:
			case 60: case 61:
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;  // keine kollision
					break;
			default:        // defaultwert: alle anderen blšcke kriegen eine kollision!
                    CollisionMap[LoadCounterX][LoadCounterY]=new sf::IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);
				break;

			}
			LoadCounterX++;
			if( LoadCounterX >= MapSizeX ){
				LoadCounterX = 0;
				LoadCounterY++;
			}
		}
		while( !openfile.eof() ){
			int idTemp, xTemp, yTemp;
			openfile >> idTemp >> xTemp >> yTemp;
			if( idTemp == 0 ){
				openfile >> TileMap[xTemp][yTemp].EnemyId;
				std::cout << "ENEMY: " << TileMap[xTemp][yTemp].EnemyId << std::endl;

				monsterCounter++;

				//Monster theEnemy;
				//theEnemy.setType(TileMap[xTemp][yTemp].EnemyId);
				//theEnemy.setPosition(xTemp*TILESIZE+TILESIZE/2,yTemp*TILESIZE);
				//monsterList.push_back(theEnemy);
				//monsterCounter++;
			}
			else if( idTemp == 1){
				TileMap[xTemp][yTemp].Teleport = new tp;
				openfile >> TileMap[xTemp][yTemp].Teleport->Map >> TileMap[xTemp][yTemp].Teleport->xDest >> TileMap[xTemp][yTemp].Teleport->yDest;
				std::cout << "TELEPORTER: " << TileMap[xTemp][yTemp].Teleport->Map << " " << TileMap[xTemp][yTemp].Teleport->xDest << " " << TileMap[xTemp][yTemp].Teleport->yDest << std::endl;
			}
            else if( idTemp == 2){
                openfile >> nextMap[xTemp]; // xTemp = Direction! // 0 = oben // 1 = unten // 2 = links // 3 = rechts
            }
		}


		#ifdef DEBUGINFO
			std::cout << "Map successfully loaded." << std::endl;
		#endif
		std::ifstream closefile(FileName.c_str());
	}
	else {
		throw "Error: " + FileName + " not found.";
	}

	FileName = PATH"include/texture/world/" + mapTheme + ".png";

	if( !LevelTexture.loadFromFile(FileName.c_str())){		// Lade Texturedatei
		throw "Error: " + FileName + " not found.";
	}
	
	this->P1.setColMap(CollisionMap);
	this->P1.setMapSize( MapSizeX, MapSizeY );

	/*for(int i = 0; i<(int)monsterList.size(); i++){
		monsterList[i].setColMap(CollisionMap);
		monsterList[i].setMapSize( MapSizeX, MapSizeY);
	}*/

	monsterList = new Monster[monsterCounter];
	for( int y=0, i=0; y<MapSizeY; y++){
		for( int x=0; x<MapSizeX; x++){
			if( TileMap[x][y].EnemyId ){
				monsterList[i].setType(TileMap[x][y].EnemyId);
				monsterList[i].setPosition(x*TILESIZE+16,y*TILESIZE);
				i++;
			}
		}
	}
	
	//Player P2(CollisionMap,currentSavegame,1);        // 2 Spieler Koop Test
	//P2.setMapSize( MapSizeX, MapSizeY );              // Funktioniert ohne probleme!

	this->LastTime = 1.f;
	
	/**

		Hier wird der Sound geladen!		

	*/
	if(ConfigFile::currentConfigFile->sound == true){
		gameMusic::music.stop();
		std::string musicFileName = PATH"include/sound/" + LevelId + ".ogg";
		gameMusic::music.openFromFile(musicFileName.c_str());
		gameMusic::music.play();
		gameMusic::music.setLoop(true);
	}
    
    if( P1.getPosX() < 0) P1.setPosition(MapSizeX*TILESIZE, P1.getPosY());
    if( P1.getPosY() < 0) P1.setPosition(P1.getPosX(), MapSizeY*TILESIZE);

	initInterface();

	iface.setTexture(ifaceImage);
	iface.setOrigin((float)ifaceImage.getSize().x/2.f,(float)ifaceImage.getSize().y);
	iface.setPosition((float)(ConfigFile::currentConfigFile->width/2),(float)(ConfigFile::currentConfigFile->height));

	HPBar.setFillColor(sf::Color(0x99,0x33,0x33));  // farbe des hp balkens
	HPBar.setPosition((float)(ConfigFile::currentConfigFile->width/2-55),(float)(ConfigFile::currentConfigFile->height-61));
	HPBar.setSize(sf::Vector2f(180.f,28.f));
	
	EXPBar.setFillColor(sf::Color(0x00,0xCC,0x33)); // farbe des exp balkens
	EXPBar.setPosition((float)(ConfigFile::currentConfigFile->width/2-55),(float)(ConfigFile::currentConfigFile->height-30));
	EXPBar.setSize(sf::Vector2f(180.f,28.f));

	DisplayHPText.Init(ConfigFile::currentConfigFile->width/2-53,ConfigFile::currentConfigFile->height-58,"HP",18,0);
	DisplayEXPText.Init(ConfigFile::currentConfigFile->width/2-53,ConfigFile::currentConfigFile->height-27,"EXP",18,0);
	DisplayHP.Init(ConfigFile::currentConfigFile->width/2+122,ConfigFile::currentConfigFile->height-58,"Error",18,0);      // default strings, falls was im spiel nicht klappt
	DisplayEXP.Init(ConfigFile::currentConfigFile->width/2+123,ConfigFile::currentConfigFile->height-27,"Error",18,0);
	DisplayLevel.Init(ConfigFile::currentConfigFile->width/2-128,ConfigFile::currentConfigFile->height-76,"Err",18,0);
	DisplayFPS.Init(0,0,"FPS: Error",20);
	DisplayKoord.Init(0,20,"X: Error Y: Error",20);
	DisplaySpeed.Init(0,40,"Speed: Error",20);

}

void Map::destory(){
    // Lšsche TileMap wieder...
    for( int x=0; x<MapSizeX; x++){
        for( int y=0; y<MapSizeY; y++){
            delete TileMap[x][y].TexturePart;   // Lšschen der TileMap - Textureausschnitte
			if( TileMap[x][y].Teleport ){
				delete TileMap[x][y].Teleport;	// Lšschen der TileMap - Teleporter
            }
            if( CollisionMap[x][y] ){
                delete CollisionMap[x][y];      // Lšschen der CollisionMap - Rechtecke
            }
        }
		delete [] TileMap[x];                      // Lšschen der TileMap - Objekte
		delete [] CollisionMap[x];                 // Lšschen der CollisionMap - Pointer
    }
	
	delete [] TileMap;                             // Lšschen der TileMap - Pointer
    delete [] CollisionMap;                        // Lšschen der CollisionMap - Pointer auf Pointer
	delete [] monsterList;							// Löschen der Monster
	std::cout << "deleted map..." << std::endl;
}

Map::~Map(){
	std::cout << "destruktor MAP!" << std::endl;
    destory();
}

MapEvent Map::Show(sf::RenderWindow& renderWindow, std::string LevelId, sf::View viewCamera){
	while( 1+3+3==7 ){
		sf::sleep(sf::milliseconds(10));	// CPU Auslastung nimmt imens ab
		if(P1.getHealth() <= 0){
			gameMusic::music.stop();	// damit im menü die musik wieder korrekt abgespielt wird
			return MapEvent(MapEvent::dead);
		}
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
        
        // prŸfen ob spieler aus dem bildschirm lŠuft
        if( CamX < 0 ) return MapEvent(MapEvent::mapchange,nextMap[2],-1,CamY/TILESIZE);                         // Links
        else if( CamX > (MapSizeX*TILESIZE)) return MapEvent(MapEvent::mapchange,nextMap[3],0,CamY/TILESIZE);   // Rechts
        if( CamY < 0 ) return MapEvent(MapEvent::mapchange,nextMap[0],CamX/TILESIZE,-1);                         // Oben
        else if( CamY > (MapSizeY*TILESIZE)) return MapEvent(MapEvent::mapchange,nextMap[1],CamX/TILESIZE,0);   // Unten

		// Wenn ich schonmal die Spieler koordinaten schonmal hab, dann nutz ich sie hier noch schnell für die Koordinatenanzeige
		// und ruf aus performancegründen die getPos methode nicht 2 mal auf.
		TileX = CamX/TILESIZE;
		TileY = (CamY+TILESIZE/2)/TILESIZE;

		if( TileX < 0 ) TileX = 0;
		else if( TileX >= MapSizeX ) TileX = MapSizeX-1;
		if( TileY < 0 ) TileY = 0;
		else if( TileY >= MapSizeY ) TileY = MapSizeY-1;

		std::ostringstream PlayerKoordText;
		PlayerKoordText.precision(0);
		PlayerKoordText << std::fixed << "X: " << CamX << " Y: " << CamY << " TX: " << TileX << " TY: " << TileY;
		DisplayKoord.Update(PlayerKoordText.str());

		// und nochmal ^^
 		if( TileMap[TileX][TileY].Teleport ){
			#ifdef DEBUGINFO
				std::cout << "Lade Map: '" << TileMap[TileX][TileY].Teleport->Map << "' X: " << TileMap[TileX][TileY].Teleport->xDest << " Y: " << TileMap[TileX][TileY].Teleport->yDest << std::endl;
			#endif
			return MapEvent(MapEvent::mapchange,TileMap[TileX][TileY].Teleport->Map,TileMap[TileX][TileY].Teleport->xDest,TileMap[TileX][TileY].Teleport->yDest);
		}

		std::ostringstream PlayerSpeedText;
		PlayerSpeedText.precision(1);
		PlayerSpeedText << std::fixed << "Speed: " << P1.getSpeed();
		DisplaySpeed.Update(PlayerSpeedText.str());

		// ...geht aber nicht übers Kartenende hinaus
		if ( MapSizeX*TILESIZE < ConfigFile::currentConfigFile->width ){
			CamX = MapSizeX*TILESIZE/2;
			renderWindow.clear();
		}
		else if ( CamX < ConfigFile::currentConfigFile->width/2 ) CamX = ConfigFile::currentConfigFile->width/2;
		else if ( CamX > MapSizeX * TILESIZE - ConfigFile::currentConfigFile->width/2 ) CamX = MapSizeX * TILESIZE - ConfigFile::currentConfigFile->width/2;
		if ( MapSizeY*TILESIZE < ConfigFile::currentConfigFile->height ) CamY = MapSizeY*TILESIZE/2;
		else if ( CamY < ConfigFile::currentConfigFile->height/2 ) CamY = ConfigFile::currentConfigFile->height/2;
		else if ( CamY > MapSizeY * TILESIZE - ConfigFile::currentConfigFile->height/2 ) CamY = MapSizeY * TILESIZE - ConfigFile::currentConfigFile->height/2;
		renderWindow.setView(viewCamera);
		viewCamera.setCenter((float)CamX,(float)CamY);	// Alles was ab hier gerendert wird, bewegt sich mit der Kamera mit

		// Hier wird die Map gerendert.
		// Orientierung an der Kamera, damit nur sichtbare Sprites neu gezeichnet werden.

		{// Klammer, wegen sichtbarkeit der variablen.
			int y = ((int)CamY-(ConfigFile::currentConfigFile->height/2))/TILESIZE;
			if( y<0 ) y=0;
			while( (y < ((int)CamY+(ConfigFile::currentConfigFile->height/2)+TILESIZE-1)/TILESIZE)&&(y<MapSizeY) ){
				int x = ((int)CamX-(ConfigFile::currentConfigFile->width/2))/TILESIZE;
				if( x<0 ) x=0;
				while( (x < ((int)CamX+(ConfigFile::currentConfigFile->width/2)+TILESIZE-1)/TILESIZE)&&(x<MapSizeX) ){
					TileMap[x][y].TexturePart->setPosition( (float)(x * TILESIZE), (float)(y * TILESIZE) );
					renderWindow.draw(*TileMap[x][y].TexturePart);
					x++;
				}
				y++;
			}
		}

		// Rendern des Spielers
		P1.Render(renderWindow);
		P1.Update(ElapsedTime);
		
		/*for(int i = 0; i < (int)monsterList.size(); i++){
			if(!monsterList[i].isActive){
				monsterList.erase(monsterList.begin() + i);
				monsterCounter--;
			}
			monsterList[i].Update(ElapsedTime);
			monsterList[i].Render(renderWindow);
		}*/

		for(int i=0;i<monsterCounter;i++){
			monsterList[i].Update(ElapsedTime);
			monsterList[i].Render(renderWindow);
			//std::cout << "Monster " << i << ": " << monsterList[i].getPosX() << "/" << monsterList[i].getPosY() << std::endl;
		}

		
		//P2.Render(renderWindow);      // 2 Spieler test... siehe weiter oben
		//P2.Update(ElapsedTime);

		sf::Event levelLoop;

		while(renderWindow.pollEvent(levelLoop)){
			if(levelLoop.type == sf::Event::KeyPressed || levelLoop.type == sf::Event::JoystickButtonPressed){
				if(levelLoop.key.code == sf::Keyboard::Escape || levelLoop.joystickButton.button == ConfigFile::currentConfigFile->controller_START ){
					return MapEvent(MapEvent::pause);
				}
				else if(levelLoop.key.code == sf::Keyboard::F10) {
					sf::Image Screen = renderWindow.capture();
					if(Screen.saveToFile("screenshots/screenshot-"__DATE__"-.png")){
						#ifdef DEBUGINFO
							std::cout << " Screenshot gespeichert.. " << std::endl;	
						#endif
					}
				}
                #ifdef DEBUGINFO
				else if(levelLoop.key.code == sf::Keyboard::E){         // ein paar debug keys
					P1.increaseSpeed(0.1f);                             // E = schneller laufen
				}                                                       // Q = langsamer laufen
				else if(levelLoop.key.code == sf::Keyboard::Q){         // 1 = 10 erfahrung kriegen
					P1.decreaseSpeed(0.1f);                             // 2 = 20 level 10 schaden kriegen
				}                                                       // ...
                else if(levelLoop.key.code == sf::Keyboard::Num1){
                    P1.playerExp(10, P1.getLvl());
                }
                else if(levelLoop.key.code == sf::Keyboard::Num2){
                    P1.playerDamage(20, 10);
                }
				else if(levelLoop.key.code == sf::Keyboard::Num0){
                    P1.setBlockControl(true);
                }
				else if(levelLoop.key.code == sf::Keyboard::Num9){
                    P1.setBlockControl(false);
                }
				else if(levelLoop.key.code == sf::Keyboard::F6){
					Savegame::currentSaveGame->saveSavegame();
                }
				else if(levelLoop.key.code == sf::Keyboard::F9){
					Savegame::currentSaveGame->loadSavegame();
				}
				else if(levelLoop.key.code == sf::Keyboard::F){
					if(ConfigFile::currentConfigFile->winmode == "window"){
						renderWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height), WINDOWTITLE, sf::Style::Fullscreen);
						renderWindow.setMouseCursorVisible(false);
						ConfigFile::currentConfigFile->winmode = "fullscreen";
						ConfigFile::currentConfigFile->saveConfigFile();
					}else{
						renderWindow.create(sf::VideoMode(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height), WINDOWTITLE, sf::Style::Close);
						renderWindow.setMouseCursorVisible(true);
						ConfigFile::currentConfigFile->winmode = "window";
						ConfigFile::currentConfigFile->saveConfigFile();
					}
				}
                #endif
			}
            else if(levelLoop.type == sf::Event::LostFocus){
				#ifdef DEBUGINFO
					std::cout << " Outside the window!.. Game will be paused" << std::endl;	
				#endif
					return MapEvent(MapEvent::pause);
			}
            else if(levelLoop.type == sf::Event::Closed){
				return MapEvent(MapEvent::exiting);
			}
			
		}
		
		renderWindow.setView(renderWindow.getDefaultView());	// Alles was ab hier gerendert wird, wird nicht mit der Kamera mit bewegt! z.b. das Interface
		
		renderWindow.draw(iface);

        // balken fŸr hp und exp werden hier angepasst.
		HPBar.setSize(sf::Vector2f((float)P1.getHealth()/(float)P1.getHealthMax()*180.f,28.f));
		EXPBar.setSize(sf::Vector2f((float)P1.getExp()/(float)P1.getExpMax()*180.f,28.f));

		renderWindow.draw(HPBar);
		renderWindow.draw(EXPBar);

		DisplayHP.printText.setOrigin(DisplayHP.printText.getGlobalBounds().width,0);			// Text Rechtsbündig
		DisplayEXP.printText.setOrigin(DisplayEXP.printText.getGlobalBounds().width,0);
		DisplayLevel.printText.setOrigin(DisplayLevel.printText.getGlobalBounds().width/2,0);	// Text Zentrieren

		std::ostringstream PlayerHealthText;
		PlayerHealthText.precision(0);
		PlayerHealthText << std::fixed << P1.getHealth() << "/" << P1.getHealthMax();
		DisplayHP.Update(PlayerHealthText.str());

		std::ostringstream PlayerExpText;
		PlayerExpText.precision(0);
		PlayerExpText << std::fixed << P1.getExp() << "/" << P1.getExpMax();
		DisplayEXP.Update(PlayerExpText.str());

		std::ostringstream PlayerLvlText;
		PlayerLvlText.precision(0);
		PlayerLvlText << std::fixed << P1.getLvl();
		DisplayLevel.Update(PlayerLvlText.str());

		DisplayHPText.Render(renderWindow);     // text "HP" auf dem balken
		DisplayEXPText.Render(renderWindow);    // text "EXP" auf dem balken
		DisplayHP.Render(renderWindow);         // hp Anzeige
		DisplayEXP.Render(renderWindow);        // exp Anzeige
		DisplayLevel.Render(renderWindow);      // level Anzeige

		DisplayFPS.Render(renderWindow);        // FPS Anzeige
		DisplayKoord.Render(renderWindow);      // Spielerkoordinaten Anzeige
		DisplaySpeed.Render(renderWindow);      // Geschwindigkeit des Players

		renderWindow.display();
		
	}
	return MapEvent(MapEvent::nothing);
}