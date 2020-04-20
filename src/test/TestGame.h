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
		std::vector<Entity *> npcCollection;
		Entity * npc;

		SDL_Texture * imgWall;
		SDL_Texture * imgBacking;
		SDL_Texture * imgCoin;
		Mix_Chunk * coin;
		Mix_Chunk * aiCollide;
		
		int width;
		int height;
		int tileSize;
		int npcCount = 3;

		Vector2i velocity;
		Vector2i npcVel;

		MazeGenerator * gen;
		AIController * theAI;

		std::vector<std::shared_ptr<Rect>> wall;
		std::vector<std::shared_ptr<Line2i>> lines;
		std::vector<std::shared_ptr<GameKey>> points;

		/* GAMEPLAY */
		int score, keys, lives;
		bool gameWon;
		bool playIntent = false;
		enum gameState { MENU, PAUSE, PLAY, WIN, LOSE };
		gameState state = MENU;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif