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
		TTF_Font * amaticFont; /**< Primary Font for TestGame */
		TTF_Font * fontSmall; /**< Secondary Font for TestGame, buttons */

		SDL_Rect * bg; /**< Display area for Background image*/

		Entity * player;
		std::vector<Entity *> npcCollection; /**< Data collection storing pointers to currently spawned NPC Entities */
		Entity * npc;

		Rect btnPlay = Rect(0, 0, 0, 0); /**< Start menu play button */
		Rect btnDiff = Rect(0, 0, 0, 0); /**< Start menu difficulty switch button */

		SDL_Texture * entityTexture;	/**< Texture for use on Entity */
		SDL_Texture * npcTexture;		/**< Texture for use on npcs */
		SDL_Texture * imgWall;			/**< Texture for use on walls */
		SDL_Texture * imgBacking;		/**< Texture for use as background */
		SDL_Texture * imgCoin;			/**< Texture for use as pickup coin */
		Mix_Chunk * coin;				/**< Sound file to play as pickup coin */
		Mix_Chunk * aiCollide;			/**< Sound File to play on npc collision with player */
		
		int width;					/**< Map width in pixels */
		int height;					/**< map height in pixels */
		int tileSize;				/**< Size of the tiles used for the map */
		int npcCount = 3;			/**< Number of npcs to spawn, default to 3 */
		bool playHover = false;		/**< Flag for player mouse hovering over play button */
		bool diffHover = false;		/**< Flag for player mouse hovering over difficulty button */
		bool btnBreak = true;		/**< Flag to stop continuous activation of button */
		bool resetBreak = false;	/**< Opposing Flag to stop free up use of the button again */

		Vector2i velocity;			
		Vector2i npcVel;			/**< Value added onto movement of an npc  */

		MazeGenerator * gen;

		std::vector<std::shared_ptr<Rect>> wall;		/**< Data collection storing the generated walls of the map */
		std::vector<std::shared_ptr<GameKey>> points;	/**< Data collection storing the generated coins of the map */

		/* GAMEPLAY */
		int score, keys, lives;
		int scoreInc;				/**< Score multiplayer based on difficulty setting */

		/**
		 * Enum keeping a record of the game state for appropriate reactions default to MENU
		 */
		enum gameState { 
			MENU,		/**< enum value MENU. */
			PAUSE,		/**< enum value PAUSE. */
			PLAY,		/**< enum value PLAY. */
			WIN,		/**< enum value WIN. */
			LOSE,		/**< enum value LOSE. */
			LIFELOSS	/**< enum value LIFELOSS. */
		};
		gameState state = MENU;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
		/**
		 * Deals with the interactivity and functionality of the start menu buttons
		 */
		void handleMenu();
		/**
		 * executes the map and coin generation code
		 */
		void generateLevel();

	public:
		TestGame();
		~TestGame();
};

#endif