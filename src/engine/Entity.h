#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "GraphicsEngine.h"
#include <vector>

class Entity {
	private:
		bool bBox;
		void addBounding();
		int h;
		int w;
		int sight = 1;
		int nodesPassed = 0;
		int speed = 1;
		std::vector<Point2> pathCheck;
	public:
		int x;
		int y;

		Rect collider;
		SDL_Texture * texture;
		SDL_Rect * display;

		Entity(int, int, int, int, bool, SDL_Texture *);
		void moveX(int);
		void moveY(int);
		void setSight(int);
		int getSight();
		void moveAlongPath();
		void aiMovePath(std::vector<Point2>);
		float getPathProgress();
};

#endif