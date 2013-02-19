#include "map.h"


/*IntRect Map::getRect(int x, int y){
	x /= TILESIZE;
	y /= TILESIZE;
	return CollisionMap[x][y];
}*/
sf::Texture Map::LevelTexture;


Map::Map(){
	std::cout << "konstruktor MAP!" << std::endl;

	#ifdef DEBUGINFO
		std::cout << "Load Map : " << Savegame::currentSaveGame->mLevelId << std::endl;
	#endif

	
	
	//renderWindow.setMouseCursorVisible(false);
	
	// Hier wird die Textur für die Map geladen.
	

	this->MapSizeX = 0U;
	this->MapSizeY = 0U;
	this->LoadCounterX = 0;
	this->LoadCounterY = 0;
	this->TileMap = 0;
	this->CollisionMap = 0;
	

	this->FileName = PATH"include/map/" + Savegame::currentSaveGame->mLevelId + ".txt";

	// Map Loader. Datei wird eingelesen und es werden dynamisch neue objekte erzeugt.
	std::ifstream openfile(FileName.c_str());
	if( openfile.is_open() ){
		openfile >> this->MapSizeX >> this->MapSizeY >> this->mapTheme;
		
		if(!(TileMap = new TilePart*[MapSizeX])){
            throw "Error: Could not allocate space for 'TilePart*'";
        }                                           // Map Speicher Dynamisch reservieren.
		for ( int i = 0 ; i < MapSizeX ; i++ ){		// Es ist nicht gewährleistet ob der Speicher an einem Stück hintereinander ist.
			if(!(TileMap[i] = new TilePart[MapSizeY])){
                throw "Error: Could not allocate space for 'TilePart'";
            }
		}

		if(!(CollisionMap = new sf::IntRect**[MapSizeX])){
            throw "Error: Could not allocate space for 'IntRect**'";
        }
		for ( int i = 0 ; i < MapSizeX ; i++ ){
			if(!(CollisionMap[i] = new sf::IntRect*[MapSizeY])){
                throw "Error: Could not allocate space for 'IntRect*'";
            }
		}


		while( !openfile.eof() ){
			openfile >> TileType;
			sf::IntRect subRect;
			subRect.height=subRect.width=TILESIZE;
			subRect.top=TileType/10*TILESIZE;   // zeile
			subRect.left=TileType%10*TILESIZE;  // spalte
			
			if(!(TileMap[LoadCounterX][LoadCounterY].TexturePart = new sf::Sprite(LevelTexture,subRect))){
                throw "Error: Could not allocate space for 'Sprite'";
            }
			switch( TileType ){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 12:
			case 13:
			case 14:
			case 15:        // alle blšcke OHNE kollision!
			case 16:
			case 17:
			case 19:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;  // keine kollision
					break;
			default:        // defaultwert: alle anderen blšcke kriegen eine kollision!
                    if(!(CollisionMap[LoadCounterX][LoadCounterY]=new sf::IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE))){
                        throw "Error: Could not allocate space for 'IntRect'";
                    }
				break;

			}
			/*if(TileType >10  ){
				CollisionMap[LoadCounterX][LoadCounterY]=new IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);
			}else{
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;
			}*/
			//CollisionMap[LoadCounterX][LoadCounterY] = IntRect(0,0,32,32) ;
			LoadCounterX++;
			if( LoadCounterX >= MapSizeX ){
				LoadCounterX = 0;
				LoadCounterY++;
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

	//Player P2(CollisionMap,currentSavegame,1);        // 2 Spieler Koop Test
	//P2.setMapSize( MapSizeX, MapSizeY );              // Funktioniert ohne probleme!

	this->LastTime = 1.f;
	
	
	if( P1.getGender() == 'F' ){
		if(!ifaceImage.loadFromFile(PATH"include/interface/interface-female.png")){
			throw "Error: include/interface/interface-female.png not found.";
		}
	}
	else {
		if(!ifaceImage.loadFromFile(PATH"include/interface/interface-male.png")){
			throw "Error: include/interface/interface-male.png not found.";
		}
	}

	iface.setTexture(ifaceImage);
	iface.setOrigin((float)ifaceImage.getSize().x/2.f,(float)ifaceImage.getSize().y);
	iface.setPosition(ConfigFile::currentConfigFile->width/2,ConfigFile::currentConfigFile->height);

	HPBar.setFillColor(sf::Color(0x99,0x33,0x33));  // farbe des hp balkens
	HPBar.setPosition(ConfigFile::currentConfigFile->width/2-55,ConfigFile::currentConfigFile->height-61);
	HPBar.setSize(sf::Vector2f(180.f,28.f));
	
	EXPBar.setFillColor(sf::Color(0x00,0xCC,0x33)); // farbe des exp balkens
	EXPBar.setPosition(ConfigFile::currentConfigFile->width/2-55,ConfigFile::currentConfigFile->height-30);
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
Map::~Map(){
	std::cout << "dekonstruktor MAP!" << std::endl;
    
    // Lšsche TileMap wieder...
    for( int x=0; x<MapSizeX; x++){
        for( int y=0; y<MapSizeY; y++){
            delete TileMap[x][y].TexturePart;   // Lšschen der TileMap - Textureausschnitte
            if( CollisionMap[x][y] != NULL ){
                delete CollisionMap[x][y];      // Lšschen der CollisionMap - Rechtecke
            }
        }
        delete TileMap[x];                      // Lšschen der TileMap - Objekte
        delete CollisionMap[x];                 // Lšschen der CollisionMap - Pointer
    }
    delete TileMap;                             // Lšschen der TileMap - Pointer
    delete CollisionMap;                        // Lšschen der CollisionMap - Pointer auf Pointer

}

int Map::Show(sf::RenderWindow& renderWindow, std::string LevelId, sf::View viewCamera){
	while( 1+3+3==7 )
	{
		if(P1.getHealth() <= 0){
			return 500;
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

		// Wenn ich schonmal die Spieler koordinaten schonmal hab, dann nutz ich sie hier noch schnell für die Koordinatenanzeige
		// und ruf aus performancegründen die getPos methode nicht 2 mal auf.
		std::ostringstream PlayerKoordText;
		PlayerKoordText.precision(0);
		PlayerKoordText << std::fixed << "X: " << CamX << " Y: " << CamY;
		DisplayKoord.Update(PlayerKoordText.str());

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
		
		//P2.Render(renderWindow);      // 2 Spieler test... siehe weiter oben
		//P2.Update(ElapsedTime);

		sf::Event levelLoop;

		while(renderWindow.pollEvent(levelLoop)){
			if(levelLoop.type == sf::Event::KeyPressed || levelLoop.type == sf::Event::JoystickButtonPressed){
				if(levelLoop.key.code == sf::Keyboard::Escape || levelLoop.joystickButton.button == 8 ){
					/*
					P1.setBlockControl(true);
					bool quitGame = pause(renderWindow,viewCamera,levelLoop,P1,LevelId, clock);
					if(quitGame)
						return 5;
					P1.setBlockControl(false);
					*/
					Map::save(P1,LevelId);
					return 5;
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
					P1.decreaseSpeed(0.1f);                             // 2 = 10 level 10 schaden kriegen
				}                                                       // ...
                else if(levelLoop.key.code == sf::Keyboard::Num1){
                    P1.playerExp(10, P1.getLvl());
                }
                else if(levelLoop.key.code == sf::Keyboard::Num2){
                    P1.playerDamage(10, 10);
                }
				else if(levelLoop.key.code == sf::Keyboard::Num0){
                    P1.setBlockControl(true);
                }
				else if(levelLoop.key.code == sf::Keyboard::Num9){
                    P1.setBlockControl(false);
                }
				else if(levelLoop.key.code == sf::Keyboard::F6){
                    Map::save(P1,LevelId);
                }
				else if(levelLoop.key.code == sf::Keyboard::F9){
					Map::load(P1);
				}
				else if(levelLoop.key.code == sf::Keyboard::N){
					return 1500;
				}
                #endif
			}
            else if(levelLoop.type == sf::Event::LostFocus){
				#ifdef DEBUGINFO
					std::cout << " Outside the window!.. " << std::endl;	
				#endif
					return 5;
					//P1.setBlockControl(true);
					//pause(renderWindow,viewCamera,levelLoop,P1,LevelId, clock);
			}
            else if(levelLoop.type == sf::Event::Closed){
				return 0;
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
		DisplayLevel.printText.setOrigin(DisplayLevel.printText.getGlobalBounds().width/2+1,0);	// Text Zentrieren

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
	return 10;
}

void Map::load(Player& P1)
{
	
	//currentSaveGame.loadSavegame(currentSavegame);

	P1.setHealth(Savegame::currentSaveGame->pHealth);
	P1.setLvl(Savegame::currentSaveGame->pLvl);
	P1.setExp(Savegame::currentSaveGame->pExp);

	// hier fehlt noch EXP Max und Health Max

	P1.setPosition(Savegame::currentSaveGame->mPosX, Savegame::currentSaveGame->mPosY);
	/*
	std::cout << "loading savegame..";
	std::ifstream loadgame;
	loadgame.open(SAVEGAME, std::ios::binary);
	if(loadgame.is_open()){

		int tmp;
		loadgame >> tmp;
		P1.setHealth(tmp);
		loadgame >> tmp;
		P1.setLvl(tmp);
		loadgame >> tmp;
		P1.setExp(tmp);
		
		char tmper;
		loadgame >> tmper; // gender
		std::string temperer;
		loadgame >> temperer; // name
		loadgame >> temperer; // mapid
		
		float X,Y;
		loadgame >> X;
		loadgame >> Y;
		
		P1.setPosition(X,Y);

		loadgame.close();
	}else{
		std::cout << "An error occurred during loading the saved game.." << std::endl;
		return false;
	}
	
	return true;
	*/
}


void Map::save(Player& P1, std::string LevelId)
{
	Savegame::currentSaveGame->pHealth = P1.getHealth();
	Savegame::currentSaveGame->pLvl = P1.getLvl();
	Savegame::currentSaveGame->pExp = P1.getExp();
	Savegame::currentSaveGame->pGender = P1.getGender();
	Savegame::currentSaveGame->pName = P1.getName();
	Savegame::currentSaveGame->mLevelId = LevelId;
	Savegame::currentSaveGame->mPosX = P1.getPosX();
	Savegame::currentSaveGame->mPosY = P1.getPosY();

	Savegame::currentSaveGame->saveSavegame(P1.getGender(),false);
/*
	std::cout << "saving..\n";
	std::ofstream savegame;
	savegame.open(SAVEGAME, std::ios::trunc | std::ios::binary);
	if(savegame.is_open()){
		// Health
		savegame << P1.getHealth() << std::endl;
		// Level
		savegame << P1.getLvl() << std::endl;
		// Exp
		savegame << P1.getExp() << std::endl;
		// Gender
		savegame << P1.getGender() << std::endl;
		// Name
		savegame << P1.getName() << std::endl;

		// Map
		savegame << LevelId << std::endl;
		// Pos X auf Map
		savegame << P1.getPosX() << std::endl;
		// Pos Y auf Map
		savegame << P1.getPosY() << std::endl;

		std::stringstream ss;
		ss << (P1.getHealth() - P1.getLvl() + P1.getExp() + P1.getGender() + (int)P1.getPosX() + (int)P1.getPosY() + CHECKSUM);
		std::string checksum = md5(ss.str());

		savegame << checksum;
		#ifdef DEBUGINFO
			std::cout << "Savegame Checksum -> " << checksum << std::endl;
		#endif

		savegame.close();
		std::cout << "saved..\n";
	}else{
		std::cout << "An error occurred during saving the game..\a\n";
	}
	
	return true;
	*/
}

bool Map::pause(sf::RenderWindow& renderWindow, sf::View viewCamera, sf::Event levelLoop, Player& P1, std::string LevelId, sf::Clock& clock){
	
	Map::save(P1, LevelId);

	#ifdef DEBUGINFO
		std::cout << "Paused.." << std::endl;	
	#endif

	int CenterX = (int)viewCamera.getCenter().x;    // pausemenŸ muss hier gegengerechnet werden,
	int CenterY = (int)viewCamera.getCenter().y;    // da sonst mapkoordinaten mit bildschirm koordinaten
                                                    // nicht Ÿbereinstimmen!
	// Hintergrund Box
	sf::RectangleShape Background(sf::Vector2f(ConfigFile::currentConfigFile->width, ConfigFile::currentConfigFile->height));
    Background.setFillColor(sf::Color(0, 0, 0,100));
	Background.setPosition((float)CenterX-(float)ConfigFile::currentConfigFile->width/2.f,(float)CenterY-(float)ConfigFile::currentConfigFile->height/2.f);

	// Pause Schriftzug
	Schrift Pause(CenterX,CenterY-100,"Paused",50);
	Pause.printText.setOrigin(Pause.printText.getGlobalBounds().width/2.f+1.f,0);	// Textbox zentrieren

	// Logo im Pausemenü
	sf::Texture LogoImage;
	if(!LogoImage.loadFromFile(PATH"include/interface/splashscreen.png")){  // selbes bild wie im intro
		throw "Error: include/interface/splashscreen.png not found.";
	}
	sf::Sprite Logo(LogoImage);
	Logo.setOrigin((float)LogoImage.getSize().x/2.f,0);
	Logo.setPosition((float)CenterX,(float)CenterY-300.f);

	// Hilfetext
	Schrift HilfeTasten(CenterX-50,CenterY,"ESC:\nF6\nF9:\nSpace:",20);
	Schrift HilfeText(CenterX+70,CenterY,"Continue\nSave\nLoad\nExit Game",20);
	HilfeTasten.printText.setOrigin(HilfeTasten.printText.getGlobalBounds().width/2.f+1.f,0);	// Textbox zentrieren
	HilfeText.printText.setOrigin(HilfeText.printText.getGlobalBounds().width/2.f+1.f,0);		// Textbox zentrieren

	// LOS! ZEICHNE ES NIEDERER SKLAVE!
	renderWindow.draw(Background);
	renderWindow.draw(Logo);
	Pause.Render(renderWindow);
	HilfeTasten.Render(renderWindow);
	HilfeText.Render(renderWindow);

	renderWindow.display();
	
	while(renderWindow.waitEvent(levelLoop)){
		clock.restart();	// damit läuft der spieler nicht weiter wenn in der pause eine taste gedrückt gehalten wurde
		if(levelLoop.type == sf::Event::KeyPressed || levelLoop.type == sf::Event::JoystickButtonPressed ){
			if (levelLoop.key.code == sf::Keyboard::Escape || levelLoop.joystickButton.button == 8 ){
				#ifdef DEBUGINFO
					std::cout << "Continue Playing.." << std::endl;
				#endif
					return false; // gebe false zurueck damit das spiel nicht beendet wird, sondern weiter geht!
			}else if(levelLoop.key.code == sf::Keyboard::Space || levelLoop.joystickButton.button == 9 ){
				#ifdef DEBUGINFO
					std::cout << "Quit Game!" << std::endl;
				#endif
					return true; // gebe true zurueck damit das spiel anschließend beendet wird
			}else if(levelLoop.key.code == sf::Keyboard::F6){
				Map::save(P1, LevelId);
			}else if(levelLoop.key.code == sf::Keyboard::F9){
				Map::load(P1);
			}else if(levelLoop.type == sf::Event::Closed){
				return true;
			}
		}
	}
	return false; // gebe false zurueck damit das spiel nicht beendet wird, sondern weiter geht!
}
