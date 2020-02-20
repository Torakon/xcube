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
		Rect player;
		Rect npc;
		//Rect light; --Required constructor if left in

		Vector2i velocity;
		Vector2i npcVel;

		MazeGenerator * gen;

		std::vector<std::shared_ptr<Line2i>> lines;
		std::vector<std::shared_ptr<GameKey>> points;

		/* GAMEPLAY */
		int score, keys, lives;
		bool gameWon;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif