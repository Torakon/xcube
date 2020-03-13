#include "AIController.h"

AIController::AIController() : tileSize(0), mapX(0), mapY(0) {
	
}

void AIController::addMap(int tile, int xPixel, int yPixel, std::vector<std::shared_ptr<Rect>> map) {
	if ((tileSize%xPixel != 0) || (tileSize%yPixel != 0)) {
		//raise map size error
	}
	tileSize = tile;
	mapX = xPixel / tile;
	mapY = yPixel / tile;

	//Following code creates a gameworld based on pixel size and tile size fed to it. 2d Vector data structure used for visualisation
	walkable.resize(mapY);
	bool wCheck = true;
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			wCheck = true;
			for (auto block : map) {
				if (Rect((j*tileSize), (i*tileSize), tileSize, tileSize).intersects(*block)) {
					wCheck = false;
					break;
				}
			}
			walkable[i].push_back(wCheck);
		}
	}	
	//This code to print a visualisation of the game world to console is edited from "Vlad from Moscow" - https://stackoverflow.com/questions/42249303/how-can-i-push-back-data-in-a-2d-vector-of-type-int 05/03/2020
	/*for (const auto v : walkable)
	{
		for (bool x : v) std::cout << x << ' ';
		std::cout << std::endl;
	}*/
}
void AIController::getPath(Entity* seeker, Entity* dest) {
	Node * compare = nullptr;
	int seekerX = seeker->x / tileSize;
	int seekerY = seeker->y / tileSize;
	int destX = dest->x / tileSize;
	int destY = dest->y / tileSize;
	int xtemp = seekerX, ytemp = seekerY;

	bool sighted = false, complete = false;;
	while (!complete) {
		if (xtemp < destX) {
			xtemp += 1;
		}
		else if (xtemp > destX) {
			xtemp -= 1;
		}
		if (ytemp < destY) {
			ytemp += 1;
		}
		else if (ytemp > destY) {
			ytemp -= 1;
		}
		if (walkable[xtemp][ytemp] == false) {
			complete = true;
			sighted = false;
			break;
		}
		if ((xtemp == destX) && (ytemp == destY)) {
			complete = true;
			break;
		}
		sighted = true;
	}
	if (sighted) {
		AStar::AStar(Point2{ seekerX, seekerY }, Point2{ destX, destY }, walkable);
	}
	else {
		int randomX = std::rand() % 10 -5;
		int randomY = std::rand() % 10 -5;
		randomX = randomX + seekerX;
		randomY = randomY + seekerY;
		AStar::AStar(Point2{ seekerX, seekerY }, Point2{ randomX, randomY }, walkable); //implement random area around seeker points to 'patrol'
	}
}

//backup of working a* algo
/*Node start = { seekerX, seekerY, 0, 0 };
	Node goal = { destX, destY, 0, 0 };
	std::vector<Node*> successor;
	int heu = ((destX - seekerX)) + ((destY - seekerY));

	open.push_back(new Node(seekerX, seekerY, 0, heu));

	while (!open.empty()) {
		//get successors
		compare = open.back();
		for (Node * x : open) {
			if (x->getWeight() < compare->getWeight()) {
				compare = x;
			}
		}
		close.push_back(compare);
		open.erase(std::find(open.begin(), open.end()-1, compare));

		if (compare->tilX < mapX-1)
		successor.push_back(new Node(compare->tilX + 1, compare->tilY, compare->disG + 1, ((dest->x - (compare->tilX + 1))) + ((dest->y - compare->tilY))));
		if (compare->tilY < mapY-1)
		successor.push_back(new Node(compare->tilX, compare->tilY + 1, compare->disG + 1, ((dest->x - compare->tilX)) + ((dest->y - (compare->tilY - 1)))));
		if (compare->tilX > 0)
		successor.push_back(new Node(compare->tilX - 1, compare->tilY, compare->disG + 1, ((dest->x - (compare->tilX - 1))) + ((dest->y - compare->tilY))));
		if (compare->tilY > 0)
		successor.push_back(new Node(compare->tilX, compare->tilY - 1, compare->disG + 1, ((dest->x - compare->tilX)) + ((dest->y - (compare->tilY - 1)))));
		//old code to remove successor, replaced with above if statements
		/*for (Node* x : successor) { //why do I need to do this twice
			if (x->tilX < 0) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilY < 0) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilX >= mapX) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilY >= mapY) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			}
		} *//*

		while (!successor.empty()) {
			if (walkable[successor.back()->tilY][successor.back()->tilX] == true) {
				bool closeContained = false;
				bool openContained = false;
				for (Node * x : close) {
					if (x->getID() == successor.back()->getID()) {
						closeContained = true;
						break;
					}
				}
				if (!closeContained) {
					for (Node * x : open) {
						if (x->getID() == successor.back()->getID()) {
							openContained = true;
							if (x->disG > successor.back()->disG) {
								successor.back()->addParent(compare);
								x = successor.back();
							}
							break;
						}
					}
					if (!openContained) {
						successor.back()->addParent(compare);
						open.push_back(successor.back());
					}
				}
			}
			successor.pop_back();
		}
		if (compare->getID() == goal.getID()) {
			open.clear();
		}
	}
	std::vector<Node*> path;
	path.push_back(compare->backtrack());
	Node * bckNode = compare->backtrack();
	while (bckNode->getID() != start.getID()) {
		path.push_back(bckNode->backtrack());
		bckNode = bckNode->backtrack();
	}

	for (Node * x : path) {
		std::cout << x->getID() << std::endl;
	} */