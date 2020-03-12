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
	public:
		Node(int, int, int, int);
		int getWeight();
		void addParent(Node*);
		std::string getID();
		Node* backtrack();
		int tilX;
		int tilY;
		int disG;
		int heuH;
};
#endif