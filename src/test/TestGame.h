#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

#include "TestMazeGenerator.h"

struct GameKey {
	Point2 pos;
	bool alive;
};

class TestGame : public AbstractGame {
	private:
		Entity * player;
		Entity * npc;
		
		int width;
		int height;
		int tileSize;

		Vector2i velocity;
		Vector2i npcVel;

		MazeGenerator * gen;
		AIController * theAI;

		std::vector<std::shared_ptr<Rect>> wall;
		std::vector<std::shared_ptr<Line2i>> lines;
		std::vector<std::shared_ptr<GameKey>> points;

		/* GAMEPLAY */
		int score, keys, lives, state;
		bool gameWon;
		enum gameState { PAUSE, PLAY, WIN, LOSE };

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif