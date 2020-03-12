#include "Entity.h"

Entity::Entity(int xPos, int yPos, int height, int width, bool bounding, SDL_Texture * image) : x(xPos), y(yPos), h(height), w(width), bBox(bounding), collider(0, 0, 0, 0) {
	texture = image; //can we set bounding box based on image size?
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

//int Entity::getX() {
	//return x;
//}

