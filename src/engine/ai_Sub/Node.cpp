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

bool Node::getEdge(edgeDir directionGet) {
	switch (directionGet) {
	case NORTH:
		return nEdge;
	case SOUTH:
		return sEdge;
	case EAST:
		return eEdge;
	case WEST:
		return wEdge;
	default:
		return false;
	}
}

std::shared_ptr<Node> Node::backtrack() {
	if (parentNode != nullptr) {
		return parentNode;
	}
	else {
		return NULL;
	}
}

bool Node::isWalkable() {
	return walk;
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

void Node::setEdge(edgeDir directionSet, bool possible) {
	switch (directionSet) {
	case NORTH :
		nEdge = possible;
		break;
	case SOUTH :
		sEdge = possible;
		break;
	case EAST :
		eEdge = possible;
		break;
	case WEST :
		wEdge = possible;
		break;
	}
}