#ifndef __AI_CONTROLLER_H__
#define __AI_CONTROLLER_H__

#include "../GraphicsEngine.h"
#include "../Entity.h"
#include "../EngineCommon.h"
#include "Node.h"
#include "AStar.h"
#include <vector>
#include <list>
#include <memory>

class AIController {
	private:
		int tileSize;	/**< Size of the tiles used in map generation */
		int mapX;		/**< Size of map along X coord */
		int mapY;		/**< Size of map along Y coord */

		std::vector<std::vector<std::shared_ptr<Node> >> walkable;	/**< Data collection storing walkable and non walkable tiles */

		std::vector<Point2> path;	/**< Data collection storing coords of calculated path */

		AStar search;	/**< Reference to AStar algorithm */

	public:
		AIController();
		~AIController();

		/**
		 *	Clears stored map data before adding new given map data to walkable, sets movable directions from each node.
		 *	@param tile		size of tiles used in map
		 *	@param xPixel	size of map along X coord
		 *	@param yPixel	size of map along Y coord
		 *	@param map		map data provided by game
		 */
		void addMap(int tile, int xPixel, int yPixel, std::vector<std::shared_ptr<Rect>> map);

		/**
		 *	If second Entity in range, path directly to it's location. Otherwise react to first Entity settings.
		 *	E.G If first Entity is set to patrol, path to a random patrol point in it's sight radius
		 *	@param seeker	The Entity that desires a path
		 *	@param dest		The Primary target of first Entity.
		 */
		void givePath(Entity* seeker, Entity* dest);
		/**
		 * Override, path Entity straight to specific point
		 * @param seeker	The Entity that desires a path
		 * @param dest		The destination tile location
		 */
		void givePath(Entity* seeker, Point2 dest);

		/**
		 * Generate a path from one point to another and return if one exists or not
		 * @param begin		The start location of the search
		 * @param key		The key end point of the search
		 * @return			Whether or not a complete path exists
		 */
		bool checkPossible(Point2 begin, Point2 key);

		/** 
		 *	Function intended for the debug of map data, prints map to console
		 */
		void DEBUG_ShowMap();
};
#endif
