/**
	Verantwortlich: Daniel Maier, Filip Menke
	Infotext: Hier findet der Hauptgameloop statt.
    Bei der Initialisierung wird die Map aus einer Datei geladen.
    
    Dateiaufbau für einen Mapfile:
    [int Größe X] [int Größe Y] [char* Texturenset] [char* Musikdatei] [int Monster Minlevel] [int Monster Maxlevel]
    [int ...
    Größe X und Größe Y großes 2D int Array um Texture aus dem Texturenset zu bestimmen
    ...]
    [Optional: zusätzliche Mapinfos meht dazu im Code]
 
    Sobald das Texturenset ausgelesen wurde, wird eine zusätzliche Datei eingeladen, welche in einen Vector
    alle Texturen OHNE kollision bestimmt.
    Alle Texturen, welche nicht in dieser Datei angegeben werden, haben eine Kollision!
 
    Nachdem die Map erfolgreich eingeladen wurde, werden alle weiteren erforderlichen Dateien wie das Texturenset
    selbst, Monster, Spieler, usw. eingelesen/erzeugt.
*/
#include "map.h"

/// Static Variablen
sf::Texture Map::LevelTexture;
Map * Map::currentMap;

Map::Map(){
	std::cout << "konstruktor MAP!" << std::endl;
}

/**
 Map wird Initislisiert:
 z.B. beim Laden einer neuen Map.
 */
