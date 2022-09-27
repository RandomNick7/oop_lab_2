//TODO !Important: Empty class methods are meant to be interactions. They haven't been added yet as they aren't required, but should be added ASAP

//TODO: Malloc everything and pass by reference to functions to avoid making the stack explode (especially with larger worlds)
//TODO: Come up with a clever way to add all drawn layers on top of each other
//TODO: Make rain cool down the ground, evaporation heat it up slightly, add some sort of temperature shift

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

#define W 30		//Width
#define H 15		//Height

#include "class.h"
#include "helper_fun.h"
#include "world_fun.h"

int main(){
	srand(time(0));
	Tile world[H][W];
	Cloud sky[H][W];
	Plant grass[H][W];
	
	smoothen(world);
	
	while(true){
		form_weather(world,sky);
		water_transfer(world);
		grow_grass(world,grass);
		show_world(world,sky,grass);
		this_thread::sleep_for(chrono::milliseconds(200));
	}
}
