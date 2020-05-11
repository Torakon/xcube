#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <string>
#include <iostream>

class Node {
	private:
		std::string id = "null";			/**< Id code of Node for identification */
		std::shared_ptr<Node> parentNode;	/**< Parent of this node used for backtracking in path calcuation */

		int weight;	/**< Cost of node in path, commonly known as 'F' in AStar algorithm */
		int tilX;	/**< Tile location along X coord */
		int tilY;	/**< Tile location along Y coord */
		int disG;	/**< Distance from start node in path, commonly known as 'G' in AStar algortihm */
		int heuH;	/**< Estimated distance to end node in path, commonly known as 'H' in AStar algorithm */

		bool walk = false;		/**< Whether or not the Node can be walked on */
		bool nEdge = false;		/**< Whether or not movement is possible to the North */
		bool sEdge = false;		/**< Whether or not movement is possible to the South */
		bool eEdge = false;		/**< Whether or not movement is possible to the East */
		bool wEdge = false;		/**< Whether or not movement is possible to the West */
	public:
		/**
		 * Used in determining which direction to set or get
		 */
		enum edgeDir { 
			NORTH,	/**< enum value NORTH. */
			SOUTH,	/**< enum value SOUTH. */
			EAST,	/**< enum value EAST. */
			WEST	/**< enum value WEST. */
		};

		/**
		 * Constructor for Node, sets variables
		 * @param xTile		The Tile X location of Node being created
		 * @param yTile		The Tile Y location of Node being created
		 * @param distFrom	Initial disG
		 * @param distTo	Initial hueH
		 */
		Node(int xTile, int yTile, int distFrom, int distTo);

		/**
		 * Get Tile X coordinate
		 * @return
		 */
		int getX();
		/**
		 * Get Tile Y coordinate
		 * @return
		 */
		int getY();
		/**
		 * Get Node ID
		 * @return	Identification of Node, tilX and tilY concatenated into a string
		 */
		std::string getID();
		/**
		 * Get distance from search start Node
		 * @return	
		 */
		int getGVal();
		/**
		 * Get estimated distance to search goal
		 * @return
		 */
		int getHVal();
		/**
		 * Get total cost of Node in search. G+H
		 * @return 
		 */
		int getWeight();
		/**
		 * Get whether or not movement is possible in a certain direction
		 * @param directionGet	Enum of edge direction
		 * @return
		 */
		bool getEdge(edgeDir directionGet);
		/**
		 * Get whether or not Node can be walked on
		 @return
		 */
		bool isWalkable();
		/**
		 * Get pointer to Parent node if one exists
		 @return
		 */
		std::shared_ptr<Node> backtrack();

		/**
		 * Sets the parent of this node to a desired Node
		 * @param node	The new parent of this Node
		 */
		void addParent(std::shared_ptr<Node> node);
		/**
		 * Sets the disG and hueH variables to their newly calculated values
		 * @param newG		New distance from start Node
		 * @param newHeu	New estimated distance from goal Node
		 */
		void setGHCost(int newG, int newHeu);
		/**
		 * Sets whether or not this Node can be walked on
		 * @param condition		Whether or not this Node should be walkable
		 */
		void setWalkable(bool condition);
		/**
		 * Sets whether or not movement is possible in a certain direction fron this Node
		 * @param directionSet	The direction to set movement possibilty
		 * @param possible		Whether or not movement is possible
		 */
		void setEdge(edgeDir directionSet, bool possible);
};
#endif