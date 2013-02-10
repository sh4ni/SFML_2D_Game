// codeschnipsel der alten kollisionsabfrage

// --> alte punktabfrage
		if( cp[0] && cp[1] && cp[10] && cp[11] && !cp[2] && !cp[3] ){ // Bei sehr starker Auslastung (sollte sehr selten anschlagen)
			blockUp = true;
		}
		if( cp[2] && cp[3] && cp[8] && cp[9] && !cp[0] && !cp[1] ){
			blockDown = true;
		}
		if( cp[0] && cp[2] && cp[5] && cp[7] && !cp[1] && !cp[3] ){
			blockLeft = true;
		}
		if( cp[1] && cp[3] && cp[4] && cp[6] && !cp[0] && !cp[2] ){
			blockRight = true;
		}
		if( cp[0] && cp[1] && cp[12] && cp[13] && !cp[10] && !cp[11] ){ // Bei starker Auslastung
			blockUp = true;
		}
		if( cp[2] && cp[3] && cp[12] && cp[13] && !cp[8] && !cp[9] ){
			blockDown = true;
		}
		if( (cp[0] && cp[4] || cp[1] && cp[5]) && (cp[8] || cp[9]) && (!cp[12] || !cp[13]) ){ // Bei Auslastung
			blockUp = true;
		}
		if( (cp[2] && cp[6] || cp[3] && cp[7]) && (cp[10] || cp[11]) && (!cp[12] || !cp[13]) ){
			blockDown = true;
		}
		if( (cp[0] && cp[8] || cp[2] && cp[10]) && (cp[4] || cp[6]) && (!cp[5] || !cp[7]) ){
			blockLeft = true;
		}
		if( (cp[1] && cp[9] || cp[3] && cp[11]) && (cp[5] || cp[7]) && (!cp[4] || !cp[6]) ){
			blockRight = true;
		}

// --> alte abfrage über boxen
	CollisionX.height = TILESIZE*2-COLLISIONTOLERANCE*2;
	CollisionX.width = TILESIZE;

	CollisionY.height = TILESIZE*2;
	CollisionY.width = TILESIZE-COLLISIONTOLERANCE*2;

	CollisionX.left = (int)x-TILESIZE/2;
	CollisionX.top = (int)y-TILESIZE+COLLISIONTOLERANCE;

	CollisionY.left = (int)x-TILESIZE/2+COLLISIONTOLERANCE;
	CollisionY.top = (int)y-TILESIZE;

	for(int i=0;i<9;i++){
		if(tx+(i%3) > 0 && ty+i/3 > 0 && ColMap[tx+(i%3)][ty+i/3] != NULL){
			if(CollisionX.intersects(*ColMap[tx+(i%3)][ty+i/3])){
			std::cout << i;
				if (!(i%3)){ // left
					//x += (Speed*ElapsedTime);
					blockLeft = true;
				}
				if( !((i-2)%3) ){ // right
					//x -= (Speed*ElapsedTime);
					blockRight = true;
				}
			}
			if(CollisionY.intersects(*ColMap[tx+(i%3)][ty+i/3])){
			std::cout << i;
				if( i <= 2 ){ // top
					//y += (Speed*ElapsedTime);
					blockUp = true;
				}
				if( i >= 6){ // bottom
					//y -= (Speed*ElapsedTime);
					blockDown = true;
				}
			}
		}
	}

// --> 