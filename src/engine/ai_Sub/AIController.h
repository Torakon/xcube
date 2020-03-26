#ifndef __AI_CONTROLLER_H__
#define __AI_CONTROLLER_H__

#include "../GraphicsEngine.h"
#include "../Entity.h"
#include "../EngineCommon.h"
#include "Node.h"
#include "AStar.h"
#include <vector>
#include <list>

class AIController {
	private:
		int tileSize;
		int mapX;
		int mapY;

		std::vector<std::vector<std::shared_ptr<Node> >> walkable;

		std::vector<Point2> path;
		AStar search;
		//Point2 patrolCent;
	public:
		AIController();
		~AIController();

		void addMap(int, int, int, std::vector<std::shared_ptr<Rect>>);

		void givePath(Entity*, Entity*);
		void givePath(Entity*, Point2);

		bool checkPossible(Point2 begin, Point2 key);
};
#endif
