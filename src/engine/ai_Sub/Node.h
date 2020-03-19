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
	public:
		Node(int, int, int, int);
		int getWeight();
		void addParent(std::shared_ptr<Node>);
		std::string getID();
		std::shared_ptr<Node> backtrack();
		void setGHCost(int, int);
		int getGVal();
		int getHVal();
		void setWalkable(bool);
		bool isWalkable();

		int cellSize;
		int tilX;
		int tilY;
		int disG;
		int heuH;
};
#endif