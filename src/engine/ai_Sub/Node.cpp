#include "Node.h"

Node::Node(int xTile, int yTile, int distFrom, int distTo) : tilX(xTile), tilY(yTile), disG(distFrom), heuH(distTo){
	id.append(std::to_string(tilX));
	id.append(std::to_string(tilY));
}

void Node::addParent(std::shared_ptr<Node> node) {
	parentNode = node;
}

std::string Node::getID() {
	return id;
}

void Node::setGHCost(int newG, int newHeu) {
	disG = newG;
	heuH = newHeu;
}

int Node::getGVal() {
	return disG;
}

int Node::getHVal() {
	return heuH;
}

void Node::setWalkable(bool condition) {
	walk = condition;
}

bool Node::isWalkable() {
	return walk;
}

int Node::getWeight() {
	weight = disG + heuH;
	return weight;
}

std::shared_ptr<Node> Node::backtrack() {
	if (parentNode != nullptr) {
		return parentNode;
	}
	else {
		return NULL;
	}
}