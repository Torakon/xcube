#include "AIController.h"

AIController::AIController() : tileSize(0), mapX(0), mapY(0) {
	
}

AIController::~AIController() {
	walkable.erase;
} 

void AIController::addMap(int tile, int xPixel, int yPixel, std::vector<std::shared_ptr<Rect>> map) {
	if ((xPixel%tile != 0) || (yPixel%tile != 0)) {
		try {
			throw 1;
		}
		catch (int e) {
			std::cout << "An exception occurred. Please make sure that the Tile Size fits into the Map Size" << std::endl;
			}
	}
	tileSize = tile;
	mapX = xPixel / tile;
	mapY = yPixel / tile;

	//On occasion of n+1 maps, Deletes old Node data
	/*while (!walkable.empty()) {
		for each (Node x in walkable[0]) {
			delete x;
		}
	}*/
	walkable.clear();

	//Following code creates a gameworld based on pixel size and tile size fed to it. 2d Vector data structure used for visualisation
	walkable.resize(mapY);
	bool wCheck = true;
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			Node push = Node(j, i, tileSize, tileSize);

			wCheck = true;
			for (auto block : map) {
				if (Rect(j * tileSize, i * tileSize, tileSize, tileSize).intersects(*block)) {
					wCheck = false;
					break;
				}
			}
			push.setWalkable(wCheck);
			walkable[i].push_back(std::make_shared<Node>(push));
		}
	}	
	//The below code to print a visualisation of the game world to console is edited from "Vlad from Moscow" - https://stackoverflow.com/questions/42249303/how-can-i-push-back-data-in-a-2d-vector-of-type-int 05/03/2020
	/*for (const auto v : walkable)
	{
		for (bool x : v) std::cout << x << ' ';
		std::cout << std::endl;
	}*/
}

void AIController::givePath(Entity* seeker, Entity* dest) {
	int sight = seeker->getSight();
	int seekerX = seeker->x / tileSize;
	int seekerY = seeker->y / tileSize;
	int destX = dest->x / tileSize;
	int destY = dest->y / tileSize;

	//basic 'line of sight' currently does not take walls into account
	bool sighted = false, complete = false;
	if (Rect((seekerX * tileSize) - (sight * tileSize), (seekerY * tileSize) - (sight * tileSize),
		(sight *2 ) * tileSize, (sight * 2) * tileSize).intersects(dest->collider)) {
		sighted = true;
	}

	//if destination entity is in sight then chase otherwise, 'patrol'
	if (sighted) {
		path = search.AStarSearch(Point2{ seekerX, seekerY }, Point2{ destX, destY }, walkable);
		seeker->setSpeed(2); //getChaseSpeed?
	}
	else if (seeker->getPatrol()) {
		seeker->setSpeed(1); //getPatrolSpeed?
		int randomX = std::rand() % (sight + 1) - (sight / 2); 
		int randomY = std::rand() % (sight + 1) - (sight / 2);
		if (randomX + seekerX < 0) {
			randomX *= -1;
		}
		if (randomY + seekerY < 0) {
			randomY *= -1;
		}
		if (randomX + seekerX > mapX - 1) {
			randomX *= -1;
		}
		if (randomY + seekerY > mapY - 1) {
			randomY *= -1;
		}

		randomX += seekerX;
		randomY += seekerY;

		//std::cout << "Current " << seekerX << " " << seekerY << std::endl; //DEBUG
		//std::cout << "Destination " << randomX << " " << randomY << std::endl; //DEBUG

		path = search.AStarSearch(Point2{ seekerX, seekerY }, Point2{ randomX, randomY }, walkable, sight); //LOOK AT THIS: why does the AI drift to the lower right corner?? is it the depth thats causing this
	}
	seeker->aiMovePath(path);
	/*for (Point2 x : path) { //DEBUG: Print path
		std::cout << x.x << x.y << std::endl;
	}*/
}