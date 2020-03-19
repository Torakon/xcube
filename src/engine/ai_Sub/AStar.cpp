#include "AStar.h"

AStar::AStar() : origin(0,0,0,0), goal(0,0,0,0) {

}

AStar::~AStar(){

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

	if (mapData[dest.y][dest.x]->isWalkable() == false) {
		path.insert(path.begin(), start);
		return path;
	}

	while (!open.empty()) {
		//get successors
		compare = open.back();
		for (std::shared_ptr<Node> x : open) {
			if (x->getWeight() < compare->getWeight()) {
				compare = x;
			}
		}
		close.push_back(compare);
		open.erase(std::find(open.begin(), open.end() - 1, compare));
		int dif = 0;
		//check if cell exists
		if (compare->tilX < mapData[0].size() - 1) {
			dif = ((dest.x - (compare->tilX + 1)) ^ 2) + ((dest.y - compare->tilY) ^ 2);
			if (dif < 0) dif *= -1;
			mapData[compare->tilY][compare->tilX + 1]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compare->tilY][compare->tilX + 1]);
		}
		if (compare->tilY < mapData.size() - 1) {
			dif = ((dest.x - (compare->tilX)) ^ 2) + ((dest.y - compare->tilY + 1) ^ 2);
			if (dif < 0) dif *= -1;
			mapData[compare->tilY + 1][compare->tilX]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compare->tilY + 1][compare->tilX]);
		}
		if (compare->tilX > 0) {
			dif = ((dest.x - (compare->tilX - 1)) ^ 2) + ((dest.y - compare->tilY) ^ 2);
			if (dif < 0) dif *= -1;
			mapData[compare->tilY][compare->tilX - 1]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compare->tilY][compare->tilX - 1]);
		}
		if (compare->tilY > 0) {
			dif = ((dest.x - (compare->tilX)) ^ 2) + ((dest.y - compare->tilY - 1) ^ 2);
			if (dif < 0) dif *= -1;
			mapData[compare->tilY - 1][compare->tilX]->setGHCost(compare->getGVal() + 1, dif);
			successor.push_back(mapData[compare->tilY - 1][compare->tilX]);
		}
		//check successors
		while (!successor.empty()) {
			if (mapData[successor.back()->tilY][successor.back()->tilX]->isWalkable() == true) {
				bool closeContained = false;
				bool openContained = false;
				for (std::shared_ptr<Node> x : close) {
					if (x->getID() == successor.back()->getID()) {
						closeContained = true;
						break;
					}
				}
				if (!closeContained) {
					for (std::shared_ptr<Node> x : open) {
						if (x->getID() == successor.back()->getID()) {
							openContained = true;
							if (x->getGVal() > successor.back()->getGVal()) {
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
		if (depthConstraint) {
			if (compare->getID() == goal.getID() || compare->getWeight() > depth) {
				open.clear();
			}
		} else if (compare->getID() == goal.getID()) { open.clear(); }
	}
	bckNode = compare;

	while (bckNode->getID() != origin.getID()) {
		//std::cout << goal.getID() << std::endl; //DEBUG
		//std::cout << "X" << bckNode->tilX << " Y" << bckNode->tilY << " weight" << bckNode->getWeight() << std::endl; //DEBUG
		path.insert(path.begin(), Point2{ bckNode->tilX, bckNode->tilY });
		bckNode = bckNode->backtrack();
	}
	close.clear();
	//std::cout << std::endl; //DEBUG

	return path;
}