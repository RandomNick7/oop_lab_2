//TODO: Fill up the half-empty classes below Fungi with working methods

// The building block of the world. Things can exist on them
class Tile{
	public:
		int altit = rand()%(3000+1)-500;		//[-500,2500]
		float water = rand()%(8000+1)/10;		//[0,800]
		float temp = rand()%(420+1)/10-11;		//[-11,31]
		int entity = 0;
};

// Entity used for water generation and transportation across the world
class Cloud{
	public:
		float water = 0;
	
		//When over a tile, rain down some amount of water (max 25%);
		void rain(Tile* ground){
			//The higher the altitude and the colder it is, the more it should rain
			float qty = water/4*(ground->altit+500)/3000*(35-ground->temp)/50;
			if(qty > 10 && ground->water+qty<=1000){
				water-=qty;
				ground->water+=qty;
			}
		}
};

//Warning! Plants & Trees may cause water cycle instability
//A grown up plant. Generates clouds through transpiration and food for animals
class Tree{
	private:
		int id = 1;
		int index;
		int energy;
		int health = 50;
		
		//Remove itself from tree vector
		void death(Tile* ground){
			if(health == 0){
				ground->water+=20;
				ground->entity = 0;
			}
		}
	public:
		int food;
		Tree(int i, int e){
			index = i;
			energy = e;
		};
		
		//Convert water to energy 
		void absorb(Tile* ground){
			if(ground->water-3>0 && energy<50){
				ground->water-=3;
				energy++;
			}else{
				if(energy>0){
					energy--;
				}else{
					health--;
					death(ground);
				}
			}
		}
		
		//Generate food that can be eaten by other animals
		void fruit(){
			if(energy>3 && food<10){
				energy-=3;
				food++;
			}
		}
		
		void transpirate(Cloud* cloud){
			cloud->water+=5;
		}
};

//The primordial "living" entity. Uses water to grow into trees. Serves as a food source for some animals 
class Plant{
	private:
		int energy = 5;
	public:
		int num = 0;
		
		//Suck water out of the ground at each time interval
		void absorb(Tile* ground){
			if(ground->water-num>0 && energy<20){
				ground->water-=num/5+1;
				energy+=num;
			}else{
				if(energy>0){
					energy--;
				}else if(num>0){
					num--;
					ground->water++;
					energy++;
				}
			}
		}
		
		//Grow based on existing energy and waterity
		void multiply(Tile* ground){
			if(energy>5 && num<=10 && rand()%100<(ground->water-300)/100){
				num++;
				energy-=3+num/5;
			}
		}
		
		//Evolve into a tree once a threshold has been reached
		void grow(Tile* ground){
			if(num>=10 && energy>=20){
				num-= 10;
				energy-=20;
				ground->entity = 1;
			}
		}
};

//Warning! Methods of classes below are untested!

//An animal's remains. Stationary entity that acts as a limited-time food source
class Corpse{
	private:
		int id = 2;
		int duration;
	public:
		int energy;
		Corpse(Tile* ground, int d, int e){
			duration = d;
			energy = e;
			ground->entity=id;
		}
		
		void decay(Tile* ground){
			duration--;
			if(duration == 0){
				ground->entity = 0;
			}
		}
};

//The Plant's evil twin. Grows in water spots and near corpses. Serves as a food source.
class Fungi{
	private:
		int energy = 3;
	public:
		int num = 0;
		void absorb(Tile* ground){
			if(ground->water-num>0 && energy<20){
				ground->water-=num/5+1;
				energy+=num;
			}else{
				if(energy>0){
					energy--;
				}else if(num>0){
					num--;
					ground->water++;
					energy++;
				}
			}
		}
		void multiply(Tile* ground){
			if(energy>3+num && num<=5 && rand()%100<(ground->water-200)/100){
				num++;
				energy-=3+num;
			}
		}
};

//Further classes could have probably been made using polymorphism... They have been done as a sketch, as no interactions are allowed anyway.

//Animals that primarily consume dead organic material
class Detritivore{
	private:
		int id = 3;
		int energy=11;
		int health=10;
		int attack=2;
		
		void death(){
			//Remove itself from the field, leave behind a corpse
		}
	public:
		void target(){
			if(energy<10){
				//Look for nearest corpse or fungus within 4 tiles
				//If corpse found, cache path to destination and move along it
				//Else, move randomly
			}else if(energy>15){
				//Set breeding flag to "on"
				//Look for another animal of the same kind with the flag on within range
				//If found, cache path, move towards them
				//Else, move randomly
			}
		}
		void hunger(){
			if(energy>0){
				energy--;
			}else if(health>0){
				health--;
			}else{
				death();
			}
		}
};


//Animals that consume plants
class Herbivore{
	private:
		int id = 4;
		int energy=14;
		int health=30;
		int attack=1;
		
		void death(){
			//Remove itself from the field, leave behind a corpse
		}
	public:
		void target(){
			if(energy<10){
				//Look for nearest grass tile within 3 tiles
				//If grass found, cache path to destination and move along it
				//Else, move randomly
			}else if(energy>15){
				//Set breeding flag to "on"
				//Look for another animal of the same kind with the flag on within range
				//If found, cache path, move towards them
				//Else, move randomly
			}
		}
		
		void hunger(){
			if(energy>0){
				energy--;
			}else if(health>0){
				health--;
			}else{
				death();
			}
		}
};

//Animals that consumes other animals
class Carnivore{
	private:
		int id = 5;
		int energy=12;
		int health=20;
		int attack=4;
		
		void death(){
			//Remove itself from the field, leave behind a corpse
		}
	public:
		void target(){
			if(energy<10){
				//Look for nearest corpse or occupied tile within 5 tiles
				//If corpse found, cache path to destination and move along it
				//If non-carnivore animal found, chase it down and attack it
				//Else, move randomly
			}else if(energy>15){
				//Set breeding flag to "on"
				//Look for another animal of the same kind with the flag on within range
				//If found, cache path, move towards them
				//Else, move randomly
			}
		}
		
		void hunger(){
			if(energy>0){
				energy--;
			}else if(health>0){
				health--;
			}else{
				death();
			}
		}
};

//Animals that consumes other animals
class Omnivore{
	private:
		int id = 6;
		int energy=13;
		int health=25;
		int attack=3;
		
		void death(){
			//Remove itself from the field, leave behind a corpse
		}
	public:
		void target(){
			if(energy<10){
				//Look for nearest tree, fungi or occupied tile within 5 tiles
				//If food or fungi found, cache path to destination and move along it
				//If non-omnivore animal found, chase it down and attack it
				//Else, move randomly
			}else if(energy>15){
				//Set breeding flag to "on"
				//Look for another animal of the same kind with the flag on within range
				//If found, cache path, move towards them
				//Else, move randomly
			}
		}
		
		void hunger(){
			if(energy>0){
				energy--;
			}else if(health>0){
				health--;
			}else{
				death();
			}
		}
};
