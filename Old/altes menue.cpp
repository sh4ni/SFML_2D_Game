// alte menü buttons...
	Schrift PlayText(WIDTH/2-BUTTONWIDTH/2+5,225,"Continue",50);
	Schrift ExitText(WIDTH/2-BUTTONWIDTH/2+5,321,"Exit",50);
	PlayText.Render(window);
	ExitText.Render(window);*/

	MenuItem buttonContinue;
	MenuItem buttonNewGame;
	MenuItem buttonExit;

	buttonContinue.image.setTexture(imageButton);
	buttonNewGame.image.setTexture(imageButton);
	buttonExit.image.setTexture(imageButton);
	
	.setTexture(texture);
	.setOrigin(16.f,32.f);
	.setTextureRect(sf::IntRect(0,0,TILESIZE,TILESIZE*2));
	.setPosition(currentSavegame.mPosX,currentSavegame.mPosY);
	
	Setup clickable regions

	Play menu item coordinates
	MenuItem playButton;
	
	playButton.rect.top= 225;
	playButton.rect.width = 288;
	playButton.rect.left = 160;
	playButton.rect.height = 862;

	playButton.action = Play;
	playButton.active = true;

	//Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.top = 321;
	exitButton.rect.width = 382;
	exitButton.rect.left = 160;
	exitButton.rect.height = 862;

	exitButton.action = Exit;

	_menuItems.push_back(playButton);
	_menuItems.push_back(exitButton);

// alte kollisionsabfrage der buttons
		if( menuItemRect.width > y 
			&& menuItemRect.top < y 
			&& menuItemRect.left < x 
			&& menuItemRect.height > x)
			{
				return (*it).action;
			}
