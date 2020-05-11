#ifndef __A_STAR_H__
#define __A_STAR_H__

#include "../Entity.h"
#include "Node.h"
#include <vector>

class AStar {
private:
	std::vector<std::shared_ptr<Node >> open;
	std::vector<std::shared_ptr<Node >> close;

	std::shared_ptr<Node> compare;
	std::shared_ptr<Node> bckNode;

	Node origin;
	Node goal;

	std::vector<std::shared_ptr<Node >> successor;
	std::vector<Point2> path;
	std::vector<Point2> pathFlipped;

	int heu;
	int dif;
	bool depthConstraint = false;
public:
	AStar();

	std::vector<Point2> AStarSearch(Point2, Point2, std::vector<std::vector<std::shared_ptr<Node> >>, int);
};

#endif
