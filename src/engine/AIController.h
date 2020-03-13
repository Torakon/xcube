#ifndef __AI_CONTROLLER_H__
#define __AI_CONTROLLER_H__

#include "GraphicsEngine.h"
#include "Entity.h"
#include "Node.h"
#include "AStar.h"
#include <vector>
#include <list>

class AIController {
	private:
		int tileSize;
		int mapX;
		int mapY;
		std::vector<Point2> path;
		std::vector<Node*> open, close;
	public:
		std::vector<std::vector<bool >> walkable;
		AIController();
		void addMap(int, int, int, std::vector<std::shared_ptr<Rect>>);
		void givePath(Entity*, Entity*);
};
#endif
