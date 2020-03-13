#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "GraphicsEngine.h"

class Entity {
	private:
		bool bBox;
		void addBounding();
		int h;
		int w;
		int sight = 0;
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
};

#endif