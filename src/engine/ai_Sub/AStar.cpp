#include "AStar.h"

AStar::AStar() : origin(0,0,0,0), goal(0,0,0,0) {

}

std::vector<Point2> AStar::AStarSearch(Point2 start, Point2 dest, std::vector<std::vector<std::shared_ptr<Node> >> mapData, int depth) {
	origin = { start.x, start.y, 0, 0 };
	goal = { dest.x, dest.y, 0, 0 };

	if (depth > 0) {
		depthConstraint = true;
	}
	else { depthConstraint = false; }

	path.clear();

	mapData[start.y][start.x]->setGHCost(0, ((dest.x - start.x) ^ 2) + ((dest.y - start.y) ^ 2));
	open.push_back(mapData[start.y][start.x]);

	//if start point not walkable, don't bother making path
	if (mapData[dest.y][dest.x]->isWalkable() == false) {
		path.insert(path.begin(), start);
		return path;
	}

	while (!open.empty()) {
		//get successors
		compare = open.back();
		//get node most likely to be way forward
		for (std::shared_ptr<Node> x : open) {
			if (x->getWeight() < compare->getWeight()) {
				compare = x;
			}
		}
		close.push_back(compare);
		open.erase(std::find(open.begin(), open.end() - 1, compare));
		int compareY = compare->getY();
		int compareX = compare->getX();

		//get successors if walkable in the four directions
		if (compare->getEdge(Node::NORTH)) {
			dif = ((dest.x - compareX) ^ 2) + ((dest.y - (compareY - 1)) ^ 2); //calculate hue
			if (dif < 0) {
				dif *= -1; //absolute value
			}
			mapData[compareY - 1][compareX]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compareY - 1][compareX]);
		}
		if (compare->getEdge(Node::SOUTH)) {
			dif = ((dest.x - compareX) ^ 2) + ((dest.y - (compareY + 1)) ^ 2);
			if (dif < 0) {
				dif *= -1;
			}
			mapData[compareY + 1][compareX]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compareY + 1][compareX]);
		}
		if (compare->getEdge(Node::WEST)) {
			dif = ((dest.x - (compareX - 1)) ^ 2) + ((dest.y - compareY) ^ 2);
			if (dif < 0) {
				dif *= -1;
			}
			mapData[compareY][compareX - 1]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compareY][compareX - 1]);
		}
		if (compare->getEdge(Node::EAST)) {
			dif = ((dest.x - (compareX + 1)) ^ 2) + ((dest.y - compareY) ^ 2);
			if (dif < 0) {
				dif *= -1;
			}
			mapData[compareY][compareX + 1]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compareY][compareX + 1]);
		}
		
		//check successors
		while (!successor.empty()) {
			bool closeContained = false;
			bool openContained = false;
			for (std::shared_ptr<Node> closeCheck : close) {
				//if in close list do nothing
				if (closeCheck->getID() == successor.back()->getID()) {
					closeContained = true;
					break;
				}
			}

			if (!closeContained) {
				for (std::shared_ptr<Node> openCheck : open) {
					//if not in close but in open
					if (openCheck->getID() == successor.back()->getID()) {
						openContained = true;
						//if entry in open is part of a longer path
						if (openCheck->getGVal() > successor.back()->getGVal()) {
							successor.back()->addParent(compare);
							openCheck = successor.back();
						}
						break;
					}
				}

				if (!openContained) {
					successor.back()->addParent(compare);
					open.push_back(successor.back());
				}
			}

			successor.pop_back();
		}

		if (depthConstraint) {
			if ((compare->getID() == goal.getID()) || (compare->getWeight() > depth)) {
				open.clear();
			}
		} else if (compare->getID() == goal.getID()) { 
			open.clear(); 
		}
	}

	//backtrack from end to start to get the path
	bckNode = compare;
	while (bckNode->getID() != origin.getID()) {
		pathFlipped.push_back(Point2{ bckNode->getX(), bckNode->getY() });
		bckNode = bckNode->backtrack();
		if (!bckNode) {
			break;
		}
	}
	while (!pathFlipped.empty()) {
		path.push_back(pathFlipped.back());
		pathFlipped.pop_back();
	}
	close.clear();

	return path;
}