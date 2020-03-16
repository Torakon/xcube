#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <string>
#include <iostream>

class Node {
	private:
		std::string id;
		int weight;
		Node * parentNode;
		bool walk = false;
	public:
		Node(int, int, int, int);
		int getWeight();
		void addParent(Node*);
		std::string getID();
		Node* backtrack();
		void setVal(int, int);
		void setWalkable(bool);
		bool isWalkable();
		char direction;
		int cellSize;
		int tilX;
		int tilY;
		int disG;
		int heuH;
};
#endif