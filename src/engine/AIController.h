#ifndef __AI_CONTROLLER_H__
#define __AI_CONTROLLER_H__

#include "../engine/GraphicsEngine.h"
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
		bool firstPass = true;

		std::vector<Point2> path;
		AStar search;
		Point2 patrolCent;
	public:
		std::vector<std::vector<Node* >> walkable;
		AIController();
		~AIController();
		void addMap(int, int, int, std::vector<std::shared_ptr<Rect>>);
		void givePath(Entity*, Entity*);
};
#endif
