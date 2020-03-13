#include "AStar.h"

AStar::AStar() : origin(0,0,0,0), goal(0,0,0,0) {

}
std::vector<Point2> AStar::AStarSearch(Point2 start, Point2 dest, std::vector<std::vector<bool >> mapData) { //maybe add tilesize to pass to nodes?
	origin = { start.x, start.y, 0, 0 };
	goal = { dest.x, dest.y, 0, 0 };
	std::vector<Node*> successor;
	heu = (dest.x - start.x) + (dest.y - start.y);

	open.push_back(new Node(start.x, start.y, 0, heu));

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
		if (compare->tilX < mapData[0].size() - 1)
			successor.push_back(new Node(compare->tilX + 1, compare->tilY, compare->disG + 1, (dest.x - (compare->tilX + 1)) + (dest.y - compare->tilY)));
		if (compare->tilY < mapData.size() - 1)
			successor.push_back(new Node(compare->tilX, compare->tilY + 1, compare->disG + 1, (dest.x - compare->tilX) + (dest.y - (compare->tilY - 1))));
		if (compare->tilX > 0)
			successor.push_back(new Node(compare->tilX - 1, compare->tilY, compare->disG + 1, (dest.x - (compare->tilX - 1)) + (dest.y - compare->tilY)));
		if (compare->tilY > 0)
			successor.push_back(new Node(compare->tilX, compare->tilY - 1, compare->disG + 1, (dest.x - compare->tilX) + (dest.y - (compare->tilY - 1))));
		//check successors
		while (!successor.empty()) {
			if (mapData[successor.back()->tilY][successor.back()->tilX] == true) {
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

	return path;
}
std::vector<Point2> AStar::AStarSearch(Point2 start, Point2 dest, std::vector<std::vector<bool >> mapData, int depth) {
	origin = { start.x, start.y, 0, 0 };
	goal = { dest.x, dest.y, 0, 0 };
	std::vector<Node*> successor;
	heu = (dest.x - start.x) + (dest.y - start.y);

	open.push_back(new Node(start.x, start.y, 0, heu));

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
		if (compare->tilX < mapData[0].size() - 1)
			successor.push_back(new Node(compare->tilX + 1, compare->tilY, compare->disG + 1, (dest.x - (compare->tilX + 1)) + (dest.y - compare->tilY)));
		if (compare->tilY < mapData.size() - 1)
			successor.push_back(new Node(compare->tilX, compare->tilY + 1, compare->disG + 1, (dest.x - compare->tilX) + (dest.y - (compare->tilY - 1))));
		if (compare->tilX > 0)
			successor.push_back(new Node(compare->tilX - 1, compare->tilY, compare->disG + 1, (dest.x - (compare->tilX - 1)) + (dest.y - compare->tilY)));
		if (compare->tilY > 0)
			successor.push_back(new Node(compare->tilX, compare->tilY - 1, compare->disG + 1, (dest.x - compare->tilX) + (dest.y - (compare->tilY - 1))));
		//check successors
		while (!successor.empty()) {
			if (mapData[successor.back()->tilY][successor.back()->tilX] == true) {
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

	return path;
}