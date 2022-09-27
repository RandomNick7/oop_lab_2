//Below functions are not part of classes as they're meant to apply to the entire map at once

//Make the world terrain be more smooth
void smoothen(Tile world[H][W]){
	//For simplicity, the world loops around the edges
	Tile new_world[H][W];
	cpy_world(&new_world[0],&world[0]);
	
	for(int i=0;i<H;i++){						//Take the average values of a tile's Moore neighborhood
		for(int j=0;j<W;j++){
			for(int k=0;k<9;k++){
				if(k==4){
					continue;
				}
				Tile curr = world[(i+(k/3)-1+H)%H][(j+(k%3)-1+W)%W];
				new_world[i][j].altit += curr.altit;
				new_world[i][j].temp += curr.temp;
				new_world[i][j].water += curr.water;
			}
			new_world[i][j].altit /= 9;
			new_world[i][j].temp = (new_world[i][j].temp/9)-(new_world[i][j].altit-1000)/250;	//The higher the tile, the colder it gets: [-500,2500] -> [-4,+4]
			new_world[i][j].water /= 9;
		}
	}
	cpy_world(&world[0],&new_world[0]);
}

//Simulate water flowing from one tile to other nearby ones
void water_transfer(Tile world[H][W]){
	Tile new_world[H][W];
	cpy_world(&new_world[0],&world[0]);
	//Spread the values to a tile's Moore neighborhood
	for(int i=0;i<H;i++){
		for(int j=0;j<W;j++){
			for(int k=0;k<9;k++){
				if(k==4){
					continue;
				}
				Tile curr = world[(i+(k/3)-1+H)%H][(j+(k%3)-1+W)%W];
				float delta = world[i][j].water/10*(world[i][j].altit-curr.altit)/3000;
				//If the water doesn't go over the max limit (1000) and doesn't go below a certain threshold, spread it
				if(delta>0 && new_world[(i+(k/3)-1+H)%H][(j+(k%3)-1+W)%W].water+delta < 1000 && new_world[i][j].water-delta > (2500-new_world[i][j].altit)/3){
					new_world[i][j].water-=delta;
					new_world[(i+(k/3)-1+H)%H][(j+(k%3)-1+W)%W].water+=delta;
				}
			}
		}
	}
	cpy_world(&world[0],&new_world[0]);
}

//Start creating clouds from evaporating water
void form_weather(Tile world[H][W], Cloud sky[H][W]){
	Tile new_world[H][W];
	Cloud new_sky[H][W];
	cpy_world(&new_world[0],&world[0]);
	cpy_sky(&new_sky[0],&sky[0]);
	
	for(int i=0;i<H;i++){
		for(int j=0;j<W;j++){	//Tiles that are warm and water enough produce clouds above them
			if(world[i][j].water>350){
				float qty = (world[i][j].water-350)/2*(world[i][j].temp)/10;
				if(new_sky[i][j].water+qty<1000){
					new_sky[i][j].water+=qty;
					new_world[i][j].water-=qty;
				}
			}
		}
	}
	
	cpy_sky(&sky[0],&new_sky[0]);
	
	for(int i=0;i<H;i++){						//Spread the values to a cloud's von Neumann neighborhood
		for(int j=0;j<W;j++){
			if(sky[i][j].water>0){
				new_sky[i][j].rain(&new_world[i][j]);	//Let a portion of the water in the cloud rain down on the ground
				float qty = sky[i][j].water/20;
				new_sky[(i-1+H)%H][j].water+=qty;
				new_sky[(i+1)%H][j].water+=qty;
				new_sky[i][(j-1+W)%W].water+=qty;
				new_sky[i][(j+1)%W].water+=qty;
				new_sky[i][j].water-=qty*4;
			}
		}
	}
	
	cpy_sky(&sky[0],&new_sky[0]);
	cpy_world(&world[0],&new_world[0]);
}

void grow_grass(Tile world[H][W], Plant grass[H][W]){
	Plant new_grass[H][W];
	cpy_grass(&new_grass[0],&grass[0]);
	for(int i=0;i<H;i++){
		for(int j=0;j<W;j++){
			new_grass[i][j].absorb(&world[i][j]);
			new_grass[i][j].multiply(&world[i][j]);
			if(grass[i][j].num == 0 && world[i][j].water > 300 && world[i][j].temp > 0){
				new_grass[i][j].num++;
			}
			new_grass[i][j].grow(&world[i][j]);
		}
	}
	cpy_grass(&grass[0],&new_grass[0]);
}

void show_world(Tile world[H][W], Cloud sky[H][W], Plant grass[H][W]){
	cout << "\033[0mTerrain: waterity, Temperature, Altitude\n";
	for(int i=0;i<H;i++){
		for(int j=0;j<W;j++){
			printf("\033[38;2;0;0;%dm%s",((int)world[i][j].water)*255/1000, "█");
		}
		cout << "\t";
		for(int j=0;j<W;j++){
			printf("\033[38;2;%d;0;0m%s",((int)world[i][j].temp+15)*255/50,"█");
		}
		cout << "\t";
		for(int j=0;j<W;j++){
			printf("\033[38;2;%d;%d;0m%s",(world[i][j].altit+500)*255/3000,(world[i][j].altit+500)*255/3000,"█");
		}
		cout << endl;
	}
	cout << "\033[0mOthers: Clouds, Living Entities\n";
	for(int i=0;i<H;i++){
		for(int j=0;j<W;j++){
			printf("\033[38;2;%d;%d;%dm%s",((int)sky[i][j].water)*255/1000,((int)sky[i][j].water)*255/1000,((int)sky[i][j].water)*255/1000, "█");
		}
		cout << "\t";
		for(int j=0;j<W;j++){
			if(world[i][j].entity!=1){
				printf("\033[38;2;0;%d;0m%s",grass[i][j].num*255/15,"▒");
			}else{
				printf("\033[38;2;199;234;70m%s","█");
			}
		}
		cout << endl;
	}
	for(int i=0;i<=2*H+1;i++){
		printf("\033[A");	//Moves cursor up to the 1st line for overwriting
	}
}