void Map::init(std::string LevelId){

	#ifdef DEBUGINFO
		std::cout << "Load Map : " << LevelId << std::endl;
	#endif
	//renderWindow.setMouseCursorVisible(false);
	
    /// Default-Werte
	this->CamX = TILESIZE;
	this->CamY = TILESIZE;
	this->MapSizeX = 0;
	this->MapSizeY = 0;
	this->TileMap = 0;
	this->CollisionMap = 0;
	this->MapLevelMin = 1;
	this->MapLevelMax = 1;
	this->monsterCounter = 0;
	this->isZoom = true;
    this->willPause = false;
    this->nextMap[0] = LevelId;
    this->nextMap[1] = LevelId;
    this->nextMap[2] = LevelId;
    this->nextMap[3] = LevelId;
	
    /// Dateipfad
	std::string FileName = PATH"include/map/" + LevelId + ".txt";

	int LoadCounterX = 0;
	int LoadCounterY = 0;
    
	/// Map Loader. Datei wird eingelesen und es werden dynamisch neue objekte erzeugt.
	std::ifstream openfile(FileName.c_str());
	if( openfile.is_open() ){
		openfile >> this->MapSizeX >> this->MapSizeY >> this->mapTheme >> this->mapMusic >> this->MapLevelMin >> this->MapLevelMax;
		
        std::vector<int> CollisionInfo;

        /// Tiles mit Kollision werden aus einer Textdatei geladen.
		std::string ThemeFileName = PATH"include/texture/world/" + mapTheme + ".txt";
        std::ifstream openThemeFile(ThemeFileName.c_str());
        if( openThemeFile.is_open()){
            while(!openThemeFile.eof()){
                int temp;
                openThemeFile >> temp;
                CollisionInfo.push_back(--temp);
            }
            std::ifstream closefile(ThemeFileName.c_str());
        }
        else {
            throw "Error: " + ThemeFileName + " not found.";
        }
#ifdef DEBUGINFO
        std::cout << mapTheme << " has " << CollisionInfo.size() << " collisionboxes" << std::endl;
#endif
    
		TileMap = new TilePart*[MapSizeX];			/// Map Speicher Dynamisch reservieren.
		for ( int i = 0 ; i < MapSizeX ; i++ ){		/// Es ist nicht gewährleistet ob der Speicher an einem Stück hintereinander ist.
			TileMap[i] = new TilePart[MapSizeY];
		}
		CollisionMap = new sf::IntRect**[MapSizeX];
		for ( int i = 0 ; i < MapSizeX ; i++ ){
			CollisionMap[i] = new sf::IntRect*[MapSizeY];
		}

        /// Hier wird die eigentliche Map geladen.
		while( LoadCounterY < MapSizeY ){
			openfile >> TileType;
			TileType--;
			sf::IntRect subRect;
			subRect.height=subRect.width=TILESIZE;
			subRect.top=TileType/10*TILESIZE;   /// Zeile
			subRect.left=TileType%10*TILESIZE;  /// Spalte
			
			TileMap[LoadCounterX][LoadCounterY].EnemyId = 0;
			TileMap[LoadCounterX][LoadCounterY].Teleport = 0;
			TileMap[LoadCounterX][LoadCounterY].TexturePart = new sf::Sprite(LevelTexture,subRect);
            
            bool foundCollision = false;
            for( unsigned i=0; i<CollisionInfo.size(); i++){
                if(CollisionInfo[i] == TileType) {
                    foundCollision = true;
                }
            }
            if(foundCollision){
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;  /// keine Kollision
            }
            else {
                CollisionMap[LoadCounterX][LoadCounterY]=new sf::IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);    /// Kollision
            }
            
			/*switch( TileType ){		// alte "hardcode" version ohne Datei
			case  0: case  1: case  2: case  3: case  4: case  5: case  6: case  7: case  8: case  9:
			case 10: case 12: case 13: case 14: case 15: case 16: case 17: case 19:
			case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29:
			case 30: case 31: case 32: case 35: case 36:
			case 41: case 42:
			case 50: case 51: case 52: case 57:
			case 60: case 61:
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;  // keine kollision
                    break;
			default:        // defaultwert: alle anderen blöcke kriegen eine kollision!
                    CollisionMap[LoadCounterX][LoadCounterY]=new sf::IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);
                    break;
			}*/
            
			LoadCounterX++;
			if( LoadCounterX >= MapSizeX ){
				LoadCounterX = 0;
				LoadCounterY++;
			}
		}
        
        /**
         Zusätzliche Mapinfos:
         Hier werden Monster, Teleporter oder Mapübergänge eingelesen.
         ID X-Koordinate Y-Koordinate (Zusätzliche Infos)
         ID 1: Mapübergänge, wenn der Spieler den Bildschirm verlässt.
            Da diese keine Position haben, wird die X-Koordinate für den Bildschirmrand benutzt.
            0 = Oberer Rand
            1 = Unterer Rand
            2 = Linker Rand
            3 = Rechter Rand
            Y-Koordinate ungenutzt.
            Ein zusätzliches Attribut für den Namen der neuen Map, welche geladen werden soll.
         
         ID 2: Gegner auf der Karte.
            X- und Y-Koordinate bestimmen den Ort, an dem der Gegner auf der Karte startet.
            Ein zusätzliches Attribut für die Gegner ID.
         
         ID 3: Teleporter auf der Karte. z.B. wenn man ein Haus betreten will.
            X- und Y-Koordinate bestimmen den Ort, an dem sich der Teleporter selbst befindet.
            Drei zusätzliche Attribute:
            1: Name der neuen Map
            2 und 3: X- und Y-Koordinate auf der Ziel-Map
         */
		while( !openfile.eof() ){
			int idTemp, xTemp, yTemp;
			openfile >> idTemp >> xTemp >> yTemp;
            if( idTemp == 1){
                openfile >> nextMap[xTemp]; /// xTemp = Direction! // 0 = oben // 1 = unten // 2 = links // 3 = rechts
            }
			else if( idTemp == 2 ){
				openfile >> TileMap[xTemp][yTemp].EnemyId;
				//std::cout << "ENEMY: " << TileMap[xTemp][yTemp].EnemyId << std::endl;
				monsterCounter++;
			}
			else if( idTemp == 3){
				TileMap[xTemp][yTemp].Teleport = new tp;
				openfile >> TileMap[xTemp][yTemp].Teleport->Map >> TileMap[xTemp][yTemp].Teleport->xDest >> TileMap[xTemp][yTemp].Teleport->yDest;
				//std::cout << "TELEPORTER: " << TileMap[xTemp][yTemp].Teleport->Map << " " << TileMap[xTemp][yTemp].Teleport->xDest << " " << TileMap[xTemp][yTemp].Teleport->yDest << std::endl;
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

    /// Lade Texturedatei.
	FileName = PATH"include/texture/world/" + mapTheme + ".png";
	if( !LevelTexture.loadFromFile(FileName.c_str())){
		throw "Error: " + FileName + " not found.";
	}
	
    /// Spieler die Kollisionsmap und die Mapgröße mitteilen.
	this->P1.setColMap(CollisionMap);
	this->P1.setMapSize( MapSizeX, MapSizeY );

    /// Monster werden erzeugt.
	monsterList = new Monster[monsterCounter];
	for( int y=0, i=0; y<MapSizeY; y++){
		for( int x=0; x<MapSizeX; x++){
			if( TileMap[x][y].EnemyId ){
				monsterList[i].setType(TileMap[x][y].EnemyId);
				monsterList[i].setPosition((float)(x*TILESIZE+16),(float)(y*TILESIZE));
                monsterList[i].setColMap(CollisionMap);
                monsterList[i].setMapSize( MapSizeX, MapSizeY );
				i++;
			}
		}
	}
	
	//Player P2(CollisionMap,currentSavegame,1);        // 2 Spieler Koop Test
	//P2.setMapSize( MapSizeX, MapSizeY );              // Funktioniert ohne probleme!

	this->LastTime = 1.f;
	
	/// Hier wird der Sound geladen!
	if(ConfigFile::currentConfigFile->sound == true){
		gameMusic::music.stop();
		std::string musicFileName = PATH"include/sound/" + mapMusic + ".ogg";
		gameMusic::music.openFromFile(musicFileName.c_str());
		gameMusic::music.play();
		gameMusic::music.setLoop(true);
	}
    
    //if( P1.getPosX() < 0) P1.setPosition((float)(MapSizeX*TILESIZE), P1.getPosY());
    //if( P1.getPosY() < 0) P1.setPosition(P1.getPosX(), (float)(MapSizeY*TILESIZE));

    /// Interface an gewählten Helden anpassen.
	initInterface();

    /// Interface wird positioniert.
	iface.setTexture(ifaceImage);
	iface.setOrigin((float)ifaceImage.getSize().x/2.f,(float)ifaceImage.getSize().y);
	iface.setPosition((float)(ConfigFile::currentConfigFile->width/2),(float)(ConfigFile::currentConfigFile->height));

	HPBar.setFillColor(sf::Color(0x99,0x33,0x33));  // farbe des hp balkens
	HPBar.setPosition((float)(ConfigFile::currentConfigFile->width/2-55),(float)(ConfigFile::currentConfigFile->height-61));
	HPBar.setSize(sf::Vector2f(180.f,28.f));
	
	EXPBar.setFillColor(sf::Color(0x00,0xCC,0x33)); // farbe des exp balkens
	EXPBar.setPosition((float)(ConfigFile::currentConfigFile->width/2-55),(float)(ConfigFile::currentConfigFile->height-30));
	EXPBar.setSize(sf::Vector2f(180.f,28.f));

	DisplayHPText.Init((float)(ConfigFile::currentConfigFile->width/2-53),(float)(ConfigFile::currentConfigFile->height-58),"HP",18,0);
	DisplayEXPText.Init((float)(ConfigFile::currentConfigFile->width/2-53),(float)(ConfigFile::currentConfigFile->height-27),"EXP",18,0);
	DisplayHP.Init((float)(ConfigFile::currentConfigFile->width/2+122),(float)(ConfigFile::currentConfigFile->height-58),"Error",18,0);      // default strings, falls was im spiel nicht klappt
	DisplayEXP.Init((float)(ConfigFile::currentConfigFile->width/2+123),(float)(ConfigFile::currentConfigFile->height-27),"Error",18,0);
	DisplayLevel.Init((float)(ConfigFile::currentConfigFile->width/2-128),(float)(ConfigFile::currentConfigFile->height-76),"Err",18,0);

#ifdef DEBUGINFO
	DisplayFPS.Init(0,0,"FPS: Error",20);
	DisplayKoord.Init(0,20,"X: Error Y: Error",20);
	DisplaySpeed.Init(0,40,"Speed: Error",20);
#endif

	// Die Map wurde erfolgreich geladen, also kann gespeichert werden
	//Savegame::currentSaveGame->saveSavegame();
}

/// Hier wird die Map wieder gelöscht.
void Map::destory(){
    // Lösche TileMap wieder...
    for( int x=0; x<MapSizeX; x++){
        for( int y=0; y<MapSizeY; y++){
            delete TileMap[x][y].TexturePart;   // Löschen der TileMap - Textureausschnitte
			if( TileMap[x][y].Teleport ){
				delete TileMap[x][y].Teleport;	// Löschen der TileMap - Teleporter
            }
            if( CollisionMap[x][y] ){
                delete CollisionMap[x][y];      // Löschen der CollisionMap - Rechtecke
            }
        }
		delete [] TileMap[x];                      // Löschen der TileMap - Objekte
		delete [] CollisionMap[x];                 // Löschen der CollisionMap - Pointer
    }
	
	delete [] TileMap;                             // Löschen der TileMap - Pointer
    delete [] CollisionMap;                        // Löschen der CollisionMap - Pointer auf Pointer
	delete [] monsterList;							// Lˆschen der Monster
#ifdef DEBUGINFO
	std::cout << "deleted map..." << std::endl;
#endif
}

/// Destruktor ruft den Destroyer auf.
Map::~Map(){
	//std::cout << "destruktor MAP!" << std::endl;
    destory();
}

/// Der eigentliche Hauptgameloop
MapEvent Map::Show(sf::RenderWindow& renderWindow, std::string LevelId, sf::View& viewCamera){
	while( 1+3+3==7 ){

        /// Bildschirm Höhe und Breite
        int width = ConfigFile::currentConfigFile->width/(isZoom?2:1);
        int height = ConfigFile::currentConfigFile->height/(isZoom?2:1);
        viewCamera.setSize((float)width,(float)height);
		if(willPause) renderWindow.clear();

		sf::sleep(sf::milliseconds(10));	/// CPU Auslastung nimmt imens ab
		if(P1.getHealth() <= 0){
			gameMusic::music.stop();	// damit im men¸ die musik wieder korrekt abgespielt wird
			return MapEvent(MapEvent::dead);
		}
		ElapsedTime = (float)clock.restart().asMilliseconds();

		Frames = 1.f /( ElapsedTime / 1000 );
		LastTime = ElapsedTime;

		/// FPS anzeige
		std::ostringstream FPSText;
		FPSText.precision(0);
		FPSText << std::fixed << "FPS: " << Frames;
		DisplayFPS.Update(FPSText.str());

		/// Kamera folgt dem Spieler.
		CamX = (int)P1.getPosX();
		CamY = (int)P1.getPosY();
        
        /// Prüfen ob Spieler aus dem Bildschirm läuft.
        if( CamX < 0 ){										// Links
			return MapEvent(MapEvent::mapchange,nextMap[2],(float)(getNextLevelSize(nextMap[2]).x-1),(float)((CamY+TILESIZE/2)/TILESIZE));
		}
        else if( CamX > (MapSizeX*TILESIZE)){				// Rechts
			return MapEvent(MapEvent::mapchange,nextMap[3],0.f,(float)((CamY+TILESIZE/2)/TILESIZE));
		}
        if( CamY < -TILESIZE/2 ){							// Oben
			return MapEvent(MapEvent::mapchange,nextMap[0],(float)(CamX/TILESIZE),(float)(getNextLevelSize(nextMap[0]).y-1));
		}
        else if( CamY > ((MapSizeY*TILESIZE)-TILESIZE/2)){	// Unten
			return MapEvent(MapEvent::mapchange,nextMap[1],(float)(CamX/TILESIZE),0.f);
		}

		/// Tile auf dem der Spieler steht.
		TileX = CamX/TILESIZE;
		TileY = (CamY+TILESIZE/2)/TILESIZE;

        /// Tiles nur im bereich der Mapgröße.
		if( TileX < 0 ) TileX = 0;
		else if( TileX >= MapSizeX ) TileX = MapSizeX-1;
		if( TileY < 0 ) TileY = 0;
		else if( TileY >= MapSizeY ) TileY = MapSizeY-1;

		std::ostringstream PlayerKoordText;
		PlayerKoordText.precision(0);
		PlayerKoordText << std::fixed << "X: " << CamX << " Y: " << CamY << " TX: " << TileX << " TY: " << TileY;
		DisplayKoord.Update(PlayerKoordText.str());

		/// Wenn Spieler auf einen Teleporter läuft
 		if( TileMap[TileX][TileY].Teleport ){
			#ifdef DEBUGINFO
				std::cout << "Lade Map: '" << TileMap[TileX][TileY].Teleport->Map << "' X: " << TileMap[TileX][TileY].Teleport->xDest << " Y: " << TileMap[TileX][TileY].Teleport->yDest << std::endl;
			#endif
			return MapEvent(MapEvent::mapchange,TileMap[TileX][TileY].Teleport->Map,(float)(TileMap[TileX][TileY].Teleport->xDest),(float)(TileMap[TileX][TileY].Teleport->yDest));
		}

		std::ostringstream PlayerSpeedText;
		PlayerSpeedText.precision(1);
		PlayerSpeedText << std::fixed << "Speed: " << P1.getSpeed();
		DisplaySpeed.Update(PlayerSpeedText.str());

		/// Kamera geht nicht über den Bildschirmrand hinaus
		if ( MapSizeX*TILESIZE < width ){
			CamX = MapSizeX*TILESIZE/2;
			renderWindow.clear();
		}
		else if ( CamX < width/2 ) CamX = width/2;
		else if ( CamX > MapSizeX * TILESIZE - width/2 ) CamX = MapSizeX * TILESIZE - width/2;
		if ( MapSizeY*TILESIZE < height ){
			CamY = MapSizeY*TILESIZE/2;
			renderWindow.clear();
		}
		else if ( CamY < height/2 ) CamY = height/2;
		else if ( CamY > MapSizeY * TILESIZE - height/2 ) CamY = MapSizeY * TILESIZE - height/2;
		renderWindow.setView(viewCamera);
		viewCamera.setCenter((float)CamX,(float)CamY);	// Alles was ab hier gerendert wird, bewegt sich mit der Kamera mit

		/**
         Hier wird die Map gerendert.
         Orientierung an der Kamera, damit nur sichtbare Sprites neu gezeichnet werden.
		*/
		{// Klammer, wegen sichtbarkeit der variablen.
			int y = ((int)CamY-(height/2))/TILESIZE;
			if( y<0 ) y=0;
			while( (y < ((int)CamY+(height/2)+TILESIZE-1)/TILESIZE)&&(y<MapSizeY) ){
				int x = ((int)CamX-(width/2))/TILESIZE;
				if( x<0 ) x=0;
				while( (x < ((int)CamX+(width/2)+TILESIZE-1)/TILESIZE)&&(x<MapSizeX) ){
					TileMap[x][y].TexturePart->setPosition( (float)(x * TILESIZE), (float)(y * TILESIZE) );
					renderWindow.draw(*TileMap[x][y].TexturePart);
					x++;
				}
				y++;
			}
		}

        /// Rendern der Monster
		for(int i=0;i<monsterCounter;i++){
			monsterList[i].Update(ElapsedTime);
			monsterList[i].Render(renderWindow);
			//std::cout << "Monster " << i << ": " << monsterList[i].getPosX() << "/" << monsterList[i].getPosY() << std::endl;
		}
        
		/// Rendern des Spielers
		P1.Render(renderWindow);
		P1.Update(ElapsedTime);

		
		//P2.Render(renderWindow);      // 2 Spieler test... siehe weiter oben
		//P2.Update(ElapsedTime);

        /**
         willPause wird gesetzt, wenn der Pause knopf gedrückt wird, oder das Fenster nicht mehr den Fokus hat.
         Es wird aber noch ein kompletter Frame gezeichnet.
         Hier wird in die Pause gesprungen.
         */
        if(willPause){
            isZoom = true;
            willPause = false;
            return MapEvent(MapEvent::pause);
        }
        
        /**
         Hier werden verschiedene Tasten abgefragt, welche nichts mit der Spielersteuerung zu tun haben.
         Esc    Pause
         F10    Screenshot
         F6     Speichern
         F9     Laden
         
         Tasten nur im Debug Modus:
         E      Schneller Laufen
         Q      Langsamer Laufen
         Z      Zoom ein-/ausschalten
         1      Level Up
         2      Spieler nimmt 20 "Level 10 Schaden"
         0      Spieler bewegungen werden geblockt
         9      Spieler bewegungen werden entblockt
         F      Toggle Fullscreen/Windowmode
         */
		sf::Event levelLoop;
		while(renderWindow.pollEvent(levelLoop)){
			if(levelLoop.type == sf::Event::KeyPressed || levelLoop.type == sf::Event::JoystickButtonPressed){
				if(levelLoop.key.code == sf::Keyboard::Escape || levelLoop.key.code == sf::Keyboard::P || levelLoop.joystickButton.button == ConfigFile::currentConfigFile->controller_START ){
                    isZoom = false;
                    willPause = true;
				}
				else if(levelLoop.key.code == sf::Keyboard::F10) {
					sf::Image Screen = renderWindow.capture();

					time_t rawtime;
					struct tm * timeinfo;

					time ( &rawtime );
					timeinfo = localtime ( &rawtime );
					
					std::stringstream ss;
					ss << "screenshots/screenshot_" << LevelId << "_" << timeinfo->tm_mday << "-" << (timeinfo->tm_mon+1) << "_" << timeinfo->tm_hour << "-" << timeinfo->tm_min << "-" <<timeinfo->tm_sec << ".png";
					std::string screenshotFileName = ss.str();
					
					if(Screen.saveToFile(screenshotFileName)){
						#ifdef DEBUGINFO
							std::cout << " Screenshot gespeichert.. " << std::endl;	
						#endif
					}
				}
				else if(levelLoop.key.code == sf::Keyboard::F6){
					Savegame::currentSaveGame->saveSavegame();
                }
				else if(levelLoop.key.code == sf::Keyboard::F9){
					Savegame::currentSaveGame->loadSavegame();
				}
#ifdef DEBUGINFO
				else if(levelLoop.key.code == sf::Keyboard::E){         // ein paar debug keys
					P1.increaseSpeed(0.1f);                             // E = schneller laufen
				}                                                       // Q = langsamer laufen
				else if(levelLoop.key.code == sf::Keyboard::Q){         // Z = zoom an/ausschalten
					P1.decreaseSpeed(0.1f);                             // 1 = levelup
				}                                                       // 2 = 20 level 10 schaden kriegen
                else if(levelLoop.key.code == sf::Keyboard::Z){
                    if(isZoom) isZoom = false;
					else isZoom = true;
                }
                else if(levelLoop.key.code == sf::Keyboard::Num1){
                    P1.playerExp(P1.getExpMax(), P1.getLvl());
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
                isZoom = false;
                willPause = true;
			}
            else if(levelLoop.type == sf::Event::Closed){
				return MapEvent(MapEvent::exiting);
			}
			
		}

        /// Alles was ab hier gerendert wird, wird nicht mit der Kamera mit bewegt! z.b. das Interface
        renderWindow.setView(renderWindow.getDefaultView());
        
		renderWindow.draw(iface);

        /// Balken für HP und EXP werden hier angepasst.
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

#ifdef DEBUGINFO
		DisplayFPS.Render(renderWindow);        // FPS Anzeige
		DisplayKoord.Render(renderWindow);      // Spielerkoordinaten Anzeige
		DisplaySpeed.Render(renderWindow);      // Geschwindigkeit des Players
#endif

		renderWindow.display();
		
	}
	return MapEvent(MapEvent::nothing);
}
