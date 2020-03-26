#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <string>
#include <iostream>

class Node {
	private:
		std::string id;
		std::shared_ptr<Node> parentNode;
		bool walk = false;
		int weight;
		int tilX;
		int tilY;
		int disG;
		int heuH;
		bool n = false;
		bool s = false;
		bool e = false;
		bool w = false;
	public:
		enum edgeDir {NORTH, SOUTH, EAST, WEST};

		Node(int, int, int, int);

		//getters
		int getX();
		int getY();
		std::string getID();
		int getGVal();
		int getHVal();
		int getWeight();
		bool getEdge(edgeDir);
		bool isWalkable();
		std::shared_ptr<Node> backtrack();

		//setters
		void addParent(std::shared_ptr<Node>);
		void setGHCost(int, int);
		void setWalkable(bool);
		void setEdge(edgeDir, bool);
};
#endif