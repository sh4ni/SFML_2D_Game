#include "map.h"

/*IntRect Map::getRect(int x, int y){
	x /= TILESIZE;
	y /= TILESIZE;
	return CollisionMap[x][y];
}*/

void Map::Show(sf::RenderWindow& renderWindow, int LevelId, sf::View viewCamera, Savegame& currentSavegame){
	renderWindow.setMouseCursorVisible(false);
	#ifdef DEBUG
		std::cout << "Lade Map Nr: " << LevelId << "." << std::endl;
	#endif
	// Hier wird die Textur für die Map geladen.
	static sf::Texture LevelTexture;
	LevelTexture.loadFromFile(PATH"include/texture/world/overworld.png");		// Lade Texturedatei

	int MapSizeX = 0U;
	int MapSizeY = 0U;
	int LoadCounterX = 0;
	int LoadCounterY = 0;
	int TileType;
	TilePart** TileMap;
	sf::IntRect*** CollisionMap;
	
	//std::ostringstream FileName;
	//FileName << PATH"include/map/map" << LevelId << ".txt";   Funktioniert NUR in Visual Studio! Nicht standard C++
    char FileName[30];
	sprintf(FileName,PATH"include/map/map%d.txt",LevelId);

	// Map Loader. Datei wird eingelesen und es werden dynamisch neue objekte erzeugt.
	std::ifstream openfile(FileName/*.str()*/);
	if( openfile.is_open() ){
		openfile >> MapSizeX >> MapSizeY;

		TileMap = new TilePart*[MapSizeX];			// Map Speicher Dynamisch reservieren.
		for ( int i = 0 ; i < MapSizeX ; i++ ){		// Es ist nicht gewährleistet ob der Speicher an einem Stück hintereinander ist.
			TileMap[i] = new TilePart[MapSizeY];
		}

		CollisionMap = new sf::IntRect**[MapSizeX];
		for ( int i = 0 ; i < MapSizeX ; i++ ){
			CollisionMap[i] = new sf::IntRect*[MapSizeY];
		}


		while( !openfile.eof() ){
			openfile >> TileType;
			sf::IntRect subRect;
			subRect.height=subRect.width=TILESIZE;
			subRect.top=TileType/10*TILESIZE;
			subRect.left=TileType%10*TILESIZE;
			
			TileMap[LoadCounterX][LoadCounterY].TexturePart = new sf::Sprite(LevelTexture,subRect);
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
			case 15:
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
				CollisionMap[LoadCounterX][LoadCounterY]=NULL;
					break;
			default:
				CollisionMap[LoadCounterX][LoadCounterY]=new sf::IntRect(LoadCounterX*TILESIZE,LoadCounterY*TILESIZE,TILESIZE,TILESIZE);
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
		#ifdef DEBUG
			std::cout << "Map erfolgreich eingelesen." << std::endl;
		#endif
		std::ifstream closefile(FileName/*.str()*/);
	}
	else {
		//throw("Map konnte nicht geoeffnet werden. Fehler: 01.1"); // 01.X = Texturenfehler allgemein 01.1 genau der hier
	}
	
	sf::Clock clock;

	Player P1(CollisionMap,currentSavegame);
	P1.setMapSize( MapSizeX, MapSizeY );

	float LastTime = 1.f;
	float ElapsedTime;
	float Frames;

	int CamX;
	int CamY;
	
	sf::Texture ifaceImage;
	if( P1.getGender() == 'F' ){
		if(!ifaceImage.loadFromFile(PATH"include/interface/interface-female.png")){
			//fehlerbehebung
		}
	}
	else {
		if(!ifaceImage.loadFromFile(PATH"include/interface/interface-male.png")){
			//fehlerbehebung
		}
	}
	sf::Sprite iface(ifaceImage);
	iface.setOrigin((float)ifaceImage.getSize().x/2.f,(float)ifaceImage.getSize().y);
	iface.setPosition(WIDTH/2,HEIGHT);

	sf::RectangleShape HPBar;
	HPBar.setFillColor(sf::Color(0x99,0x33,0x33));
	HPBar.setPosition(WIDTH/2-55,HEIGHT-61);
	HPBar.setSize(sf::Vector2f(180.f,28.f));

	sf::RectangleShape EXPBar;
	EXPBar.setFillColor(sf::Color(0x00,0xCC,0x33));
	EXPBar.setPosition(WIDTH/2-55,HEIGHT-30);
	EXPBar.setSize(sf::Vector2f(180.f,28.f));

	Schrift DisplayHPText(WIDTH/2-53,HEIGHT-58,"HP",18,0);
	Schrift DisplayEXPText(WIDTH/2-53,HEIGHT-27,"EXP",18,0);
	Schrift DisplayHP(WIDTH/2+122,HEIGHT-58,"Error",18,0);
	Schrift DisplayEXP(WIDTH/2+123,HEIGHT-27,"Error",18,0);
	Schrift DisplayLevel(WIDTH/2-128,HEIGHT-76,"Err",18,0);

	Schrift DisplayFPS(0,0,"FPS: Error",20);
	Schrift DisplayKoord(0,20,"X: Error Y: Error",20);
	Schrift DisplaySpeed(0,40,"Speed: Error",20);

	while(true)
	{
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
		if ( MapSizeX*TILESIZE < WIDTH ){
			CamX = MapSizeX*TILESIZE/2;
			renderWindow.clear();
		}
		else if ( CamX < WIDTH/2 ) CamX = WIDTH/2;
		else if ( CamX > MapSizeX * TILESIZE - WIDTH/2 ) CamX = MapSizeX * TILESIZE - WIDTH/2;
		if ( MapSizeY*TILESIZE < HEIGHT ) CamY = MapSizeY*TILESIZE/2;
		else if ( CamY < HEIGHT/2 ) CamY = HEIGHT/2;
		else if ( CamY > MapSizeY * TILESIZE - HEIGHT/2 ) CamY = MapSizeY * TILESIZE - HEIGHT/2;
		renderWindow.setView(viewCamera);
		viewCamera.setCenter((float)CamX,(float)CamY);	// Alles was ab hier gerendert wird, bewegt sich mit der Kamera mit

		// Hier wird die Map gerendert.
		// Orientierung an der Kamera, damit nur sichtbare Sprites neu gezeichnet werden.

		{// Klammer, wegen sichtbarkeit der variablen.
			int y = ((int)CamY-(HEIGHT/2))/TILESIZE;
			if( y<0 ) y=0;
			while( (y < ((int)CamY+(HEIGHT/2)+TILESIZE-1)/TILESIZE)&&(y<MapSizeY) ){
				int x = ((int)CamX-(WIDTH/2))/TILESIZE;
				if( x<0 ) x=0;
				while( (x < ((int)CamX+(WIDTH/2)+TILESIZE-1)/TILESIZE)&&(x<MapSizeX) ){
					TileMap[x][y].TexturePart->setPosition( (float)(x * TILESIZE), (float)(y * TILESIZE) );
					renderWindow.draw(*TileMap[x][y].TexturePart);
					x++;
				}
				y++;
			}
		}

		// Rendern des Spielers
		P1.Render(renderWindow);
		P1.Update(renderWindow, ElapsedTime);
		
		sf::Event levelLoop;
		while(renderWindow.pollEvent(levelLoop)){
			if(levelLoop.type == sf::Event::KeyPressed){
				if(levelLoop.key.code == sf::Keyboard::Escape){
					bool quitGame = pause(renderWindow,viewCamera,levelLoop,P1,LevelId, currentSavegame);
					if(quitGame)
						return;
				}
				else if(levelLoop.key.code == sf::Keyboard::F10) {
					sf::Image Screen = renderWindow.capture();
					if(Screen.saveToFile("screenshots\\screenshot-"__DATE__"-.png")){
						#ifdef DEBUG
							std::cout << " Screenshot gespeichert.. " << std::endl;	
						#endif
					}
				}
				else if(levelLoop.key.code == sf::Keyboard::E){
					// player speed up
					P1.increaseSpeed(0.1f);
				}
				else if(levelLoop.key.code == sf::Keyboard::Q){
					// player speed runter
					P1.decreaseSpeed(0.1f);
				}
			}
			else if(levelLoop.type == sf::Event::LostFocus){
				#ifdef DEBUG
					std::cout << " Ausserhalb Fenster!.. " << std::endl;	
				#endif
					pause(renderWindow,viewCamera,levelLoop,P1,LevelId, currentSavegame);
			}
			else if(levelLoop.type == sf::Event::Closed){
				return;
			}
		}

		renderWindow.setView(renderWindow.getDefaultView());	// Alles was ab hier gerendert wird, wird nicht mit der Kamera mit bewegt! z.b. das Interface
		
		renderWindow.draw(iface);

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

		DisplayHPText.Render(renderWindow);
		DisplayEXPText.Render(renderWindow);
		DisplayHP.Render(renderWindow);
		DisplayEXP.Render(renderWindow);
		DisplayLevel.Render(renderWindow);

		DisplayFPS.Render(renderWindow);	// FPS Anzeige
		DisplayKoord.Render(renderWindow);	// Spielerkoordinaten Anzeige
		DisplaySpeed.Render(renderWindow);	// Geschwindigkeit des Players

		renderWindow.display();
	}
}

bool Map::load(Player& P1)
{
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
		loadgame >> tmp; // mapid
		
		float temp;
		float temp2;
		loadgame >> temp;
		//P1.setPosX(temp);
		loadgame >> temp2;
		
		P1.setPosition(temp,temp2);


		loadgame.close();
	}else{
		std::cout << "An error occurred during loading the saved game.." << std::endl;
		return false;
	}

	return true;
}

