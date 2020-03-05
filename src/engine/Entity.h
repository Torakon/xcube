#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "GraphicsEngine.h"

class Entity {
	private:
		bool bBox;
		void addBounding();
		int h;
		int w;
	public:
		Rect collider;
		SDL_Texture * texture;
		SDL_Rect tester;
		Entity(int, int, int, int, bool, SDL_Texture *);
		void moveX(int);
		void moveY(int);
		SDL_Rect * display;
		int x;
		int y;
};

#endif