#include "AStar.h"

AStar::AStar() : origin(0,0,0,0), goal(0,0,0,0) {

}

AStar::~AStar(){

}

std::vector<Point2> AStar::AStarSearch(Point2 start, Point2 dest, std::vector<std::vector<Node* >> mapData) { //maybe add tilesize to pass to nodes?
	origin = { start.x, start.y, 0, 0 };
	goal = { dest.x, dest.y, 0, 0 };

	path.clear();

	mapData[start.y][start.x]->setVal(0, (dest.x - start.x) + (dest.y - start.y));
	open.push_back(mapData[start.y][start.x]);

	while (!open.empty()) {
		//get successors
		compare = open.back();
		for (Node * x : open) {
			if (x->getWeight() < compare->getWeight()) {
				compare = x;
			}
		}
		close.push_back(compare);
		open.erase(std::find(open.begin(), open.end() - 1, compare));
		//check if cell exists
		if (compare->tilX < mapData[0].size() - 1) {
			mapData[compare->tilY][compare->tilX + 1]->setVal(compare->disG + 1, (dest.x - (compare->tilX + 1)) + (dest.y - compare->tilY));
			successor.push_back(mapData[compare->tilY][compare->tilX + 1]);
		}
		if (compare->tilY < mapData.size() - 1) {
			mapData[compare->tilY + 1][compare->tilX]->setVal(compare->disG + 1, (dest.x - (compare->tilX)) + (dest.y - compare->tilY + 1)); //was -1 in original?
			successor.push_back(mapData[compare->tilY + 1][compare->tilX]);
		}
		if (compare->tilX > 0) {
			mapData[compare->tilY][compare->tilX - 1]->setVal(compare->disG + 1, (dest.x - (compare->tilX - 1)) + (dest.y - compare->tilY));
			successor.push_back(mapData[compare->tilY][compare->tilX - 1]);
		}
		if (compare->tilY > 0) {
			mapData[compare->tilY - 1][compare->tilX]->setVal(compare->disG + 1, (dest.x - (compare->tilX)) + (dest.y - compare->tilY - 1));
			successor.push_back(mapData[compare->tilY - 1][compare->tilX]);
		}
		//check successors
		while (!successor.empty()) {
			if (mapData[successor.back()->tilY][successor.back()->tilX]->isWalkable() == true) {
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
	//path.push_back(compare);
	bckNode = compare;

	while (bckNode->getID() != origin.getID()) {
		path.insert(path.begin(), Point2{ bckNode->tilX, bckNode->tilY });
		bckNode = bckNode->backtrack();
	}
	close.clear();
	return path;
}
std::vector<Point2> AStar::AStarSearch(Point2 start, Point2 dest, std::vector<std::vector<Node* >> mapData, int depth) {
	origin = { start.x, start.y, 0, 0 };
	goal = { dest.x, dest.y, 0, 0 };

	path.clear();

	mapData[start.y][start.x]->setVal(0, (dest.x - start.x) + (dest.y - start.y));
	open.push_back(mapData[start.y][start.x]);

	while (!open.empty()) {
		//get successors
		compare = open.back();
		for (Node * x : open) {
			if (x->getWeight() < compare->getWeight()) {
				compare = x;
			}
		}
		close.push_back(compare);
		open.erase(std::find(open.begin(), open.end() - 1, compare));
		//check if cell exists
		if (compare->tilX < mapData[0].size() - 1) {
			mapData[compare->tilY][compare->tilX + 1]->setVal(compare->disG + 1, (dest.x - (compare->tilX + 1)) + (dest.y - compare->tilY));
			successor.push_back(mapData[compare->tilY][compare->tilX + 1]);
		}
		if (compare->tilY < mapData.size() - 1) {
			mapData[compare->tilY + 1][compare->tilX]->setVal(compare->disG + 1, (dest.x - (compare->tilX)) + (dest.y - compare->tilY + 1)); //was -1 in original?
			successor.push_back(mapData[compare->tilY + 1][compare->tilX]);
		}
		if (compare->tilX > 0) {
			mapData[compare->tilY][compare->tilX - 1]->setVal(compare->disG + 1, (dest.x - (compare->tilX - 1)) + (dest.y - compare->tilY));
			successor.push_back(mapData[compare->tilY][compare->tilX - 1]);
		}
		if (compare->tilY > 0) {
			mapData[compare->tilY - 1][compare->tilX]->setVal(compare->disG + 1, (dest.x - (compare->tilX)) + (dest.y - compare->tilY - 1));
			successor.push_back(mapData[compare->tilY - 1][compare->tilX]);
		}
		//check successors
		while (!successor.empty()) {
			if (mapData[successor.back()->tilY][successor.back()->tilX]->isWalkable() == true) {
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
		if (compare->getID() == goal.getID()||compare->disG >= depth) {
			open.clear();
		}
	}
	bckNode = compare;

	while (bckNode->getID() != origin.getID()) {
		path.insert(path.begin(), Point2{ bckNode->tilX, bckNode->tilY });
		bckNode = bckNode->backtrack();
	}
	close.clear();
	return path;
}