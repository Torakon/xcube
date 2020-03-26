#include "Node.h"

Node::Node(int xTile, int yTile, int distFrom, int distTo) : tilX(xTile), tilY(yTile), disG(distFrom), heuH(distTo){
	id.append(std::to_string(tilX));
	id.append(std::to_string(tilY));
}

int Node::getX() {
	return tilX;
}

int Node::getY() {
	return tilY;
}

std::string Node::getID() {
	return id;
}

int Node::getGVal() {
	return disG;
}

int Node::getHVal() {
	return heuH;
}

int Node::getWeight() {
	weight = disG + heuH;
	return weight;
}

void Node::addParent(std::shared_ptr<Node> node) {
	parentNode = node;
}

void Node::setGHCost(int newG, int newHeu) {
	disG = newG;
	heuH = newHeu;
}

void Node::setWalkable(bool condition) {
	walk = condition;
}

bool Node::isWalkable() {
	return walk;
}

std::shared_ptr<Node> Node::backtrack() {
	if (parentNode != nullptr) {
		return parentNode;
	}
	else {
		return NULL;
	}
}

void Node::setEdge(edgeDir x, bool possible) {
	switch (x) {
	case NORTH :
		n = possible;
		break;
	case SOUTH :
		s = possible;
		break;
	case EAST :
		e = possible;
		break;
	case WEST :
		w = possible;
		break;
	}
}

bool Node::getEdge(edgeDir x) {
	switch (x) {
	case NORTH:
		return n;
	case SOUTH:
		return s;
	case EAST:
		return e;
	case WEST:
		return w;
	default :
		return false;
	}
}