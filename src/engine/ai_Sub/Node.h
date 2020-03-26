#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <string>
#include <iostream>

class Node {
	private:
		std::string id;
		int weight;
		std::shared_ptr<Node> parentNode;
		bool walk = false;
		int tilX;
		int tilY;
		int disG;
		int heuH;
	public:
		enum edgeDir {NORTH, SOUTH, EAST, WEST};
		bool n = false;
		bool s = false;
		bool e = false;
		bool w = false;

		Node(int, int, int, int);

		int getX();
		int getY();
		std::string getID();
		int getGVal();
		int getHVal();
		int getWeight();

		void addParent(std::shared_ptr<Node>);
		void setGHCost(int, int);
		void setWalkable(bool);
		bool isWalkable();
		std::shared_ptr<Node> backtrack();
		void setEdge(edgeDir, bool);
		bool getEdge(edgeDir);
};
#endif