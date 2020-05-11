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
		TTF_Font * font;
		TTF_Font * amaticFont;
		TTF_Font * fontSmall;

		SDL_Rect * bg;

		Entity * player;
		std::vector<Entity *> npcCollection;
		Entity * npc;

		Rect btnPlay = Rect(0, 0, 0, 0);
		Rect btnDiff = Rect(0, 0, 0, 0);

		SDL_Texture * entityTexture;
		SDL_Texture * imgWall;
		SDL_Texture * imgBacking;
		SDL_Texture * imgCoin;
		Mix_Chunk * coin;
		Mix_Chunk * aiCollide;
		
		int width;
		int height;
		int tileSize;
		int npcCount = 3;
		bool playHover = false;
		bool diffHover = false;
		bool btnBreak = true;
		bool resetBreak = false;

		Vector2i velocity;
		Vector2i npcVel;

		MazeGenerator * gen;
		AIController * theAI;

		std::vector<std::shared_ptr<Rect>> wall;
		std::vector<std::shared_ptr<Line2i>> lines;
		std::vector<std::shared_ptr<GameKey>> points;

		/* GAMEPLAY */
		int score, keys, lives, scoreInc;
		bool gameWon;
		enum gameState { MENU, PAUSE, PLAY, WIN, LOSE, LIFELOSS };
		gameState state = MENU;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
		void handleMenu();
		void generateLevel();
	public:
		TestGame();
		~TestGame();
};

#endif