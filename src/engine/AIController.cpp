#include "AIController.h"

AIController::AIController() : tileSize(0), mapX(0), mapY(0) {
	
}

void AIController::addMap(int tile, int xPixel, int yPixel, std::vector<std::shared_ptr<Rect>> map) {
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
				if (Rect(i*tileSize, j*tileSize, tileSize, tileSize).intersects(*block)) {
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
	}
	//std::cout << walkable[0][0] << std::endl;*/
}
void AIController::getPath(Entity* seeker, Entity* dest) {
	Node * compare = nullptr;
	int seekerX = seeker->x / tileSize;
	int seekerY = seeker->y / tileSize;
	int destX = dest->x / tileSize;
	int destY = dest->y / tileSize;
	Node start = { seekerX, seekerY, 0, 0 };
	Node goal = { destX, destY, 0, 0 };
	std::vector<Node*> successor;
	int heu = ((destX - seekerX) ^ 2) + ((destY - seekerY)^2);

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

		successor.push_back(new Node(compare->tilX + 1, compare->tilY, compare->disG + 1, ((dest->x - (compare->tilX + 1)) ^ 2) + ((dest->y - compare->tilY) ^ 2)));
		successor.push_back(new Node(compare->tilX, compare->tilY + 1, compare->disG + 1, ((dest->x - compare->tilX) ^ 2) + ((dest->y - (compare->tilY - 1)) ^ 2)));
		successor.push_back(new Node(compare->tilX - 1, compare->tilY, compare->disG + 1, ((dest->x - (compare->tilX - 1)) ^ 2) + ((dest->y - compare->tilY) ^ 2)));
		successor.push_back(new Node(compare->tilX, compare->tilY - 1, compare->disG + 1, ((dest->x - compare->tilX) ^ 2) + ((dest->y - (compare->tilY - 1)) ^ 2)));

		/*for (Node* x : successor) {
			std::cout << x->getID() << "  " << std::endl;
		}
		std::cout << std::endl; */

		for (Node* x : successor) {
			if (x->tilX < 0) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilY < 0) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilX >= mapX) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilY >= mapY) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			}
		}
		for (Node* x : successor) { //why do I need to do this twice
			if (x->tilX < 0) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilY < 0) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilX >= mapX) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			} else if (x->tilY >= mapY) {
				successor.erase(std::find(successor.begin(), successor.end()-1, x));
			}
		}

		/*std::cout << std::endl;
		for (Node* x : successor) {
			std::cout << x->getID() << "  " << std::endl;
		}
		std::cout << std::endl; */
		while (!successor.empty()) {
			if (walkable[successor.back()->tilX][successor.back()->tilY] == true) {
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
	}
}
void AIController::getPath(Entity seeker, Point2 dest) {

}