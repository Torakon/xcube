#include "AIController.h"

AIController::AIController() : tileSize(0), mapX(0), mapY(0) {

}

AIController::~AIController() {
	walkable.clear();
}

void AIController::addMap(int tile, int xPixel, int yPixel, std::vector<std::shared_ptr<Rect>> map) {
	// If tile cannot fit neatly into map size
	if (((xPixel % tile) != 0) || ((yPixel % tile) != 0)) {
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

	// On occasion of n+1 maps/levels, Removes old Node data
	walkable.clear();

	// Following code creates a gameworld based on pixel size and tile size fed to it. 2d Vector data structure used for representation
	walkable.resize(mapY);
	bool wCheck = true;
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			Node push = Node(j, i, 0, 0); /**< A Node generated based on map coordinates to be pushed onto walkable */

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

	// Check and set moveable edges
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			if (i > 0) {
				if (walkable[i - 1][j]->isWalkable()) {
					walkable[i][j]->setEdge(Node::NORTH, true);
				}
			}
			if (i < walkable.size()-1) {
				if (walkable[i + 1][j]->isWalkable()) {
					walkable[i][j]->setEdge(Node::SOUTH, true);
				}
			}
			if (j > 0) {
				if (walkable[i][j - 1]->isWalkable()) {
					walkable[i][j]->setEdge(Node::WEST, true);
				}
			}
			if (j < walkable[0].size()-1) {
				if (walkable[i][j + 1]->isWalkable()) {
					walkable[i][j]->setEdge(Node::EAST, true);
				}
			}
		}
	}
}

void AIController::givePath(Entity* seeker, Entity* dest) {
	int sight = seeker->getSight();				/**< The sight radius of seeker Entity */
	int seekerX = seeker->getX() / tileSize;	/**< Tile location X of seeker Entity */
	int seekerY = seeker->getY() / tileSize;	/**< Tile location Y of seeker Entity */
	int destX = dest->getX() / tileSize;		/**< Tile location X of destination Entity */
	int destY = dest->getY() / tileSize;		/**< Tile location Y of destination Entity */

	path.clear();
	// If second entity is in range
	bool sighted = false;	/**< Flag to show if Entity is in range */
	if (sight > 0) {
		if (Rect((seekerX * tileSize) - (sight * tileSize), (seekerY * tileSize) - (sight * tileSize),
			(sight * 2) * tileSize, (sight * 2) * tileSize).intersects(dest->getCollider())) {
			sighted = true;
		}
	}
	else { 
		sighted = true; 
	}

	// If destination entity is in sight then chase otherwise, check for patrol behaviour and react
	if (sighted) {
		sighted = false;
		path = search.AStarSearch(Point2{ seekerX, seekerY }, Point2{ destX, destY }, walkable, 0);
	}
	else if (seeker->getPatrol()) {
		int randomX = std::rand() % (sight + 1) - (sight / 2);	/**< Random point X within sight range of Entity */
		int randomY = std::rand() % (sight + 1) - (sight / 2);	/**< Random point Y within sight range of Entity */
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

		path = search.AStarSearch(Point2{ seekerX, seekerY }, Point2{ randomX, randomY }, walkable, sight);
	}

	seeker->aiMovePath(path);
}

void AIController::givePath(Entity* seeker, Point2 dest) {
	int sight = seeker->getSight();
	int seekerX = seeker->getX() / tileSize;
	int seekerY = seeker->getY() / tileSize;
	int destX = dest.x / tileSize;
	int destY = dest.y / tileSize;

	path = search.AStarSearch(Point2{ seekerX, seekerY }, Point2{ destX, destY }, walkable, sight);

	seeker->aiMovePath(path);
}

bool AIController::checkPossible(Point2 begin, Point2 key) {
	std::vector<Point2> check = search.AStarSearch(begin, key, walkable, 0); /**< Data collection storing coords of calculated path */
	if (!check.empty()) {
		Point2 point = check[check.size() - 1]; /**< Last coord in provided path if not empty */
		if (point.x == key.x && point.y == key.y) {
			return true;
		}
	}
	return false;
}

void AIController::DEBUG_ShowMap() {
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			std::cout << !walkable[i][j]->isWalkable() << ' ';
		}
		std::cout << std::endl;
	}
}