bool Map::save(Player& P1, int LevelId)
{
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
		ss << (P1.getHealth() - P1.getLvl() + P1.getExp() + P1.getGender() + (int)P1.getPosX() + (int)P1.getPosY() +LevelId + CHECKSUM);
		std::string checksum = md5(ss.str());

		savegame << checksum;
		#ifdef DEBUG
			std::cout << "Savegame Checksum -> " << checksum << std::endl;
		#endif

		savegame.close();
		std::cout << "saved..\n";
	}else{
		std::cout << "An error occurred during saving the game..\a\n";
	}
	
	return true;
}

bool Map::pause(sf::RenderWindow& renderWindow, sf::View viewCamera, sf::Event levelLoop, Player& P1, int LevelId, Savegame& currentSavegame){
	
	//Map::load(P1);
	#ifdef DEBUG
		std::cout << "Paused.." << std::endl;	
	#endif

	int CenterX = (int)viewCamera.getCenter().x;
	int CenterY = (int)viewCamera.getCenter().y;

	// Hintergrund Box
	sf::RectangleShape Background(sf::Vector2f(WIDTH, HEIGHT));
    Background.setFillColor(sf::Color(0, 0, 0,100));
	Background.setPosition((float)CenterX-(float)WIDTH/2.f,(float)CenterY-(float)HEIGHT/2.f);

	// Pause Schriftzug
	Schrift Pause(CenterX,CenterY-100,"Paused",50);
	Pause.printText.setOrigin(Pause.printText.getGlobalBounds().width/2.f+1.f,0);	// Textbox zentrieren

	// Logo im Pausemenü
	sf::Texture LogoImage;
	if(!LogoImage.loadFromFile(PATH"include/interface/splashscreen.png")){
		// Fehlerbehebung
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
		if(levelLoop.type == sf::Event::KeyPressed){
			if (levelLoop.key.code == sf::Keyboard::Escape){
				#ifdef DEBUG
					std::cout << "Continue Playing.." << std::endl;
				#endif
					return false; // gebe false zurueck damit das spiel nicht beendet wird, sondern weiter geht!
			}else if(levelLoop.key.code == sf::Keyboard::Space){
				#ifdef DEBUG
					std::cout << "Quit Game!" << std::endl;
				#endif
					return true; // gebe true zurueck damit das spiel anschließend beendet wird
			}else if(levelLoop.key.code == sf::Keyboard::F6){
				Map::save(P1, LevelId);
			}else if(levelLoop.key.code == sf::Keyboard::F9){
				Map::load(P1);
			}
		}
	}
	return false; // gebe false zurueck damit das spiel nicht beendet wird, sondern weiter geht!
}
