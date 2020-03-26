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
		int cY = compare->getY();
		int cX = compare->getX();

		//get successors if walkable in the four directions
		if (compare->getEdge(Node::NORTH)) {
			dif = ((dest.x - cX) ^ 2) + ((dest.y - (cY - 1)) ^ 2); //calculate hue
			if (dif < 0) {
				dif *= -1; //absolute value
			}
			mapData[cY - 1][cX]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[cY - 1][cX]);
		}
		if (compare->getEdge(Node::SOUTH)) {
			dif = ((dest.x - cX) ^ 2) + ((dest.y - (cY + 1)) ^ 2);
			if (dif < 0) {
				dif *= -1;
			}
			mapData[cY + 1][cX]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[cY + 1][cX]);
		}
		if (compare->getEdge(Node::WEST)) {
			dif = ((dest.x - (cX - 1)) ^ 2) + ((dest.y - cY) ^ 2);
			if (dif < 0) {
				dif *= -1;
			}
			mapData[cY][cX - 1]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[cY][cX - 1]);
		}
		if (compare->getEdge(Node::EAST)) {
			dif = ((dest.x - (cX + 1)) ^ 2) + ((dest.y - cY) ^ 2);
			if (dif < 0) {
				dif *= -1;
			}
			mapData[cY][cX + 1]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[cY][cX + 1]);
		}
		
		//check successors
		while (!successor.empty()) {
			bool closeContained = false;
			bool openContained = false;
			for (std::shared_ptr<Node> c : close) {
				//if in close list do nothing
				if (c->getID() == successor.back()->getID()) {
					closeContained = true;
					break;
				}
			}

			if (!closeContained) {
				for (std::shared_ptr<Node> o : open) {
					//if not in close but in open
					if (o->getID() == successor.back()->getID()) {
						openContained = true;
						//if entry in open is part of a longer path
						if (o->getGVal() > successor.back()->getGVal()) {
							successor.back()->addParent(compare);
							o = successor.back();
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
		path.insert(path.begin(), Point2{ bckNode->getX(), bckNode->getY() });
		bckNode = bckNode->backtrack();
	}
	close.clear();

	return path;
}