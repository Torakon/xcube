#include "Node.h"

Node::Node(int xTile, int yTile, int distFrom, int distTo) : tilX(xTile), tilY(yTile), disG(distFrom), heuH(distTo){
	id.append(std::to_string(tilX));
	id.append(std::to_string(tilY));
}

int Node::getWeight() {
	weight = disG + heuH;
	return weight;
}

void Node::addParent(std::shared_ptr<Node> node) {
	parentNode = node;
}

std::string Node::getID() {
	return id;
}

std::shared_ptr<Node> Node::backtrack() {
	if (parentNode != nullptr) {
		return parentNode;
	}
	else {
		return NULL;
	}
}

void Node::setVal(int newG, int newHeu) {
	disG = newG;
	heuH = newHeu;
}

void Node::setWalkable(bool condition) {
	walk = condition;
}

bool Node::isWalkable() {
	return walk;
}