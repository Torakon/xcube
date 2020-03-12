#ifndef __AI_CONTROLLER_H__
#define __AI_CONTROLLER_H__

#include "GraphicsEngine.h"
#include "Entity.h"
#include "Node.h"
#include <vector>
#include <list>

class AIController {
	private:
		int tileSize;
		int mapX;
		int mapY;
		std::vector<Node*> open, close;
	public:
		std::vector<std::vector<bool >> walkable;
		AIController();
		void addMap(int, int, int, std::vector<std::shared_ptr<Rect>>);
		void getPath(Entity *, Entity *);
		void getPath(Entity, Point2);
};
#endif
