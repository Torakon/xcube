#ifndef __A_STAR_H__
#define __A_STAR_H__

#include "../Entity.h"
#include "Node.h"
#include <vector>
#include <memory>

class AStar {
private:
	std::vector<std::shared_ptr<Node >> open;	/**< Data collection storing Nodes that could be the next step in the path */
	std::vector<std::shared_ptr<Node >> close;	/**< Data collection storing Nodes that have been declared wanted */

	std::shared_ptr<Node> compare;	/**< The node currently being looked at */
	std::shared_ptr<Node> bckNode;	/**< Backtracking node to calculate path from close vector */

	Node origin;	/**< Node at start of search */
	Node goal;		/**< Desired Node at end of search */

	std::vector<Node> successor;		/**< Data collection storing the Node on each edge of the current Node */
	std::vector<Point2> path;			/**< True path followable by current Entity code */
	std::vector<Point2> pathFlipped;	/**< Initial path calculated by back tracking */

	int newH;						/**< Heuristic, commonly known as H, estimation of distance from 'here' to goal */
	bool depthConstraint = false;	/**< Whether or not there is a limit to the amount of steps available */

	/**
	 * Internal function that calculates and sets an edge node's new hueristic
	 * @param mapData	The current map representation, used to set the newH
	 * @param x			The X coordinate of the Node being recalculated
	 * @param y			The Y coordinate of the Node being recalculated
	 */
	void calcHue(std::vector<std::vector<std::shared_ptr<Node> >> mapData, int x, int y);

public:
	AStar();

	/**
	 * The AStar search algorithm. Starts with the current Node in the open list. Switch the lowest cost Node from current node
	 * to the close list. Add each of the edge nodes from here to open vector if not there already, making the current node the parent.
	 * If in closed then do nothing. If already in open but as part of a longer path, change the parent to current node and 
	 * recalculate costs. Loop until goal is added to close vector or open vector is empty. Then backtrack through close list for path.
	 * @param start		The starting location
	 * @param dest		The end goal location
	 * @param mapData	Collection of nodes
	 * @param depth		Step limitation, 0 if unlimited
	 * @return			Data collection of Point2 coordinates
	 */
	std::vector<Point2> AStarSearch(Point2 start, Point2 dest, std::vector<std::vector<std::shared_ptr<Node> >> mapData, int depth);
};

#endif
