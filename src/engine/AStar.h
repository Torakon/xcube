#ifndef __A_STAR_H__
#define __A_STAR_H__

#include "Entity.h"
#include "Node.h"
#include <vector>

class AStar {
private:
	std::vector<Node*> open, close;
public:
	AStar(Point2, Point2, std::vector<std::vector<bool >>);
};

#endif
