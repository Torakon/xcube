#include "AIController.h"

AIController::AIController(int tile, int xPixel, int yPixel) : tileSize(tile), mapX(xPixel/tile), mapY(yPixel/tile) {
	
}

AIController::AIController(int tile, int xPixel, int yPixel, std::vector<std::shared_ptr<Rect>> map) : tileSize(tile), mapX(xPixel / tile), mapY(yPixel / tile) {
//Following code creates a gameworld based on pixel size and tile size fed to it. 2d Vector data structure used for visualisation
	walkable.resize(mapY);
	bool wCheck = true;
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			wCheck = true;
			for (auto block : map) {
				if (Rect(i*tileSize, j*tileSize, tileSize, tileSize).intersects(*block)) {
					wCheck = false;
					
					break;
				}
			}
			walkable[i].push_back(wCheck);
		}
	}
	
	//This code to print a visualisation of the map to console is edited from "Vlad from Moscow" - https://stackoverflow.com/questions/42249303/how-can-i-push-back-data-in-a-2d-vector-of-type-int 05/03/2020
	/*for (const auto v : walkable)
	{
		for (bool x : v) std::cout << x << ' ';
		std::cout << std::endl;
	}
	//std::cout << walkable[0][0] << std::endl;*/
}

