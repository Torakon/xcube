#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "GraphicsEngine.h"
#include <vector>

class Entity {
	private:
		bool randomPatrolB = false;
		int x;
		int y;
		int h;
		int w;
		int sight = 1;
		int nodesPassed = 0;
		int speed = 1;

		Rect collider;
		SDL_Texture * texture;
		SDL_Rect * display;
		std::vector<Point2> pathCheck;
	
	public:
		Entity(int, int, int, int, bool, SDL_Texture *);

		//getters
		int getX();
		int getY();
		int getSight();
		int getSpeed();
		bool getPatrol();
		Rect getCollider();
		SDL_Texture* getTexture();
		SDL_Rect * getDisplay();
		float getPathProgress();

		//setters
		void setXY(Point2);
		void setSight(int);
		void setSpeed(int);
		void patrol(bool);

		//functional
		void moveX(int);
		void moveY(int);

		void moveAlongPath();
		void aiMovePath(std::vector<Point2>);
		void clearPath();
};

#endif