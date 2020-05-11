#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "GraphicsEngine.h"
#include <vector>

class Entity {
	private:
		bool randomPatrolB = false;	/**< Flag for patrol behaviour, default to false */
		int x;						/**< Recorded X coordinate */
		int y;						/**< Recorded Y coordinate */
		int h;						/**< Recorded height of Entity */
		int w;						/**< Recorded width of Entity */
		int sight = 0;				/**< Recorded sight radius (square), default to 0 (unlimited) */
		int nodesPassed = 0;		/**< Amount of nodes passed in path, default to 0 */
		int speed = 1;				/**< Speed of Entity movement, default to 1 */
		int initX = 0;				/**< Initial X coordinate, default to 0 */
		int initY = 0;				/**< Initial Y coordinate, default to 0 */

		Rect collider;					/**< Rectangle used for collision*/
		SDL_Texture * texture;			/**< Reference to texture to be used for Entity */
		SDL_Rect * display;				/**< Rectangle for texture display for Entity */
		std::vector<Point2> pathCheck;	/**< Data collection storing the path that is currently being traversed */
	
	public:
		/**
		 * Standard constructor for an Entity
		 * @param xPos			The initial X coordinate of the Entity
		 * @param yPos			The initial Y coordinate of the Entity
		 * @param height		The height of the Entity
		 * @param width			The width of the Entity
		 * @param bounding		Whether or not the Entity has a collision box
		 * @param inputTexture	Pointer to the texture to be used to represent this Entity
		 */
		Entity(int xPos, int yPos, int height, int width, bool bounding, SDL_Texture * inputTexture);

		/**
		 * Get X coordinate
		 * @return
		 */
		int getX();
		/**
		 * Get Y coordinate
		 * @return
		 */
		int getY();
		/**
		 * Get sight distance
		 * @return
		 */
		int getSight();
		/**
		 * Get speed of Entity
		 * @return
		 */
		int getSpeed();
		/**
		 * Get patrol behaviour
		 * @return
		 */
		bool getPatrol();
		/**
		 * Get Collider rectangle
		 * @return
		 */
		Rect getCollider();
		/**
		 * Get texture pointer
		 * @return
		 */
		SDL_Texture* getTexture();
		/**
		 * Get display location
		 * @return
		 */
		SDL_Rect * getDisplay();
		/**
		 * Get progress of currently traversing path
		 * @return
		 */
		float getPathProgress();
		/**
		 * Get location at time of initialisation
		 * @return
		 */
		Point2 getInitLoc();

		/**
		 * Set X and Y coordinate
		 * @param xy	The location to move Entity to
		 */
		void setXY(Point2 xy);
		/**
		 * Set sight distance
		 * @param dist	Detection distance from Entity
		 */
		void setSight(int dist);
		/**
		 * Set the speed of the Entity
		 * @param x		Speed of movement per tick
		 */
		void setSpeed(int x);
		/**
		 * Get patrol behaviour flag
		 * @param yesNo	Whether or not this Entity possesses this behaviour
		 */
		void patrol(bool yesNo);

		/**
		 * Move the Entity along X axis
		 * @param xDif		Speed of movement along axis
		 */
		void moveX(int xDif);
		/**
		 * Move the Entity along Y axis
		 * @param yDif		Speed of movement along axis
		 */
		void moveY(int);

		/**
		 * Move the Entity along the given path
		 */
		void moveAlongPath();
		/**
		 * Set a new path for the Entity to follow
		 * @param path	A new path for the Entity to traverse
		 */
		void aiMovePath(std::vector<Point2> path);
		/**
		 * Remove the currently set path
		 */
		void clearPath();
};

#endif