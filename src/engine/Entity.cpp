#include "Entity.h"

Entity::Entity(int xPos, int yPos, int height, int width, bool bounding, SDL_Texture * inputTexture) : x(xPos), y(yPos), h(height), w(width), bBox(bounding), collider(0, 0, 0, 0) {
	texture = inputTexture; //can we set bounding box based on image size?
	display = new SDL_Rect{ x, y, h, w }; 

	if (bBox) {
		addBounding();
	}
}

//adds a collider in the form of a box
void Entity::addBounding() {
	collider = Rect(x, y, h, w);
}

//moves entity's collider and texture display on the X axis
void Entity::moveX(int xDif) {
	x += xDif;
	display->x = x;
	collider.x = x;
}

//moves entity's collider and texture display on the Y axis
void Entity::moveY(int yDif) {
	y += yDif;
	display->y = y;
	collider.y = y;
}

void Entity::setSight(int dist) {
	sight = dist;
}

int Entity::getSight() {
	return sight;
}

void Entity::patrol(bool yesNo) {
	randomPatrolB = yesNo; //possibly promote to Component type and just call one method of get behaviour that returns a list of Components for access
}

bool Entity::getPatrol() {
	return randomPatrolB;
}

void Entity::moveAlongPath() {
	if (nodesPassed < pathCheck.size()) {
		int destinationX = pathCheck[nodesPassed].x;
		int destinationY = pathCheck[nodesPassed].y;
		if ((w+1)*(destinationX) > x) {
			moveX(speed);
		}
		else if ((w+1)*(destinationX) < x) {
			moveX(-speed);
		}
		if ((w+1)*(destinationY) > y) {
			moveY(speed);
		}
		else if ((w+1)*(destinationY) < y) {
			moveY(-speed);
		}
		if ((w+1)*(destinationX) == x && (w+1)*(destinationY) == y) {
			nodesPassed += 1;
			pathCheck.erase(pathCheck.begin());
		}
	}
}

void Entity::aiMovePath(std::vector<Point2> path) {
	pathCheck = path;
}

float Entity::getPathProgress() {
	if (pathCheck.size() == 0) {
		return 1;
	}
	if (nodesPassed == 0) {
		return 0;
	}
	if (nodesPassed == pathCheck.size()) {
		nodesPassed = 0;
		return 1;
	}
	nodesPassed = 0;
	return (nodesPassed/pathCheck.size());
}


