#ifndef __A_STAR_H__
#define __A_STAR_H__

#include "../Entity.h"
#include "Node.h"
#include <vector>

class AStar {
private:
	std::vector<Node*> open;
	std::vector<Node*> close;

	Node * compare;
	Node * bckNode;

	Node origin;
	Node goal;

	std::vector<Node *> successor;
	std::vector<Point2> path;

	int heu;
public:
	AStar();
	~AStar();
	std::vector<Point2> AStarSearch(Point2, Point2, std::vector<std::vector<Node* >>);
	std::vector<Point2> AStarSearch(Point2, Point2, std::vector<std::vector<Node* >>, int);
};

#endif
