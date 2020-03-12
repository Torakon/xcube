#include "Node.h"

Node::Node(int xTile, int yTile, int distFrom, int distTo) : tilX(xTile), tilY(yTile), disG(distFrom), heuH(distTo){
	id.append(std::to_string(tilX));
	id.append(std::to_string(tilY));
	weight = disG + heuH;
}
int Node::getWeight() {
	return weight;
}
void Node::addParent(Node* node) {
	parentNode = node;
}
std::string Node::getID() {
	return id;
}
Node* Node::backtrack() {
	if (parentNode != nullptr) {
		return parentNode;
	}
	else {
		return this;
	}
}