#ifndef __AI_CONTROLLER_H__
#define __AI_CONTROLLER_H__

#include "GraphicsEngine.h"
#include <vector>

class AIController {
	private:
		int tileSize;
		int mapX;
		int mapY;
		std::vector<std::vector<bool >> walkable;
	public:
		AIController(int, int, int);
		AIController(int, int, int, std::vector<std::shared_ptr<Line2i>>);
};
#endif
