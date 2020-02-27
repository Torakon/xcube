#include "AIController.h"

AIController::AIController(int tile, int xPixel, int yPixel) : tileSize(tile), mapX(xPixel/tile), mapY(yPixel/tile) {
	
}

AIController::AIController(int tile, int xPixel, int yPixel, std::vector<std::shared_ptr<Line2i>> map): tileSize(tile), mapX(xPixel/tile), mapY(yPixel/tile) {
	walkable.resize(mapY);

	//The below crashes game, too long -- how can I get a representation of the walkable area??
	/*for (int i = 0; i < mapX; i += tileSize) {
		for (int j = 0; j < mapY; i += tileSize) {
			for (auto line : map) {
				if (Rect(i, j, tileSize, tileSize).intersects(*line)) {
					//walkable[i].push_back(false);
					break;
				}
			}
			//walkable[i].push_back(true);
		}
	}*/
}